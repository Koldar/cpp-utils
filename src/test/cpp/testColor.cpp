#include "catch.hpp"

#include "Color.hpp"

using namespace cpp_utils;

SCENARIO("test colors") {

    GIVEN("red") {

        REQUIRE(color_t::RED.getRed() == wrapped_uchar{255});
        REQUIRE(color_t::RED.getGreen() == wrapped_uchar{0});
        REQUIRE(color_t::RED.getBlue() == wrapped_uchar{0});

        REQUIRE(color_t::RED == color_t::RED);
        REQUIRE(color_t::RED != color_t::GREEN);

        REQUIRE(color_t::RED.darker(0.0) == color_t::RED);
        REQUIRE(color_t::RED.darker(0.1) == color_t{229, 0, 0});
        REQUIRE(color_t::RED.darker(0.5) == color_t{127, 0, 0});
        REQUIRE(color_t::RED.darker(0.9) == color_t{25, 0, 0});
        REQUIRE(color_t::RED.darker(1.0) == color_t::BLACK);

        REQUIRE(color_t::RED.lighter(0.0) == color_t::RED);
        REQUIRE(color_t::RED.lighter(0.1) == color_t{255, 25, 25});
        REQUIRE(color_t::RED.lighter(0.5) == color_t{255, 127, 127});
        REQUIRE(color_t::RED.lighter(0.9) == color_t{255, 229, 229});
        REQUIRE(color_t::RED.lighter(1.0) == color_t::WHITE);
    }
}