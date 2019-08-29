#ifndef _ADJACENTGRAPH_HEADER__
#define _ADJACENTGRAPH_HEADER__

#include <vector>
#include "igraph.hpp"

namespace cpp_utils::graphs {


template <typename G, typename V, typename E>
class AdjacentGraph: public INonExtendableGraph<G, V, E> {

    class vertex_iterator: public AbstractNumberContainerBasedIterator<IGraph<G,V,E>, std::pair<node_id, V&>, std::pair<node_id, V*>> {
        public:
            vertex_iterator(IGraph<G,V,E>& g) : AbstractNumberContainerBasedIterator<IGraph<G,V,E>, std::pair<node_id, V>&, std::pair<node_id, V>*>{g} { }
            virtual std::pair<node_id, V&> operator*() const {
                return std::pair<node_id, V&>{this->index, this->container[this->index]};
            }
            virtual std::pair<node_id, V*> operator->() const {
                return std::pair<node_id, V*>{this->index, &this->container[this->index]};
            }
    };
    class const_vertex_iterator: public AbstractNumberContainerBasedIterator<IGraph<G,V,E>, std::pair<node_id, V&>, std::pair<node_id, V*>> {
        public:
            const_vertex_iterator(const IGraph<G,V,E>& g) : AbstractNumberContainerBasedIterator<IGraph<G,V,E>, std::pair<int, V>&, std::pair<int, V>*>{g} { }
            virtual std::pair<node_id, V&> operator*() const {
                return std::pair<node_id, V&>{this->index, this->container[this->index]};
            }
            virtual std::pair<node_id, V*> operator->() const {
                return std::pair<node_id, V*>{this->index, &this->container[this->index]};
            }
    };
    
    friend class const_edge_iterator;
    class const_edge_iterator: public AbstractConstIterator<Edge<E>&, Edge<E>*> {
        private:
            const IGraph<G,V,E>& g;
            int edge;
            node_id vertex;
            Edge<E> current;
        public:
        const_edge_iterator(const IGraph<G,V,E>& g, node_id vertex=0, node_id edge=0):  g{g}, edge{edge}, vertex{vertex} {
            computeNextEdge();
        }
        const_edge_iterator<REF,PTR>& operator=(const AbstractConstIterator<REF,PTR>& other) {
            this->g = other.g;
            this->edge = other.edge;
            this->vertex = other.vertex;
            this->current = other.current;
            return *this;
        }
        virtual bool operator==(const AbstractConstIterator<REF,PTR>& other) const {
            return this->g == other.g && this->edge == other.edge && this->vertex == other.vertex;
        }
        virtual bool operator!=(const AbstractConstIterator<REF,PTR>& other) const {
            return !(*this == other);
        }

        virtual void computeNextEdge() {
            //if vertex < 0 we need to start the edges iteration. if vertex > g.size we reached the end
            if (this->vertex >= this->g.size()) {
                this->vertex = this->g.size();
                this->edge = 0;
                return;
            } else if (this->vertex < 0) {
                //start to the beginning
                this->vertex = 0;
                this->edge = 0;
            } else {
                //gio the next possible edge
                this->edge += 1;
            }
            //now the next edge might be inexistent, so go to the next compliant edge
            if (this->edge > this->g.getOutDegree(this->vertex)) {
                //we finished the edges of this vertex. Go to the next one which has at least one out edge
                //if we have no more vertices we return end (-1, -1)
                this->edge = -1;
                while (true) {
                    this->vertex += 1;
                    if (this->vertex >= this->g.size()) {
                        this->vertex = this->g.size();
                        this->edge = 0;
                        return;
                    }
                    if (this->g.hasSuccessors(this->vertex)) {
                        this->edge = 0;
                        break;
                    }
                }
                //no more vertices. return end
                if (this->edge == -1) {
                    this->vertex = -1;
                }
            }
            if (this->edge != -1) {
                this->current = Edge<E>{
                    this->vertex, 
                    this->g.getOutEdge(this->vertex, this->edge).getSinkId(), 
                    this->g.getOutEdge(this->vertex, this->edge).getPayload()
                };
            }
        }
        virtual ContextBasedIterator<CONTEXT, STATE, REF, PTR>& operator++() {
            this->computeNextEdge();
            return *this;
        }
        virtual Edge<E>& operator*() const {
            return this->current;
        }
        virtual Edge<E>* operator->() const {
            return &this->current;
        }
    };

public:
    virtual AdjacentGraph<G,V,E>(const IImmutableGraph& other) : payload{other.payload}, vertexPayload{}, edges{}, outEdgesOfvertexBegin{} {
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
        return this->edgePayload.size();
    }
    virtual typename IGraph<G,V,E>::const_vertex_iterator beginVertices() const {
        return AdjacentGraph<G,V,E>::const_vertex_iterator{0, this->vertexPayload};
    }
    virtual typename IGraph<G,V,E>::const_vertex_iterator endVertices() const {
        return AdjacentGraph::const_vertex_iterator{-1, *this->vertexPayload};
    }
    virtual typename IGraph<G,V,E>::const_edge_iterator beginEdges() const {
        return AdjacentGraph
        return AdjacentGraph::const_edge_iterator{*this, -1, 0};
    }
    virtual typename IGraph<G,V,E>::const_edge_iterator endEdges() const {
        return AdjacentGraph<G,V,E>::const_edge_iterator{*this, this->size(), 0};
    }
public:
    virtual void changeWeightEdge(node_id sourceId, node_id sinkId, const E& newPayload) {
        for (auto i=this->outEdgesOfvertexBegin[sourceId]; i<this->outEdgesOfvertexBegin[sourceId+1]; ++i) {
            if (this->edges[i].getSinkId() == sinkId) {
                return this->edges[i] = newPayload;
            }
        }
        throw ElementNotFoundException<node_id, AdjacentGraph<G,V,E>>{sinkId, *this};
    }
    virtual void changeWeightOutEdge(node_id sourceId, int index, const E& newPayload) {
        this->edges[this->outEdgesOfvertexBegin[sourceId] + index] = newPayload;
    }
public:
    virtual const V& getVertex(node_id id) const {
        return this->vertexPayload[id];
    }
    virtual const E& getEdge(node_id sourceId, node_id sinkId) const {
        for (auto i=this->outEdgesOfvertexBegin[sourceId]; i<this->outEdgesOfvertexBegin[sourceId+1]; ++i) {
            if (this->edges[i].getSinkId() == sinkId) {
                return this->edges[i].getPayload();
            }
        }
        throw ElementNotFoundException<node_id, AdjacentGraph<G,V,E>>{sinkId, *this};
    }
    virtual size_t getInDegree(node_id id) const {
        size_t result = 0;
    for (auto it=this->beginVertices(); it!=this->endVertices(); ++it) {
        if (it->first == id) {
            continue;
        }
        if (this->hasEdge(id, it->first) {
            result += 1;
        }
    }
    return result;
    }
    virtual size_t getOutDegree(node_id id) const {
        return this->outEdgesOfvertexBegin[id+1] - this->outEdgesOfvertexBegin[id];
    }
    virtual size_t getDegree(node_id id) const {
        return this->getOutDegree(id) + this->getInDegree(id);
    }
    virtual bool hasSuccessors(node_id id) const {
        return this->getOutDegree() > 0;
    }
    virtual bool hasPredecessors(node_id id) const {
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
    virtual const OutEdge<E>& getOutEdge(node_id id, int index) const {
        return this->edges[this->outEdgesOfvertexBegin[id] + index];
    }
    virtual OutEdge<E>& getOutEdge(node_id id, int index) {
        return this->edges[this->outEdgesOfvertexBegin[id] + index];
    }
    virtual bool hasEdge(node_id sourceId, node_id sinkId) const {
        for (auto i=this->outEdgesOfvertexBegin[sourceId]; i<this->outEdgesOfvertexBegin[sourceId+1]; ++i) {
        if (this->edges[i].getSinkId() == sinkId) {
            return true;
        }
    }
    return false;
    }
    virtual std::vector<InEdge<E>> getInEdges(node_id id) const {
        std::vector<InEdge<E>> result{};
    
        for (auto sourceId=0; sourceId<this->vertexPayload.size(); ++sourceId) {
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
    virtual std::vector<OutEdge<E>> getOutEdges(node_id id) const {
        std::vector<OutEdge<E>> result{};
        for (auto i=0; i<this->getOutDegree(id); ++i) {
            result.push_back(this->edges[this->outEdgesOfvertexBegin[id] + i]);
        }

        return result;
    }
    virtual bool isEmpty() const {
        return this->vertexPayload.size() == 0;
    }
private:
    G payload;
    std::vector<V> vertexPayload;
    std::vector<OutEdge<E>> edges;
    /**
     * @brief vector, as long as AdjacentGraph::vertexPayload representing the index where
     * in AdjacentGraph::edgePayload the edges going out from the particular vertex starts
     * 
     */
    std::vector<int> outEdgesOfvertexBegin;
};

}

#endif 