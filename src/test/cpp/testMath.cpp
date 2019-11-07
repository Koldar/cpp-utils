#include "catch.hpp"

#include "math.hpp"

using namespace cpp_utils;

SCENARIO("test math") {

    GIVEN("test argmin") {
        REQUIRE(argmin(1) == 0);
        REQUIRE(argmin(1, 2) == 0);
        REQUIRE(argmin(2, 1) == 1);
        REQUIRE(argmin(2, 1, 5, 3, 6, 4) == 1);
    }

    GIVEN("test argmax") {
        REQUIRE(argmax(1) == 0);
        REQUIRE(argmax(1, 2) == 1);
        REQUIRE(argmax(2, 1) == 0);
        REQUIRE(argmax(2, 1, 5, 3, 6, 4) == 4);
    }

    GIVEN("test ratio conversion") {
        int n;
        int d;

        WHEN("integer number") {
            getRatioOf(13., n, d, 1e-6, 10);
            REQUIRE(n == 13);
            REQUIRE(d == 1);
        }
        WHEN("decimal number") {
            getRatioOf(1.3, n, d, 1e-6, 10);
            REQUIRE(n == 13);
            REQUIRE(d == 10);
        }
        WHEN("decimal number") {
            getRatioOf(1.33, n, d, 1e-6, 10);
            REQUIRE(n == 133);
            REQUIRE(d == 100);
        }
        WHEN("decimal number") {
            getRatioOf(0.3, n, d, 1e-6, 10);
            REQUIRE(n == 3);
            REQUIRE(d == 10);
        }
        WHEN("decimal number") {
            getRatioOf(0.003, n, d, 1e-6, 10);
            REQUIRE(n == 3);
            REQUIRE(d == 1000);
        }
    }

    GIVEN("test ceil power") {
        REQUIRE(pow2GreaterThan(0) == 0);
        REQUIRE(pow2GreaterThan(1) == 1);
        REQUIRE(pow2GreaterThan(2) == 2);
        REQUIRE(pow2GreaterThan(3) == 4);
        REQUIRE(pow2GreaterThan(4) == 4);
        REQUIRE(pow2GreaterThan(5) == 8);
    }

    GIVEN("testnig int number parsing") {
        REQUIRE(parseFromString<int>("0") == 0);
        REQUIRE(parseFromString<int>("12") == 12);
        REQUIRE(parseFromString<int>("-12") == -12);
    }

    GIVEN("testing double parsing ") {
        REQUIRE(isApproximatelyEqual(parseFromString<double>("0"), 0., 1e-3));
        REQUIRE(isApproximatelyEqual(parseFromString<double>("12"), 12., 1e-3));
        REQUIRE(isApproximatelyEqual(parseFromString<double>("-12"), -12., 1e-3));
        REQUIRE(isApproximatelyEqual(parseFromString<double>("12.33"), 12.33, 1e-3));
        REQUIRE(isApproximatelyEqual(parseFromString<double>("-12.33"), -12.33, 1e-3));
    }

    GIVEN("test log integer") {
        REQUIRE(cpp_utils::log2(1) == 0);
        REQUIRE(cpp_utils::log2(2) == 1);
        REQUIRE(cpp_utils::log2(4) == 2);
        REQUIRE(cpp_utils::log2(8) == 3);
        REQUIRE(cpp_utils::log2(64) == 6);

        REQUIRE(cpp_utils::log2(6) == 2);
    }

    GIVEN("test log float") {
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(1.f), 0.f, 0.01f));
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(2.f), 1.f, 0.01f));
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(4.f), 2.f, 0.01f));
    }

    GIVEN("test log double") {
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(1.), 0., 0.01));
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(2.), 1., 0.01));
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(4.), 2., 0.01));
    }

    GIVEN("operations") {
        uint32_t n = 64;
        REQUIRE(cpp_utils::log2(n) == 6);
        REQUIRE((0xFFFFFFFF << (cpp_utils::log2(n)+ 1)) == 0xFFFFFF80);
        REQUIRE((0xFFFFFFFF << (cpp_utils::log2(n) + 1) ^ ~n) == 63);
    }

    GIVEN("test isDecimal") {
        REQUIRE(isDecimal(3) == true);
        REQUIRE(isDecimal(0) == true);
        REQUIRE(isDecimal(-3) == true);

        REQUIRE(isDecimal(3.) == true);
        REQUIRE(isDecimal(0.) == true);
        REQUIRE(isDecimal(-3.) == true);

        REQUIRE(isDecimal(3.5) == false);
        REQUIRE(isDecimal(0.5) == false);
        REQUIRE(isDecimal(-0.5) == false);
        REQUIRE(isDecimal(-3.5) == false);
    }
}