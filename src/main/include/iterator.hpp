#ifndef _ITERATOR_HEADER__
#define _ITERATOR_HEADER__

#include <functional>
#include <memory>
#include "varHolder.hpp"

namespace cpp_utils {

    template<typename CONTAINER, typename REF, typename PTR>
    class AbstractNumberContainerBasedConstIterator;

    template <typename REF, typename PTR>
    class ProxyConstIterator;

    template <typename REF, typename PTR>
    bool operator == (const ProxyConstIterator<REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b);

    template <typename REF, typename PTR>
    bool operator != (const ProxyConstIterator<REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b);

    template<typename CONTAINER, typename REF, typename PTR>
    bool operator == (const ProxyConstIterator<REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b);

    template<typename CONTAINER, typename REF, typename PTR>
    bool operator != (const ProxyConstIterator<REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b);

    template<typename CONTAINER, typename REF, typename PTR>
    bool operator == (const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b);

    template<typename CONTAINER, typename REF, typename PTR>
    bool operator != (const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b);

    template<typename CONTAINER, typename REF, typename PTR>
    bool operator == (const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b);

    template<typename CONTAINER, typename REF, typename PTR>
    bool operator != (const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b);

    /**
     * @brief the possible state an iterator might be
     * 
     */
    enum class iterator_state_t {
        /**
         * @brief an iterator has been constructed but it still doesn't point to an actual element.
         * 
         * This usually implies we need to call something like `computeNextItem` to generate a valid item
         */
        NOT_INITIALIZED,
        /**
         * @brief the iterator is pointing to an actual element
         * 
         * This means the iterator can be used to iterate things
         */
        STARTED,
        /**
         * @brief the iterator has ended
         * 
         * This means the iterator has iterated over all the elements and now it's pointing to no element altogether
         * 
         */
        ENDED
    };

    /**
     * @brief an iterator which doesn't allow modifications while iteration on the iterated structure
     * 
     * @tparam REF return type (automatically consted) of operator *
     * @tparam PTR  return type (automatically consted) of operator -> 
     */
    template<typename REF, typename PTR>
    class AbstractConstIterator {
        public:
            AbstractConstIterator<REF,PTR>& operator++(int) = delete;
            virtual AbstractConstIterator<REF,PTR>& operator++() = 0;

            virtual REF operator* () const = 0;
            virtual PTR operator->() const = 0;

            virtual bool isEnded() const = 0;
        public:
            virtual bool isEqualTo(const AbstractConstIterator<REF,PTR>* o) const = 0;
    };

    template<typename REF, typename PTR>
    class ConstIteratorWrapper : public AbstractConstIterator<REF, PTR> {
        typedef ConstIteratorWrapper<REF, PTR> ConstIteratorWrapperInstance;
    private:
        AbstractConstIterator<REF, PTR>* it;
    public:
        ConstIteratorWrapper(AbstractConstIterator<REF, PTR>* it): it{it} {

        }
        ConstIteratorWrapper(const ConstIteratorWrapperInstance& o) = delete;
        ConstIteratorWrapper(ConstIteratorWrapperInstance&& o): it{it} {
            o.it = nullptr;
        }
        ~ConstIteratorWrapper() {
            delete this->it;
        }
        ConstIteratorWrapperInstance& operator = (const ConstIteratorWrapperInstance& o) = delete;
        ConstIteratorWrapperInstance& operator = (ConstIteratorWrapperInstance&& o) {
            this->it = o.it;
            o.it = nullptr;
            return *this;
        }
    public:
        virtual AbstractConstIterator<REF,PTR>& operator=(const AbstractConstIterator<REF,PTR>& other) {
            this->it->operator=(other);
            return *this;
        }
        virtual AbstractConstIterator<REF,PTR>& operator++() {
            return it->operator++();
        }

        virtual REF operator* () const {
            return it->operator*();
        }
        virtual PTR operator->() const {
            return it->operator->();
        }

        virtual bool isEnded() const {
            return it->isEnded();
        }
    public:
        virtual bool isEqualTo(const AbstractConstIterator<REF, PTR>* o) const {
            return this->it->isEqualTo(o);
        }
    public:
        friend bool operator ==(const ConstIteratorWrapperInstance& a, const ConstIteratorWrapperInstance& b) {
            return a.isEqualTo(b.it);
        }
        friend bool operator !=(const ConstIteratorWrapperInstance& a, const ConstIteratorWrapperInstance& b) {
            return !a.isEqualTo(b.it);
        }
    };

    //TODO this should be called AbstractContainerBasedConstIterator
    /**
     * @brief a const iterator that iterate over an underlying container
     * 
     * @tparam CONTAINER the type of the underlying container we iterate over
     * @tparam REF return type (automatically consted) of operator *
     * @tparam PTR return type (automatically consted) of operator ->  
     */
    template<typename CONTAINER, typename REF, typename PTR>
    class AbstractNumberContainerBasedConstIterator : public AbstractConstIterator<REF, PTR> {
        typedef AbstractNumberContainerBasedConstIterator<CONTAINER, REF, PTR> AbstractNumberContainerBasedConstIteratorInstance;
    public:
        /**
         * @brief the index pointing to the element we should return during the iteration
         * 
         * If less than 0 it means we have reached the end of the iterator
         * 
         */
        int index;
        const CONTAINER& container;
    public:
        /**
         * @brief Construct a new Number Container Based Iterator object
         * 
         * 
         * @param index index representing the element where the iteration point. Does not necessarly
         *  represents a valid element. If negative we will start counting from the last element (plus 1).
         *  For example to represent end() iterator just use -1.
         * @param container 
         */
        AbstractNumberContainerBasedConstIterator(int index, const CONTAINER& container): index{index}, container{container} {

            if (container.size() == 0) {
                //empty container
                this->index = -1;
            } else if (index < 0) {
                //index from tail
                this->index = static_cast<int>(container.size()) + 1 + index;
            }
            //if index is out of bounds
            if (this->index < 0) {
                this->index = -1;
            }
            if (this->index >= container.size()) {
                this->index = -1;
            }
        }
        AbstractNumberContainerBasedConstIterator(const AbstractNumberContainerBasedConstIteratorInstance& other) : index{other.index}, container{other.container} {

        }
        AbstractNumberContainerBasedConstIterator(AbstractNumberContainerBasedConstIteratorInstance&& other) : index{other.index}, container{other.container} {

        }
        AbstractNumberContainerBasedConstIteratorInstance& operator =(const AbstractNumberContainerBasedConstIteratorInstance& o) {
            if (this->container != o.container) {
                throw cpp_utils::exceptions::InvalidArgumentException{"the 2 containers do not match!"};
            }
            this->index = o.index;
            this->container = o.container;
            return *this;
        }
        AbstractNumberContainerBasedConstIteratorInstance& operator =(AbstractNumberContainerBasedConstIteratorInstance&& o) {
            if (this->container != o.container) {
                throw cpp_utils::exceptions::InvalidArgumentException{"the 2 containers do not match!"};
            }
            this->index = o.index;
            this->container = o.container;
            return *this;
        }
        ~AbstractNumberContainerBasedConstIterator() {

        }
    public:
        virtual bool isEnded() const {
            return this->index < 0;
        }
        virtual AbstractNumberContainerBasedConstIteratorInstance& operator++() {
            if (this->isEnded()) {
                return *this;
            }
            this->index += 1;
            if (this->index >= this->container.size()){
                this->index = -1;
            }
            return *this;
        }
    public:
        bool isEqualTo(const AbstractConstIterator<REF,PTR>* o) const {
            debug("checking if abstract const iterator is equal to container number iterator...");
            auto o2 = static_cast<const AbstractNumberContainerBasedConstIteratorInstance*>(o);

            if (&(this->container) != &(o2->container)) {
                throw cpp_utils::exceptions::ImpossibleException{"container differs!"};
            }
            debug("done!", this->index, o2->index, this->index == o2->index);

            return this->index == o2->index;
        }
    };

    /**
     * @brief A default implementation of the AbstractNumberContainerBasedIterator
     * 
     * @tparam CONTAINER a container
     * @tparam TYPE type inside the container
     */
    template<typename CONTAINER, typename TYPE>
    class DefaultNumberContainerBasedConstIterator: public AbstractNumberContainerBasedConstIterator<CONTAINER, TYPE&, TYPE*> {
        typedef DefaultNumberContainerBasedConstIterator<CONTAINER, TYPE> DefaultNumberContainerBasedConstIteratorInstance;
        typedef AbstractNumberContainerBasedConstIterator<CONTAINER, TYPE&, TYPE*> Super;
    public:
        DefaultNumberContainerBasedConstIterator(int index, const CONTAINER& container) : Super{index, container} {

        }
        DefaultNumberContainerBasedConstIterator(const DefaultNumberContainerBasedConstIteratorInstance& o): Super{o} {

        }
        DefaultNumberContainerBasedConstIterator(DefaultNumberContainerBasedConstIteratorInstance&& o): Super{::std::move(o)} {

        }
        virtual ~DefaultNumberContainerBasedConstIterator() {

        }
        DefaultNumberContainerBasedConstIteratorInstance& operator=(const DefaultNumberContainerBasedConstIteratorInstance& other) {
            Super::operator =(other);
            return *this;
        }
        DefaultNumberContainerBasedConstIteratorInstance& operator=(DefaultNumberContainerBasedConstIteratorInstance&& other) {
            Super::operator =(::std::move(other));
            return *this;
        }
    public:
        virtual TYPE& operator* () const {
            debug("value is, ", this->index, " ref value is", this->container[this->index]);
            return (TYPE&)this->container[this->index];
        }
        virtual TYPE* operator->() const {
            debug("ptr value is", this->container[this->index]);
            return (TYPE*)&this->container[this->index];
        }
    };

    /**
     * @brief An iterator based on a container which yields not the value in the container, but a function of it.
     * 
     * The function used to map teach container value into something else is of type ::MapNumberContainerBasedConstIterator::map_function_t.
     * 
     * @tparam CONTAINER the type representing the container
     * @tparam INTYPE the type which the container is holding
     * @tparam OUTTYPE the type that this iterator needs to return
     */
    template <typename CONTAINER, typename INTYPE, typename OUTTYPE>
    class MapNumberContainerBasedConstIterator: public AbstractNumberContainerBasedConstIterator<CONTAINER, const OUTTYPE&, const OUTTYPE*> {
        typedef MapNumberContainerBasedConstIterator<CONTAINER, INTYPE,OUTTYPE> MapNumberContainerBasedConstIteratorInstance;
        typedef AbstractNumberContainerBasedConstIterator<CONTAINER, const OUTTYPE&, const OUTTYPE*> Super;
        /**
         * @brief mapping function
         * 
         * @param index the index of the element to convert
         * @param container the container holding the element
         * @param element the element to be converted
         * @return the converted value
         */
        using map_function_t = std::function<OUTTYPE(int, const CONTAINER&, const INTYPE&)>;
    protected:
        map_function_t mapFunction;
        /**
         * @brief temporary place where to store the output of mapFunction, in order to have a `lvalue`
         * 
         */
        mutable VarHolder<OUTTYPE> tmp;
        /**
         * @brief true if the value in `tmp` is the good value for the index `index`. False if we need to call mapFunction to generate it
         * 
         */
        mutable bool isTmpSet;
    public:
        MapNumberContainerBasedConstIterator(int index, const CONTAINER& container, map_function_t mapFunction) : Super{index, container}, mapFunction{mapFunction}, isTmpSet{false} {
        }
        virtual ~MapNumberContainerBasedConstIterator() {

        }
        MapNumberContainerBasedConstIterator(const MapNumberContainerBasedConstIteratorInstance& o): Super{o}, mapFunction{o.mapFunction}, tmp{tmp}, isTmpSet{o.isTmpSet} {

        }
        MapNumberContainerBasedConstIterator(MapNumberContainerBasedConstIteratorInstance&& o): Super{::std::move(o)}, mapFunction{o.mapFunction}, tmp{tmp}, isTmpSet{o.isTmpSet}{

        }
        MapNumberContainerBasedConstIteratorInstance& operator=(const MapNumberContainerBasedConstIteratorInstance& other) {
            Super::operator =(other);
            this->mapFunction = other.mapFunction;
            this->tmp = other.tmp;
            this->isTmpSet = other.isTmpSet;
            return *this;
        }
        MapNumberContainerBasedConstIteratorInstance& operator=(MapNumberContainerBasedConstIteratorInstance&& other) {
            Super::operator =(::std::move(other));
            this->mapFunction = other.mapFunction;
            this->tmp = other.tmp;
            this->isTmpSet = other.isTmpSet;
            return *this;
        }
        virtual OUTTYPE& operator* () const {
            if (!isTmpSet) {
                this->tmp = this->mapFunction(this->index, this->container, this->container[this->index]);
                this->isTmpSet = true;
            }
            
            return this->tmp;
        }
        virtual OUTTYPE* operator->() const {
            if (!isTmpSet) {
                this->tmp = this->mapFunction(this->index, this->container, this->container[this->index]);
                this->isTmpSet = true;
            }
            return &this->tmp;
        }
    public:
        virtual Super& operator++() {
            this->isTmpSet = false;
            return Super::operator++();
        }
    public:
        static MapNumberContainerBasedConstIterator<CONTAINER, INTYPE, OUTTYPE> cbegin(const CONTAINER& container, map_function_t mapFunction) {
            return MapNumberContainerBasedConstIterator<CONTAINER, INTYPE, OUTTYPE>{0, container, mapFunction};
        }
        static MapNumberContainerBasedConstIterator<CONTAINER, INTYPE, OUTTYPE> cend(const CONTAINER& container, map_function_t mapFunction) {
            return MapNumberContainerBasedConstIterator<CONTAINER, INTYPE, OUTTYPE>{-1, container, mapFunction};
        }
    };

    /**
     * @brief An implementation of the AbstractNumberContainerBasedIterator which yields at each iteration pair of data, not the data itself
     * 
     * the first item of the pair is the index in the container while the second item of the pair is the actual value contained
     * 
     * @tparam CONTAINER a container
     * @tparam TYPE type inside the container
     */
    template<typename CONTAINER, typename KEYTYPE, typename VALUETYPE>
    class PairNumberContainerBasedConstIterator: public AbstractNumberContainerBasedConstIterator<CONTAINER, std::pair<KEYTYPE, const VALUETYPE&>, std::pair<KEYTYPE, const VALUETYPE&>*> {
        typedef AbstractNumberContainerBasedConstIterator<CONTAINER, std::pair<KEYTYPE, const VALUETYPE&>, std::pair<KEYTYPE, const VALUETYPE&>*> Super;
        typedef PairNumberContainerBasedConstIterator<CONTAINER, KEYTYPE, VALUETYPE> PairNumberContainerBasedConstIteratorInstance;
    private:
        mutable VarHolder<std::pair<KEYTYPE, const VALUETYPE&>> tmp;
    public:
        PairNumberContainerBasedConstIterator(int index, const CONTAINER& container) : Super{index, container}, tmp{} {

        }
        virtual std::pair<KEYTYPE, const VALUETYPE&>  operator* () const {
            return std::pair<KEYTYPE, const VALUETYPE&>{static_cast<KEYTYPE>(this->index), this->container[this->index]};
        }
        virtual std::pair<KEYTYPE, const VALUETYPE&>* operator->() const {
            this->tmp = std::pair<KEYTYPE, const VALUETYPE&>{static_cast<KEYTYPE>(this->index), this->container[this->index]};
            return &tmp;
        }
    public:
        static PairNumberContainerBasedConstIterator cbegin(const CONTAINER& container) {
            return PairNumberContainerBasedConstIterator{0, container};
        }
        static PairNumberContainerBasedConstIterator cend(const CONTAINER& container) {
            return PairNumberContainerBasedConstIterator{-1, container};
        }
    };



}

#endif 