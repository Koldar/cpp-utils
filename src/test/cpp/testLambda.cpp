#include "catch.hpp"


class MyFoo1 {

};

class MyFoo2 {

};

template <typename FROM, typename LAMBDA>
int use(const FROM& f, LAMBDA mapper) {
    return mapper(f);
}


template <typename E>
class Bar {
public:
    virtual int use(E e, std::function<int(const E&)> mapper) {
        return mapper(e);
    }
};


SCENARIO("test lambdas") {

    REQUIRE(use(MyFoo1{}, [&](const MyFoo1&) { return 1; }) == 1);
    REQUIRE(use(MyFoo2{}, [&](const MyFoo2&) { return 2; }) == 2);

    auto b = Bar<char>{};
    REQUIRE(b.use('a', [&](const char& e) {return 3;}));
}