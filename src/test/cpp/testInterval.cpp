#include "catch.hpp"
#include "Interval.hpp"
#include "math.hpp"

using namespace cpp_utils;

SCENARIO("test Interval") {

    GIVEN("testing methods") {

        Interval<int> a{4, 6, true, false};

        REQUIRE(a.getLB() == 4);
        REQUIRE(a.getUB() == 5);
    }

    GIVEN("parse int interval") {

        WHEN("both exclusive") {
            auto a = Interval<int>::fromMath("(5,16)");

            REQUIRE(a.getLB() == 6);
            REQUIRE(a.getUB() == 15);

            a = Interval<int>::fromMath("]5,16[");

            REQUIRE(a.getLB() == 6);
            REQUIRE(a.getUB() == 15);
        }

        WHEN("lb inclusive") {
            auto a = Interval<int>::fromMath("[5,16)");

            REQUIRE(a.getLB() == 5);
            REQUIRE(a.getUB() == 15);

            a = Interval<int>::fromMath("[5,16[");

            REQUIRE(a.getLB() == 5);
            REQUIRE(a.getUB() == 15);
        }

        WHEN("ub inclusive") {
            auto a = Interval<int>::fromMath("(5,16]");

            REQUIRE(a.getLB() == 6);
            REQUIRE(a.getUB() == 16);

            a = Interval<int>::fromMath("]5,16]");

            REQUIRE(a.getLB() == 6);
            REQUIRE(a.getUB() == 16);
        }

        WHEN("both inclusive") {
            auto a = Interval<int>::fromMath("[5,16]");

            REQUIRE(a.getLB() == 5);
            REQUIRE(a.getUB() == 16);
        }

    }

    GIVEN("parse double interval") {

        WHEN("both exclusive") {
            auto a = Interval<double>::parseDoubleInterval("(5,16)");

            REQUIRE(cpp_utils::isApproximatelyEqual(a.getLB(), 6., 1e-3));
            REQUIRE(cpp_utils::isApproximatelyEqual(a.getUB(), 15., 1e-3));

            a = Interval<double>::parseDoubleInterval("]5,16[");

            REQUIRE(cpp_utils::isApproximatelyEqual(a.getLB(), 6., 1e-3));
            REQUIRE(cpp_utils::isApproximatelyEqual(a.getUB(), 15., 1e-3));
        }

        WHEN("lb inclusive") {
            auto a = Interval<double>::parseDoubleInterval("[5,16)");

            REQUIRE(cpp_utils::isApproximatelyEqual(a.getLB(), 5., 1e-3));
            REQUIRE(cpp_utils::isApproximatelyEqual(a.getUB(), 15., 1e-3));

            a = Interval<double>::parseDoubleInterval("[5,16[");

            REQUIRE(cpp_utils::isApproximatelyEqual(a.getLB(), 5., 1e-3));
            REQUIRE(cpp_utils::isApproximatelyEqual(a.getUB(), 15., 1e-3));
        }

        WHEN("ub inclusive") {
            auto a = Interval<double>::parseDoubleInterval("(5,16]");

            REQUIRE(cpp_utils::isApproximatelyEqual(a.getLB(), 6., 1e-3));
            REQUIRE(cpp_utils::isApproximatelyEqual(a.getUB(), 16., 1e-3));

            a = Interval<double>::parseDoubleInterval("]5,16]");

            REQUIRE(cpp_utils::isApproximatelyEqual(a.getLB(), 6., 1e-3));
            REQUIRE(cpp_utils::isApproximatelyEqual(a.getUB(), 16., 1e-3));
        }

        WHEN("both inclusive") {
            auto a = Interval<double>::parseDoubleInterval("[5,16]");

            REQUIRE(a.getLB() == 5);
            REQUIRE(a.getUB() == 16);
        }

    }

    GIVEN("interval with one value") {
        auto a = Interval<int>::fromMath("[5,5]");

        REQUIRE(a.getLB() == 5);
        REQUIRE(a.getUB() == 5);
    }
    
}