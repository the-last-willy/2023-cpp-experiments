#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

class Data;
class DataVisitor;
class Int;
class Float;
class List;
class String;
class Struct;

class Data
{
public:
    enum class Type
    {
        Float,
        Int,
        List,
        String,
        Struct,
    };

    virtual ~Data() = default;

    virtual Type getType() const = 0;

    virtual void accept(DataVisitor& visitor) = 0;
};

class DataVisitor
{
public:
    virtual ~DataVisitor() = default;

    virtual void visit(Float& floating) = 0;
    virtual void visit(List& list) = 0;
    virtual void visit(Int& integer) = 0;
    virtual void visit(String& str) = 0;
    virtual void visit(Struct& obj) = 0;
};

class Float : public Data
{
public:
    explicit Float(double value = {})
        : m_value(value)
    {
    }

    double& getValue()
    {
        return m_value;
    }

public:
    void accept(DataVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    Type getType() const override
    {
        return Type::Float;
    }

private:
    double m_value;
};

class Int : public Data
{
public:
    explicit Int(long long value = {})
        : m_value(value)
    {
    }

    long long& getValue()
    {
        return m_value;
    }

public:
    void accept(DataVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    Type getType() const override
    {
        return Type::Int;
    }

private:
    long long m_value;
};

class List : public Data
{
public:
    List() = default;

    std::vector<std::unique_ptr<Data>>& getValues()
    {
        return m_values;
    }

public:
    void accept(DataVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    Type getType() const override
    {
        return Type::List;
    }

private:
    std::vector<std::unique_ptr<Data>> m_values;
};

class String : public Data
{
public:
    explicit String(std::string value = {})
        : m_value(std::move(value))
    {
    }

    std::string& getValue()
    {
        return m_value;
    }

public:
    void accept(DataVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    Type getType() const override
    {
        return Type::String;
    }

private:
    std::string m_value;
};

class Struct : public Data
{
public:
    Struct() = default;

    std::map<std::string, std::unique_ptr<Data>>& getMembers()
    {
        return m_members;
    }

public:
    void accept(DataVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    Type getType() const override
    {
        return Type::Struct;
    }

private:
    std::map<std::string, std::unique_ptr<Data>> m_members;
};
