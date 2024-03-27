#pragma once

#include <utility>

namespace dyn
{
    template<typename...>
    class stateful_fn;

    template<typename Ret, typename... Args>
    class stateful_fn<Ret(Args...)>
    {
    public:
        using inner_fn_type = Ret (*)(void*, Args...);

        stateful_fn(Ret(*fn)(void*, Args...), void* state = nullptr)
        : fn(fn)
        , state(state)
        {}

        Ret operator()(Args&&... args)
        {
            return fn(state, std::forward<Args>(args)...);
        }

    private:
        Ret (*fn)(void*, Args...) = nullptr;

        void* state = nullptr;
    };
}
