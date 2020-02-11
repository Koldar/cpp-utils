#include "catch.hpp"

#include "optional.hpp"

using namespace cpp_utils::datastructures;

SCENARIO("test optional") {
    optional<int> a;

    REQUIRE(a.isPresent() == false);
    REQUIRE(a.isAbsent() == true);
    REQUIRE(a.getOrElse(5) == 5);

    optional<int> b{10};

    REQUIRE(b.isPresent() == true);
    REQUIRE(b.isAbsent() == false);
    REQUIRE(b.getOrElse(5) == 10);

    optional<int> c = a;

    REQUIRE(c.isPresent() == false);
    REQUIRE(c.isAbsent() == true);
    REQUIRE(c.getOrElse(5) == 5);
    c = b;
    REQUIRE(c.isPresent() == true);
    REQUIRE(c.isAbsent() == false);
    REQUIRE(c.getOrElse(5) == 10);
    b.cleanup();
    REQUIRE(b.isPresent() == false);
    REQUIRE(b.isAbsent() == true);
    REQUIRE(b.getOrElse(5) == 5);
    REQUIRE(c.isPresent() == true);
    REQUIRE(c.isAbsent() == false);
    REQUIRE(c.getOrElse(5) == 10);

    b = 10;

    REQUIRE(b.isPresent() == true);
    REQUIRE(b.isAbsent() == false);
    REQUIRE(b.getOrElse(5) == 10);

}