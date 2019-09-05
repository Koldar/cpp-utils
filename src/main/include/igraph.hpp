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

namespace cpp_utils {

/**
 * @brief an id identifying each vertex in the graph
 * 
 */
typedef u_int64_t nodeid_t;

template<typename E>
class Edge;

template<typename E>
bool operator == (const Edge<E>& a, const Edge<E>& b);

template<typename E>
class OutEdge;

template<typename E>
class InEdge;

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
    virtual size_t size() const = 0;
    /**
     * @brief the number of vertices in the graph
     * 
     * @return size_t the number of vertices in the graph
     */
    virtual size_t numberOfVertices() const = 0;
    /**
     * @brief the number of edges in the graph
     * 
     * @return size_t the number of edges in the graph
     */
    virtual size_t numberOfEdges() const = 0;
    virtual IImmutableGraph<G,V,E>::const_vertex_iterator beginVertices() const = 0;
    virtual IImmutableGraph<G,V,E>::const_vertex_iterator endVertices() const = 0;
    virtual IImmutableGraph<G,V,E>::const_edge_iterator beginEdges() const = 0;
    virtual IImmutableGraph<G,V,E>::const_edge_iterator endEdges() const = 0;
public:
    /**
     * @brief Get the Vertex object
     * 
     * @param id the id of the vertex to fetch
     * @return const V& the payload associated to such vertex id
     */
    virtual const V& getVertex(nodeid_t id) const = 0;
    virtual const E& getEdge(nodeid_t sourceId, nodeid_t sinkId) const = 0;
    virtual const G& getPayload() const = 0;
    virtual G& getPayload() = 0;
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
    virtual OutEdge<E> getOutEdge(nodeid_t id, int index) const = 0;
    virtual bool hasEdge(nodeid_t sourceId, nodeid_t sinkId) const = 0;
    virtual std::vector<InEdge<E>> getInEdges(nodeid_t id) const = 0;
    virtual std::vector<OutEdge<E>> getOutEdges(nodeid_t id) const = 0;
    virtual bool isEmpty() const = 0;
    virtual const PPMImage* getPPM() const {
        callExternalProgram("rm -f /tmp/getPPM.png /tmp/getPPM.ppm /tmp/getPPM.dot");

        std::ofstream f;
        f.open("/tmp/getPPM.dot", std::ofstream::out | std::ofstream::trunc);

        f << "digraph {\n";
        info("iterate over vertices...");
        for (auto it=this->beginVertices(); it!=this->endVertices(); ++it) {
            info("drawing vertex", it->first);
            f << "N" << it->first << " [label=\"" << it->second << "\"];\n";
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
    virtual void changeWeightOutEdge(nodeid_t sourceId, int index, const E& newPayload) = 0;
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