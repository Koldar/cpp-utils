#include "catch.hpp"
#include "system.hpp"

using namespace cpp_utils;

SCENARIO("test system interface") {

    GIVEN("isProgramInstalled") {

        REQUIRE(isProgramInstalled("echo"));
        REQUIRE_FALSE(isProgramInstalled("echobooo"));
    }

    GIVEN("callPyEval") {

        REQUIRE(callPyEval("100*{V}", false, "V", 50) == "100*50");
        REQUIRE(callPyEvalWithEval("100*{V}", "V", 50) == "5000");
        REQUIRE(callPyEvalWithEval("100*{V}+{E}", "V", 50, "E", 100) == "5100");
        REQUIRE((callPyEvalAndCastNumberTo<int>("100*{V}+{E}", "V", 50, "E", 100) == 5100));
        REQUIRE((callPyEvalAndCastNumberTo<int>("100*{V}+log({E}, 10)", "V", 50, "E", 100) == 5002));
    }
}