#include "catch.hpp"

#include "vectorplus.hpp"

using namespace cpp_utils;

SCENARIO("test vector plus") {

    GIVEN("testing constructors") {
        cpp_utils::vectorplus<int> a{2};
        cpp_utils::vectorplus<int> b{100, 2};
        cpp_utils::vectorplus<int> c{1,2,3,4,5};

        REQUIRE(a.size() == 20);
        REQUIRE(b.size() == 100);
        REQUIRE(c.size() == 5);
        REQUIRE(c[0] == 1);
        REQUIRE(c[1] == 2);
        REQUIRE(c[2] == 3);
        REQUIRE(c[3] == 4);
        REQUIRE(c[4] == 5);
    }

    GIVEN("a vector plus") {
         vectorplus<int> a{};

         WHEN("basics") {

            REQUIRE(a.isEmpty() == true);
            REQUIRE(a.size() == 0L);

            a.add(5);
            a.add(6);
            a.add(7);

            REQUIRE(a.contains(8) == false);
            REQUIRE(a.contains(5) == true);
            REQUIRE(a.contains(6) == true);
            REQUIRE(a.contains(7) == true);

            REQUIRE(a.size() == 3L);
            REQUIRE(a[0] == 5);
            REQUIRE(a[1] == 6);
            REQUIRE(a[2] == 7);
            REQUIRE(a[-1] == 7);
            REQUIRE(a[-2] == 6);
            REQUIRE(a[-3] == 5);

            a.removeAt(2);

            REQUIRE(a.contains(7) == false);
            REQUIRE(a.size() == 2L);
         }

         WHEN("first and last index on singleton") {
            a.add(5);

            REQUIRE(a.firstIndex() == 0);
            REQUIRE(a.lastIndex() == 0);
            REQUIRE(a.getHead() == 5);
            REQUIRE(a.getTail() == 5);
         }

         WHEN("first and last on size=2") {
            a.add(5);
            a.add(6);

            REQUIRE(a.firstIndex() == 0);
            REQUIRE(a.lastIndex() == 1);
            REQUIRE(a.getHead() == 5);
            REQUIRE(a.getTail() == 6);
         }

         WHEN("reverse") {
             a.add(5);
             a.add(6);
             a.add(7);

             a.reverse();

             REQUIRE(a[0] == 7);
             REQUIRE(a[1] == 6);
             REQUIRE(a[2] == 5);
             REQUIRE(a.size() == 3);
         }

         WHEN("reverse empty vector") {
             a.reverse();

             REQUIRE(a.size() == 0);
         }

         WHEN("reverse singleton vector") {
             a.add(5);

             a.reverse();

             REQUIRE(a[0] == 5);
             REQUIRE(a.size() == 1);
         }

         WHEN("fill") {
             a.add(5);
             a.add(6);
             a.add(7);

             a.fill(10);

             REQUIRE(a.size() == 3);
             REQUIRE(a[0] == 10);
             REQUIRE(a[1] == 10);
             REQUIRE(a[2] == 10);
         }

         WHEN("adding eleemnts") {
             a.add(5);
             REQUIRE(a[0] == 5);
             a.add(6,7,8);
             REQUIRE(a == vectorplus<int>::make(5,6,7,8));
         }
        
        WHEN("sorting empty") {
            a.cleanup();
            a.sort([&](int a, int b) { return a < b;});

            REQUIRE(a.isEmpty());
        }

        WHEN("sorting with size=1") {
            a.add(4);
            a.sort([&](int a, int b) { return a < b;});

            REQUIRE(a == vectorplus<int>::make(4));
        }

        WHEN("sorting with size>1") {
            a.add(4);
            a.add(3);
            a.add(5);

            a.sort([&](int a, int b) { return a < b;});
            REQUIRE(a == vectorplus<int>::make(3,4,5));
        }

        WHEN("functional with empty vector") {
            a.cleanup();

            REQUIRE(a.map<long>([](auto x) { return 2*x; }) == vectorplus<long>{});
            REQUIRE(a.select([](int x) {return x > 6; }).map<int>([](int x) { return 2*x; }) == vectorplus<int>{});
            REQUIRE(a.reject([](int x) {return x > 5; }) == vectorplus<int>{});
            REQUIRE(a.lreduce<std::string>(std::string{""},  [&](int x, std::string tmp) { 
                std::stringstream ss;
                ss << tmp << x;
                return ss.str();
            }) == std::string{""});
            REQUIRE(a.rreduce<std::string>(std::string{""},  [&](int x, std::string tmp) { 
                std::stringstream ss;
                ss << tmp << x;
                return ss.str();
            }) == std::string{""});
        }

        WHEN("functional with size=1") {
            a.add(5);

            REQUIRE(a.map<long>([](int x) { return 2*x; }) == vectorplus<long>::make(10));
            REQUIRE(a.select([](int x) {return x > 6; }).map<int>([](int x) { return 2*x; }) == vectorplus<int>{});
            REQUIRE(a.reject([](int x) {return x > 5; }) == vectorplus<int>::make(5));
            REQUIRE(a.lreduce<std::string>(std::string{""},  [&](int x, std::string tmp) { 
                std::stringstream ss;
                ss << tmp << x;
                return ss.str();
            }) == std::string{"5"});
            REQUIRE(a.rreduce<std::string>(std::string{""},  [&](int x, std::string tmp) { 
                std::stringstream ss;
                ss << tmp << x;
                return ss.str();
            }) == std::string{"5"});
        }

        WHEN("functional with size=2") {
            a.add(5);
            a.add(6);

            REQUIRE(a.map<long>([](int x) { return 2*x; }) == vectorplus<long>::make(10,12));
            REQUIRE(a.select([](int x) {return x > 6; }).map<int>([](int x) { return 2*x; }) == vectorplus<int>{});
            REQUIRE(a.reject([](int x) {return x > 5; }) == vectorplus<int>::make(5));
            REQUIRE(a.lreduce<std::string>(std::string{""},  [&](int x, std::string tmp) { 
                std::stringstream ss;
                ss << tmp << x;
                return ss.str();
            }) == std::string{"56"});
            REQUIRE(a.rreduce<std::string>(std::string{""},  [&](int x, std::string tmp) { 
                std::stringstream ss;
                ss << tmp << x;
                return ss.str();
            }) == std::string{"65"});
        }

        WHEN("functional with size>=2") {
            a.add(5);
            a.add(6);
            a.add(7);
            a.add(8);

            REQUIRE(a.map<long>([](int x) { return 2*x; }) == vectorplus<long>::make(10,12,14,16));
            REQUIRE(a.select([](int x) {return x > 6; }).map<int>([](int x) { return 2*x; }) == vectorplus<int>::make(14,16));
            REQUIRE(a.reject([](int x) {return x > 5; }) == vectorplus<int>::make(5));
            REQUIRE(a.lreduce<std::string>(std::string{""},  [&](int x, std::string tmp) { 
                std::stringstream ss;
                ss << tmp << x;
                return ss.str();
            }) == std::string{"5678"});
            REQUIRE(a.rreduce<std::string>(std::string{""},  [&](int x, std::string tmp) { 
                std::stringstream ss;
                ss << tmp << x;
                return ss.str();
            }) == std::string{"8765"});
        }

        WHEN("testing subvector") {

            a.add(6); // 0
            a.add(6); // 1
            a.add(1); // 2
            a.add(3); // 3
            a.add(5); // 4

            REQUIRE(a.at(0, 3) == vectorplus<int>::make(6, 6, 1));
            REQUIRE(a.at(1,-1) == vectorplus<int>::make(6, 1, 3));
        }
        
     }

     GIVEN("constant vector plus") {
         const vectorplus<int> a{vectorplus<int>::make(5,6,7,8)};

         REQUIRE(a.size() == 4);
         REQUIRE(a[2] == 7);
     }
 }