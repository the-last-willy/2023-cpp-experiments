#pragma once

#include "raw_vector.hpp"

namespace dyn {

template<typename T>
T& as_ref(void* raw)
{
    return *static_cast<T*>(raw);
}

template<typename T>
class vector
{
public:
    vector()
        : raw_vec(alignof(T), sizeof(T))
    {}

    T& operator[](std::size_t i)
    {
        return *static_cast<T*>(raw_vec[i]);
    }

    void resize(std::size_t new_size)
    {

    }

    void push_back(T value)
    {
        auto ctor_fn = stateful_fn<void(void*)>(
            [](void* value, void* mem)
            {
                new(mem) T(std::move(*static_cast<T*>(value)));
            },
            &value);
        auto move_ctor_fn = stateful_fn<void(void* src, void* dst)>(
            [](void* state, void* self, void* other)
            {
                as_ref<T>(self) = std::move(as_ref<T>(other));
            },
            nullptr);
        raw_vec.push_back(ctor_fn, move_ctor_fn);
    }

    std::size_t size() const
    {
        return raw_vec.size();
    }

    std::size_t capacity() const
    {
        return raw_vec.capacity();
    }

    T& back()
    {
        return as_ref<T>(raw_vec.operator[](raw_vec.size() - 1));
    }

private:
    raw_vector raw_vec;
};

}
