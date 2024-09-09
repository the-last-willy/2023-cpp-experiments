#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

[[noreturn]]
inline void not_implemented()
{
    throw std::logic_error("not implemeted");
}

template <typename T, typename Allocator = std::allocator<T>>
class MyVector
{
public:
    MyVector() {}

    MyVector(std::initializer_list<T> il)
    {
        not_implemented();
    }

    // Element access



    // Iterators

    T* begin() const
    {
        not_implemented();
    }

    T* end() const
    {
        not_implemented();
    }

    // Capacity

    std::size_t capacity() const
    {
        not_implemented();
    }

    std::size_t size() const
    {
        not_implemented();
    }
};

template<typename... Traits>
bool operator==(const MyVector<Traits...>& l, const std::vector<Traits...>& r)
{
    not_implemented();
}
