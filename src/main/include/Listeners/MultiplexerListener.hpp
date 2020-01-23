#ifndef _CPPUTILS_MULTIPLEXERLISTENER_HEADER
#define _CPPUTILS_MULTIPLEXERLISTENER_HEADER

#include "vectorplus.hpp"
#include "imemory.hpp"
#include "ICleanable.hpp"

namespace cpp_utils {

    /**
     * @brief a listeners which can be used to store other listeners
     * 
     * @note
     * This object is not the **owner** of the listners, so it doesn't create them nor free them
     * 
     * @tparam LISTENER type of listners to store
     */
    template <typename LISTENER>
    class MultiplexerListener, public ICleanable, public IMemorable {
    public:
        using This = MultiplexerListener<LISTENER>;
    private:
        vectorplus<LISTENER*> listeners;
    public:
        MultiplexerListener(): listeners{} {

        }
        template <typename ...OTHER>
        MultiplexerListener(const OTHER...& others): listeners{} {
            this->add(others);
        }
        virtual ~MultiplexerListener() {

        }
        MultiplexerListener(const This& o): listeners{o.listeners} {

        }
        MultiplexerListener(This&& o): listeners{o.listeners} {

        }
        This& operator=(const This& o) {
            this->listeners = o.listeners;
            return *this;
        }
        This& operator=(This&& o) {
            this->listeners = o.listeners;
            o.listeners.clear();
            return *this;
        }
    public:
        /**
         * @brief add a new listener in the multiplexer
         * 
         * @param l the listener to add
         */
        void add(const LISTENER& l) {
            this->listeners.add(&l);
        }

        /**
         * @brief adds new listeners in the multiplexer
         * 
         * @tparam OTHER others listeners
         * @param l the listener to add
         * @param others other listeners to add
         */
        template <typename ...OTHER>
        void add(const LISTENER& l, const OTHER&... others) {
            this->listeners.add(&l);
            this->add(others);
        }
        /**
         * @brief fire a given event on every listeners
         * 
         * @param lambda the labmda to call to
         */
        void fireEvent(const consumer_t<OBSERVER&>& lambda) {
            for (auto l : this->listeners) {
                lambda(l);
            }
        }
    }

}

#endif