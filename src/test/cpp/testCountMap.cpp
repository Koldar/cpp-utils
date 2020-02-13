#include "catch.hpp"

#include "CountMap.hpp"

using namespace cpp_utils;
using namespace cpp_utils::datastructures;

SCENARIO("test count map") {

    CountMap<char> a{};
    
    REQUIRE(a.count('a') == 0);
    REQUIRE(a.count('b') == 0);
    a.increase1('a');
    REQUIRE(a.count('a') == 1);
    REQUIRE(a.count('b') == 0);
    a.decrease1('a');
    REQUIRE(a.count('a') == 0);
    REQUIRE(a.count('b') == 0);
    a.increase('b', 5);
    REQUIRE(a.count('a') == 0);
    REQUIRE(a.count('b') == 5);
    a.decrease('b', 3);
    REQUIRE(a.count('a') == 0);
    REQUIRE(a.count('b') == 2);
    a.decrease('b', 3);
    REQUIRE(a.count('a') == 0);
    REQUIRE(a.count('b') == 0);
}