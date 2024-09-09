#include "Data.h"

#include <gtest/gtest.h>

#include <memory>

TEST(DataTests, typeShouldReturnTypeOfData) {
    auto c = std::unique_ptr<Data>(new Float(2.));
    EXPECT_EQ(c->type(), Data::Type::floating);
}

TEST(DataTests, asBoooleanShouldReturnBoolean) {
    auto c = std::unique_ptr<Data>(new Bool(true));
    EXPECT_EQ(c->asBoolean(), true);
}

TEST(DataTests, asFloatShouldReturnFloatingPointValue) {
    auto c = std::unique_ptr<Data>(new Float(2.));
    EXPECT_EQ(c->asFloat(), 2.);
}

TEST(DataTests, asFloatShouldThrowIfDataIsNotFloat) {
    auto c = std::unique_ptr<Data>(new Bool(false));
    EXPECT_THROW(c->asFloat(), std::exception);
}

TEST(ListTests, type_should_return_list) {
    auto l = List();
    EXPECT_EQ(l.type(), Data::Type::list);
}

TEST(ListTests, len_should_return_0_for_default_list) {
    auto l = List();
    EXPECT_EQ(l.len(), 0);
}

TEST(ListTests, at_should_return_data) {
    auto l = List();
    l.append(std::unique_ptr<Data>(new Bool(true)));
    auto first = l.at(0);
    EXPECT_EQ(first->asBoolean(), true);
}

TEST(ListTests, len_should_increase_by_1_when_appending) {
    auto l = List();
    l.append(std::unique_ptr<Data>(new Bool(false)));
    EXPECT_EQ(l.len(), 1);
}