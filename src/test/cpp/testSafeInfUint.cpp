#include "catch.hpp"

#include "safe_inf_uint.hpp"
#include "math.hpp"

using namespace cpp_utils;

SCENARIO("test safe inf uint") {

    GIVEN("casts") {
        REQUIRE(safe_inf_uint{0L} == 0L);
        REQUIRE(safe_inf_uint{10L} == 10L);
        REQUIRE(safe_inf_uint{UINT64_MAX} == UINT64_MAX);

        REQUIRE(isApproximatelyEqual(static_cast<float>(safe_inf_uint{0L}), ((float)0), 0.001f));
        REQUIRE(isApproximatelyEqual(static_cast<float>(safe_inf_uint{10L}), ((float)10), 0.001f));
        REQUIRE(isApproximatelyEqual(static_cast<float>(safe_inf_uint{UINT64_MAX}), ((float)UINT64_MAX), 0.001f));

        REQUIRE(isApproximatelyEqual(static_cast<double>(safe_inf_uint{0L}), ((double)0), 0.001));
        REQUIRE(isApproximatelyEqual(static_cast<double>(safe_inf_uint{10L}), ((double)10), 0.001));
        REQUIRE(isApproximatelyEqual(static_cast<double>(safe_inf_uint{UINT64_MAX}), ((double)UINT64_MAX), 0.001));
    }

    GIVEN("non infinite operations") {
        safe_inf_uint a = 150;
        safe_inf_uint b = 50;

        REQUIRE(((a + b) == safe_inf_uint{200}));
        REQUIRE((a - b) == safe_inf_uint{100});
        REQUIRE((a * b) == safe_inf_uint{7500});
        REQUIRE((a / b) == safe_inf_uint{3});
        REQUIRE(a.isInfinity() == false);
        REQUIRE(b.isInfinity() == false);
    }

    GIVEN("constants") {
        REQUIRE(safe_inf_uint::ZERO == safe_inf_uint{0});
        REQUIRE(safe_inf_uint::MIN == safe_inf_uint{0});
        REQUIRE(safe_inf_uint::MAX == safe_inf_uint{UINT64_MAX - 1});
        REQUIRE(safe_inf_uint::INFTY == safe_inf_uint{UINT64_MAX});

        REQUIRE(safe_inf_uint::ZERO.isInfinity() == false);
        REQUIRE(safe_inf_uint::MIN.isInfinity() == false);
        REQUIRE(safe_inf_uint::MAX.isInfinity() == false);
        REQUIRE(safe_inf_uint::INFTY.isInfinity() == true);
    }

    GIVEN("first is infinite, the other is not") {
        safe_inf_uint a = 100;
        safe_inf_uint b = safe_inf_uint::INFTY;

        REQUIRE((b + a).isInfinity());
        REQUIRE((b - a).isInfinity());
        REQUIRE((b * a).isInfinity());
        REQUIRE((b / a).isInfinity());
        REQUIRE(a.isInfinity() == false);
        REQUIRE(b.isInfinity() == true);
    }

    GIVEN("second is infinite, first is not") {
        safe_inf_uint a = 100;
        safe_inf_uint b = safe_inf_uint::INFTY;

        REQUIRE((a + b).isInfinity());
        REQUIRE_THROWS_AS((a - b).isInfinity(), cpp_utils::exceptions::NumericalOperationException);
        REQUIRE((a * b).isInfinity());
        REQUIRE((a / b) == safe_inf_uint::ZERO);
        REQUIRE(a.isInfinity() == false);
        REQUIRE(b.isInfinity() == true);
    }

    GIVEN("both are infinite") {
        safe_inf_uint a = safe_inf_uint::INFTY;
        safe_inf_uint b = safe_inf_uint::INFTY;

        REQUIRE((b + a).isInfinity());
        REQUIRE_THROWS_AS((b - a).isInfinity(), cpp_utils::exceptions::NumericalOperationException);
        REQUIRE((b * a).isInfinity());
        REQUIRE_THROWS_AS((b / a).isInfinity(), cpp_utils::exceptions::NumericalOperationException);
        REQUIRE(a.isInfinity() == true);
        REQUIRE(b.isInfinity() == true);
    }

    GIVEN("random operations") {
        safe_inf_uint ab = 0;
        safe_inf_uint bc = 100;
        safe_inf_uint ac = 100;

        REQUIRE((ab + bc) >= ac);

        safe_inf_uint a = 100;
        safe_inf_uint b = safe_inf_uint::INFTY;

        REQUIRE(a < b);
    }
}