#pragma once

#include "Data.h"

class DataPrinter : public DataVisitor
{
public:
    void visit(Float& floating) override
    {
        std::cout << indentation() << floating.getValue() << std::endl;
    }

    void visit(Int& integer) override
    {
        std::cout << indentation() << integer.getValue() << std::endl;
    }

    void visit(List& list) override
    {
        std::cout << indentation() << "list:\n";
        m_indent += 1;
        for (auto&& value : list.getValues())
            value->accept(*this);
        m_indent -= 1;
    }

    void visit(String& str) override
    {
        std::cout << indentation() << std::quoted(str.getValue()) << std::endl;
    }

    void visit(Struct& obj) override
    {
        std::cout << indentation() << "struct:\n";
        m_indent += 1;
        for (auto&& [name, data] : obj.getMembers())
        {
            std::cout << indentation() << std::quoted(name) << ": ";
            m_skipNextIndent = true;
            data->accept(*this);
        }
        m_indent -= 1;
    }

private:
    std::string indentation()
    {
        if(m_skipNextIndent)
        {
            m_skipNextIndent = false;
            return std::string();
        }
        else
            return std::string(2 * m_indent, ' ');
    }

private:
    int m_indent = 0;
    bool m_skipNextIndent = false;
};
