#ifndef _COMMONS_HEADER__
#define _COMMONS_HEADER__

template <typename A, typename B>
struct pair {
private:
    A first;
    B second;
public:
    pair(const A& a, const B& b) : first{a}, second{b} {

    }
    pair(const pair<>& other): first{other.first}, second{other.second} {

    }
    pair(const pair<>&& other): first{other.first}, second{other.second} {
    }
    operator =(const pair<>& other) {
        this->first = other.first;
        this->second = other.second;
        return *this;
    }
    const A& getFirst() const {
        return this->first;
    } 
    const B& getSecond() const {
        return this->second;
    }
    A& getFirst() {
        return this->first;
    }
    B& getSecond() {
        return this->second;
    }
};

//from https://stackoverflow.com/a/7759622/1887602
class iterator { 
    public:
        typedef typename A::difference_type difference_type;
        typedef typename A::value_type value_type;
        typedef typename A::reference reference;
        typedef typename A::pointer pointer;
        typedef std::random_access_iterator_tag iterator_category; //or another tag

        iterator();
        iterator(const iterator&);
        ~iterator();

        iterator& operator=(const iterator&);
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;

        iterator& operator++();
        
        reference operator*() const;
        pointer operator->() const;
    };

class const_iterator {
public:
    typedef typename A::difference_type difference_type;
    typedef typename A::value_type value_type;
    typedef typename const A::reference reference;
    typedef typename const A::pointer pointer;
    typedef std::random_access_iterator_tag iterator_category; //or another tag

    const_iterator ();
    const_iterator (const const_iterator&);
    const_iterator (const iterator&);
    ~const_iterator();

    const_iterator& operator=(const const_iterator&);
    bool operator==(const const_iterator&) const;
    bool operator!=(const const_iterator&) const;

    const_iterator& operator++();

    reference operator*() const;
    pointer operator->() const;
};

#endif 