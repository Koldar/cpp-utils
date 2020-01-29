
#include <boost/algorithm/string.hpp>
#include <boost/heap/d_ary_heap.hpp>

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
#include "NumTracker.hpp"
#include "BoostQueue.hpp"

#include "UncopiableClass.hpp"

using namespace cpp_utils;
using namespace cpp_utils::graphs;

class RefAndPtrTest {
public:
    int a;
public:
    RefAndPtrTest(int a): a{a} {

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

SCENARIO("test NumTracker") {


    NumTracker<int> tracker;

    REQUIRE(tracker.getCount() == 0);

    tracker.update(4);

    REQUIRE(tracker.getCount() == 1);
    REQUIRE(tracker.getLastValue() == 4);
    REQUIRE(isApproximatelyEqual(tracker.getAverage(), 4.0, 1e-3));

    tracker.update(10);

    REQUIRE(tracker.getCount() == 2);
    REQUIRE(tracker.getLastValue() == 10);
    REQUIRE(isApproximatelyEqual(tracker.getAverage(), 7.0, 1e-3));

    tracker.update(7);

    REQUIRE(tracker.getCount() == 3);
    REQUIRE(tracker.getLastValue() == 7);
    REQUIRE(isApproximatelyEqual(tracker.getAverage(), 7.0, 1e-3));

    tracker.cleanup();

    REQUIRE(tracker.getCount() == 0);
}

SCENARIO("test getProcessUsedRAM") {
    pid_t pid = getCurrentPID();
    auto ram = getProcessUsedRAM(pid);
    auto totalRam = getSystemRAMUsed();
    critical("RAM is", ram);
    critical("used RAM is", totalRam);
    REQUIRE(ram > MemoryConsumption{0});
    REQUIRE(totalRam > MemoryConsumption{0});
}

SCENARIO("test open file descriptors") {

    GIVEN("testing pid") {

        REQUIRE(cpp_utils::getCurrentPID() > 0);

        critical("open files: ", cpp_utils::getOpenFileDescriptors());
        REQUIRE(cpp_utils::getOpenFileDescriptors().size() >= 6);
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
