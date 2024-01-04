#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

class Data {
public:
    enum class Type {
        list,
        boolean,
        floating
    };

    virtual Type type() = 0;

    virtual bool asBoolean() {
        throw std::exception("Composite does not contain a boolean.");
    }

    virtual double asFloat() {
        throw std::exception("Composite does not contain a floating point.");
    }
};

class List : public Data {
public:
    List() = default;

    Type type() override {
        return Type::list;
    }

    void append(std::unique_ptr<Data> data) {
        elements.push_back(std::move(data));
    }

    Data* at(std::size_t i) {
        return elements.at(i).get();
    }

    std::size_t len() {
        return size(elements);
    }

private:
    std::vector<std::unique_ptr<Data>> elements;
};

class Bool : public Data {
public:
    explicit Bool(bool value)
            : m_value(value) {}

    Type type() override {
        return Type::boolean;
    }

    bool asBoolean() override {
        return m_value;
    }

private:
    bool m_value = false;
};

class Float : public Data {
public:
    explicit Float(double value)
            : m_value(value) {}

    Type type() override {
        return Type::floating;
    }

    double asFloat() override {
        return m_value;
    }

private:
    double m_value = 0.;
};
