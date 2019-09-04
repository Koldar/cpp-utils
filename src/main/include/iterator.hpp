#ifndef _ITERATOR_HEADER__
#define _ITERATOR_HEADER__

#include <functional>
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
 * @brief an iterator which doesn't allow modifications while iteration on the iterated structure
 * 
 * @tparam REF return type (automatically consted) of operator *
 * @tparam PTR  return type (automatically consted) of operator -> 
 */
template<typename REF, typename PTR>
class AbstractConstIterator {
    public:
        AbstractConstIterator<REF,PTR>& operator++(int) = delete;

        virtual AbstractConstIterator<REF,PTR>& operator=(const AbstractConstIterator<REF,PTR>& other) { 
            throw exceptions::AbstractMethodCalledException{__func__};
        }
        virtual AbstractConstIterator<REF,PTR>& operator++() { 
            throw exceptions::AbstractMethodCalledException{__func__};
        }

        virtual REF operator* () const { 
            throw exceptions::AbstractMethodCalledException{__func__};
        }
        virtual PTR operator->() const { 
            throw exceptions::AbstractMethodCalledException{__func__};
        }

        virtual bool isEqual(const AbstractConstIterator<REF,PTR>* other) const {
            throw exceptions::AbstractMethodCalledException{__func__};
        }
};

/**
 * @brief 
 * 
 * AbstractConstIterator is "abstract" and since iterators are passed via value you shouldn't be able to
 * initialize it.
 * 
 * To surpass this we wrap the iterator inside a ProxyConstIterator, which is the unique owner of the
 * AbstractConstIterator
 * 
 */
template <typename REF, typename PTR>
class ProxyConstIterator {
public:
    friend bool operator == <>(const ProxyConstIterator<REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b);
    friend bool operator != <>(const ProxyConstIterator<REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b);
    template <typename CONTAINER>
    friend bool operator == (const ProxyConstIterator<REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b);
    template <typename CONTAINER>
    friend bool operator != (const ProxyConstIterator<REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b);
    template <typename CONTAINER>
    friend bool operator == (const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b);
    template <typename CONTAINER>
    friend bool operator != (const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b);
public:
    ProxyConstIterator(AbstractConstIterator<REF,PTR>* wrapped) : wrapped{wrapped} {
    }
    virtual ~ProxyConstIterator() {
        delete this->wrapped;
    }
private:
    AbstractConstIterator<REF,PTR>* wrapped;
public:
    virtual ProxyConstIterator<REF,PTR>& operator=(AbstractConstIterator<REF,PTR>* other) { 
        this->wrapped = other;
        return *this;
    }
    virtual ProxyConstIterator<REF,PTR>& operator++() { 
        ++(*this->wrapped);
        return *this;
    }
    virtual REF operator*() const { 
        return this->wrapped->operator*();
    }
    virtual PTR operator->() const { 
        return this->wrapped->operator->();
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
public:
    friend bool operator == <>(const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b);
    friend bool operator != <>(const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b);
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
    AbstractNumberContainerBasedConstIterator(int index, const CONTAINER& container): 
        index{index}, container{container} {

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
    AbstractNumberContainerBasedConstIterator(const AbstractNumberContainerBasedConstIterator& other) : index{other.index}, container{other.container} {

    }
    ~AbstractNumberContainerBasedConstIterator() {

    }
public:
    virtual AbstractNumberContainerBasedConstIterator<CONTAINER, REF, PTR>& operator=(const AbstractNumberContainerBasedConstIterator<CONTAINER, REF, PTR>& other) {
        if (this->container != other.container) {
            throw cpp_utils::exceptions::InvalidArgumentException{"the 2 containers do not match!"};
        }
        this->index = other.index;
        return *this;
    }
    
    virtual AbstractNumberContainerBasedConstIterator<CONTAINER, REF, PTR>& operator++() {
        if (this->index < 0) {
            return *this;
        }
        this->index += 1;
        if (this->index >= this->container.size()){
            this->index = -1;
        }
        return *this;
    }
    virtual bool isEqual(const AbstractConstIterator<REF,PTR>* other) const {
        return *this == *(static_cast<const AbstractNumberContainerBasedConstIterator<CONTAINER, REF, PTR>*>(other));
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
public:
    DefaultNumberContainerBasedConstIterator(int index, const CONTAINER& container) : AbstractNumberContainerBasedConstIterator<CONTAINER, TYPE&, TYPE*>{index, container} {

    }
    virtual ~DefaultNumberContainerBasedConstIterator() {

    }
    DefaultNumberContainerBasedConstIterator& operator=(const DefaultNumberContainerBasedConstIterator& other) {
        this->index = other.index;
        this->container = other.container;
        return *this;
    }
    virtual TYPE& operator* () const {
        return (TYPE&)this->container[this->index];
    }
    virtual TYPE* operator->() const {
        return (TYPE*)&this->container[this->index];
    }
public:
    static DefaultNumberContainerBasedConstIterator<CONTAINER, TYPE> cbegin(const CONTAINER& container) {
        return DefaultNumberContainerBasedConstIterator<CONTAINER, TYPE>{0, container};
    }
    static DefaultNumberContainerBasedConstIterator<CONTAINER, TYPE> cend(const CONTAINER& container) {
        return DefaultNumberContainerBasedConstIterator<CONTAINER, TYPE>{-1, container};
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
class MapNumberContainerBasedConstIterator: public AbstractNumberContainerBasedConstIterator<CONTAINER, OUTTYPE&, OUTTYPE*> {
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
    MapNumberContainerBasedConstIterator(int index, const CONTAINER& container, map_function_t mapFunction) : AbstractNumberContainerBasedConstIterator<CONTAINER, OUTTYPE&, OUTTYPE*>{index, container}, mapFunction{mapFunction}, isTmpSet{false} {
    }
    virtual ~MapNumberContainerBasedConstIterator() {

    }
    MapNumberContainerBasedConstIterator& operator=(const MapNumberContainerBasedConstIterator& other) {
        this->index = other.index;
        this->container = other.container;
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
    virtual AbstractNumberContainerBasedConstIterator<CONTAINER, OUTTYPE&, OUTTYPE*>& operator++() {
        this->isTmpSet = false;
        return AbstractNumberContainerBasedConstIterator<CONTAINER, OUTTYPE&, OUTTYPE*>::operator++();
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
private:
    mutable VarHolder<std::pair<KEYTYPE, const VALUETYPE&>> tmp;
public:
    PairNumberContainerBasedConstIterator(int index, const CONTAINER& container) : AbstractNumberContainerBasedConstIterator<CONTAINER, std::pair<KEYTYPE, const VALUETYPE&>, std::pair<KEYTYPE, const VALUETYPE&>*>{index, container} {

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

template<typename REF, typename PTR>
bool operator == (const ProxyConstIterator<REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b) {
    return a.wrapped == b.wrapped;
}

template<typename REF, typename PTR>
bool operator != (const ProxyConstIterator<REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b) {
    return !a.wrapped->isEqual(b.wrapped);
}

template<typename CONTAINER, typename REF, typename PTR>
bool operator == (const ProxyConstIterator<REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b) {
    return a.wrapped == b;
}

template<typename CONTAINER, typename REF, typename PTR>
bool operator != (const ProxyConstIterator<REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b) {
    critical("ProxyConstIterator != AbstractNumberContainerBasedConstIterator");
    return a.wrapped != b;
}

template<typename CONTAINER, typename REF, typename PTR>
bool operator == (const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b) {
    return a == *b.wrapped;
}

template<typename CONTAINER, typename REF, typename PTR>
bool operator != (const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const ProxyConstIterator<REF,PTR>& b) {
    return a != *b.wrapped;
}

template<typename CONTAINER, typename REF, typename PTR>
bool operator == (const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b) {
    return a.index == b.index && a.container == b.container;
}

template<typename CONTAINER, typename REF, typename PTR>
bool operator != (const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& a, const AbstractNumberContainerBasedConstIterator<CONTAINER, REF,PTR>& b) {
    return a.index != b.index || a.container != b.container;
}

}

#endif 