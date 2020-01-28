#include "catch.hpp"
#include "igraph.hpp"
#include "adjacentGraph.hpp"
#include "listGraph.hpp"

using namespace cpp_utils;
using namespace cpp_utils::graphs;

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

        WHEN("testing iterator on graph with no edges") {
            AdjacentGraph<int, int, bool> empty{};

            int i= 0;
            for (auto it=empty.beginEdges(); it!=empty.endEdges(); ++it) {
                i += 1;
            }
            REQUIRE(i == 0);
        }

        WHEN("testing iterator on graph with vertices but no edges") {
            ListGraph<int, int, bool> listGraph{1};

            listGraph.addVertex(5);
            listGraph.addVertex(10);
            listGraph.addVertex(15);

            AdjacentGraph<int, int, bool> empty{listGraph};
            //empty.saveBMP("emptyGraphOnIterate");

            int i = 0;
            for (auto it=empty.beginEdges(); it!=empty.endEdges(); ++it) {
                i += 1;
            }

            REQUIRE(i == 0);
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

        WHEN("mapping vertices") {
            function_t<int, char> lambda = [&](const int& i) { return 'A' + (char)i;};
            IImmutableGraph<int, char, bool>* tmp = ag.mapVertices(lambda);
            AdjacentGraph<int, char, bool> ag3{*tmp};

            REQUIRE(ag3.getPayload() == ag.getPayload());
            REQUIRE(ag3.numberOfVertices() == ag.numberOfVertices());
            REQUIRE(ag3.numberOfEdges() == ag.numberOfEdges());

            REQUIRE(ag3.getVertex(n0) == 'B');
            REQUIRE(ag3.getVertex(n1) == 'C');
            REQUIRE(ag3.getVertex(n2) == 'D');
            REQUIRE(ag3.getVertex(n3) == 'E');
            REQUIRE(ag3.getVertex(n4) == 'F');
            
            delete tmp;
        }

        WHEN("testing out and in max degrees") {
            REQUIRE(lg.getMaxOutDegree() == 2);
            REQUIRE(lg.getMaxInDegree() == 1);
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