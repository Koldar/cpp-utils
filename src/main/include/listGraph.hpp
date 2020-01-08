#ifndef _LISTGRAPH_HEADER__
#define _LISTGRAPH_HEADER__

#include <algorithm>
#include <vector>
#include "igraph.hpp"

namespace cpp_utils::graphs {

/**
 * @brief A list graph is a graph where each vertex and edges are stored in a vector
 * 
 * vertex index is the id of the vertex.
 * edges are sorted in a vector such that:
 *  - edges with sourceId comes first
 *  - edges with the same sourceId are unsorted;
 * 
 * @tparam G 
 * @tparam V 
 * @tparam E 
 */
template <typename G, typename V, typename E>
class ListGraph : public IVertexExtendableGraph<G,V,E> {
public:
    using const_vertex_iterator = PairNumberContainerBasedConstIterator<std::vector<V>, nodeid_t, V>;
private:
    std::vector<V> vertexPayload;
    std::vector<Edge<E>> edges;
    G payload;
public:
    ListGraph(const G& g): edges{}, vertexPayload{}, payload{g} {

    }
    ListGraph(const IImmutableGraph<G,V,E>& other) : ListGraph{other.getPayload()} {
        for (auto it=other.beginVertices(); it!=other.endVertices(); ++it) {
            nodeid_t id = this->addVertex(it->second);
            if (id != it->first) {
                throw cpp_utils::exceptions::InvalidArgumentException{"other graph do not start from node 0 or its ids are not contiguous!"};
            }
        }
        for (auto it=other.beginEdges(); it!=other.endEdges(); ++it) {
            this->addEdge(it->getSourceId(), it->getSinkId(), it->getPayload());
        }
    }
    virtual ~ListGraph() {

    }
    virtual size_t size() const {
        return this->vertexPayload.size();
    }
    virtual size_t numberOfVertices() const {
        return this->vertexPayload.size();
    }
    virtual size_t numberOfEdges() const {
        return this->edges.size();
    }
    virtual typename IImmutableGraph<G,V,E>::const_vertex_iterator beginVertices() const {
        const_vertex_iterator* it = new const_vertex_iterator{const_vertex_iterator::cbegin(this->vertexPayload)};
        return typename IImmutableGraph<G,V,E>::const_vertex_iterator{it};
    }
    virtual typename IImmutableGraph<G,V,E>::const_vertex_iterator endVertices() const {
        debug("vertex paylaod is long", this->vertexPayload.size());
        auto val = const_vertex_iterator::cend(this->vertexPayload);
        debug("val index is ", val.index);
        const_vertex_iterator* it = new const_vertex_iterator{val};
        debug("it index is ", it->index);

        return typename IImmutableGraph<G,V,E>::const_vertex_iterator{it};
    }

    virtual typename IImmutableGraph<G,V,E>::const_edge_iterator beginEdges() const {
        debug("calling beginEdges...");
        return typename IImmutableGraph<G,V,E>::const_edge_iterator{new DefaultNumberContainerBasedConstIterator<std::vector<Edge<E>>, Edge<E>>{0, this->edges}};
    }
    virtual typename IImmutableGraph<G,V,E>::const_edge_iterator endEdges() const {
        debug("calling endEdges...");
        return typename IImmutableGraph<G,V,E>::const_edge_iterator{new DefaultNumberContainerBasedConstIterator<std::vector<Edge<E>>, Edge<E>>{-1, this->edges}};
    }
    virtual const V& getVertex(nodeid_t id) const {
        return this->vertexPayload[id];
    }
    virtual const E& getEdge(nodeid_t sourceId, nodeid_t sinkId) const {
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSourceId() > sourceId) {
                //since in the list graph the edges are sorted from sourceId, as soon we find an edge which sourceId is greater than id
                // we can safely stop
                break;
            }
            if (it->isCompliant(sourceId, sinkId)) {
                return (*it).getPayload();
            }
        }
        throw exceptions::ElementNotFoundException<std::pair<nodeid_t, nodeid_t>, const G&>{std::pair<nodeid_t, nodeid_t>{sourceId, sinkId}, this->getPayload()};
    }

    virtual const G& getPayload() const {
        return this->payload;
    }
    virtual G& getPayload() {
        return this->payload;
    }

    virtual size_t getInDegree(nodeid_t id) const {
        size_t result = 0;
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSinkId() == id) {
                result += 1;
            }
        }
        return result;
    }
    virtual size_t getOutDegree(nodeid_t id) const {
        size_t result = 0;
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSourceId() > id) {
                //since in the list graph the edges are sorted from sourceId, as soon we find an edge which sourceId is greater than id
                // we can safely stop
                break;
            }
            if (it->getSourceId() == id) {
                result += 1;
            }
        }
        return result;
    }
    virtual size_t getDegree(nodeid_t id) const {
        size_t result = 0;
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSinkId() == id || it->getSourceId() == id) {
                result += 1;
            }
        }
        return result;
    }
    virtual bool hasSuccessors(nodeid_t id) const {
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSourceId() > id) {
                //since in the list graph the edges are sorted from sourceId, as soon we find an edge which sourceId is greater than id
                // we can safely stop
                break;
            }
            if (it->getSourceId() == id) {
                return true;
            }
        }
        return false;
    }
    virtual bool hasPredecessors(nodeid_t id) const {
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSinkId() == id) {
                return true;
            }
        }
        return false;
    }
    virtual OutEdge<E> getOutEdge(nodeid_t id, moveid_t index) const {
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSourceId() > id) {
                //since in the list graph the edges are sorted from sourceId, as soon we find an edge which sourceId is greater than id
                // we can safely stop
                break;
            }
            if (it->getSourceId() == id) {
                if (index == 0) {
                    return OutEdge<E>{it->getSinkId(), it->getPayload()};
                } else {
                    index -= 1;
                }
            }
        }
        throw exceptions::ElementNotFoundException<std::pair<nodeid_t, int>, const G&>{std::pair<nodeid_t, nodeid_t>{id, index}, this->getPayload()};
    }
    
    virtual bool hasEdge(nodeid_t sourceId, nodeid_t sinkId) const {
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSourceId() > sourceId) {
                //since in the list graph the edges are sorted from sourceId, as soon we find an edge which sourceId is greater than id
                // we can safely stop
                break;
            }
            if (it->isCompliant(sourceId, sinkId)) {
                return true;
            }
        }
        return false;
    }
    virtual std::vector<InEdge<E>> getInEdges(nodeid_t id) const {
        std::vector<InEdge<E>> result{};
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->hasSink(id)) {
                result.push_back(InEdge<E>{*it});
            }
        }
        return result;
    }
    virtual std::vector<OutEdge<E>> getOutEdges(nodeid_t id) const {
        std::vector<OutEdge<E>> result{};
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSourceId() > id) {
                //since in the list graph the edges are sorted from sourceId, as soon we find an edge which sourceId is greater than id
                // we can safely stop
                break;
            }
            if (it->hasSource(id)) {
                result.push_back(OutEdge<E>{*it});
            }
        }
        return result;
    }
    virtual bool isEmpty() const {
        return this->size() == 0;
    }
    virtual bool containsVertex(nodeid_t id) const {
        return id < this->size();
    }
    virtual bool containsEdge(nodeid_t sourceId, nodeid_t sinkId, const E& payload) const {
        for (auto i=0; i<this->edges.size(); ++i) {
            if (this->edges[i].getSourceId() > sourceId) {
                //since in the list graph the edges are sorted from sourceId, as soon we find an edge which sourceId is greater than id
                // we can safely stop
                break;
            }
            if (this->edges[i].isCompliant(sourceId, sinkId) && this->edges[i].getPayload() == payload) {
                return true;
            }
        }
        return false;
    }
    //FIXME remove HasEdge from the interface... containsEdge is the same!
    virtual bool containsEdge(nodeid_t sourceId, nodeid_t sinkid) const {
        for (auto i=0; i<this->edges.size(); ++i) {
            if (this->edges[i].getSourceId() > sourceId) {
                //since in the list graph the edges are sorted from sourceId, as soon we find an edge which sourceId is greater than id
                // we can safely stop
                break;
            }
            if (this->edges[i].isCompliant(sourceId, sinkid)) {
                return true;
            }
        }
        return false;
    }
public:
    virtual void changeVertexPayload(nodeid_t vertexId, const V& payload) {
        this->vertexPayload[vertexId] = payload;
    }
    virtual void changeWeightEdge(nodeid_t sourceId, nodeid_t sinkId, const E& newPayload) {
        for (auto i=0; i<this->edges.size(); ++i) {
            if (this->edges[i].getSourceId() > sourceId) {
                //since in the list graph the edges are sorted from sourceId, as soon we find an edge which sourceId is greater than id
                // we can safely stop
                break;
            }
            if (this->edges[i].isCompliant(sourceId, sinkId)) {
                this->edges[i].setPayload(newPayload);
            }
        }
    }
    virtual void changeWeightOutEdge(nodeid_t sourceId, moveid_t index, const E& newPayload) {
        for (auto i=0; i<this->edges.size(); ++i) {
            if (this->edges[i].getSourceId() > sourceId) {
                //since in the list graph the edges are sorted from sourceId, as soon we find an edge which sourceId is greater than id
                // we can safely stop
                break;
            }
            if (this->edges[i].hasSource(sourceId)) {
                if (index == 0) {
                    this->edges[i].setPayload(newPayload);
                } else {
                    index -= 1;
                }
            }
        }
    }
public:
    virtual nodeid_t addVertex(const V& payload) {
        this->vertexPayload.push_back(payload);
        return this->vertexPayload.size() - 1;
    }
    virtual void addEdge(nodeid_t sourceId, nodeid_t sinkId, const E& payload) {
        if ((this->edges.size() > 0) && (this->edges.back().getSourceId() > sourceId)) {
            log_error("trying to add edge ", sourceId, "->", sinkId, "on graph with #edges=", this->edges.size(), "but the last edge has source", this->edges.back().getSourceId());
            throw cpp_utils::exceptions::ImpossibleException{"source ids are not compliant with ListGraph invariant"};
        }
        this->edges.push_back(Edge<E>{sourceId, sinkId, payload});
    }
    virtual void removeEdge(nodeid_t sourceId, nodeid_t sinkId) {
        this->edges.erase(std::remove_if(this->edges.begin(), this->edges.end(), [&sourceId,&sinkId](const Edge<E>& x) {
            return x.isCompliant(sourceId, sinkId);
        }));
    }
    virtual void removeAllEdges() {
        this->edges.clear();
    }
public:
    MemoryConsumption getByteMemoryOccupied() const {
        return MemoryConsumption{sizeof(*this), MemoryConsumptionEnum::BYTE};
    }
};

}

#endif