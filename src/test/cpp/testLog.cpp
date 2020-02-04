#include "catch.hpp"

#include "log.hpp"

using namespace cpp_utils;


SCENARIO("test log.h") {

    //normal
    critical("this is a test!", 20);

    int a = 56;
    float b = 3.4;
    char c = 'd';

    auto_critical(a);
    auto_critical(b);
    auto_critical(c);
    
}