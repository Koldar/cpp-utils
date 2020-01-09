#include "catch.hpp"
#include "Random.hpp"

using namespace cpp_utils;

SCENARIO("test random") {

    timespec seed;
    clock_gettime(CLOCK_MONOTONIC , &seed);

    Random rnd{(unsigned int)seed.tv_nsec};

    GIVEN("random") {

        WHEN("calling random statically") {
            bool five= false;
            bool six = false;
            for (int i=0; i<1000; ++i) {
                auto x = Random::next(5, 6, true);
                if (x == 5) {
                    five = true;
                }
                if (x == 6) {
                    six = true;
                }
                if (x != 5 && x != 6) {
                    critical("wrong x was", x);
                    REQUIRE(false);
                }
            }
            REQUIRE(five);
            REQUIRE(six);
        }

        // REQUIRE(rnd.nextInt(5, 6, false) == 5);
        WHEN("calling random as object") {
            bool five= false;
            bool six = false;
            for (int i=0; i<1000; ++i) {
                auto x = rnd.nextInt(5, 6, true);
                if (x == 5) {
                    five = true;
                }
                if (x == 6) {
                    six = true;
                }
                if (x != 5 && x != 6) {
                    critical("wrong x was", x);
                    REQUIRE(false);
                }
            }
            REQUIRE(five);
            REQUIRE(six);
        }

        WHEN("calling random as object with a wider range") {
            for (int i=0; i<1000; ++i) {
                auto x = rnd.nextInt(5, 10, false);
                if (x != 5 && x != 6 && x != 7 && x!= 8 && x != 9) {
                    REQUIRE(false);
                }
            }
        }

        WHEN("testing empty range") {
            REQUIRE(rnd.nextInt(5,6, false) == 5);
            REQUIRE(rnd.nextInt(5,5, true) == 5);
        }

        
    }

    GIVEN("flip") {
        

        int trials = 10000;
        int okNum = 0;
        int koNum = 0;
        int fairDiff = 500;

        WHEN("fair coin") {
            for (int i=0; i<trials; ++i) {
                if (rnd.flip(0.5)) {
                    okNum += 1;
                } else {
                    koNum += 1;
                }
            }

            REQUIRE(Interval<int>{static_cast<int>(trials*0.5 - fairDiff), static_cast<int>(trials*0.5 + fairDiff), true, true}.contains(okNum));
        }

        WHEN("not fair coin") {
            for (int i=0; i<trials; ++i) {
                if (rnd.flip(0.8)) {
                    okNum += 1;
                } else {
                    koNum += 1;
                }
            }

            REQUIRE(Interval<int>{static_cast<int>(trials*0.8 - fairDiff), static_cast<int>(trials*0.8 + fairDiff), true, true}.contains(okNum));
        }
    }
}