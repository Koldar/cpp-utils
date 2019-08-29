#ifndef _LISTGRPAH_HEADER__
#define _LISTGRPAH_HEADER__

#include <algorithm>
#include <vector>
#include "igraph.hpp"

namespace cpp_utils::graphs {

template <typename G, typename V, typename E>
class ListGraph : public IVertexExtendableGraph<G,V,E> {
public:
    using const_vertex_iterator = PairNumberContainerBasedConstIterator<std::vector<V>, node_id, V>;
    using const_edge_iterator = DefaultNumberContainerBasedConstIterator<std::vector<Edge<E>>, Edge<E>>;
private:
    std::vector<V> vertexPayload;
    std::vector<Edge<E>> edges;
    G payload;
public:
    ListGraph(const G& g): edges{}, vertexPayload{}, payload{g} {

    }
    ListGraph(const IImmutableGraph<G,V,E>& other) : ListGraph{} {
        for (auto it=other.beginVertices(); it!=other.endVertices(); ++it) {
            this->addVertex(it->first, it->second);
        }
        for (auto it=other.beginEdges(); it!=other.endEdges(); ++it) {
            this->addEdge(it->getSourceid(), it->getSinkId(), it->getPayload());
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
        critical("vertex paylaod is long", this->vertexPayload.size());
        auto val = const_vertex_iterator::cend(this->vertexPayload);
        critical("val index is ", val.index);
        const_vertex_iterator* it = new const_vertex_iterator{val};
        critical("it index is ", it->index);

        return typename IImmutableGraph<G,V,E>::const_vertex_iterator{it};
    }

    virtual typename IImmutableGraph<G,V,E>::const_edge_iterator beginEdges() const {
        info("calling beginEdges...");
        const_edge_iterator* it = new const_edge_iterator{const_edge_iterator::cbegin(this->edges)};
        return typename IImmutableGraph<G,V,E>::const_edge_iterator{it};
    }
    virtual typename IImmutableGraph<G,V,E>::const_edge_iterator endEdges() const {
        info("calling endEdges...");
        const_edge_iterator* it = new const_edge_iterator{const_edge_iterator::cend(this->edges)};
        return typename IImmutableGraph<G,V,E>::const_edge_iterator{it};
    }
    virtual const V& getVertex(node_id id) const {
        return this->vertexPayload[id];
    }
    virtual const E& getEdge(node_id sourceId, node_id sinkId) const {
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->isCompliant(sourceId, sinkId)) {
                return (*it).getPayload();
            }
        }
        throw exceptions::ElementNotFoundException<std::pair<node_id, node_id>, const G&>{std::pair<node_id, node_id>{sourceId, sinkId}, this->getPayload()};
    }

    virtual const G& getPayload() const {
        return this->payload;
    }
    virtual G& getPayload() {
        return this->payload;
    }

    virtual size_t getInDegree(node_id id) const {
        size_t result = 0;
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSinkId() == id) {
                result += 1;
            }
        }
        return result;
    }
    virtual size_t getOutDegree(node_id id) const {
        size_t result = 0;
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSourceId() == id) {
                result += 1;
            }
        }
        return result;
    }
    virtual size_t getDegree(node_id id) const {
        size_t result = 0;
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSinkId() == id || it->getSourceId() == id) {
                result += 1;
            }
        }
        return result;
    }
    virtual bool hasSuccessors(node_id id) const {
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSourceId() == id) {
                return true;
            }
        }
        return false;
    }
    virtual bool hasPredecessors(node_id id) const {
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSinkId() == id) {
                return true;
            }
        }
        return false;
    }
    virtual OutEdge<E> getOutEdge(node_id id, int index) const {
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->getSourceId() == id) {
                if (index == 0) {
                    return OutEdge<E>{it->getSinkId(), it->getPayload()};
                } else {
                    index -= 1;
                }
            }
        }
        throw exceptions::ElementNotFoundException<std::pair<node_id, int>, const G&>{std::pair<node_id, node_id>{id, index}, this->getPayload()};
    }
    
    virtual bool hasEdge(node_id sourceId, node_id sinkId) const {
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->isCompliant(sourceId, sinkId)) {
                return true;
            }
        }
        return false;
    }
    virtual std::vector<InEdge<E>> getInEdges(node_id id) const {
        std::vector<InEdge<E>> result{};
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->hasSink(id)) {
                result.push_back(InEdge<E>{*it});
            }
        }
        return result;
    }
    virtual std::vector<OutEdge<E>> getOutEdges(node_id id) const {
        std::vector<OutEdge<E>> result{};
        for (auto it=this->beginEdges(); it!=this->endEdges(); ++it) {
            if (it->hasSource(id)) {
                result.push_back(OutEdge<E>{*it});
            }
        }
        return result;
    }
    virtual bool isEmpty() const {
        return this->size() == 0;
    }
public:
    virtual void changeWeightEdge(node_id sourceId, node_id sinkId, const E& newPayload) {
        for (auto i=0; i<this->edges.size(); ++i) {
            if (this->edges[i].isCompliant(sourceId, sinkId)) {
                this->edges[i].setPayload(newPayload);
            }
        }
    }
    virtual void changeWeightOutEdge(node_id sourceId, int index, const E& newPayload) {
        for (auto i=0; i<this->edges.size(); ++i) {
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
    virtual node_id addVertex(const V& payload) {
        this->vertexPayload.push_back(payload);
        return this->vertexPayload.size() - 1;
    }
    virtual void addEdge(node_id sourceId, node_id sinkId, const E& payload) {
        this->edges.push_back(Edge<E>{sourceId, sinkId, payload});
    }
    virtual void removeEdge(node_id sourceId, node_id sinkId) {
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