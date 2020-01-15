#include "catch.hpp"

#include "MyEnum.hpp"
#include "strings.hpp"

using namespace cpp_utils;

SCENARIO("given an enum") {

    REQUIRE(MyEnum::OK != MyEnum::KO);

    MyEnum a = MyEnum::KO;

    REQUIRE(a == MyEnum::KO);
    REQUIRE(scout(a) == "KO");
    REQUIRE(a.getName() == "KO");

}