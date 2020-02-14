#include "catch.hpp"

#include "Timer.hpp"

using namespace cpp_utils;

SCENARIO("test timing_t") {

    GIVEN("natural order") {
        REQUIRE(timing_t{30, timeunit_e::NANO}.toHumanReadable() == "30ns");
        REQUIRE(timing_t{30, timeunit_e::MICRO}.toHumanReadable() == "30us");
        REQUIRE(timing_t{30, timeunit_e::MILLI}.toHumanReadable() == "30ms");
        REQUIRE(timing_t{30, timeunit_e::SECOND}.toHumanReadable() == "30s");
        REQUIRE(timing_t{30, timeunit_e::MINUTE}.toHumanReadable() == "30m");
    }
    GIVEN("big numbers") {
        REQUIRE(timing_t{30000000, timeunit_e::MICRO}.toHumanReadable() == "30s");
        REQUIRE(timing_t{31353745.65, timeunit_e::MICRO}.toHumanReadable() == "31.3537s");
    }

}