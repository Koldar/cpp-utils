#include "catch.hpp"

#include "fractional_number.hpp"
#include "math.hpp"

using namespace cpp_utils;

SCENARIO("test fractional number") {

    REQUIRE((fractional_number<int>{1.0, 1e-3} == fractional_number<int>{1, 1}));
    REQUIRE(cpp_utils::isApproximatelyEqual(fractional_number<int>{1.0, 1e-3}.getRatio(), 1.0, 1e-6));

    REQUIRE((fractional_number<int>{1.1, 1e-3} == fractional_number<int>{11, 10}));
    REQUIRE(cpp_utils::isApproximatelyEqual(fractional_number<int>{1.1, 1e-3}.getRatio(), 1.1, 1e-6));

    REQUIRE((fractional_number<int>{0.9, 1e-3} == fractional_number<int>{9, 10}));
    REQUIRE(cpp_utils::isApproximatelyEqual(fractional_number<int>{0.9, 1e-3}.getRatio(), 0.9, 1e-6));

    REQUIRE((fractional_number<int>{15.15, 1e-3} == fractional_number<int>{1515, 100}));
    REQUIRE(cpp_utils::isApproximatelyEqual(fractional_number<int>{15.15, 1e-3}.getRatio(), 15.15, 1e-6));

    GIVEN("examples that failed in my history") {
        REQUIRE((fractional_number<int>{1.1, 1e-6} == fractional_number<int>{11, 10}));
        REQUIRE(cpp_utils::isApproximatelyEqual(fractional_number<int>{1.1, 1e-6}.getRatio(), 1.1, 1e-6));

        REQUIRE((fractional_number<int>{1.1000000, 1e-6} == fractional_number<int>{11, 10}));
        REQUIRE(cpp_utils::isApproximatelyEqual(fractional_number<int>{1.1, 1e-6}.getRatio(), 1.1, 1e-6));

        REQUIRE((fractional_number<unsigned int>{1.1000000, 1e-6} == fractional_number<unsigned int>{(unsigned int)11, (unsigned int)10}));
        REQUIRE(cpp_utils::isApproximatelyEqual(fractional_number<unsigned int>{1.1, 1e-6}.getRatio(), 1.1, 1e-6));
    }

}