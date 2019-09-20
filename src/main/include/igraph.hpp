#ifndef _IGRAPH_HEADER__
#define _IGRAPH_HEADER__

#include "imemory.hpp"
#include "IImageable.hpp"
#include "iterator.hpp"
#include "operators.hpp"
#include "ppmImage.hpp"
#include <climits>
#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>

namespace cpp_utils::graphs {

/**
 * @brief an id identifying each vertex in the graph
 * 
 */
typedef u_int64_t nodeid_t;

/**
 * @brief an id identifying the out (or the in) edge of a vertex
 * 
 * For instance:
 * 
 * @dot
 * digraph {
 *  A -> B [label="0"];
 *  A -> C [label="1"];
 *  A -> D [label="2"];
 *  A -> E [label="3"];
 * }
 * @enddot
 * 
 * Given A, you can identify the outedge to "C" by the *moveid* 1
 */
typedef unsigned short moveid_t;

template<typename E>
class Edge;

template<typename E>
bool operator == (const Edge<E>& a, const Edge<E>& b);

template<typename E>
class OutEdge;

template<typename E>
class InEdge;

template <typename G, typename V, typename E>
class ListGraph;

/**
 * @brief an edge where it's specified both the source node and the sink one
 * 
 * Note that this structure contains the paylaod as E. This means that it can also store E& as E (or E*);
 * this means that it doens't encessarly mean that this structure OWN the payload of the arcs, maybe they are
 * stored somewhere else. This is the main reason why the destructor is totally empty.
 * 
 * The owner of the payload depend how the container decides to store them
 * 
 * @tparam E type of the label of the edge. The type is stored inside the edge strucutre
 */
template<typename E>
class Edge {
    friend bool operator == <E>(const Edge<E>& a, const Edge<E>& b);
    friend std::ostream& operator << (std::ostream& ss, const Edge<E>& e) {
        ss << "(" << e.getSourceId() << "-> " << e.getSinkId() << ": " << e.getPayload() << ")";
        return ss;
    }
public:
    Edge<E>(nodeid_t sourceId, nodeid_t sinkId, const E& payload) : payload{payload}, sourceId{sourceId}, sinkId{sinkId} {};
    Edge<E>(const Edge<E>& other) : payload{other.payload}, sourceId{other.sourceId}, sinkId{other.sinkId} {};
    Edge<E>(nodeid_t sourceId, const OutEdge<E>& other): Edge<E>{sourceId, other.getSinkId(), other.getPayload()} {

    }
    Edge<E>(nodeid_t sinkId, const InEdge<E>& other): Edge<E>{other.getSourceId(), sinkId, other.getPayload()} {
        
    }
    virtual ~Edge<E>() {};
public:
    Edge<E>& operator =(const Edge<E> other) {
        this->sourceId = other.sourceId;
        this->sinkId = other.sinkId;
        this->payload = other.payload;
        return *this;
    }
public:
    void setPayload(const E& newPayload) {
        this->payload = newPayload;
    }
    const E& getPayload() const {
        return this->payload;
    }
    nodeid_t getSourceId() const {
        return this->sourceId;
    }
    nodeid_t getSinkId() const {
        return this->sinkId;
    }
    bool isCompliant(nodeid_t sourceId, nodeid_t sinkId) const {
        return this->sourceId == sourceId && this->sinkId == sinkId;
    }
    bool hasSource(nodeid_t sourceId) const {
        return this->sourceId == sourceId;
    }
    bool hasSink(nodeid_t sinkId) const {
        return this->sinkId == sinkId;
    }
private:
    E payload;
    nodeid_t sourceId;
    nodeid_t sinkId;
};

template <typename E>
bool operator ==(const Edge<E>& a, const Edge<E>& b) {
    return a.sourceId == b.sourceId && a.sinkId == b.sinkId && a.payload == b.payload;
}

/**
 * @brief a graph where it's specified only the source, not the sink
 * 
 * @tparam E type of the label of the edge. The type is stored inside the edge strucutre
 */
template<typename E>
class InEdge {
public:
    InEdge<E>(nodeid_t sourceId, const E& payload) : payload{payload}, sourceId{sourceId} {};
    InEdge<E>(const InEdge<E>& other) : payload{other.payload}, sourceId{other.sourceId} {};
    InEdge<E>(const Edge<E>& other) : payload{other.getPayload()}, sourceId{other.getSourceId()} {};
    virtual ~InEdge<E>() {};
public:
    InEdge<E>& operator =(const InEdge<E>& other);
    friend bool operator ==(const InEdge<E>& a, const InEdge<E>& b) {
        return a.sourceId == b.sourceId && a.payload == b.payload;
    }
    friend std::ostream& operator << (std::ostream& ss, const InEdge<E>& e) {
        ss << "(" << e.sourceId << "-> : " << e.payload << ")";
        return ss;
    }
public:
    E getPayload();
    const E& getPayload() const;
    nodeid_t getSourceId() const;
private:
    E payload;
    nodeid_t sourceId;
};

/**
 * @brief an edge where it's specified only the sink, but not the source
 * 
 * @tparam E type of the label of the edge. The type is stored inside the edge strucutre
 */
template<typename E>
class OutEdge {
public:
    OutEdge<E>(): payload{}, sinkId{0} {}
    OutEdge<E>(nodeid_t sinkId, const E& payload) : payload{payload}, sinkId{sinkId} {};
    OutEdge<E>(const OutEdge<E>& other) : payload{other.payload}, sinkId{other.sinkId} {};
    OutEdge<E>(const Edge<E>& other) : payload{other.getPayload()}, sinkId{other.getSinkId()} {};
    virtual ~OutEdge() {};
public:
    OutEdge<E>& operator =(const OutEdge<E>& other) {
        this->payload = other.payload;
        this->sinkId = other.sinkId;
        return *this;
    }
    friend bool operator ==(const OutEdge<E>& a, const OutEdge<E>& b) {
        return a.sinkId == b.sinkId && a.payload == b.payload;
    }
    friend std::ostream& operator << (std::ostream& ss, const OutEdge<E>& e) {
        ss << "( -> " << e.getSinkId() << ": " << e.getPayload() << ")";
        return ss;
    }
public:
    E& getPayload() {
        return this->payload;
    }
    const E& getPayload() const {
        return this->payload;
    }
    nodeid_t getSinkId() const {
        return this->sinkId;
    }
private:
    E payload;
    nodeid_t sinkId;
};

template <typename G, typename V, typename E>
class IImmutableGraph: public IMemorable, public IImageable {
public:
    friend std::ostream& operator << (std::ostream& ss, const IImmutableGraph<G,V,E>& g) {
        ss << g.getPayload();
        return ss;
    }
    friend bool operator ==(const IImmutableGraph<G,V,E>& a, const IImmutableGraph<G,V,E>& b) {
        if (&a == &b) {
            return true;
        }
        if (a.numberOfVertices() != b.numberOfVertices()) {
            return false;
        }
        if (a.numberOfEdges() != b.numberOfEdges()) {
            return false;
        }

        //vertices
        for (auto it=a.beginVertices(); it!= a.endVertices(); ++it) {
            nodeid_t id1 = it->first;
            if (!b.containsVertex(id1)) {
                return false;
            }
            if (a.getVertex(id1) != b.getVertex(id1)) {
                return false;
            }
        }

        //edges
        for (auto it=a.beginEdges(); it!= a.endEdges(); ++it) {
            Edge<E> e = *it;
            if (!b.containsEdge(e.getSourceId(), e.getSinkId(), e.getPayload())) {
                return false;
            }
        }

        return true;
    }
public:
    using const_vertex_iterator = ProxyConstIterator<std::pair<nodeid_t, const V&>, std::pair<nodeid_t, const V&>*>;
    using const_edge_iterator = ProxyConstIterator<Edge<E>&, Edge<E>*>;
public:
    IImmutableGraph() {

    }
    virtual ~IImmutableGraph() {

    }
public:
    /**
     * @brief the number of vertices in the graph
     * 
     * @return size_t the number of vertices in the graph
     */
    virtual std::size_t size() const = 0;
    /**
     * @brief the number of vertices in the graph
     * 
     * @return size_t the number of vertices in the graph
     */
    virtual std::size_t numberOfVertices() const = 0;
    /**
     * @brief the number of edges in the graph
     * 
     * @return size_t the number of edges in the graph
     */
    virtual std::size_t numberOfEdges() const = 0;
    virtual IImmutableGraph<G,V,E>::const_vertex_iterator beginVertices() const = 0;
    virtual IImmutableGraph<G,V,E>::const_vertex_iterator endVertices() const = 0;
    virtual IImmutableGraph<G,V,E>::const_edge_iterator beginEdges() const = 0;
    virtual IImmutableGraph<G,V,E>::const_edge_iterator endEdges() const = 0;
    /**
     * @brief the maximum number of edges you can have in this graph
     * 
     * e.g., when the graph is complete, namely when every **ordered** pair of vertices has associated a single direct edge
     * 
     * for example, if the graph has 4 vertices the function will return 12:
     * @dot
     *  A -> B; B -> A;
     *  B -> C; C -> B;
     *  C -> D; D -> C;
     *  D -> A; A -> D;
     *  A -> C; C -> A;
     *  B -> D; D -> B;
     * @enddot
     * 
     * @return std::size_t \f$ n(n-1)\f$
     */
    virtual std::size_t getMaximumNumberOfEdges() const {
        auto n = this->numberOfVertices();
        return (n*(n-1));
    }
    /**
     * @brief the number of edges over the total number of possible edges
     * 
     * @return double number between 0 and 1.
     */
    virtual double getDensity() const {
        return (this->numberOfEdges() + 0.0)/(this->getMaximumNumberOfEdges());
    }
    /**
     * @brief the number of edges in the graph from graph density
     * 
     * @param ratio number between 0 and 1. Output of ::getDensity
     * @return number of edges in the graph
     */
    virtual std::size_t getNumberOfEdgesFromDensity(double density) const {
        return density * this->getMaximumNumberOfEdges();
    }
    /**
     * @brief Get the Vertex payload
     * 
     * @param id the id of the vertex to fetch
     * @return const V& the payload associated to such vertex id
     */
    virtual const V& getVertex(nodeid_t id) const = 0;
    /**
     * @brief check if the graph has a vertex with the given id
     * 
     * @param id the id to check
     * @return true if there exists a vertgex which id is @c id
     * @return false otherwise
     */
    virtual bool containsVertex(nodeid_t id) const = 0;
    /**
     * @brief Get an edge
     * 
     * @param sourceId the id of the vertex which is the source of the edge to retrieve
     * @param sinkId the id of the vertex which is the sink of the edge to retrieve
     * @return const E& the payload of such edge
     */
    virtual const E& getEdge(nodeid_t sourceId, nodeid_t sinkId) const = 0;
    /**
     * @brief check if the graph has declared an edge as structure in the parameters
     * 
     * @param sourceId the id of the source of the edge
     * @param sinkId  the id of the sink of the edge
     * @param payload the payload of the edge
     * @return true if the graph has declare such an edge
     * @return false otherwise
     */
    virtual bool containsEdge(nodeid_t sourceId, nodeid_t sinkId, const E& payload) const = 0;
    /**
     * @brief check if the graph has declared an edge as structure in the parameters
     * 
     * @param sourceId the id of the source of the edge
     * @param sinkId  the id of the sink of the edge
     * @return true if the graph has declare such an edge
     * @return false otherwise
     */
    virtual bool containsEdge(nodeid_t sourceId, nodeid_t sinkid) const = 0;
    /**
     * @brief Get the Payload object
     * 
     * @return const G& the payload of the entire graph
     */
    virtual const G& getPayload() const = 0;
    /**
     * @brief Get the Payload object
     * 
     * @return G& the payload of the entire graph
     */
    virtual G& getPayload() = 0;
    /**
     * @brief obtain the first vertex id of a vertex which has the given payload
     * 
     * @note
     * what compliant means depends on the implementation of the graph
     * 
     * The default operation is really slow. Do not use it for performance reasons!
     * 
     * @param payload the payload requested
     * @return nodeid_t id which vertex has paylaod compliant with @c payload
     * @throw cpp_utils::exceptions::ElementNotFoundException if no vertex contains such payload
     */
    virtual nodeid_t idOfVertex(const V& payload) const {
        for (auto it=this->beginVertices(); it!=this->endVertices(); ++it) {
            if (it->second == payload) {
                return it->first;
            }
        }
        throw cpp_utils::exceptions::ElementNotFoundException<V, G>{payload, this->getPayload()};
    }
    virtual size_t getInDegree(nodeid_t id) const = 0;
    virtual size_t getOutDegree(nodeid_t id) const = 0;
    virtual size_t getDegree(nodeid_t id) const = 0;
    virtual bool hasSuccessors(nodeid_t id) const = 0;
    virtual bool hasPredecessors(nodeid_t id) const = 0;
    /**
     * @brief Get the Out Edge object
     * 
     * @param id id of the source id
     * @param index index of the out edge to retrieve. index starts from 0 (inclusive)
     * @return const OutEdge<E>& 
     */
    virtual OutEdge<E> getOutEdge(nodeid_t id, moveid_t index) const = 0;
    virtual bool hasEdge(nodeid_t sourceId, nodeid_t sinkId) const = 0;
    virtual std::vector<InEdge<E>> getInEdges(nodeid_t id) const = 0;
    virtual std::vector<OutEdge<E>> getOutEdges(nodeid_t id) const = 0;
    virtual bool isEmpty() const = 0;
public:
    /**
     * @brief lots of graph implementation require that the ids starts from 0 and are contiguous
     * 
     * @return true if such assertion is true
     * @return false otherwise
     */
    virtual bool areVertexIdsContiguous() const {
        std::unordered_set<int> result{};
        for (auto it=this->beginVertices(); it!=this->endVertices(); ++it) {
            result.insert(it->first);
        }
        //check
        for (nodeid_t id=0; id<this->numberOfVertices(); ++id) {
            if (result.find(id) == result.end()) {
                return false;
            }
        }
        return true;
    }
    /**
     * @brief apply a permutation of the vertex ids of this graph
     * 
     * @dot
     *  A [label="0"];
     *  B [label="1"];
     *  C [label="2"];
     * 
     *  A -> B -> C;
     * @enddot
     * 
     * if we specifiy:
     * @li @c \f$ fromOldToNew = \{ 0 \rightarrow 1, 1 \rightarrow 2, 2 \rightarrow 0 \}\f$;
     * @li @c \f$ fromNewToOld = \{ 0 \rightarrow 2, 1 \rightarrow 0, 2 \rightarrow 1 \}\f$;
     * 
     * the graph will be:
     * 
     * @dot
     *  A [label="1"];
     *  B [label="2"];
     *  C [label="0"];
     * 
     *  A -> B -> C;
     * @enddot
     * 
     * @pre
     *  @li vertices ids are contiguous;
     *  @li vertices starts from 0;
     * 
     * @note
     * All payloads are copy by value
     * 
     * @param fromOldToNew a vector where each index is the id of a vertex in the old coordinate system while the associated cell represents the id of the same vertex in the new coordinate system.
     * @param fromNewToOld a vector where each index is the id of a vertex in the new coordinate system while the associated cell represents the id of the same vertex in the old coordinate system
     * @return a new graph where each vertex has been ordered as specified by the parameters
     */
    virtual std::unique_ptr<IImmutableGraph<G, V, E>> reorderVertices(const std::vector<nodeid_t>& fromOldToNew, const std::vector<nodeid_t>& fromNewToOld) const {

        ListGraph<G,V,E>* result = new ListGraph<G,V,E>{this->getPayload()};

        //vertices
        for (nodeid_t newId=0; newId<this->numberOfVertices(); ++newId) {
            result->addVertex(this->getVertex(fromNewToOld[newId]));
        }

        //edges
        for (nodeid_t oldSourceId=0; oldSourceId<this->numberOfVertices(); ++oldSourceId) {
            for (auto outEdge: this->getOutEdges(oldSourceId)) {
                result->addEdge(
                    fromOldToNew[oldSourceId], 
                    fromOldToNew[outEdge.getSinkId()], 
                    outEdge.getPayload()
                );
            }
        }

        return std::unique_ptr<ListGraph<G,V,E>>{result};
    }
    virtual const PPMImage* getPPM() const {
        callExternalProgram("rm -f /tmp/getPPM.png /tmp/getPPM.ppm /tmp/getPPM.dot");

        std::ofstream f;
        f.open("/tmp/getPPM.dot", std::ofstream::out | std::ofstream::trunc);

        f << "digraph {\n";
        info("iterate over vertices...");
        for (auto it=this->beginVertices(); it!=this->endVertices(); ++it) {
            info("drawing vertex", it->first);
            f << "N" << it->first << " [label=\"id:" << it->first << "\\n" << it->second << "\"];\n";
        }

        info("iterate over edges...");
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            info("drawing edge", it->getSourceId(), "->", it->getSinkId());
            f << "N" << it->getSourceId() << " -> N" << it->getSinkId() << " [label=\"" << it->getPayload() << "\"];\n";
        }
        f << "}\n";

        f.close();

        callExternalProgram("dot -Tpng -o /tmp/getPPM.png /tmp/getPPM.dot");
        //https://askubuntu.com/a/84415/703658
        callExternalProgram("convert -depth 8 -compress none /tmp/getPPM.png /tmp/getPPM.ppm");
        PPMImage* result = new PPMImage{"/tmp/getPPM.ppm"};
        return result;
    }
    /**
     * @brief true if there are no edges in the graph
     * 
     * @return true no edges are present
     * @return false at least one edge is present
     */
    virtual bool hasNoEdges() const {
        return this->numberOfEdges() == 0;
    }
};

/**
 * @brief A graph whose only change is to alter the edge weights
 * 
 * @tparam G 
 * @tparam V 
 * @tparam E 
 */
template <typename G, typename V, typename E>
class INonExtendableGraph: public IImmutableGraph<G,V,E> {
public:
    virtual void changeWeightEdge(nodeid_t sourceId, nodeid_t sinkId, const E& newPayload) = 0;
    virtual void changeWeightOutEdge(nodeid_t sourceId, moveid_t index, const E& newPayload) = 0;
};

/**
 * @brief  a graph where you can alter edges, add and remove edges but where you can only add vertices, not removing them
 * 
 * @tparam G payload of the entire graph
 * @tparam V payload of a vertex
 * @tparam E paylaod of an arc
 */
template <typename G, typename V, typename E>
class IVertexExtendableGraph: public INonExtendableGraph<G,V,E> {
public:
    IVertexExtendableGraph() {

    }
    IVertexExtendableGraph(const IImmutableGraph<G,V,E>& other) {
        for(auto it=other.beginVertices(); it!=it.endVertices(); ++it) {
            this->addVertex(it->first, it->second);
        }
        for (auto it=other.beginEdges(); it != other.endEdges(); ++it) {
            this->addEdge(it->getSourceId(), it->getSinkId(), it->getPayload());
        }
    }

    virtual nodeid_t addVertex(const V& payload) = 0;
    virtual void addEdge(nodeid_t sourceId, nodeid_t sinkId, const E& payload) = 0;
    virtual void removeEdge(nodeid_t sourceId, nodeid_t sinkId) = 0;
    virtual void removeAllEdges() = 0;
};





}

#endif