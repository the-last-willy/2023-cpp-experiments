#include <gtest/gtest.h>
#include <iostream>

class Environment : public ::testing::Environment {
public:
    Environment() {
        std::cout << "Environment::Environment()" << std::endl;
    }

    ~Environment() override {
        std::cout << "Environment::~Environment()" << std::endl;
    }

    // Override this to define how to set up the environment.
    void SetUp() override {
        std::cout << "Environment::SetUp()" << std::endl;
    }

    // Override this to define how to tear down the environment.
    void TearDown() override {
        std::cout << "Environment::TearDown()" << std::endl;
    }
};

testing::Environment* const foo_env = testing::AddGlobalTestEnvironment(new Environment);

TEST(FooTest, Foo) {
    std::cout << "FooTest::Foo()" << std::endl;
}
