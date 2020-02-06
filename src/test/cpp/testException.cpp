#include "catch.hpp"

#include "exceptions.hpp"

using namespace cpp_utils;

SCENARIO("test exception") {

    try {
        int a = 5;
        throw cpp_utils::exceptions::InvalidArgumentException{"hello", a};
    } catch (const cpp_utils::exceptions::InvalidArgumentException& e) {
        REQUIRE(e.getMessage() == "hello 5");
        critical(e.what());
        REQUIRE(cpp_utils::startWith(std::string{e.what()}, "hello 5\n"));
    }

}