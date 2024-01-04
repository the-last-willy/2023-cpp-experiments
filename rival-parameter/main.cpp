#include <cassert>
#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct NoValidationPolicy {
    template<typename T>
    bool isValid(const T &value) {
        return true;
    }
};

// Note that PositiveValidationPolicy is a fully specified type.
// It doesn't need to be for a given type specifically,
// only for `T >= 0` to be a valid expression.
struct PositiveValidationPolicy {
    template<typename T>
    bool isValid(const T &value) {
        return value >= 0;
    }
};

// Fully static validation policies become possible
// if we can get rid of the (ValueType, ValidationPolicy) combinatorics.
template<typename T, T lowerBound, T upperBound>
struct StaticClampValidationPolicy {
    bool isValid(const T &value) {
        return lowerBound <= value && value <= upperBound;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct IParameter {
    // Basically getInstanceRtti() for the parameter's value.
    // Useful so the user can create a visitor or whatever.
    // We can use any RTTI mechanism here.
    virtual const std::type_info &getValueType() = 0;

    virtual const std::type_info &getType() = 0;

    // Generic get/set services.
    // As long as the value is gettable/settable from an IParameter
    // the (ValueType, ValidationPolicy) combinatorics is no longer necessary.

    // Note that the validation policy still applies on the value through the setter.

    // Error handling is not specified here.
    // We could implement any behaviour (exception, conversion, optional, silent, ...).

    // Uses type erasure internally.
    // But this is not the only way.
    // E.g. we could use a variant instead.

    template<typename T>
    const T &get() {
        return *reinterpret_cast<const T *>(getImpl(typeid(T)));
    }

    template<typename T>
    void set(const T &value) {
        setImpl(typeid(T), &value);
    }

private:
    // Template method pattern
    // so `get` and `set` can be shadowed in the derived class
    // in order to use the concrete type.

    virtual const void *getImpl(const std::type_info &type) { throw std::exception(); }

    virtual void setImpl(const std::type_info &type, const void *data) { throw std::exception(); }
};

struct ParameterGroup : IParameter {
    const std::type_info &getType() override {
        return typeid(ParameterGroup);
    }

    const std::type_info &getValueType() override {
        return typeid(void);
    }

    std::vector<IParameter*> children;
};

struct IValueParameter : IParameter {
};

// Validation policies do not necessarily need to be template template parameters.
// See PositiveValidationPolicy.
template<typename ValueT, typename ValidationPolicyT>
struct Parameter : IValueParameter {
    using ValidationPolicyType = ValidationPolicyT;
    using ValueType = ValueT;

    Parameter(ValueType value, ValidationPolicyType vp)
            : value(std::move(value)), vp(std::move(vp)) {}

    const std::type_info &getType() override {
        return typeid(Parameter);
    }

    const std::type_info &getValueType() override {
        return typeid(ValueType);
    }

    // Shadowing the base class is intended,
    // so we can have the same API for both
    // without having to specify the value type
    // as it is known.

    const ValueType &get() {
        return value;
    }

    void set(const ValueType &val) {
        // Validation is done here.
        if (!vp.isValid(val))
            throw std::exception("Failed to validate value.");
        else
            value = val;
    }

private:
    const void *getImpl(const std::type_info &type) override {
        // Since the requested type provided
        // might be different from the parameter type
        // we can implement any conversion behaviour here
        // or throw an exception like the following.
        if (type != typeid(ValueType))
            throw std::exception();
        else
            return reinterpret_cast<const void *>(&get());
    }

    void setImpl(const std::type_info &type, const void *ptr) override {
        // Same as `getImpl`,
        // we can implement any behaviour here.
        if (type != typeid(ValueType))
            throw std::exception();
        set(*reinterpret_cast<const ValueType *>(ptr));
    }

    ValueType value = {};
    ValidationPolicyType vp;
};

void parameter_example() {
    auto staticparam = Parameter<int, PositiveValidationPolicy>(2, {});
//    auto staticparam = Parameter<int, StaticClampValidationPolicy<int, 0, 3>>(2, {});
    IParameter &dynparam = staticparam;

    assert(staticparam.get() == 2);
    assert(staticparam.get() == dynparam.get<int>());

    dynparam.set(3);

    assert(staticparam.get() == 3);
    assert(staticparam.get() == dynparam.get<int>());

    try {
        // Throws because of the validation policy.
        dynparam.set(-1);
        assert(false);
    } catch (...) {}

    try {
        // Throws because wrong type.
        dynparam.set("ok");
        assert(false);
    } catch (...) {}

    try {
        // Throws because wrong type.
        dynparam.get<bool>();
        assert(false);
    } catch (...) {}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ParameterValueVisitor {
    virtual void visit(IParameter &param) = 0;
};

// This class visits parameters statically.
// There's no need to have a map of RTTI types to functions.
template<typename Handler>
struct StaticParameterDispatcher : ParameterValueVisitor {
    using TypeList = typename Handler::TypeList;

    explicit StaticParameterDispatcher(Handler handler)
            : handler(handler) {}

    void visit(IParameter &param) final {
        visitLoop(param, (TypeList *) nullptr);
    }

    Handler handler;

private:
    // The following functions loop through the type list,
    // it could be better with an adequate (static) data structure.

    void visitLoop(IParameter &param, const std::tuple<> * = {}) {
        handler.visitNothing();
    }

    template<typename Head, typename... Tail>
    void visitLoop(IParameter &param, const std::tuple<Head, Tail...> * = {}) {
        if (param.getValueType() == typeid(Head))
            handler.visitValue(param.get<Head>());
        else
            visitLoop(param, (std::tuple<Tail...> *) nullptr);
    }
};

// Small helper to hide the StaticParameterDispatcher.
template<typename Handler>
void visit(IParameter &param, Handler &handler) {
    auto dispatcher = StaticParameterDispatcher(handler);
    dispatcher.visit(param);
}

// Users only have to provide handlers in order to visit parameters.
struct ParameterPrinter {
    using TypeList = std::tuple<bool, int, std::string>;

    // Pre, post, ...

    template<typename T>
    void visitValue(const T &value) {
        std::cout << "Type = " << typeid(T).name() << ", Value = " << value << std::endl;
    }

    // Fallback case in case the type was not listed.
    void visitNothing() {
        throw std::exception();
    }
};

void visitor_example() {
    std::cout << std::endl << "--Visiting parameters" << std::endl << std::endl;

    auto p0 = Parameter<bool, NoValidationPolicy>(true, {});
    auto p1 = Parameter<int, NoValidationPolicy>(2, {});
    auto p2 = Parameter<std::string, NoValidationPolicy>("my-str", {});

    auto ps = std::vector<IParameter *>{&p0, &p1, &p2};

    auto printer = ParameterPrinter();

    for (auto p: ps)
        visit(*p, printer);

    auto p3 = Parameter<short, NoValidationPolicy>(2, {});
    try {
        visit(p3, printer);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename...>
struct TypeList {
};

struct IParameterVisitor {
    using SupportedParameterTypes = TypeList<ParameterGroup, IValueParameter>;

    void visit(IParameter &param) {
        dispatch(param);
    }

    virtual void visit(IValueParameter &vp) { onUnsupportedParameterType(); }

    void visit(ParameterGroup &group) {
        enter(group);
        if(!group.children.empty()) {
            visit(*group.children.front());
            for(std::size_t i = 1; i < group.children.size(); ++i) {
                reenter(group);
                visit(*group.children[i]);
            }
        }
        exit(group);
    }

protected:
    void dispatch(IParameter &param) {
        dispatchLoop(param, SupportedParameterTypes());
    }

private:
    void dispatchLoop(IParameter &param, TypeList<> = {}) {

    }

    template<typename Head, typename... Tail>
    void dispatchLoop(IParameter &param, TypeList<Head, Tail...> = {}) {
        if (auto concrete = dynamic_cast<Head *>(&param)) {
            visit(*concrete);
        } else {
            dispatchLoop(param, TypeList<Tail...>());
        }
    }

    virtual void onUnsupportedParameterType() = 0;

    virtual void enter(ParameterGroup &group) { onUnsupportedParameterType(); }

    virtual void reenter(ParameterGroup &group) { onUnsupportedParameterType(); }

    virtual void exit(ParameterGroup &group) { onUnsupportedParameterType(); }
};

struct ParameterJsonSerializer final : IParameterVisitor {
    using IParameterVisitor::visit;

    void onUnsupportedParameterType() override {

    }

    void enter(ParameterGroup &group) override {
        content += "[";
    }

    void reenter(ParameterGroup &group) override {
        content += ",";
    }

    void exit(ParameterGroup &group) override {
        content += "]";
    }

    void visit(IValueParameter &param) override {
        content += "X";
    }

    std::string content;
};

void traverse_parameters(IParameter &param) {
    std::vector<std::tuple<IParameter *, int>> stack = {{&param, 0}};
    while (!stack.empty()) {

    }
}

void traverser_example() {
    auto params = ParameterGroup();
    {
        auto group = (ParameterGroup*) params.children.emplace_back(new ParameterGroup());
        {
            auto value = group->children.emplace_back(new Parameter<int, NoValidationPolicy>(1, {}));
        }
    }
    {
        auto& group = params.children.emplace_back(new ParameterGroup());
    }

    auto visitor = ParameterJsonSerializer();
    visitor.visit(params);

    std::cout << visitor.content << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
//    parameter_example();
//    visitor_example();
    traverser_example();
}
