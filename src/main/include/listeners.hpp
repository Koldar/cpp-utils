#ifndef _LISTENERS_HEADER__
#define _LISTENERS_HEADER__

#include <functional>

namespace cpp_utils {

    /**
     * @brief partial class that allows a class to own a **single** listener
     * 
     * Use this class when you don't care to have multiple listeners, just one
     */
    template <typename OBSERVER>
    class ISingleListenable {
        typedef ISingleListenable<OBSERVER> This;
    protected:
        /**
         * @brief the listener.
         * 
         * If null the class has no listener
         * Note that this class **do not own** the object, hence, it just has a pointer
         * (not a reference, since there might be no observers)
         */
        mutable OBSERVER* listener;
    public:
        ISingleListenable(): listener{nullptr} {

        }
        virtual ~ISingleListenable() {
            debug("destroying ISingleListenable");
            //we do not own the listner, hence it is not our duty to deallocate it
            this->listener = nullptr;
        }
        ISingleListenable(const This& o): listener{o.listener} {

        }
        ISingleListenable(This&& o) : listener{o.listener} {
            o.listener = nullptr;
        }
        This& operator=(const This& o) {
            debug("trying to copy the listeneral");
            if (this->listener != nullptr && o.listener != nullptr && this->listener != o.listener) {
                throw cpp_utils::exceptions::InvalidArgumentException{"impossible to copy the listeners: we would lose a listener! this=", this->listener, "other=", o.listener};
            }
            if (this->listener == nullptr && o.listener != nullptr) {
                debug("this listener is copied from other listers.. (this = ", this->listener, ", other=", o.listener, ")");
                this->listener = o.listener;
            }
            debug("returning this");
            return *this;
        }
        /**
         * @brief copy the structure
         * 
         * If both structures has listener, we will copy only if *this has no listener. If both has listeners, an error is thrown
         * 
         * @param o 
         * @return This& 
         */
        This& operator=(This&& o) {
            debug("trying to move listeners...");
            if (this->listener != nullptr && o.listener != nullptr && this->listener != o.listener) {
                throw cpp_utils::exceptions::InvalidArgumentException{"impossible to copy the listeners: we would lose a listener! this=", this->listener, "other=", o.listener};
            }
            if (this->listener == nullptr && o.listener != nullptr) {
                debug("this listener is copied from other listers.. (this = ", this->listener, ", other=", o.listener, ")");
                this->listener = o.listener;
            }
            debug("setting other listener to null");
            o.listener = nullptr;
            debug("returning this");
            return *this;
        }
    public:
        void doOnObserver(const std::function<void(const OBSERVER&)>& lambda) const {
            if (this->listener != nullptr) {
                lambda(*this->listener);
            }
        }
        void doOnObserver(const std::function<void(OBSERVER&)>& lambda) {
            if (this->listener != nullptr) {
                lambda(*this->listener);
            }
        }
        void setListener(const OBSERVER& listener) {
            this->listener = const_cast<OBSERVER*>(&listener);
        }
        void removeListener() {
            this->listener = nullptr;
        }
        
        void fireEvent(const std::function<void(const OBSERVER&)>& lambda) const {
            if (this->listener != nullptr) {
                critical("fire event in single listeners const");
                lambda(*this->listener);
            }
        }
        void fireEvent(const std::function<void(OBSERVER&)>& lambda) {
            if (this->listener != nullptr) {
                debug("fire event in single listeners non-const");
                lambda(*this->listener);
            }
        }
        void fireEvent(std::function<void(OBSERVER&)>&& lambda) {
            if (this->listener != nullptr) {
                debug("fire move event in single listeners non-const");
                lambda(*this->listener);
            }
        }
        template <typename LAMBDA>
        void fireEvent(const LAMBDA& lambda) {
            if (this->listener != nullptr) {
                debug("fire template event in single listeners non-const");
                lambda(*this->listener);
            }
        }
        /**
         * @brief check if the structure has a listener or not
         * 
         * @return true if the object has a listener
         * @return false otherwise
         */
        bool hasListener() const {
            return this->listener != nullptr;
        }
    };

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
        std::vector<OBSERVER*> listeners;
    public:
        Listenable(): listeners {} {

        }
        virtual ~Listenable() {

        }
    public:
        void addListener(const OBSERVER& listener) {
            this->listeners.push_back(&listener);
        }
        void removeListener(const OBSERVER& listener) {
            this->listeners.erase(std::remove(this->listeners.begin(), this->listeners.end(), listener), this->listeners.end());
        }
        void removeAllListeners() {
            this->listners.clear();
        }
        void fireEvent(std::function<void(const OBSERVER&)> lambda) const {
            for (OBSERVER* l : this->listeners) {
                lambda(*l);
            }
        }
        void fireEvent(std::function<void(OBSERVER&)> lambda) const {
            for (OBSERVER* l : this->listeners) {
                lambda(*l);
            }
        }
    };

}

#endif 