#include "catch.hpp"

#include "OrderedCountMap.hpp"

using namespace cpp_utils;
using namespace cpp_utils::datastructures;

SCENARIO("test ordered count map") {

    OrderedCountMap<char> a{};
    
    REQUIRE(a.count('a') == 0);
    REQUIRE(a.count('b') == 0);

    a.increase1('a');
    REQUIRE(a.count('a') == 1);
    REQUIRE(a.count('b') == 0);
    REQUIRE(a.getFirstKey() == 'a');

    a.decrease1('a');
    REQUIRE(a.count('a') == 0);
    REQUIRE(a.count('b') == 0);

    a.increase('b', 5);
    REQUIRE(a.count('a') == 0);
    REQUIRE(a.count('b') == 5);
    REQUIRE(a.getFirstKey() == 'b');

    a.decrease('b', 3);
    REQUIRE(a.count('a') == 0);
    REQUIRE(a.count('b') == 2);

    a.increase1('a');
    REQUIRE(a.count('a') == 1);
    REQUIRE(a.count('b') == 2);
    REQUIRE(a.getFirstKey() == 'a');

    a.decrease('b', 3);
    REQUIRE(a.count('a') == 1);
    REQUIRE(a.count('b') == 0);
}