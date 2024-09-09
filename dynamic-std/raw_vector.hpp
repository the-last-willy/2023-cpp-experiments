#pragma once

#include <cstddef>
#include <cstdlib>
#include <stdexcept>

#include "stateful_fn.hpp"

namespace dyn
{
    template <typename T>
    using function_t = T;

    class raw_vector
    {
    public:
        using ctor_fn_t = stateful_fn<void(void* self)>;
        using move_ctor_fn_t = stateful_fn<void(void* self, void* other)>;

        using dtor_fn_t = stateful_fn<void(void* self)>;
        using move_fn_t = stateful_fn<void(void* from, void* to)>;

        raw_vector() = default;

        raw_vector(std::size_t alignment, std::size_t size) :
            type_align(alignment),
            type_size(size)
        {
        }

        raw_vector(const raw_vector&) = delete;

        raw_vector(raw_vector&& other) noexcept
        {
            swap(other);
        }

        raw_vector& operator=(const raw_vector&) = delete;

        raw_vector& operator=(raw_vector&& other)
        {
            auto tmp = raw_vector(std::move(other));
            swap(tmp);
            return *this;
        }

        ~raw_vector()
        {
            std::free(memory);
        }

        std::size_t capacity() const
        {
            return vec_cap;
        }

        std::size_t size() const
        {
            return vec_size;
        }

        void swap(raw_vector& other)
        {
            std::swap(type_align, other.type_align);
            std::swap(type_align, other.type_align);

            std::swap(vec_cap, other.vec_cap);
            std::swap(vec_size, other.vec_size);

            std::swap(memory, other.memory);
        }

        void* operator[](std::size_t i)
        {
            return static_cast<char*>(memory) + i * type_size;
        }

        void push_back(ctor_fn_t ctor_fn, move_ctor_fn_t move_ctor_fn)
        {
            if(vec_size == vec_cap)
            {
                auto new_cap = vec_cap == 0 ? 1 : 2 * vec_cap;
                reserve(new_cap, move_ctor_fn);
            }
            ctor_fn(operator[](vec_size));
            vec_size += 1;
        }

        void resize(std::size_t new_size, ctor_fn_t ctor_fn, dtor_fn_t dtor_fn, move_fn_t move_fn)
        {
            if (vec_cap != 0)
                throw std::logic_error("not implemented");

            memory = std::aligned_alloc(type_align, type_size * new_size);
            vec_cap = new_size;
            vec_size = new_size;

            for (std::size_t i = 0; i < vec_size; ++i)
                ctor_fn(operator[](i));
        }

        void reserve(std::size_t new_cap, move_ctor_fn_t move_ctor_fn)
        {
            if(new_cap <= vec_cap)
                return;

            auto new_mem = std::aligned_alloc(type_align, new_cap * type_size);
            for(std::size_t i = 0; i < vec_size; ++i)
            {
                auto off = idx_mem_offset(i);
                move_ctor_fn(memory + off, new_mem + off);
            }
            std::swap(memory, new_mem);
            vec_cap = new_cap;
            std::free(new_mem);
        }

        void clear(stateful_fn<void(void*)> dtor_fn)
        {
            for (int i = 0; i < vec_size; ++i)
                dtor_fn(operator[](i));
        }

        void* data()
        {
            return begin();
        }

        void* begin()
        {
            return operator[](0);
        }

        void* end()
        {
            return operator[](vec_size);
        }

    private:
        std::size_t idx_mem_offset(std::size_t idx) const
        {
            return idx * type_size;
        }

    private:
        std::size_t type_align = 0;
        std::size_t type_size = 0;

        std::size_t vec_size = 0;
        std::size_t vec_cap = 0;

        void* memory = nullptr;
    };
}
