#include "raw_vector.hpp"

#include <catch2/catch_all.hpp>

using namespace dyn;

const auto null_ctor = stateful_fn<void(void*)>([](void*, void*){}, nullptr);

TEST_CASE("raw_vector::resize")
{
    SECTION("init the elements")
    {
        auto vec = raw_vector(alignof(int), sizeof(int));

        auto ctor_fn = stateful_fn<void(void*)>([](void* state, void* obj) { new(obj) int(1); }, nullptr);

        vec.resize(3, ctor_fn);

        for(int i = 0; i < 3; ++i)
            REQUIRE(*static_cast<int*>(vec[i]) == 1);
    }

    SECTION("clear calls dtor_fn once for every element")
    {
        auto vec = raw_vector(alignof(int), sizeof(int));

        auto count = 0;

        auto dtor_fn = stateful_fn<void(void*)>(
            [](void* count, void* obj) { *static_cast<int*>(count) += 1; },
            &count);

        vec.resize(3, null_ctor);

        vec.clear(dtor_fn);

        REQUIRE(count == 3);
    }
}
