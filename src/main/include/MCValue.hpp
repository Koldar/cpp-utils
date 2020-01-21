#ifndef _CPPUTILS_MCVALUE_HEADER__
#define _CPPUTILS_MCVALUE_HEADER__

#include "exceptions.hpp"
#include "ICleanable.hpp"
#include "imemory.hpp"
#include "NumberListener.hpp"
#include "listeners.hpp"

namespace cpp_utils {

    template <typename NUM>
    class MDValue;

    /**
     * @brief a number which keeps being updating but each time it is updated its value is check to ensure that its value never decreases (value keep being the same is accepted though)
     * 
     * @note
     * MC stands for "Monotonically Crescent"
     * 
     * You can watch the change of this number via ::NumberListener
     * 
     * @tparam NUM type of the internal value
     */
    template <typename NUM>
    class MCValue: public ICleanable, public IMemorable, public ISingleListenable<NumberListener<NUM>> {
    public:
        typedef MCValue<NUM> This;
        typedef NumberListener<NUM> Listener;
    public:
        friend class MDValue<NUM>;
    private:
        /**
         * @brief value which can only decreases
         * 
         */
        NUM val;
    public:
        /*implicit*/ MCValue(const NUM& val): val{val} {

        }
        virtual ~MCValue() {

        }
        MCValue(const This& o): val{o.val} {

        }
        MCValue(This&& o): val{o.val} {

        }
        This& operator=(const This& o) {
            if (this->val > o.val) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically crescent! This assignment will make it decrease");
            }
            if (this->val != o.val) {
                this->fireEvent([&](Listener& l) { l.onNumberIncreased(this->val, o.val); });
            }
            this->val = o.val;
            return *this;
        }
        This& operator=(This&& o) {
            if (this->val > o.val) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically crescent! This assignment will make it decrease");
            }
            if (this->val != o.val) {
                this->fireEvent([&](Listener& l) { l.onNumberIncreased(this->val, o.val); });
            }
            this->val = o.val;
            return *this;
        }
    public:
       explicit operator NUM() const {
            return this->val;
        }
    public:
        This& add(const This& b) {
            if (b.val < 0) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically crescent! This sum will make it ddcrease!");
            }
            if (b.val != 0) {
                this->fireEvent([&](Listener& l) { l.onNumberIncreased(this->val, this->val + b.val); });
            }
            this->val += b.val;
            return *this;
        }
        This& subtract(const This& b) {
            if (b.val > 0) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically crescent! This subtraction will make it ddcrease!");
            }
            if (b.val != 0) {
                this->fireEvent([&](Listener& l) { l.onNumberIncreased(this->val, this->val - b.val); });
            }
            this->val -= b.val;
            return *this;
        }
        This& multiply(const This& b) {
            //(a*b)<a => ab -a < 0 => a(b-1)<0
            if (this->val * (b.val - 1) < 0) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically crescent! This multiplication will make it ddcrease!");
            }
            if (b.val != 1) {
                this->fireEvent([&](Listener& l) { l.onNumberIncreased(this->val, this->val * b.val); });
            }
            this->val *= b.val;
            return *this;
        }
        This& divide(const This& b) {
            //(a/b)<a => a<ab => ab-a>0 => a(b-1)>0
            if (this->val * (b.val - 1) > 0) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically crescent! This division will make it ddcrease!");
            }
            if (b.val != 1) {
                this->fireEvent([&](Listener& l) { l.onNumberIncreased(this->val, this->val / b.val); });
            }
            this->val /= b.val;
            return *this;
        }
    public:
        friend bool operator ==(const This& a, const This& b) {
            return a.val == b.val;
        }
        friend bool operator !=(const This& a, const This& b) {
            return a.val != b.val;
        }
        friend bool operator <(const This& a, const This& b) {
            return a.val < b.val;
        }
        friend bool operator <=(const This& a, const This& b) {
            return a.val <= b.val;
        }
        friend bool operator >(const This& a, const This& b) {
            return a.val > b.val;
        }
        friend bool operator >=(const This& a, const This& b) {
            return a.val >= b.val;
        }
        friend std::ostream& operator <<(std::ostream& out, const This& a) {
            out << a.val;
            return out;
        }
        friend This operator +(const This& a, const This& b) {
            This result{a};
            return result += b;
        }
        friend This operator -(const This& a, const This& b) {
            This result{a};
            return result -= b;
        }
        friend This operator *(const This& a, const This& b) {
            This result{a};
            return result *= b;
        }
        friend This operator /(const This& a, const This& b) {
            This result{a};
            return result /= b;
        }
    public:
        This& operator +=(const This& o) {
            return this->add(o);
        }
        This& operator -=(const This& o) {
            return this->subtract(o);
        }
        This& operator *=(const This& o) {
            return this->multiply(o);
        }
        This& operator /=(const This& o) {
            return this->divide(o);
        }
    public:
        void cleanup() {
            this->val = 0;
            if (this->listener != nullptr) {
                this->listener->cleanup();
            }
        }
        MemoryConsumption getByteMemoryOccupied() const {
            return MemoryConsumption{sizeof(*this)};
        }
    };

}

#endif