#include "catch.hpp"


class MyFoo1 {

};

class MyFoo2 {

};

template <typename FROM, typename LAMBDA>
int use(const FROM& f, LAMBDA mapper) {
    return mapper(f);
}

SCENARIO("test lambdas") {

    REQUIRE(use(MyFoo1{}, [&](const MyFoo1&) { return 1; }) == 1);
    REQUIRE(use(MyFoo2{}, [&](const MyFoo2&) { return 2; }) == 2);
}