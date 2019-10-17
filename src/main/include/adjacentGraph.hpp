#ifndef _ADJACENTGRAPH_HEADER__
#define _ADJACENTGRAPH_HEADER__

#include <vector>
#include "igraph.hpp"
#include "serializers.hpp"

namespace cpp_utils::graphs {

template <typename G, typename V, typename E>
class AdjacentGraph;

}

namespace cpp_utils::serializers {

    /**
     * Save the current graph into a file
     *
     * @pre
     *  @li @c f open with "wb";
     * @post
     *  @li @c f modified;
     *  @li @c f cursor modified;
     *
     * @param[in] f the file to save the graph into
     */
    template <typename G, typename V, typename E>
    void saveToFile(FILE* f, const cpp_utils::graphs::AdjacentGraph<G,V,E>& g)  {
        saveToFile(f, g.payload);
        saveToFile<V>(f, g.vertexPayload);
        saveToFile<cpp_utils::graphs::OutEdge<E>>(f, g.edges);
        saveToFile<int>(f, g.outEdgesOfvertexBegin);
    }

    /**
     * Load a graph from a file in the filesystem
     *
     * @pre
     *  @li @c f open in "rb";
     * @post
     *  @li @c f cursor modified;
     *
     * @param[in] f the file to read the graph from;
     * @return the graph loaded
     */
    template <typename G, typename V, typename E>
    cpp_utils::graphs::AdjacentGraph<G,V,E>& loadFromFile(FILE* f, cpp_utils::graphs::AdjacentGraph<G,V,E>& result) {
        loadFromFile(f, result.payload);
        loadFromFile(f, result.vertexPayload);
        loadFromFile<cpp_utils::graphs::OutEdge<E>>(f, result.edges);
        loadFromFile<int>(f, result.outEdgesOfvertexBegin);

        return result;
    }

}

namespace cpp_utils::graphs {

/**
 * @brief A graph which encodes its edges in an adjacent vector
 * 
 * It contains 2 vectors: one contains the actual edges where each edge with the same source is in a contiguous zone of the vector while
 * the other contains, for each vertex, the index of such contiguous zone.
 * 
 * @tparam G custom payload of the whole graph
 * @tparam V custom payload of each vertex
 * @tparam E custom payload of each edge
 */
template <typename G, typename V, typename E>
class AdjacentGraph: public INonExtendableGraph<G, V, E> {
    using const_vertex_iterator = PairNumberContainerBasedConstIterator<std::vector<V>, nodeid_t, V>;
    using const_edge_iterator = MapNumberContainerBasedConstIterator<std::vector<OutEdge<E>>, OutEdge<E>, Edge<E>>;
private:
    /**
     * @brief the value attached to the whole graph. Owned by the graph itself
     * 
     */
    G payload;
    std::vector<V> vertexPayload;
    std::vector<OutEdge<E>> edges;
    /**
     * @brief vector, as long as AdjacentGraph::vertexPayload representing the index where
     * in AdjacentGraph::edgePayload the edges going out from the particular vertex starts
     * 
     */
    std::vector<int> outEdgesOfvertexBegin;
public:
    friend void cpp_utils::serializers::saveToFile<>(FILE* f, const AdjacentGraph<G,V,E>& g);
    friend AdjacentGraph<G,V,E>& cpp_utils::serializers::loadFromFile<>(FILE* f, AdjacentGraph<G,V,E>& result);
public:
    AdjacentGraph<G,V,E>(): payload{}, vertexPayload{}, edges{}, outEdgesOfvertexBegin{} {

    }
    /**
     * @brief create an empty graph
     * 
     * @param payload value attached to the whole graph
     */
    AdjacentGraph<G,V,E>(const G& payload): payload{payload}, vertexPayload{}, edges{}, outEdgesOfvertexBegin{} {

    }
    /**
     * @brief Construct a new Adjacent Graph< G, V, E> object
     * 
     * @param other another graph. It's mandatory that the ids of `other` are **contiguous** and they start from 0!
     */
    AdjacentGraph<G,V,E>(const IImmutableGraph<G,V,E>& other) : payload{other.getPayload()}, vertexPayload{}, edges{}, outEdgesOfvertexBegin{} {
        info("the payload is ", this->payload);
        init(other);
    }
    AdjacentGraph<G,V,E>(IImmutableGraph<G,V,E>&& other) : payload{other.getPayload()}, vertexPayload{}, edges{}, outEdgesOfvertexBegin{} {
        this->init(other);
    }
    virtual ~AdjacentGraph<G,V,E>() {

    }
public:
    virtual size_t size() const {
        return this->vertexPayload.size();
    }
    virtual size_t numberOfVertices() const {
        return this->vertexPayload.size();
    }
    virtual size_t numberOfEdges() const {
        return this->edges.size();
    }
    /**
     * @brief 
     * 
     * @code
     *  it->first id of vertex
     *  it->second payload of vertex
     * @endcode
     * 
     * @return IImmutableGraph<G,V,E>::const_vertex_iterator 
     */
    virtual typename IImmutableGraph<G,V,E>::const_vertex_iterator beginVertices() const {
        auto it = new AdjacentGraph<G,V,E>::const_vertex_iterator{0, this->vertexPayload};
        return typename IImmutableGraph<G,V,E>::const_vertex_iterator{it};
    }
    virtual typename IImmutableGraph<G,V,E>::const_vertex_iterator endVertices() const {
        auto it = new AdjacentGraph::const_vertex_iterator{-1, this->vertexPayload};
        return typename IImmutableGraph<G,V,E>::const_vertex_iterator{it};
    }
    /**
     * @brief 
     * 
     * @note
     * iteration is slow! Do not use it for performance reasons!
     * 
     * @return IImmutableGraph<G,V,E>::const_edge_iterator 
     */
    virtual typename IImmutableGraph<G,V,E>::const_edge_iterator beginEdges() const {
        const_edge_iterator* it = new const_edge_iterator{const_edge_iterator::cbegin(
            this->edges, 
            [&](int index, const std::vector<OutEdge<E>>& container, const OutEdge<E>& outEdge) -> Edge<E> {
                return Edge<E>{this->getSourceOfOutEdge(index), outEdge};
            }
        )};
        return typename IImmutableGraph<G,V,E>::const_edge_iterator{it};
    }
    virtual typename IImmutableGraph<G,V,E>::const_edge_iterator endEdges() const {
        const_edge_iterator* it = new const_edge_iterator{const_edge_iterator::cend(
            this->edges,
            [&](int index, const std::vector<OutEdge<E>>& container, const OutEdge<E>& outEdge) -> Edge<E> {
                return Edge<E>{this->getSourceOfOutEdge(index), outEdge};
            }
        )};
        return typename IImmutableGraph<G,V,E>::const_edge_iterator{it};
    }
    virtual const V& getVertex(nodeid_t id) const {
        return this->vertexPayload[id];
    }
    virtual const E& getEdge(nodeid_t sourceId, nodeid_t sinkId) const {
        for (auto i=this->outEdgesOfvertexBegin[sourceId]; i<this->outEdgesOfvertexBegin[sourceId+1]; ++i) {
            if (this->edges[i].getSinkId() == sinkId) {
                return this->edges[i].getPayload();
            }
        }
        throw cpp_utils::exceptions::ElementNotFoundException<nodeid_t, AdjacentGraph<G,V,E>>{sinkId, *this};
    }
    virtual const G& getPayload() const {
        return this->payload;
    }
    virtual G& getPayload() {
        return this->payload;
    }
    virtual size_t getInDegree(nodeid_t id) const {
        size_t result = 0;
        debug("new computation!");
        for (auto it=this->beginVertices(); it!=this->endVertices(); ++it) {
            debug("it is ", it->first, "paylaod is ", it->second);
            if (this->hasEdge(it->first, id)) {
                debug("there is an edge from ", id, " to", it->first);
                result += 1;
            }
        }
        return result;
    }
    virtual size_t getOutDegree(nodeid_t id) const {
        return this->outEdgesOfvertexBegin[id+1] - this->outEdgesOfvertexBegin[id];
    }
    virtual size_t getDegree(nodeid_t id) const {
        return this->getOutDegree(id) + this->getInDegree(id);
    }
    virtual bool hasSuccessors(nodeid_t id) const {
        return this->getOutDegree(id) > 0;
    }
    virtual bool hasPredecessors(nodeid_t id) const {
        for (auto sourceId=0; sourceId<this->vertexPayload.size(); ++sourceId) {
            if (sourceId == id) {
                continue;
            }
            for (auto j=0; j<this->getOutDegree(sourceId); ++j) {
                if (this->getOutEdge(sourceId, j).getSinkId() == id) {
                    return true;
                }
            }
        }
        return false;
    }
    virtual OutEdge<E> getOutEdge(nodeid_t id, moveid_t index) const {
        return this->edges[this->outEdgesOfvertexBegin[id] + index];
    }
    virtual bool hasEdge(nodeid_t sourceId, nodeid_t sinkId) const {
        for (auto i=this->outEdgesOfvertexBegin[sourceId]; i<this->outEdgesOfvertexBegin[sourceId+1]; ++i) {
            if (this->edges[i].getSinkId() == sinkId) {
                return true;
            }
        }
        return false;
    }
    virtual std::vector<InEdge<E>> getInEdges(nodeid_t id) const {
        std::vector<InEdge<E>> result{};
    
        for (nodeid_t sourceId=0; sourceId<this->vertexPayload.size(); ++sourceId) {
            if (sourceId == id) {
                continue;
            }
            for (auto j=0; j<this->getOutDegree(sourceId); ++j) {
                if (this->getOutEdge(sourceId, j).getSinkId() == id) {
                    result.push_back(InEdge<E>{sourceId, this->getOutEdge(sourceId, j).getPayload()});
                }
            }
        }
        return result;
    }
    virtual std::vector<OutEdge<E>> getOutEdges(nodeid_t id) const {
        std::vector<OutEdge<E>> result{};
        for (auto i=0; i<this->getOutDegree(id); ++i) {
            result.push_back(this->edges[this->outEdgesOfvertexBegin[id] + i]);
        }

        return result;
    }
    virtual bool isEmpty() const {
        return this->vertexPayload.size() == 0;
    }
    virtual OutEdge<E>& getOutEdge(nodeid_t id, moveid_t index) {
        return this->edges[this->outEdgesOfvertexBegin[id] + index];
    }
    virtual bool containsVertex(nodeid_t id) const {
        return id < this->vertexPayload.size();
    }
    virtual bool containsEdge(nodeid_t sourceId, nodeid_t sinkId, const E& payload) const {
        for (auto i=this->outEdgesOfvertexBegin[sourceId]; i<this->outEdgesOfvertexBegin[sourceId+1]; ++i) {
            if (this->edges[i].getSinkId() == sinkId && this->edges[i].getPayload() == payload) {
                return true;
            }
        }
        return false;
    }
    virtual bool containsEdge(nodeid_t sourceId, nodeid_t sinkId) const {
        for (auto i=this->outEdgesOfvertexBegin[sourceId]; i<this->outEdgesOfvertexBegin[sourceId+1]; ++i) {
            if (this->edges[i].getSinkId() == sinkId) {
                return true;
            }
        }
        return false;
    }
public:
    virtual void changeWeightEdge(nodeid_t sourceId, nodeid_t sinkId, const E& newPayload) {
        for (auto i=this->outEdgesOfvertexBegin[sourceId]; i<this->outEdgesOfvertexBegin[sourceId+1]; ++i) {
            if (this->edges[i].getSinkId() == sinkId) {
                this->edges[i].getPayload() = newPayload;
                return;
            }
        }
        throw cpp_utils::exceptions::ElementNotFoundException<nodeid_t, AdjacentGraph<G,V,E>>{sinkId, *this};
    }
    virtual void changeWeightOutEdge(nodeid_t sourceId, moveid_t index, const E& newPayload) {
        this->edges[this->outEdgesOfvertexBegin[sourceId] + index].getPayload() = newPayload;
    }
public:
    virtual MemoryConsumption getByteMemoryOccupied() const {
        return MemoryConsumption{sizeof(*this), MemoryConsumptionEnum::BYTE};
    }
protected:
    /**
     * @brief retrieve the id of the vertex which is the source of a given OutEdge
     * 
     * @note
     * this operation takes \f$O(n)\f$
     * 
     * @param outEdgeIndex the index of the OutEdge inside the vector edges
     * @return nodeid_t the id of the vertex which is the source of the given OutEdge
     */
    nodeid_t getSourceOfOutEdge(int outEdgeIndex) const {
        for (nodeid_t i=0; i<this->outEdgesOfvertexBegin.size(); ++i) {
            if (this->outEdgesOfvertexBegin[i] > outEdgeIndex) {
                //we have just surpassed th out edge, hence the source was the last one we have visisted
                return i - 1;
            }
        }
        throw cpp_utils::exceptions::ImpossibleException{};
    }
    void init(const IImmutableGraph<G,V,E>& other) {
        //nodes
        for (auto id = 0; id<other.size(); ++id) {
            this->vertexPayload.push_back(other.getVertex(id));
        }
        int edgesNextCell = 0;
        for (auto sourceId=0; sourceId<this->vertexPayload.size(); ++sourceId) {
            this->outEdgesOfvertexBegin.push_back(edgesNextCell);
            for (int i=0; i<other.getOutDegree(sourceId); ++i) {
                this->edges.push_back(OutEdge<E>{other.getOutEdge(sourceId, i)});
                edgesNextCell += 1;
            }
        }
        this->outEdgesOfvertexBegin.push_back(edgesNextCell);
    }
    void init(IImmutableGraph<G,V,E>&& other) {
        //nodes
        for (auto id = 0; id<other.size(); ++id) {
            this->vertexPayload.push_back(std::move(other.getVertex(id)));
        }
        int edgesNextCell = 0;
        for (auto sourceId=0; sourceId<this->vertexPayload.size(); ++sourceId) {
            this->outEdgesOfvertexBegin.push_back(edgesNextCell);
            for (int i=0; i<other.getOutDegree(sourceId); ++i) {
                this->edges.push_back(OutEdge<E>{other.getOutEdge(sourceId, i)});
                edgesNextCell += 1;
            }
        }
        this->outEdgesOfvertexBegin.push_back(edgesNextCell);
    }

};

}

#endif 