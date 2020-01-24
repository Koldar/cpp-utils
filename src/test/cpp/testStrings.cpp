#include "catch.hpp"

#include "strings.hpp"
#include "UncopiableClass.hpp"

using namespace cpp_utils;

SCENARIO("test scout") {
    GIVEN("scout on primitives") {
        REQUIRE(scout("the basic is ", 5) == "the basic is 5");

        char c = 'a';
        REQUIRE(scout("additional is ", c) == "additional is a");
    }

    GIVEN("scout on uncopiable class") {
        UncopiableClass foo{5};

        REQUIRE(scout("uncopiable class is ", "the actual ", foo) == "uncopiable class is the actual 5");
    }
}

SCENARIO("test swcout") {
    GIVEN("scout on primitives") {
        REQUIRE(swcout("the basic is", 5) == "the basic is 5");

        char c = 'a';
        REQUIRE(swcout("additional is", c) == "additional is a");
    }

    GIVEN("banal swcout") {

        char c = 'a';
        REQUIRE(swcout() == "");
        REQUIRE(swcout("a") == "a");
        REQUIRE(swcout(c) == "a");

    }

    GIVEN("swcout on uncopiable class") {
        UncopiableClass foo{5};

        REQUIRE(swcout("uncopiable class is", "the actual", foo) == "uncopiable class is the actual 5");
    }
}