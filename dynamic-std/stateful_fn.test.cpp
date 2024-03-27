#include "stateful_fn.hpp"

#include <catch2/catch_all.hpp>

using namespace dyn;

TEST_CASE("stateful_fn operator() calls function with state")
{
    bool called = false;
    auto set_to_true = [](void* b) { *static_cast<bool*>(b) = true; };

    auto fn = stateful_fn<void()>(set_to_true, &called);
    fn();

    REQUIRE(called);
}
