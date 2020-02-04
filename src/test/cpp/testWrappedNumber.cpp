#include "catch.hpp"

#include "wrapped_number.hpp"

using namespace cpp_utils;

SCENARIO("test safe number") {

     using safe_int = wrapped_number<int, 1, 10, int>;

     GIVEN("2 numbers") {
         REQUIRE(safe_int{2} + safe_int{3} == safe_int{5});
         REQUIRE(safe_int{2} + safe_int{13} == safe_int{10});
         REQUIRE(safe_int{2} + safe_int{9} == safe_int{10});

         REQUIRE(safe_int{2} - safe_int{1} == safe_int{1});
         REQUIRE(safe_int{2} - safe_int{0} == safe_int{1});
         REQUIRE(safe_int{2} - safe_int{2} == safe_int{1});

         REQUIRE(safe_int{2} * safe_int{3} == safe_int{6});
         REQUIRE(safe_int{2} * safe_int{6} == safe_int{10});
     }
 }
