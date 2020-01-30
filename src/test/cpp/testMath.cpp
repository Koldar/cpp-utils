#include "catch.hpp"

#include "math.hpp"
#include "MCValue.hpp"
#include "MDValue.hpp"

#include "LogNumberListener.hpp"

using namespace cpp_utils;

SCENARIO("test math") {

    GIVEN("test getIntegerDigits") {
        critical("testing getIntegerDigits");
        REQUIRE(getIntegerDigits(0, false) == 0);
        REQUIRE(getIntegerDigits(1, false) == 1);
        REQUIRE(getIntegerDigits(11, false) == 2);
        REQUIRE(getIntegerDigits(0.3, false) == 0);
        REQUIRE(getIntegerDigits(1.3, false) == 1);
        REQUIRE(getIntegerDigits(11.3, false) == 2);
        REQUIRE(getIntegerDigits(-0.3, false) == 0);
        REQUIRE(getIntegerDigits(-1.3, false) == 1);
        REQUIRE(getIntegerDigits(-11.3, false) == 2);

        REQUIRE(getIntegerDigits(0, true) == 1);
        REQUIRE(getIntegerDigits(1, true) == 1);
        REQUIRE(getIntegerDigits(11, true) == 2);
        REQUIRE(getIntegerDigits(0.3, true) == 1);
        REQUIRE(getIntegerDigits(1.3, true) == 1);
        REQUIRE(getIntegerDigits(11.3, true) == 2);
        REQUIRE(getIntegerDigits(-0.3, true) == 1);
        REQUIRE(getIntegerDigits(-1.3, true) == 1);
        REQUIRE(getIntegerDigits(-11.3, true) == 2);
    }

    GIVEN("test pow10") {
        REQUIRE(pow10<double>(0) == 1);
        REQUIRE(pow10<double>(1) == 10);
        REQUIRE(pow10<double>(2) == 100);
        REQUIRE(pow10<double>(3) == 1000);
        REQUIRE(pow10<double>(4) == 10000);

        REQUIRE(pow10<double>(-1) == 0.1);
        REQUIRE(pow10<double>(-2) == 0.01);
        REQUIRE(pow10<double>(-3) == 0.001);
        REQUIRE(pow10<double>(-4) == 0.0001);
    }

    GIVEN("isApproximatelyEqual") {

        REQUIRE(isApproximatelyEqual(5., 5., 0.001) == true);
        REQUIRE(isApproximatelyEqual(5., 6., 0.001) == false);

        REQUIRE(isApproximatelyEqual(5.001, 5.003, 0.0001) == false);

        WHEN("problematic tests") {
            auto_critical(isApproximatelyEqual(0.135335, 0.0, 1e-0));
            REQUIRE(isApproximatelyEqual(0.135335, 0.0, 1e-0) == true);
            REQUIRE(isApproximatelyEqual(0.135335, 0.1, 1e-1) == true);
            REQUIRE(isApproximatelyEqual(0.135335, 0.13, 1e-2));
        }


        REQUIRE(isDefinitelyGreaterThan(5.001, 4.001, 0.002) == true);
        REQUIRE(isDefinitelyGreaterThan(5.002, 5.001, 0.0001) == true);
        REQUIRE(isDefinitelyGreaterThan(5.002, 5.001, 0.002) == false);

        REQUIRE(isDefinitelyLessThan(4.001, 5.001, 0.002) == true);
        REQUIRE(isDefinitelyLessThan(5.001, 5.002, 0.0001) == true);
        REQUIRE(isDefinitelyLessThan(5.001, 5.002, 0.002) == false);
    }

    GIVEN("test ringBound") {
        REQUIRE(ringBound(5, 10) == 5);
        REQUIRE(ringBound(0, 10) == 0);
        REQUIRE(ringBound(10, 10) == 0);
        REQUIRE(ringBound(11, 10) == 1);
        REQUIRE(ringBound(-1, 10) == 9);
        REQUIRE(ringBound(20, 10) == 0);
        REQUIRE(ringBound(-10, 10) == 0);
        REQUIRE(ringBound(-11, 10) == 9);
    }

    GIVEN("test general ringbound") {
        REQUIRE(ringBound(5, 2, 10) == 5);
        REQUIRE(ringBound(2, 2, 10) == 2);
        REQUIRE(ringBound(10, 2, 10) == 2);
        REQUIRE(ringBound(9, 2, 10) == 9);
        REQUIRE(ringBound(1, 2, 10) == 9);
        REQUIRE(ringBound(2, 2, 10) == 2);
    }

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

SCENARIO("test MC and MD values", "[MCMDValues]") {
    GIVEN("a mc value") {
        MCValue<int> a = 4;

        REQUIRE(a == 4);
        a += 4;
        REQUIRE(a == 8);
        a -= -2;
        REQUIRE(a == 10);
        a += 0;
        REQUIRE(a == 10);
        a -= 0;
        REQUIRE(a == 10);

        a = 11;
        REQUIRE(a == 11);
        a = 11;
        REQUIRE(a == 11);

        a.cleanup();
        REQUIRE(a == 0);

        REQUIRE_THROWS(a = -4);
        REQUIRE_THROWS(a += -1);
        REQUIRE_THROWS(a -= 1);

        a = 10;
        REQUIRE(a == 10);
        a *= 5;
        REQUIRE(a == 50);
        a /= 1;
        REQUIRE(a == 50);

        REQUIRE_THROWS(a /= 2);
    }

    GIVEN("a md value") {
        MDValue<int> a = 40;

        REQUIRE(a == 40);
        a += -4;
        REQUIRE(a == 36);
        a -= 2;
        REQUIRE(a == 34);
        a += 0;
        REQUIRE(a == 34);
        a -= 0;
        REQUIRE(a == 34);

        a = 11;
        REQUIRE(a == 11);
        a = 11;
        REQUIRE(a == 11);

        REQUIRE_THROWS(a *= 2);

        a.cleanup();
        REQUIRE(a == 0);

        REQUIRE_THROWS(a = 40);
        REQUIRE_THROWS(a += 1);
        REQUIRE_THROWS(a -= -1);

        a = -10;
        REQUIRE(a == -10);
        a *= 1;
        REQUIRE(a == -10);
        a *= 3;
        REQUIRE(a == -30);

        a.cleanup();
        a /= 2;
        REQUIRE(a == 0);
    }

    GIVEN("a MD value with listener") {
        MDValue<int> upperbound = 40;
        auto upperboundListener = LogNumberListener<int>{"upperbound", 8};
        upperbound.setListener(upperboundListener);

        upperbound = 30;
    }
}

SCENARIO("test linearTransform") {

    GIVEN("computing m") {

        WHEN("first point is origin") {
            REQUIRE(isApproximatelyEqual(getM(0., 0., 1., 1.), 1., 1e-3));
            REQUIRE(isApproximatelyEqual(getM(0., 0., 2., 2.), 1., 1e-3));
            REQUIRE(isApproximatelyEqual(getM(0., 0., 1., 2.), 2., 1e-3));
            REQUIRE(isApproximatelyEqual(getM(0., 0., 1., 3.), 3., 1e-3));
        }

        WHEN("first point is not origin") {
            REQUIRE(isApproximatelyEqual(getM(1., 1., 2., 3.), 2., 1e-3));
        }

        WHEN("problematic") {
            REQUIRE(isApproximatelyEqual(getM(0, 1.0, 482, 0.1), -0.00186, 1e-5));
        }
    }

    GIVEN("computing linearTransform") {

        WHEN("first point is origin") {
            REQUIRE(isApproximatelyEqual(linearTransform(0., 0., 0., 1., 1.), 0., 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(0.3, 0., 0., 1., 1.), 0.3, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(0.6, 0., 0., 1., 1.), 0.6, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(0.9, 0., 0., 1., 1.), 0.9, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(1.0, 0., 0., 1., 1.), 1.0, 1e-3));

            REQUIRE(isApproximatelyEqual(linearTransform(0., 0., 0., 2., 4.), 0.0, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(0.5, 0., 0., 2., 4.), 1.0, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(1.0, 0., 0., 2., 4.), 2.0, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(1.5, 0., 0., 2., 4.), 3.0, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(2.0, 0., 0., 2., 4.), 4.0, 1e-3));
        }

        WHEN("first point is not origin") {
            REQUIRE(isApproximatelyEqual(linearTransform(1.0, 1., 2., 2., 4.), 2.0, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(1.3, 1., 2., 2., 4.), 2.6, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(1.6, 1., 2., 2., 4.), 3.2, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(1.9, 1., 2., 2., 4.), 3.8, 1e-3));
            REQUIRE(isApproximatelyEqual(linearTransform(2.0, 1., 2., 2., 4.), 4.0, 1e-3));
        }
    }
}

SCENARIO("test monotonically crescent") {

    GIVEN("between 0 and 1") {
        critical("getMonotonicallyCrescent(0, 0, 1) = ", getMonotonicallyCrescent(0, 0, 1));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0, 0, 1), 0., 1e-3));
        critical("getMonotonicallyCrescent(1, 0, 1) = ", getMonotonicallyCrescent(1, 0, 1));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(1, 0, 1), 0.5, 1e-3));
        critical("getMonotonicallyCrescent(2, 0, 1) = ", getMonotonicallyCrescent(2, 0, 1));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(2, 0, 1), 0.704, 1e-3));
        critical("getMonotonicallyCrescent(3, 0, 1) = ", getMonotonicallyCrescent(3, 0, 1));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(3, 0, 1), 0.795, 1e-3));
        critical("getMonotonicallyCrescent(4, 0, 1) = ", getMonotonicallyCrescent(4, 0, 1));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(4, 0, 1), 0.844, 1e-3));
    }

    GIVEN("between 1 and 3") {
        critical("getMonotonicallyCrescent(0, 0, 1) = ", getMonotonicallyCrescent(0, 0, 1));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0, 1, 3), 1.0, 1e-3));
        critical("getMonotonicallyCrescent(1, 0, 1) = ", getMonotonicallyCrescent(1, 0, 1));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(1, 1, 3), 1+2*0.5, 1e-3));
        critical("getMonotonicallyCrescent(2, 0, 1) = ", getMonotonicallyCrescent(2, 0, 1));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(2, 1, 3), 1+2*0.704, 1e-3));
        critical("getMonotonicallyCrescent(3, 0, 1) = ", getMonotonicallyCrescent(3, 0, 1));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(3, 1, 3), 1+2*0.795, 1e-3));
        critical("getMonotonicallyCrescent(4, 0, 1) = ", getMonotonicallyCrescent(4, 0, 1));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(4, 1, 3), 1+2*0.844, 1e-3));
    }

    GIVEN("between y=[1,3] and x=[5,10], standard ratio") {
        auto_finest(getMonotonicallyCrescent(5, 1.0, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(5, 1.0, 5, 10, 1, 3), 1.0, 1e-3));
        auto_finest(getMonotonicallyCrescent(6, 1.0, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(6, 1.0, 5, 10, 1, 3), 2.999, 1e-3));
        auto_finest(getMonotonicallyCrescent(7, 1.0, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(7, 1.0, 5, 10, 1, 3), 2.999, 1e-3));
        auto_finest(getMonotonicallyCrescent(8, 1.0, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(8, 1.0, 5, 10, 1, 3), 2.999, 1e-3));
        auto_finest(getMonotonicallyCrescent(9, 1.0, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(9, 1.0, 5, 10, 1, 3), 2.999, 1e-3));
        auto_finest(getMonotonicallyCrescent(10, 1.0, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(10, 1.0, 5, 10, 1, 3), 3.0, 1e-3));
    }

    GIVEN("between y=[1,3] and x=[5,10], smaller ratio") {
        auto_info(getMonotonicallyCrescent(5, 0.1, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(5, 0.1, 5, 10, 1, 3), 1.0, 1e-3));
        auto_finest(getMonotonicallyCrescent(6, 0.1, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(6, 0.1, 5, 10, 1, 3), 2.993, 1e-3));
        auto_finest(getMonotonicallyCrescent(7, 0.1, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(7, 0.1, 5, 10, 1, 3), 2.999, 1e-3));
        auto_finest(getMonotonicallyCrescent(8, 0.1, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(8, 0.1, 5, 10, 1, 3), 2.999, 1e-3));
        auto_finest(getMonotonicallyCrescent(9, 0.1, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(9, 0.1, 5, 10, 1, 3), 2.999, 1e-3));
        auto_finest(getMonotonicallyCrescent(10, 0.1, 5, 10, 1, 3));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(10, 0.1, 5, 10, 1, 3), 3.0, 1e-3));
    }

    GIVEN("problematic with same ratio") {
        auto_critical(getMonotonicallyCrescent(0.1, 0.1, 0.1, 1.0, 0.1, 1.0));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0.1, 0.1, 0.1, 1.0, 0.1, 1.0), 0.1, 1e-5));
        auto_critical(getMonotonicallyCrescent(0.2, 0.1, 0.1, 1.0, 0.1, 1.0));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0.2, 0.1, 0.1, 1.0, 0.1, 1.0), 0.99484, 1e-5));
        auto_critical(getMonotonicallyCrescent(0.3, 0.1, 0.1, 1.0, 0.1, 1.0));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0.3, 0.1, 0.1, 1.0, 0.1, 1.0), 0.99742, 1e-5));
        auto_critical(getMonotonicallyCrescent(0.4, 0.1, 0.1, 1.0, 0.1, 1.0));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0.4, 0.1, 0.1, 1.0, 0.1, 1.0), 0.99828, 1e-5));
        auto_critical(getMonotonicallyCrescent(0.5, 0.1, 0.1, 1.0, 0.1, 1.0));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0.5, 0.1, 0.1, 1.0, 0.1, 1.0), 0.99871, 1e-5));
        auto_critical(getMonotonicallyCrescent(0.6, 0.1, 0.1, 1.0, 0.1, 1.0));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0.6, 0.1, 0.1, 1.0, 0.1, 1.0), 0.99896, 1e-5));
        auto_critical(getMonotonicallyCrescent(0.7, 0.1, 0.1, 1.0, 0.1, 1.0));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0.7, 0.1, 0.1, 1.0, 0.1, 1.0), 0.99914, 1e-5));
        auto_critical(getMonotonicallyCrescent(0.8, 0.1, 0.1, 1.0, 0.1, 1.0));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0.8, 0.1, 0.1, 1.0, 0.1, 1.0), 0.99926, 1e-5));
        auto_critical(getMonotonicallyCrescent(0.9, 0.1, 0.1, 1.0, 0.1, 1.0));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(0.9, 0.1, 0.1, 1.0, 0.1, 1.0), 0.99935, 1e-5));
        auto_critical(getMonotonicallyCrescent(1.0, 0.1, 0.1, 1.0, 0.1, 1.0));
        REQUIRE(isApproximatelyEqual(getMonotonicallyCrescent(1.0, 0.1, 0.1, 1.0, 0.1, 1.0), 0.99942, 1e-5));
    }

}

SCENARIO("test gaussian") {

    REQUIRE(isApproximatelyEqual(pi(), 3.14, 1e-2));

    REQUIRE(isApproximatelyEqual(getGaussian(0, 1), 1., 1e-3));
    critical("getGaussian(-1, 1)=", getGaussian(-1, 1));
    REQUIRE(isApproximatelyEqual(getGaussian(-1, 1), 0.6, 1e-1));
    REQUIRE(isApproximatelyEqual(getGaussian(1, 1), 0.6, 1e-1));
    critical("getGaussian(2, 1)=", getGaussian(2, 1));
    REQUIRE(isApproximatelyEqual(getGaussian(2, 1), 0.13, 1e-2));
    REQUIRE(isApproximatelyEqual(getGaussian(-2, 1), 0.13, 1e-2));
    critical("getGaussian(200, 1)=", getGaussian(200, 1));
    REQUIRE(isApproximatelyEqual(getGaussian(200, 1), 0., 1e-3));
    
    REQUIRE(isApproximatelyEqual(getGaussian(2, 3, 0, 1), 3., 1e-3));
    REQUIRE(isApproximatelyEqual(getGaussian(2, 3, 1, 1), 2.4, 1e-1));
    REQUIRE(isApproximatelyEqual(getGaussian(2, 3, 200, 1), 2., 1e-3));

    critical("getLeftGaussian(5, 0.5, 5, 10, 2, 3)=", getLeftGaussian(5, 0.5, 5, 10, 2, 3));
    REQUIRE(isApproximatelyEqual(getLeftGaussian(5, 0.5, 5, 10, 2, 3), 2., 1e-3));
    critical("getLeftGaussian(10, 0.5, 5, 10, 2, 3)=", getLeftGaussian(10, 0.5, 5, 10, 2, 3));
    REQUIRE(isApproximatelyEqual(getLeftGaussian(10, 0.5, 5, 10, 2, 3), 3., 1e-3));
    critical("getLeftGaussian(6, 0.5, 5, 10, 2, 3)=", getLeftGaussian(6, 0.5, 5, 10, 2, 3));
    REQUIRE(isApproximatelyEqual(getLeftGaussian(6, 0.5, 5, 10, 2, 3), 2., 1e-3));
    critical("getLeftGaussian(7, 0.5, 5, 10, 2, 3)=", getLeftGaussian(7, 0.5, 5, 10, 2, 3));
    REQUIRE(isApproximatelyEqual(getLeftGaussian(7, 0.5, 5, 10, 2, 3), 2., 1e-3));
    critical("getLeftGaussian(8, 0.5, 5, 10, 2, 3)=", getLeftGaussian(8, 0.5, 5, 10, 2, 3));
    REQUIRE(isApproximatelyEqual(getLeftGaussian(8, 0.5, 5, 10, 2, 3), 2., 1e-3));
    critical("getLeftGaussian(9, 0.5, 5, 10, 2, 3)=", getLeftGaussian(9, 0.5, 5, 10, 2, 3));
    REQUIRE(isApproximatelyEqual(getLeftGaussian(9, 0.5, 5, 10, 2, 3), 2., 1e-3));
}