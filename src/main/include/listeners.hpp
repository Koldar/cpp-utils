#ifndef _LISTENERS_HEADER__
#define _LISTENERS_HEADER__

#include <functional>

namespace cpp_utils {

/**
 * @brief partial class which allows a class to own a listener vector
 * 
 * @code
 *  class Foo: public Listenable<FooListener> {
 *  };
 *  Foo foo{};
 *  foo.addListener(FooListener{});
 * @endcode
 * 
 * @tparam OBSERVER 
 */
template <typename OBSERVER>
class Listenable {
protected:
    std::vector<OBSERVER> listeners;
public:
    Listenable(): listeners {} {

    }
    ~Listenable() {

    }
public:
    void addListener(const OBSERVER& listener) {
        this->listeners.push_back(listener);
    }
    void removeListener(const OBSERVER& listener) {
        this->listeners.erase(std::remove(this->listeners.begin(), this->listeners.end(), listener), this->listeners.end());
    }
    void removeAllListeners() {
        this->listners.clear();
    }
    void fireEvent(std::function<void(const OBSERVER&)> lambda) const {
        for (OBSERVER l : this->listeners) {
            lambda(l);
        }
    }
};

}

#endif 