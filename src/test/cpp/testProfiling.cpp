#include "catch.hpp"
#include "Timer.hpp"
#include "profiling.hpp"
#include "log.hpp"

#include <unistd.h>

using namespace cpp_utils;

SCENARIO("test timer") {

    GIVEN("timing") {
        timing_t t{100, timeunit_e::MICRO};
        REQUIRE((t * 5) == timing_t{500, timeunit_e::MICRO});
    }
    
    GIVEN("a timer") {
        Timer t{};


        WHEN("start and stop") {
            REQUIRE(t.isRunning() == false);
            
            t.start();
            
            REQUIRE(t.isRunning() == true);
            usleep(5000); //microseconds
            
            t.stop();

            REQUIRE(t.isRunning() == false);
            REQUIRE(t.getElapsedMicroSeconds() == timing_t{5, timeunit_e::MICRO});
        }

        WHEN("start and measure") {
            REQUIRE(t.isRunning() == false);

            t.start();
            
            REQUIRE(t.isRunning() == true);
            usleep(5000); //microseconds
            
            REQUIRE(t.getCurrentElapsedMicroSeconds() == timing_t{5, timeunit_e::MICRO});
            t.stop();
            REQUIRE(t.isRunning() == false);
        }
    }

    GIVEN("PROFILE_TIME") {
        timing_t timeGap;
        PROFILE_TIME(timeGap) {
            usleep(5000); //microseconds
        }

        critical("timeGap is", timeGap);
        REQUIRE(((timeGap >= 4900) && (timeGap <= 5100)));
    }
}
