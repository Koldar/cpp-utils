#ifndef _ADJACENTGRAPH_HEADER__
#define _ADJACENTGRAPH_HEADER__

#include <vector>
#include "igraph.hpp"
#include "serializers.hpp"
#include "iterator.hpp"
#include "assertions.hpp"

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

    template <typename G, typename V, typename E, typename DEBUG_REF=Edge<E>&, typename DEBUG_PTR=Edge<E>*>
    class AdjacentGraphEdgesIterator: public ::cpp_utils::AbstractConstIterator<Edge<E>&, Edge<E>*> {
        typedef AdjacentGraphEdgesIterator<G, V, E> This;
    private:
        const AdjacentGraph<G, V, E>& graph;
        iterator_state_t state;
        nodeid_t vertex;
        moveid_t move;
        mutable Edge<E> tmp;
    public:
        AdjacentGraphEdgesIterator(iterator_state_t state, nodeid_t vertex, moveid_t move, const AdjacentGraph<G, V, E>& graph): graph{graph}, vertex{vertex}, move{move}, state{state}, tmp{} {
            this->computeNext();
        }
        AdjacentGraphEdgesIterator(const This& o): vertex{o.vertex}, move{o.move}, graph{o.graph}, state{o.state}, tmp{o.tmp} {
            this->computeNext();
        }
        AdjacentGraphEdgesIterator(This&& o): vertex{o.vertex}, move{o.move}, graph{o.graph}, state{o.state}, tmp{::std::move(o.tmp)} {

        }
        virtual ~AdjacentGraphEdgesIterator() {

        }
        This& operator=(const This& o) { 
            this->vertex = o.vertex;
            this->move = o.move;
            this->graph = o.graph;
            this->state = o.state;
            this->tmp = o.tmp;
            return *this;
        }
        This& operator=(This&& o) { 
            this->vertex = o.vertex;
            this->move = o.move;
            this->graph = o.graph;
            this->state = o.state;
            this->tmp = ::std::move(o.tmp);
            return *this;
        }
        This& operator++() { 
            this->computeNext();
            return *this;
        }

        cpp_utils::graphs::Edge<E>& operator*() const {
            debug("vertex is", this->vertex, "move", this->move);
            this->tmp = Edge<E>{this->vertex, this->graph.getOutEdge(this->vertex, this->move)};
            debug("edge is", tmp);
            return tmp;
        }

        cpp_utils::graphs::Edge<E>* operator->() const {
            this->tmp = Edge<E>{this->vertex, this->graph.getOutEdge(this->vertex, this->move)};
            return &tmp;
        }
    protected:
        void computeNext() {
            switch (this->state) {
                case iterator_state_t::ENDED: {
                    return;
                }
                case iterator_state_t::STARTED: {
                    this->move += 1;
                    //no break!
                }
                case iterator_state_t::NOT_INITIALIZED: {
                    //check if the current vertex/move exists
                    while (true) {
                        if (this->vertex >= this->graph.numberOfVertices()) {
                            this->state = iterator_state_t::ENDED;
                            return;
                        }
                        if (this->graph.hasOutMove(this->vertex, this->move)) {
                            this->state = iterator_state_t::STARTED;
                            return;
                        } else {
                            this->vertex += 1;
                            this->move = 0;
                        }
                    }
                    
                    break;
                }
            }
        }
    public:
        bool isEnded() const {
            return this->state == iterator_state_t::ENDED;
        }
    public:
        bool isEqualTo(const AbstractConstIterator<Edge<E>&, Edge<E>*>* o) const {
            auto b = static_cast<const This*>(o);

            if (this->isEnded() && b->isEnded()) {
                return true;
            } else if (this->isEnded() || b->isEnded()) {
                return false;
            } else {
                return this->vertex == b->vertex && this->move == b->move;
            }
        }
    };

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
    public:
        using This = AdjacentGraph<G,V,E>;
        using Super = INonExtendableGraph<G,V,E>;
        using const_vertex_iterator = PairNumberContainerBasedConstIterator<std::vector<V>, nodeid_t, V>;
        using Super::changeVertexPayload;
        friend IImmutableGraph<G,V,E>;
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
        friend void cpp_utils::serializers::saveToFile<>(FILE* f, const This& g);
        friend This& cpp_utils::serializers::loadFromFile<>(FILE* f, This& result);
        friend class AdjacentGraphEdgesIterator<G, V, E>;
    public:
        AdjacentGraph(): payload{}, vertexPayload{}, edges{}, outEdgesOfvertexBegin{} {

        }
        /**
         * @brief create an empty graph
         * 
         * @param payload value attached to the whole graph
         */
        AdjacentGraph(const G& payload): payload{payload}, vertexPayload{}, edges{}, outEdgesOfvertexBegin{} {

        }
        /**
         * @brief Create a graph from the components creating an AdjacentGraph
         * 
         * Use it ony when you know what you are doing. Otherwise, ignore this constructor
         * 
         * @param payload 
         * @param vertexPayload 
         * @param edges 
         * @param outEdgesOfvertexBegin 
         */
        AdjacentGraph(const G& payload, const std::vector<V>& vertexPayload, const std::vector<OutEdge<E>>& edges, const std::vector<int>& outEdgesOfvertexBegin): payload{payload}, vertexPayload{vertexPayload}, edges{edges}, outEdgesOfvertexBegin{outEdgesOfvertexBegin} {
        }
        /**
         * @brief Construct a new Adjacent Graph< G, V, E> object
         * 
         * @param other another graph. It's mandatory that the ids of `other` are **contiguous** and they start from 0!
         */
        AdjacentGraph(const IImmutableGraph<G,V,E>& other) : payload{other.getPayload()}, vertexPayload{}, edges{}, outEdgesOfvertexBegin{} {
            info("the payload is ", this->payload);
            this->init(other);
        }
        AdjacentGraph(IImmutableGraph<G,V,E>&& other) : payload{other.getPayload()}, vertexPayload{}, edges{}, outEdgesOfvertexBegin{} {
            this->init(other);
        }
        /**
         * @brief Build an adjacent graph from a unique pointer. Then it deallocates it
         * 
         * @note
         * this operation is slow
         * 
         * After this operation, the pointer managed by the smart pointer will not refer to anything anymore and the associated memory area will be freed
         * 
         * @param other the unique_pointer whose ownership we need to transfer
         */
        AdjacentGraph(std::unique_ptr<IImmutableGraph<G,V,E>>&& other): payload{other->getPayload()}, vertexPayload{}, edges{}, outEdgesOfvertexBegin{} {
            auto* ptr = other.release();
            this->init(*ptr);
            delete ptr;
        }
        AdjacentGraph(const AdjacentGraph<G, V,E>& other): payload{other.payload}, vertexPayload{other.vertexPayload}, edges{other.edges}, outEdgesOfvertexBegin{other.outEdgesOfvertexBegin} {

        }
        
        AdjacentGraph(AdjacentGraph<G, V,E>&& other): payload{::std::move(other.payload)}, vertexPayload{::std::move(other.vertexPayload)}, edges{::std::move(other.edges)}, outEdgesOfvertexBegin{::std::move(other.outEdgesOfvertexBegin)} {
            
        }
        AdjacentGraph& operator = (const This& o) {
            this->payload = o.payload;
            this->vertexPayload = o.vertexPayload;
            this->edges = o.edges;
            this->outEdgesOfvertexBegin = o.outEdgesOfvertexBegin;
            return *this;
        }
        This& operator = (This&& o) {
            this->payload = ::std::move(o.payload);
            this->vertexPayload = ::std::move(o.vertexPayload);
            this->edges = ::std::move(o.edges);
            this->outEdgesOfvertexBegin = ::std::move(o.outEdgesOfvertexBegin);
            return *this;
        }
        virtual ~AdjacentGraph() {

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
            AdjacentGraphEdgesIterator<G, V, E>* it = new AdjacentGraphEdgesIterator<G, V, E>{iterator_state_t::NOT_INITIALIZED, 0, 0, *this};
            typename IImmutableGraph<G,V,E>::const_edge_iterator result{it};
            return result;
        }
        virtual typename IImmutableGraph<G,V,E>::const_edge_iterator endEdges() const {
            AdjacentGraphEdgesIterator<G, V, E>* it = new AdjacentGraphEdgesIterator<G, V, E>{iterator_state_t::ENDED, 0, 0, *this};
            //const_edge_iterator* it = new const_edge_iterator{const_edge_iterator{true, 0, 0, *this}};
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
    protected:
        virtual nodeid_t getLastVertexId() const {
            return this->vertexPayload.size() - 1;
        }
    public:
        virtual void changeVertexPayload(nodeid_t vertexId, const V& payload) {
            this->vertexPayload[vertexId] = payload;
        }
        virtual void changeWeightEdge(nodeid_t sourceId, nodeid_t sinkId, const E& newPayload) {
            for (auto i=this->outEdgesOfvertexBegin[sourceId]; i<this->outEdgesOfvertexBegin[sourceId+1]; ++i) {
                if (this->edges[i].getSinkId() == sinkId) {
                    this->edges[i].setPayload(newPayload);
                    return;
                }
            }
            throw cpp_utils::exceptions::ElementNotFoundException<nodeid_t, AdjacentGraph<G,V,E>>{sinkId, *this};
        }
        virtual void changeWeightOutEdge(nodeid_t sourceId, moveid_t index, const E& newPayload) {
            assertInRange(0, index, this->getOutDegree(sourceId), true, false);
            this->edges[this->outEdgesOfvertexBegin[sourceId] + index].setPayload(newPayload);
        }
    public:
        virtual MemoryConsumption getByteMemoryOccupied() const {
            return MemoryConsumption{sizeof(*this), MemoryConsumptionEnum::BYTE};
        }
    public:
        // /**
        //  * @brief add an edge
        //  * 
        //  * @pre
        //  *  @li no edges with greater `sourceId` are present in the graph
        //  *  @li the last edge added (if any) needs to have a smaller or at most equal value of soruceId;
        //  * 
        //  * @note
        //  * Be sure to call ::finalizeGraph to stop the add edge procedure. After calling the function, addEdgeTail should **not** be called again
        //  * 
        //  * @param sourceId the id of the source vertex to add
        //  * @param sinkId the id of the sink vertex to add
        //  * @param payload paylaod of the edge
        //  */
        void addEdgeTail(nodeid_t sourceId, nodeid_t sinkId, const E& payload) {
            if (this->outEdgesOfvertexBegin.size() == 0) {
                nodeid_t lastSourceId = 0;
                //we need to add in outEdgesOfvertexBegin the data for the vertices which do not have any outEdges as well!
                for (nodeid_t emptyId=lastSourceId; emptyId<sourceId; ++emptyId) {
                    this->outEdgesOfvertexBegin.push_back(0);
                }
                //just add the new edge
                this->edges.push_back(OutEdge<E>{sinkId, payload});
                this->outEdgesOfvertexBegin.push_back(0);
            } else {
                nodeid_t lastSourceIdHandled = this->outEdgesOfvertexBegin.size() - 1;
                if (lastSourceIdHandled < sourceId) {
                    debug("lastSourceIdHandled=", lastSourceIdHandled, "sourceId", sourceId);
                    //we need to add in outEdgesOfvertexBegin the data for the vertices which do not have any outEdges as well!
                    for (nodeid_t emptyId=(lastSourceIdHandled + 1); emptyId<sourceId; ++emptyId) {
                        debug("padding vertex without outEdges", emptyId);
                        this->outEdgesOfvertexBegin.push_back(this->edges.size());
                    }
                    //add outEdgesOfvertexBegin for sourceId
                    debug("padding sourceId", sourceId);
                    this->outEdgesOfvertexBegin.push_back(this->edges.size());
                    this->edges.push_back(OutEdge<E>{sinkId, payload});
                } else if (lastSourceIdHandled == sourceId) {
                    this->edges.push_back(OutEdge<E>{sinkId, payload});
                } else {
                    log_error("last source id=", lastSourceIdHandled, "current sourceId=", sourceId);
                    throw cpp_utils::exceptions::ImpossibleException{"last source id is greater than sourceId"};
                } 
            }
        }
        void finalizeGraph() {
            debug("in finalize this->outEdgesOfvertexBegin ", this->outEdgesOfvertexBegin);
            debug("in finalize this->edges ", this->edges);

            if (this->vertexPayload.size() > 0) {
                //if graph is empty, we don't have edges, hence we don't need to pad the last for loop
                //we need to add in outEdgesOfvertexBegin the data for the vertices which do not have any outEdges as well!
                nodeid_t lastSourceIdToHandle = 0;
                if (this->outEdgesOfvertexBegin.size() > 0) {
                    lastSourceIdToHandle = this->outEdgesOfvertexBegin.size() - 1;
                } else {
                    lastSourceIdToHandle = 0;
                }
                for (nodeid_t emptyId=lastSourceIdToHandle; emptyId<(this->vertexPayload.size()); ++emptyId) {
                    this->outEdgesOfvertexBegin.push_back(this->edges.size());
                }
            }
            //add final value to have correct semantic of getOutEdge()
            this->outEdgesOfvertexBegin.push_back(this->edges.size());
        }

        long addVertex(const V& vertex) {
            this->vertexPayload.push_back(vertex);
            return this->vertexPayload.size();
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
            debug("adding vertices");
            for (auto id = 0; id<other.size(); ++id) {
                this->vertexPayload.push_back(other.getVertex(id));
            }

            debug("handling edges...");
            for (auto it=other.beginEdges(); it!=other.endEdges(); ++it) {
                debug("hello");
                debug("edge to add is ", *it);
                this->addEdgeTail(it->getSourceId(), it->getSinkId(), it->getPayload());
            }
            this->finalizeGraph();
            debug("outside copy edges!");

            debug("this->outEdgesOfvertexBegin ", this->outEdgesOfvertexBegin);
            debug("this->edges ", this->edges);
        }
        void init(IImmutableGraph<G,V,E>&& other) {
            //nodes
            for (auto id = 0; id<other.size(); ++id) {
                this->vertexPayload.push_back(other.getVertex(id));
                
            }

            for (auto it=other.beginEdges(); it!=other.endEdges(); ++it) {
                debug("edge to add is ", *it);
                this->addEdgeTail(it->getSourceId(), it->getSinkId(), it->getPayload());
            }
            this->finalizeGraph();

            debug("this->outEdgesOfvertexBegin ", this->outEdgesOfvertexBegin);
            debug("this->edges ", this->edges);
        }

    };

}

#endif 