#include "vector-impl.h"

#include "helpers.hpp"

#include <catch2/catch_all.hpp>
#include <vector>

using Elem = Logging<Int>;

void start_logging_memory()
{
    on_new = [](void* ptr, std::size_t count)
    {
        std::cout << "allocated " << count << " bytes at " << ptr << std::endl;
    };
    on_delete = [](void* ptr, std::size_t count)
    {
        std::cout << "deallocated " << count << " bytes at " << ptr << std::endl;
    };
}

void stop_logging_memory()
{
    on_new = [](void* ptr, std::size_t count)
    {

    };
    on_delete = [](void* ptr, std::size_t count)
    {

    };
}

TEMPLATE_TEST_CASE("requirements", "", std::vector<Elem>, MyVector<Elem>)
{
    SECTION("is default constructible")
    {
        REQUIRE(std::is_default_constructible_v<TestType>);
    }

    SECTION("is nothrow default constructible")
    {
        REQUIRE(std::is_nothrow_default_constructible_v<TestType>);
    }
}

TEMPLATE_TEST_CASE("constructor / default", "", std::vector<Elem>, MyVector<Elem>)
{
    auto vec = TestType();

    SECTION("has size of 0")
    {
        REQUIRE(vec.size() == 0);
    }

    SECTION("has capacity of 0")
    {
        REQUIRE(vec.capacity() == 0);
    }
}

TEST_CASE("constructor / copy", "")
{
    auto theirs = std::vector{1, 2, 3};

    auto ours = MyVector{1, 2, 3};

    REQUIRE(ours == theirs);
}

TEMPLATE_TEST_CASE("Element access / operator[] / at", "", std::vector<Elem>, MyVector<Elem>)
{

}

TEMPLATE_TEST_CASE("Iterators: range-base for loop support", "", std::vector<Elem>, MyVector<Elem>)
{
    auto vec = TestType{Elem(1), Elem(2), Elem(3)};

    auto sum = 0;
    for(auto e : vec)
        sum += e;

    REQUIRE(sum == 6);
}

TEMPLATE_TEST_CASE("Modifiers / push_back", "", std::vector<Elem>)
{
    auto vec = TestType();
    vec.push_back(Elem(1));
}

TEMPLATE_TEST_CASE("Modifiers / emplace_back", "", std::vector<Elem>)
{
    start_logging_memory();

    {
        auto vec = TestType();
        std::cout << "push_back()" << std::endl;
        vec.push_back(Elem(1));
        std::cout << "resize()" << std::endl;
        vec.resize(2);
    }

    stop_logging_memory();
}
