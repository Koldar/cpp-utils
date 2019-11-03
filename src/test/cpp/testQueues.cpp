#include "catch.hpp"

#include <boost/heap/d_ary_heap.hpp>

#include "KHeaps.hpp"
#include "BoostQueue.hpp"
#include "IQueue.hpp"
#include "StaticPriorityQueue.hpp"

using namespace cpp_utils;

class Foo: public HasPriority<priority_t> {
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

class FooBoost {
    public:
        friend std::ostream& operator <<(std::ostream& ss, const FooBoost& f) {
            ss << f.content << "(" << f.value << ")";
            return ss;
        }
        friend bool operator < (const FooBoost& a, const FooBoost& b) {
            return a.value < b.value;
        }
        friend bool operator ==(const FooBoost& a, const FooBoost& b) {
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
    public:
        FooBoost(): content{""}, value{0} {

        }
        FooBoost(std::string content, int value) : content{content}, value{value} {

        }
        virtual ~FooBoost() {

        }
        FooBoost& operator =(const FooBoost& other) {
            this->content = other.content;
            this->value = other.value;
            return *this;
        }
};

struct NewOrder {
    bool operator() (const FooBoost& a, const FooBoost& b) {
        //reverse the order of them  heap
        return (b < a);
    }
};

SCENARIO("test boostQueue") {

    GIVEN("boost Queue") {
        BoostQueue<FooBoost> q{};

        WHEN("empty") {
            REQUIRE(q.isEmpty());
            REQUIRE(q.size() == 0);
        }

        WHEN("non empty") {
            FooBoost o1 = FooBoost{std::string{"a"}, 5};
            FooBoost o2 = FooBoost{std::string{"b"}, 10};
            FooBoost o3 = FooBoost{std::string{"c"}, 1};
            FooBoost o4 = FooBoost{std::string{"d"}, 11};

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

    GIVEN("boost Queue with a custom order") {
        BoostQueue<FooBoost, NewOrder> q{};

        WHEN("empty") {
            REQUIRE(q.isEmpty());
            REQUIRE(q.size() == 0);
        }

        WHEN("non empty") {
            FooBoost o1 = FooBoost{std::string{"a"}, 5};
            FooBoost o2 = FooBoost{std::string{"b"}, 10};
            FooBoost o3 = FooBoost{std::string{"c"}, 1};
            FooBoost o4 = FooBoost{std::string{"d"}, 11};

            q.push(o1);
            q.push(o2);
            q.push(o3);
            q.push(o4);

            REQUIRE(!q.isEmpty());
            REQUIRE(q.size() == 4);

            info("queue is", q);
            REQUIRE(q.peek() == o4);
            REQUIRE(q.pop() == o4);
            info("queue minus or", q);
            REQUIRE(q.pop() == o2);
            REQUIRE(q.pop() == o1);

            info("queue before readding o2 ", q);
            q.push(o2);
            info("queue after readding o2 ", q);
            REQUIRE(!q.contains(o1));
            REQUIRE(q.contains(o2));
            REQUIRE(q.contains(o3));
            REQUIRE(!q.contains(o4));
            
            REQUIRE(q.size() == 2);
            REQUIRE(q.pop() == o2);
            REQUIRE(q.pop() == o3);
            REQUIRE(q.isEmpty());
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