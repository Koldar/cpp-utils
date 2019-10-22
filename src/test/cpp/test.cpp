#include "catch.hpp"
#include "wrapped_number.hpp"
#include "imemory.hpp"
#include "ks001.hpp"
#include "ppmImage.hpp"
#include "listGraph.hpp"
#include "StaticPriorityQueue.hpp"
#include "vectorplus.hpp"
#include "mapplus.hpp"
#include "safe_inf_uint.hpp"
#include "math.hpp"
#include "strings.hpp"
#include "adjacentGraph.hpp"
#include "pool.hpp"
#include "commons.hpp"
#include "KHeaps.hpp"
#include "Timer.hpp"
#include "operators.hpp"
#include "functional.hpp"
#include "Interval.hpp"
#include <unistd.h>
#include <functional>
#include "CSVWriter.hpp"
#include "Random.hpp"
#include "profiling.hpp"

using namespace cpp_utils;
using namespace cpp_utils::graphs;

class RefAndPtrTest {
public:
    int a;
public:
    RefAndPtrTest(int a): a{a} {

    }
};

class UncopiableClass {
public:
    int a;
public:
    UncopiableClass(int a): a{a} {

    }
    ~UncopiableClass() {

    }
    UncopiableClass(const UncopiableClass& other) = delete;
    UncopiableClass& operator =(const UncopiableClass& other) = delete;
    friend std::ostream& operator <<(std::ostream& out, const UncopiableClass& other) {
        out << other.a;
        return out;
    }
};

class RefClass {
private:
    int& a;  
public:
    RefClass(int& a): a{a} {

    }
    RefClass(const RefClass& o): a{o.a} {

    }
    RefClass& operator =(const RefClass& o) {
        this->a = o.a;
        return *this;
    }
public:
    int getA() const {
        return this->a;
    }
    static RefClass make() {
        int refGoingToDie = 5;
        return RefClass{refGoingToDie};
    }
};

SCENARIO("test open file descriptors") {

    GIVEN("testing pid") {

        REQUIRE(cpp_utils::getCurrentPID() > 0);

        info("open files: ", cpp_utils::getOpenFileDescriptors());
        REQUIRE(cpp_utils::getOpenFileDescriptors().size() >= 6);
    }
}

SCENARIO("test random") {

    timespec seed;
    clock_gettime(CLOCK_MONOTONIC , &seed);

    Random rnd{(unsigned int)seed.tv_nsec};

    GIVEN("random") {
        // REQUIRE(rnd.next(5, 6, false) == 5);

        for (int i=0; i<1000; ++i) {
            auto x = rnd.next(5, 6, true);
            if (x != 5 && x != 6) {
                REQUIRE(false);
            }
        }

        for (int i=0; i<1000; ++i) {
            auto x = rnd.next(5, 10, false);
            if (x != 5 && x != 6 && x != 7 && x!= 8 && x != 9) {
                REQUIRE(false);
            }
        }

        WHEN("testing empty range") {
            REQUIRE(rnd.next(5,6, false) == 5);
            REQUIRE(rnd.next(5,5, true) == 5);
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

SCENARIO("test SetPlus") {

    GIVEN("a set") {

        SetPlus<int> set{};

        set.add(5);
        set.add(7);
        set.add(9);

        WHEN("saving an loading") {
            boost::filesystem::path p{"./saveSet.dat"};
            FILE* f = fopen(p.native().c_str(), "wb");
            serializers::saveToFile(f, set);
            fclose(f);

            f = nullptr;

            f = fopen(p.native().c_str(), "rb");
            SetPlus<int> set2{};
            serializers::loadFromFile(f, set2);
            fclose(f);

            REQUIRE(set2 == set);
        }
    }
}

SCENARIO("test csv") {
    vectorplus<std::string> header{};
    header.add("int_field");
    header.add("bool_field");
    boost::filesystem::path p{"./test.csv"};
    CSVWriter<int, bool> writer{p, ',', header};
    writer.writeRow(5, true);
    writer.writeRow(6, false);
}

SCENARIO("test system interface") {

    GIVEN("isProgramInstalled") {

        REQUIRE(isProgramInstalled("echo"));
        REQUIRE_FALSE(isProgramInstalled("echobooo"));
    }

    GIVEN("callPyEval") {

        REQUIRE(callPyEval("100*{V}", false, "V", 50) == "100*50");
        REQUIRE(callPyEvalWithEval("100*{V}", "V", 50) == "5000");
        REQUIRE(callPyEvalWithEval("100*{V}+{E}", "V", 50, "E", 100) == "5100");
        REQUIRE((callPyEvalAndCastNumberTo<int>("100*{V}+{E}", "V", 50, "E", 100) == 5100));
        REQUIRE((callPyEvalAndCastNumberTo<int>("100*{V}+log({E}, 10)", "V", 50, "E", 100) == 5002));
    }
}

SCENARIO("test Interval") {

    GIVEN("testing methods") {

        Interval<int> a{4, 6, true, false};

        REQUIRE(a.getLB() == 4);
        REQUIRE(a.getUB() == 5);
    }

    GIVEN("parse interval") {

        WHEN("both exclusive") {
            auto a = Interval<int>::fromMath("(5,16)");

            REQUIRE(a.getLB() == 6);
            REQUIRE(a.getUB() == 15);

            a = Interval<int>::fromMath("]5,16[");

            REQUIRE(a.getLB() == 6);
            REQUIRE(a.getUB() == 15);
        }

        WHEN("lb inclusive") {
            auto a = Interval<int>::fromMath("[5,16)");

            REQUIRE(a.getLB() == 5);
            REQUIRE(a.getUB() == 15);

            a = Interval<int>::fromMath("[5,16[");

            REQUIRE(a.getLB() == 5);
            REQUIRE(a.getUB() == 15);
        }

        WHEN("ub inclusive") {
            auto a = Interval<int>::fromMath("(5,16]");

            REQUIRE(a.getLB() == 6);
            REQUIRE(a.getUB() == 16);

            a = Interval<int>::fromMath("]5,16]");

            REQUIRE(a.getLB() == 6);
            REQUIRE(a.getUB() == 16);
        }

        WHEN("both inclusive") {
            auto a = Interval<int>::fromMath("[5,16]");

            REQUIRE(a.getLB() == 5);
            REQUIRE(a.getUB() == 16);
        }

    }

    GIVEN("interval with one value") {
        auto a = Interval<int>::fromMath("[5,5]");

        REQUIRE(a.getLB() == 5);
        REQUIRE(a.getUB() == 5);
    }
    
}

SCENARIO("functional test") {

    identity<int> x{};
    identity<int*> y;

    REQUIRE(identity<int>{}(5) == 5);

    REQUIRE(x(5) == 5);
    REQUIRE(identity<int>::value(5) == 5);
    REQUIRE(y(nullptr) == nullptr);
    REQUIRE(identity<int*>::value(nullptr) == nullptr);
}


SCENARIO("test reference dying lead to UB", "[cpp-tests]") {
    int uselessRef = 6;
    RefClass refClassMain{uselessRef};

    REQUIRE(refClassMain.getA() == 6);
    refClassMain = RefClass::make();
    //refGoingToDie went out of scope

    //REQUIRE((refClassMain.getA() == 5));
}

SCENARIO("test operators") {
    std::tuple<int, char, long> t{5, 'a', 57};

    std::stringstream ss;
    ss << t;
    REQUIRE(ss.str() == std::string{"<5, a, 57>"});
}

SCENARIO("test hash") {

    GIVEN("hashing pair") {
        WHEN("simple hashing") {
            std::pair<int, char> p{5, 'a'};
            //hash value
            REQUIRE(std::hash<std::pair<int, char>>{}(p) == 5473661158804778096);
        }

        WHEN("using hash inside container") {
            std::unordered_set<std::pair<int, char>> a{};

            a.insert(std::pair<int, char>{5, 'a'});
            REQUIRE(a.size() == 1);
        }
    }
}

SCENARIO("test scout") {
    GIVEN("scout on primitives") {
        REQUIRE(scout("the basic is ", 5) == "the basic is 5");

        char c = 'a';
        REQUIRE(scout("additional is ", c) == "additional is a");
    }

    GIVEN("scout on uncopiable class") {
        UncopiableClass foo{5};

        REQUIRE(scout("uncopiable class is ", "the actual ", foo) == "uncopiable class is the actual 5");
    }
}

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


        REQUIRE(timeGap >= 4950);
    }
}

SCENARIO("test ref and ptr") {

    GIVEN("feeding value") {
        RefAndPtrTest a{10};
        const RefAndPtrTest constA{20};

        REQUIRE(cpp_utils::Types<RefAndPtrTest>::ref(a).a == 10);
        REQUIRE(cpp_utils::Types<RefAndPtrTest>::ref(constA).a == 20);
        REQUIRE(cpp_utils::Types<RefAndPtrTest>::ptr(a)->a == 10);
        REQUIRE(cpp_utils::Types<RefAndPtrTest>::ptr(constA)->a == 20);
    }

    GIVEN("feeding pointer") {
        RefAndPtrTest* a = new RefAndPtrTest{10};
        const RefAndPtrTest* constA = new RefAndPtrTest{20};

        REQUIRE(cpp_utils::Types<RefAndPtrTest*>::ref(a).a == 10);
        REQUIRE(cpp_utils::Types<RefAndPtrTest*>::ref(constA).a == 20);
        REQUIRE(cpp_utils::Types<RefAndPtrTest*>::ptr(a)->a == 10);
        REQUIRE(cpp_utils::Types<RefAndPtrTest*>::ptr(constA)->a == 20);

        delete a;
        delete constA;
    }

}

SCENARIO("test elvis") {
    int a = 5;
    int* b = nullptr;
    int c = 0;

    float d = 7;

    REQUIRE(*elvis(&a, &c) == 5);
    REQUIRE(*elvis(b, &c) == 0);

    REQUIRE(elvis(&a, &d) == static_cast<void*>(&a));
    REQUIRE(elvis(b, &d) == static_cast<void*>(&d));
}

SCENARIO("test cpool") {
    cpool<int> pool{20};

    int* a = new (pool.allocate()) int{5};

    REQUIRE(*a == 5);

    pool.reclaim();
}

SCENARIO("test ceil power") {
    REQUIRE(pow2GreaterThan(0) == 0);
    REQUIRE(pow2GreaterThan(1) == 1);
    REQUIRE(pow2GreaterThan(2) == 2);
    REQUIRE(pow2GreaterThan(3) == 4);
    REQUIRE(pow2GreaterThan(4) == 4);
    REQUIRE(pow2GreaterThan(5) == 8);
}

SCENARIO("test number parsing") {

    GIVEN("testnig int") {
        REQUIRE(parseFromString<int>("0") == 0);
        REQUIRE(parseFromString<int>("12") == 12);
        REQUIRE(parseFromString<int>("-12") == -12);
    }

    GIVEN("testing double") {
        REQUIRE(isApproximatelyEqual(parseFromString<double>("0"), 0., 1e-3));
        REQUIRE(isApproximatelyEqual(parseFromString<double>("12"), 12., 1e-3));
        REQUIRE(isApproximatelyEqual(parseFromString<double>("-12"), -12., 1e-3));
        REQUIRE(isApproximatelyEqual(parseFromString<double>("12.33"), 12.33, 1e-3));
        REQUIRE(isApproximatelyEqual(parseFromString<double>("-12.33"), -12.33, 1e-3));
    }

}

SCENARIO("test log") {
    GIVEN("integer") {
        REQUIRE(cpp_utils::log2(1) == 0);
        REQUIRE(cpp_utils::log2(2) == 1);
        REQUIRE(cpp_utils::log2(4) == 2);
        REQUIRE(cpp_utils::log2(8) == 3);
        REQUIRE(cpp_utils::log2(64) == 6);

        REQUIRE(cpp_utils::log2(6) == 2);
    }

    GIVEN("float") {
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(1.f), 0.f, 0.01f));
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(2.f), 1.f, 0.01f));
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(4.f), 2.f, 0.01f));
    }

    GIVEN("double") {
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(1.), 0., 0.01));
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(2.), 1., 0.01));
        REQUIRE(isApproximatelyEqual(cpp_utils::log2(4.), 2., 0.01));
    }

    GIVEN("operations") {
        uint32_t n = 64;
        REQUIRE(cpp_utils::log2(n) == 6);
        REQUIRE((0xFFFFFFFF << (cpp_utils::log2(n)+ 1)) == 0xFFFFFF80);
        REQUIRE((0xFFFFFFFF << (cpp_utils::log2(n) + 1) ^ ~n) == 63);
    }
}

SCENARIO("test var holders") {
    GIVEN("a var holder on bool") {
        VarHolder<bool> a;

        a = true;
        REQUIRE(a == true);

        a = false;
        REQUIRE(a == false);
    }

    GIVEN("a var holder on int") {
        VarHolder<int> a;

        a = 6;
        REQUIRE(a == 6);

        a += 7;
        REQUIRE(a == 13);
    }
}

SCENARIO("test strings") {
    REQUIRE(join("-", "a", "b", "c") == std::string{"a-b-c"});
    REQUIRE(join("-", "a", 2, "c") == std::string{"a-2-c"});
    REQUIRE(join(3, "a", 2, "c") == std::string{"a323c"});
    REQUIRE(cpp_utils::join("-", "a") == std::string{"a"});
    REQUIRE(cpp_utils::join("-") == std::string{});
    REQUIRE(cpp_utils::join(3) == std::string{});
}

SCENARIO("test float operations") {

    GIVEN("double") {

        REQUIRE(isApproximatelyEqual(5., 5., 0.001) == true);
        REQUIRE(isApproximatelyEqual(5., 6., 0.001) == false);

        REQUIRE(isApproximatelyEqual(5.001, 5.002, 0.002) == true);
        REQUIRE(isApproximatelyEqual(5.001, 5.003, 0.0001) == false);

        REQUIRE(isDefinitelyGreaterThan(5.001, 4.001, 0.002) == true);
        REQUIRE(isDefinitelyGreaterThan(5.002, 5.001, 0.0001) == true);
        REQUIRE(isDefinitelyGreaterThan(5.002, 5.001, 0.002) == false);

        REQUIRE(isDefinitelyLessThan(4.001, 5.001, 0.002) == true);
        REQUIRE(isDefinitelyLessThan(5.001, 5.002, 0.0001) == true);
        REQUIRE(isDefinitelyLessThan(5.001, 5.002, 0.002) == false);
    }

}

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

            REQUIRE(a.map<long>([](int x) { return 2*x; }) == vectorplus<long>{});
            REQUIRE(a.filter([](int x) {return x > 6; }).map<int>([](int x) { return 2*x; }) == vectorplus<int>{});
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
            REQUIRE(a.filter([](int x) {return x > 6; }).map<int>([](int x) { return 2*x; }) == vectorplus<int>{});
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
            REQUIRE(a.filter([](int x) {return x > 6; }).map<int>([](int x) { return 2*x; }) == vectorplus<int>{});
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
            REQUIRE(a.filter([](int x) {return x > 6; }).map<int>([](int x) { return 2*x; }) == vectorplus<int>::make(14,16));
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
        
     }

     GIVEN("constant vector plus") {
         const vectorplus<int> a{vectorplus<int>::make(5,6,7,8)};

         REQUIRE(a.size() == 4);
         REQUIRE(a[2] == 7);
     }
 }

SCENARIO("test MapPlus") {

    GIVEN("an empty map") {
        MapPlus<char, int> a;

        a['a'] = 6;
        a['b'] = 3;
        a['c'] = 2;

        REQUIRE(a.size() == 3);
        REQUIRE(a.containsKey('a') == true);
        REQUIRE(a.containsKey('d') == false);

        REQUIRE(a.containsValue(3) == true);
        REQUIRE(a.containsValue(7) == false);
    }
}

SCENARIO("test edges") {

    GIVEN("an edge") {
        Edge<bool> edge{0, 5, true};

        WHEN("save edge") {
            boost::filesystem::path p{"./saveEdge.dat"};
            FILE* f = fopen(p.native().c_str(), "wb");

            cpp_utils::serializers::saveToFile(f, edge);
            fclose(f);

            f = nullptr;

            Edge<bool> edge2;
            f = fopen(p.native().c_str(), "rb");
            cpp_utils::serializers::loadFromFile(f, edge2);
            fclose(f);

            REQUIRE(edge == edge2);
        }
    }
    

}

SCENARIO("test adjacent graph") {
    GIVEN("an empty graph") {
        ListGraph<int, int, int> lg{20};
        AdjacentGraph<int, int, int> ag{lg};

        REQUIRE(ag.size() == 0);
        REQUIRE(ag.isEmpty() == true);
        REQUIRE(ag.getPayload() == 20);

        ag.saveBMP("empty_adjacentGraph");
    }

    GIVEN("non empty graph") {
        ListGraph<int, int, bool> lg{20};
        auto n0 = lg.addVertex(1);
        auto n1 = lg.addVertex(2);
        auto n2 = lg.addVertex(3);
        auto n3 = lg.addVertex(4);
        auto n4 = lg.addVertex(5);
        lg.addEdge(n0, n1, true);
        lg.addEdge(n0, n2, true);
        lg.addEdge(n2, n3, true);
        lg.addEdge(n2, n0, true);
        lg.addEdge(n3, n4, true);

        REQUIRE(lg.containsEdge(n0, n2));
        REQUIRE(lg.containsEdge(n2, n0));

        AdjacentGraph<int, int, bool> ag{lg};

        // ag.saveBMP("adj_nonempty");
        // lg.saveBMP("list_nonempty");
        REQUIRE(lg == ag);
        REQUIRE(ag.getPayload() == 20);
        REQUIRE(ag.containsEdge(n0, n2));
        REQUIRE(ag.containsEdge(n2, n0));

        WHEN("testing iterator") {
            int i= 0;
            for (auto it=ag.beginEdges(); it!=ag.endEdges(); ++it) {
                i += 1;
            }
            REQUIRE(i == 5);
        }

        WHEN("test getEdgeSet") {
            auto set = ag.getEdgeSet(false);

            critical("set is", set);
            REQUIRE(set.contains(Edge<bool>{n0, n1, true}));
            REQUIRE_FALSE(set.contains(Edge<bool>{n0, n1, false}));
            REQUIRE_FALSE(set.contains(Edge<bool>{n0, n4, false}));
            REQUIRE(set.contains(Edge<bool>{n0, n2, true}));
            REQUIRE(set.contains(Edge<bool>{n2, n0, true}));
            REQUIRE(set.size() == 5);

            set = ag.getEdgeSet(true);

            REQUIRE(set.contains(Edge<bool>{n0, n1, true}));
            REQUIRE_FALSE(set.contains(Edge<bool>{n0, n1, false}));
            REQUIRE_FALSE(set.contains(Edge<bool>{n0, n4, false}));
            REQUIRE(set.contains(Edge<bool>{n0, n2, true}) != set.contains(Edge<bool>{n2, n0, true})); //XOR
            REQUIRE(set.size() == 4);
        }

        WHEN("saving and loading") {
            boost::filesystem::path p{"./save.dat"};
            FILE* f = fopen(p.native().c_str(), "wb");

            //save
            cpp_utils::serializers::saveToFile(f, ag);
            fclose(f);
            REQUIRE(boost::filesystem::exists(p));

            //load
            AdjacentGraph<int, int, bool> ag2;
            f = fopen(p.native().c_str(), "rb");
            cpp_utils::serializers::loadFromFile(f, ag2);
            fclose(f);

            ag.saveBMP("ag1");
            ag2.saveBMP("ag2");

            REQUIRE(ag == ag2);
            REQUIRE(ag.numberOfVertices() == ag2.numberOfVertices());
            
        }

        WHEN("testing indexOf") {
            REQUIRE(ag.idOfVertex(1) == n0);
            REQUIRE(ag.idOfVertex(3) == n2);
            REQUIRE(ag.idOfVertex(5) == n4);
            REQUIRE_THROWS(ag.idOfVertex(7));
        }

        WHEN("testing constructor") {
            //reference
            AdjacentGraph<int, int, bool> ag1{ag};
            //rvalue
            AdjacentGraph<int, int, bool> ag2{AdjacentGraph<int, int, bool>{ag}};
        }

        WHEN("testing the graph") {
            REQUIRE(ag.getEdge(n0, n1) == true);
            REQUIRE(ag.getEdge(n0, n2) == true);
            REQUIRE(ag.hasEdge(n0, n3) == false);
            REQUIRE(ag.hasEdge(n0, n4) == false);
            REQUIRE(ag.getEdge(n2, n3) == true);
            REQUIRE(ag.getEdge(n2, n0) == true);

            REQUIRE(ag.containsEdge(n4, n3) == false);
        }

        WHEN("testing saving image") {
            ag.saveBMP("adjacentGraph");
        }

        WHEN("testing getters") {
            REQUIRE(ag.size() == 5);
            REQUIRE_FALSE(ag.isEmpty());

            REQUIRE(ag.getOutDegree(n0) == 2);
            REQUIRE(ag.getOutDegree(n4) == 0);
            REQUIRE(ag.getOutDegree(n2) == 2);
            REQUIRE(ag.getOutDegree(n3) == 1);
            
            REQUIRE(ag.hasSuccessors(n0));
            REQUIRE(ag.hasSuccessors(n2));
            REQUIRE(ag.hasSuccessors(n4) == false);

            REQUIRE(ag.hasPredecessors(n0));
            REQUIRE(ag.hasPredecessors(n2));
            REQUIRE(ag.hasPredecessors(n4));

            REQUIRE(ag.numberOfVertices() == 5);
            REQUIRE(ag.numberOfEdges() == 5);

            REQUIRE(ag.getInDegree(n0) == 1);
            REQUIRE(ag.getInDegree(n2) == 1);
            REQUIRE(ag.getInDegree(n4) == 1);

            REQUIRE(ag.getInEdges(n4) == std::vector<InEdge<bool>>{InEdge<bool>{n3, true}});
            REQUIRE(ag.getInEdges(n0) == std::vector<InEdge<bool>>{InEdge<bool>{n2, true}});

            REQUIRE(ag.getOutEdges(n0) == std::vector<OutEdge<bool>>{OutEdge<bool>{n1, true}, OutEdge<bool>{n2, true}});
            REQUIRE(ag.getOutEdges(n2) == std::vector<OutEdge<bool>>{OutEdge<bool>{n3, true}, OutEdge<bool>{n0, true}});
            REQUIRE(ag.getOutEdges(n4) == std::vector<OutEdge<bool>>{});
            REQUIRE(ag.getOutEdges(n3) == std::vector<OutEdge<bool>>{OutEdge<bool>{n4, true}});
        }

        WHEN("testing change edges in single way") {
            //no change
            ag.changeWeightEdge(n2, n3, true);
            REQUIRE(ag.getEdge(n2, n3) == true);

            //change
            ag.changeWeightEdge(n2, n3, false);
            REQUIRE(ag.getEdge(n2, n3) == false);
        }

        WHEN("testing change edges in both way") {
            ag.saveBMP("adjacentGraph_bothways");
            //change undirect
            debug("test frst");
            REQUIRE(ag.containsEdge(n0, n2) == true);
            REQUIRE(ag.containsEdge(n2, n0) == true);
            ag.changeWeightUndirectedEdge(n0, n2, true);
            debug("checkin");
            REQUIRE(ag.getEdge(n0, n2) == true);
            debug("checkin");
            REQUIRE(ag.getEdge(n2, n0) == true);
            debug("test second");
            ag.changeWeightUndirectedEdge(n0, n2, false);
            REQUIRE(ag.getEdge(n0, n2) == false);
            REQUIRE(ag.getEdge(n2, n0) == false);

            //change undirect but in opposite direction
            ag.changeWeightUndirectedEdge(n2, n0, true);
            REQUIRE(ag.getEdge(n0, n2) == true);
            REQUIRE(ag.getEdge(n2, n0) == true);
        }

        WHEN("testing mapper") {
            AdjacentGraph<int, int, int> ag3{*ag.mapEdges<int>([&](bool b) { return b? 10: 5;})};

            REQUIRE(ag3.getPayload() == ag.getPayload());
            REQUIRE(ag3.numberOfVertices() == ag.numberOfVertices());
            REQUIRE(ag3.numberOfEdges() == ag.numberOfEdges());

            REQUIRE(ag3.getEdge(n0, n1) == 10);
            REQUIRE(ag3.getEdge(n0, n2) == 10);
            REQUIRE(ag3.getEdge(n2, n3) == 10);
            REQUIRE(ag3.getEdge(n2, n0) == 10);

            REQUIRE(ag.containsEdge(n4, n3) == false);
        }
    }
}

SCENARIO("test graphs") {
    GIVEN("an empty graph") {
        ListGraph<int, int, bool> g{20};

        REQUIRE(g.getPayload() == 20);
        REQUIRE(g.size() == 0);
        REQUIRE(g.isEmpty());

        g.saveBMP("emptygraph");
    }
    GIVEN("a graph") {
        ListGraph<int, int, bool> g{20};
        auto n0 = g.addVertex(1);
        auto n1 = g.addVertex(2);
        auto n2 = g.addVertex(3);
        auto n3 = g.addVertex(4);
        auto n4 = g.addVertex(5);
        g.addEdge(n0, n1, true);
        g.addEdge(n0, n2, true);
        g.addEdge(n2, n3, true);
        g.addEdge(n3, n4, true);

        WHEN("iterate over graph edges") {
            int i= 0;
            for (auto it=g.beginEdges(); it!=g.endEdges(); ++it) {
                i += 1;
            }
            REQUIRE(i == 4);
        }

        WHEN("copy listgraph") {
            ListGraph<int, int, bool> g2{g};

            REQUIRE(g == g2);
            REQUIRE(g.numberOfVertices() == g2.numberOfVertices());
            REQUIRE(g.numberOfEdges() == g2.numberOfEdges());
        }

        WHEN("testing density") {
            REQUIRE(g.getMaximumNumberOfEdges() == 20);
            info("density", g.getDensity());
            REQUIRE(cpp_utils::isApproximatelyEqual(g.getDensity(), 0.2, 1e-3));
            REQUIRE(g.getNumberOfEdgesFromDensity(0.2) == 4);
        }

        WHEN("testing indexOf") {
            REQUIRE(g.idOfVertex(1) == n0);
            REQUIRE(g.idOfVertex(3) == n2);
            REQUIRE(g.idOfVertex(5) == n4);
            REQUIRE_THROWS(g.idOfVertex(7));
        }

        WHEN("testing the graph") {
            REQUIRE(g.getEdge(n0, n1) == true);
            REQUIRE(g.getEdge(n0, n2) == true);
            REQUIRE(g.hasEdge(n0, n3) == false);
            REQUIRE(g.hasEdge(n0, n4) == false);
            REQUIRE(g.getEdge(n2, n3) == true);
            REQUIRE(g.getEdge(n3, n4) == true);
        }

        WHEN("testing saving image") {
            g.saveBMP("listGraph");
        }

        WHEN("testing getters") {
            REQUIRE(g.size() == 5);
            REQUIRE_FALSE(g.isEmpty());
            
            REQUIRE(g.hasSuccessors(n0));
            REQUIRE(g.hasSuccessors(n2));
            REQUIRE_FALSE(g.hasSuccessors(n4));

            REQUIRE_FALSE(g.hasPredecessors(n0));
            REQUIRE(g.hasPredecessors(n2));
            REQUIRE(g.hasPredecessors(n4));

            REQUIRE(g.numberOfVertices() == 5);
            REQUIRE(g.numberOfEdges() == 4);

            REQUIRE(g.getInDegree(n0) == 0);
            REQUIRE(g.getInDegree(n2) == 1);
            REQUIRE(g.getInDegree(n4) == 1);

            REQUIRE(g.getOutDegree(n0) == 2);
            REQUIRE(g.getOutDegree(n4) == 0);
            REQUIRE(g.getOutDegree(n2) == 1);

            REQUIRE(g.getInEdges(n4) == std::vector<InEdge<bool>>{InEdge<bool>{n3, true}});
            REQUIRE(g.getInEdges(n0) == std::vector<InEdge<bool>>{});

            REQUIRE(g.getOutEdges(n0) == std::vector<OutEdge<bool>>{OutEdge<bool>{n1, true}, OutEdge<bool>{n2, true}});
            REQUIRE(g.getOutEdges(n2) == std::vector<OutEdge<bool>>{OutEdge<bool>{n3, true}});
            REQUIRE(g.getOutEdges(n4) == std::vector<OutEdge<bool>>{});
        }

        WHEN("testing reordering") {

            std::vector<nodeid_t> fromOldToNew{1,2,3,4,0};
            std::vector<nodeid_t> fromNewToOld{4,0,1,2,3};
            auto g2 = g.reorderVertices(
                fromOldToNew,
                fromNewToOld
            );
            
            g.saveBMP("listGraph_ordering_old");
            g2->saveBMP("listGraph_ordering_new");

            REQUIRE(g2->numberOfVertices() == g.numberOfVertices());
            REQUIRE(g2->numberOfEdges() == g.numberOfEdges());
            REQUIRE(g.getVertex(0) == g2->getVertex(1));
            REQUIRE(g.getVertex(1) == g2->getVertex(2));
            REQUIRE(g.getVertex(2) == g2->getVertex(3));
            REQUIRE(g.getVertex(3) == g2->getVertex(4));
            REQUIRE(g.getVertex(4) == g2->getVertex(0));

            REQUIRE(g.getEdge(0, 1) == g2->getEdge(1,2));
            REQUIRE(g.getEdge(0, 2) == g2->getEdge(1,3));
            REQUIRE(g.getEdge(2, 3) == g2->getEdge(3,4));
            REQUIRE(g.getEdge(3, 4) == g2->getEdge(4,0));
        }
        
    }
}

class Foo: public HasPriority {
    public:
        friend std::ostream& operator <<(std::ostream& ss, const Foo& f) {
            ss << f.content << "(" << f.value << ", priority=" << f.p << ")";
            return ss;
        }
        friend bool operator < (const Foo& a, const Foo& b) {
            return a.value < b.value;
        }
        friend bool operator ==(const Foo& a, const Foo& b) {
            if (&a == &b) {
                info("checking if foo", a, " is equal to foo ", b, " => YES");
                return true;
            }
            info("checking if foo", a, " is equal to foo ", b, " => ", a.content == b.content && a.value == b.value);
            return a.content == b.content && a.value == b.value;
        }
    private:
        std::string content;
        int value;
        priority_t p;
    public:
        Foo(): content{""}, value{0}, p{0} {

        }
        Foo(std::string content, int value) : content{content}, value{value}, p{0} {

        }
        virtual ~Foo() {

        }
        Foo& operator =(const Foo& other) {
            this->content = other.content;
            this->value = other.value;
            this->p = other.p;
            return *this;
        }
        priority_t getPriority(const void* q) const {
            return this->p;
        }
        void setPriority(const void* q, priority_t p) {
            this->p = p;
        }
    };

SCENARIO("test queue") {

    GIVEN("a queue of objects of pointers") {
        StaticPriorityQueue<Foo> q{1000, true};

        WHEN("empty") {
            REQUIRE(q.isEmpty());
            REQUIRE(q.size() == 0);
        }

        WHEN("non empty") {
            Foo o1 = Foo{std::string{"a"}, 5};
            Foo o2 = Foo{std::string{"b"}, 10};
            Foo o3 = Foo{std::string{"c"}, 1};
            Foo o4 = Foo{std::string{"d"}, 11};

            q.push(o1);
            q.push(o2);
            q.push(o3);
            q.push(o4);

            REQUIRE(!q.isEmpty());
            REQUIRE(q.size() == 4);

            info("queue is", q);
            REQUIRE(q.peek() == o3);
            REQUIRE(q.pop() == o3);
            info("queue minus o3", q);
            REQUIRE(q.pop() == o1);
            REQUIRE(q.pop() == o2);

            info("queue before readding o2 ", q);
            q.push(o2);
            info("queue after readding o2 ", q);
            REQUIRE(!q.contains(o1));
            REQUIRE(q.contains(o2));
            REQUIRE(!q.contains(o3));
            REQUIRE(q.contains(o4));
            
            REQUIRE(q.size() == 2);
            REQUIRE(q.pop() == o2);
            REQUIRE(q.pop() == o4);
            REQUIRE(q.isEmpty());
        }
    }

}

SCENARIO("test min_id_heap") {

    GIVEN("create queue") {
        min_id_heap<int, int> heap{100};

        WHEN("block add and remove") {

            heap.pushOrDecrease(50, 3);
            heap.pushOrDecrease(60, 4);
            heap.pushOrDecrease(70, 2);
            heap.pushOrDecrease(80, 1);

            REQUIRE(heap.pop() == 80);
            REQUIRE(heap.pop() == 70);
            REQUIRE(heap.pop() == 50);
            REQUIRE(heap.pop() == 60);
        }

        WHEN("interleaved add and remove") {
            heap.pushOrDecrease(50, 3);
            heap.pushOrDecrease(60, 2);

            REQUIRE(heap.pop() == 60);

            heap.pushOrDecrease(70, 1);

            REQUIRE(heap.pop() == 70);
            REQUIRE(heap.pop() == 50);
            REQUIRE(heap.isEmpty());
        }

        WHEN("revising element priority") {
            heap.pushOrDecrease(50, 3);
            heap.pushOrDecrease(60, 4);
            heap.pushOrDecrease(70, 2);
            heap.pushOrDecrease(80, 5);

            heap.pushOrDecrease(80, 1);

            REQUIRE(heap.pop() == 80);
            REQUIRE(heap.pop() == 70);
            REQUIRE(heap.pop() == 50);
            REQUIRE(heap.pop() == 60);
        }
    }

    GIVEN("create queue with idT != keyT") {
        min_id_heap<long, int> heap{100};

        WHEN("block add and remove") {

            heap.pushOrDecrease(50L, 3);
            heap.pushOrDecrease(60L, 4);
            heap.pushOrDecrease(70L, 2);
            heap.pushOrDecrease(80L, 1);

            REQUIRE(heap.pop() == 80L);
            REQUIRE(heap.pop() == 70L);
            REQUIRE(heap.pop() == 50L);
            REQUIRE(heap.pop() == 60L);
        }

        WHEN("interleaved add and remove") {
            heap.pushOrDecrease(50L, 3);
            heap.pushOrDecrease(60L, 2);

            REQUIRE(heap.pop() == 60L);

            heap.pushOrDecrease(70L, 1);

            REQUIRE(heap.pop() == 70L);
            REQUIRE(heap.pop() == 50L);
            REQUIRE(heap.isEmpty());
        }

        WHEN("revising element priority") {
            heap.pushOrDecrease(50L, 3);
            heap.pushOrDecrease(60L, 4);
            heap.pushOrDecrease(70L, 2);
            heap.pushOrDecrease(80L, 5);

            heap.pushOrDecrease(80L, 1);

            REQUIRE(heap.pop() == 80L);
            REQUIRE(heap.pop() == 70L);
            REQUIRE(heap.pop() == 50L);
            REQUIRE(heap.pop() == 60L);
        }
    }
}

template <typename T>
class QueueHolder: public IMemorable {
private:
    StaticPriorityQueue<T> q;
public:
    QueueHolder(): q{20, true} {

    }
public:
    MemoryConsumption memory() const {
        return this->q.getByteMemoryOccupied();
    }
public:
    MemoryConsumption getByteMemoryOccupied() const {
        return this->q.getByteMemoryOccupied();
    }
};

SCENARIO("test access of a virtual method from a template base class inside another template class") {
    QueueHolder<Foo> qh{};
    qh.getByteMemoryOccupied();
    qh.memory();
}

namespace another_namespace {


template <typename T>
class QueueHolderDifferentNameSpace: public IMemorable {
private:
    StaticPriorityQueue<T>& q;
public:
    QueueHolderDifferentNameSpace(StaticPriorityQueue<T>& q): q{q} {

    }
public:
    MemoryConsumption memory() const {
        return this->q.getByteMemoryOccupied();
    }
public:
    MemoryConsumption getByteMemoryOccupied() const {
        return this->q.getByteMemoryOccupied();
    }
};

SCENARIO("test access of a virtual method from a template base class inside another template class from different namespace") {
    StaticPriorityQueue<Foo> q{20, true};
    QueueHolderDifferentNameSpace<Foo> qh{q};
    qh.getByteMemoryOccupied();
    qh.memory();
}

}

SCENARIO("test ppmImage") {
    GIVEN("an image to print") {
        PPMImage image{5,3};
        WHEN("generating a simple image") {
            image.saveBMP("image01");
        }

        WHEN("set a pixel") {
            image.setPixel(2, 1, color_t::BLUE);
            image.saveBMP("image02");
        }

        WHEN("adding 2 images") {
            PPMImage image2{5, 3, color_t::RED};
            image.setAllPixels(color_t::WHITE);
            (image + image2).saveBMP("image03");
        }
    }
}


SCENARIO("test ks001 implementation") {
    GIVEN("a nameless ks001") {

        KS001 ks001{};

        WHEN("simple test") {
            ks001.add_key_value(0, "k1", "v1");
            
            REQUIRE(ks001.toString() == "|k1=v1|");
        }

        WHEN("multiple entries in a dict") {
            ks001.add_key_value(0, "k1", "v1");
            ks001.add_key_value(0, "k2", "v2");
            
            REQUIRE(ks001.toString() == "|k1=v1_k2=v2|");
        }

        WHEN("single entries in multiple dicts") {
            ks001.add_key_value(0, "k1", "v1");
            ks001.add_key_value(1, "k2", "v2");
            
            REQUIRE(ks001.toString() == "|k1=v1|k2=v2|");
        }

        WHEN("single entries in multiple dicts") {
            ks001.add_key_value(0, "k1", "v1");
            ks001.add_key_value(0, "k2", "v2");
            ks001.add_key_value(1, "k4", "v4");
            ks001.add_key_value(1, "k3", "v3");
            ks001.add_key_value(1, "k5", "v5");
            
            REQUIRE(ks001.toString() == "|k1=v1_k2=v2|k3=v3_k4=v4_k5=v5|");
        }
    }
}

SCENARIO("test conversion") {

    GIVEN("2 memory consumptions") {
        MemoryConsumption a{2048, MemoryConsumptionEnum::BYTE};
        MemoryConsumption b{10, MemoryConsumptionEnum::BYTE};

        REQUIRE(a.to(MemoryConsumptionEnum::KILOBYTE) == MemoryConsumption{2, MemoryConsumptionEnum::KILOBYTE});
        REQUIRE(a.to(MemoryConsumptionEnum::KILOBYTE).to(MemoryConsumptionEnum::BYTE) == MemoryConsumption{2048, MemoryConsumptionEnum::BYTE});
    }
}

SCENARIO("test safe number") {

     using safe_int = wrapped_number<int, 1, 10>;

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

SCENARIO("test log.h") {

    //normal
    critical("this is a test!", 20);


    
}