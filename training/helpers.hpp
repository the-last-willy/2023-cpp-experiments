#pragma once

#include <functional>
#include <iostream>
#include <stdexcept>

std::function on_new = [](void*, std::size_t){};
std::function on_delete = [](void*, std::size_t){};

inline void* operator new(std::size_t count)
{
    auto ptr = std::malloc(count);
    on_new(ptr, count);
    return ptr;
}

inline void operator delete(void* ptr, std::size_t count)
{
    std::free(ptr);
    on_delete(ptr, count);
}

inline void operator delete[](void* ptr, std::size_t count)
{
    std::free(ptr);
    on_delete(ptr, count);
}

struct Int
{
    Int() = default;

    Int(int i)
        : value(i)
    {}

    Int(const Int&) = default;
    Int(Int&&) = default;

    operator int&()
    {
        return value;
    }

    int value = 0;
};

template<typename T>
class Logging : public T
{
public:
    Logging()
    {
        std::cout << "default ctor" << std::endl;
    }

    Logging(T value)
        : T(value)
    {
        std::cout << "value ctor " << *this << std::endl;
    }

    Logging(const Logging& other)
        : T(other.value)
    {
        std::cout << "copy ctor " << *this << std::endl;
    }

    Logging(Logging&& other)
        : T(std::move(other.value))
    {
        std::cout << "move ctor " << *this << std::endl;
    }

    Logging& operator=(const Logging& other)
    {
        T::operator=(other.value);
        std::cout << "copy assign " << *this << std::endl;
    }

    Logging& operator=(Logging&& other)
    {
        T::operator=(std::move(other.value));
        std::cout << "move assign " << *this << std::endl;
    }

    ~Logging()
    {
        std::cout << "dtor " << *this << std::endl;
    }
};

template<typename T>
class ThrowWhenCopied : public T
{
public:
    ThrowWhenCopied() = default;

    ThrowWhenCopied(const ThrowWhenCopied&)
    {
        throw std::logic_error("copy is forbidden");
    }

    ThrowWhenCopied(ThrowWhenCopied&&) noexcept = default;

    ThrowWhenCopied& operator=(const ThrowWhenCopied&)
    {
        throw std::logic_error("copy is forbidden");
    }

    ThrowWhenCopied& operator=(ThrowWhenCopied&&) = default;

    ~ThrowWhenCopied() = default;
};
