#include "catch.hpp"
#include "wrapped_number.hpp"
#include "imemory.hpp"
#include "ks001.hpp"
#include "ppmImage.hpp"
#include "listGraph.hpp"
#include "StaticPriorityQueue.hpp"
#include "vectorplus.hpp"
#include "safe_inf_uint.hpp"
#include "math.hpp"

using namespace cpp_utils;
using namespace cpp_utils::graphs;

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
}

SCENARIO("test vector plus") {

     GIVEN("a vector plus") {
         vectorplus<int> a;

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
        
    }
}

class Foo: public HasPriority {
    public:
        friend std::ostream& operator <<(std::ostream& ss, const Foo& f) {
            ss << f.content;
            return ss;
        }
        friend bool operator < (const Foo& a, const Foo& b) {
            return a.value < b.value;
        }
    private:
        std::string content;
        int value;
        priority_t p;
    public:
        Foo(std::string content, int value) : content{content}, value{value}, p{0} {

        }
        priority_t getPriority() const {
            return this->p;
        }
        void setPriority(priority_t p) {
            this->p = p;
        }
    };


SCENARIO("test macros") {

    GIVEN("elvis") {
        int a = 5;
        int* b = &a;
        int* c = nullptr;

        REQUIRE(ELVIS(b, 10) == 5); 
        REQUIRE(ELVIS(c, 10) == 10);
    }

}

SCENARIO("test queue") {

    GIVEN("a queue") {
        StaticPriorityQueue<Foo> q{1000, true};

        WHEN("empty") {
            REQUIRE(q.isEmpty());
            REQUIRE(q.size() == 0);
        }

        WHEN("non empty") {
            Foo o1{"a", 5};
            Foo o2{"b", 10};
            Foo o3{"c", 1};
            Foo o4{"d", 11};


            q.push(o1);
            q.push(o2);
            q.push(o3);
            q.push(o4);

            REQUIRE(!q.isEmpty());
            REQUIRE(q.size() == 4);

            REQUIRE(&q.peek() == &o3);
            REQUIRE(&q.pop() == &o3);
            REQUIRE(&q.pop() == &o1);
            REQUIRE(&q.pop() == &o2);

            q.push(o2);
            REQUIRE(!q.contains(o1));
            REQUIRE(q.contains(o2));
            REQUIRE(!q.contains(o3));
            REQUIRE(q.contains(o4));
            
            REQUIRE(q.size() == 2);
            REQUIRE(&q.pop() == &o2);
            REQUIRE(&q.pop() == &o4);
            REQUIRE(q.isEmpty());
        }
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