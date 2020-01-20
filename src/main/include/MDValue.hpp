#ifndef _CPPUTILS_MDVALUE_HEADER__
#define _CPPUTILS_MDVALUE_HEADER__

#include "exceptions.hpp"
#include "ICleanable.hpp"
#include "imemory.hpp"

namespace cpp_utils {


    template <typename NUM>
    class MCValue;

    /**
     * @brief a number which keeps being updating but each time it is updated its value is check to ensure that its value never increases
     * 
     * @note
     * MD stands for "Monotonically Decrescent"
     * 
     * @tparam NUM type of the internal value
     */
    template <typename NUM>
    class MDValue: public ICleanable, public IMemorable {
    public:
        typedef MDValue<NUM> This;
    public:
        friend class MCValue<NUM>;
    private:
        /**
         * @brief value which can only decreases
         * 
         */
        NUM val;
    public:
        /*implicit*/ MDValue(const NUM& val): val{val} {

        }
        virtual ~MDValue() {

        }
        MDValue(const This& o): val{o.val} {

        }
        MDValue(This&& o): val{o.val} {

        }
        This& operator=(const This& o) {
            if (this->val < o.val) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically decrescent! This assignment with", o, "will make it increase");
            }
            this->val = o.val;
            return *this;
        }
        This& operator=(This&& o) {
            if (this->val < o.val) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically decrescent! This assignment with", o, "will make it increase");
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
            if (b.val > 0) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically decrescent! This sum will make it increase!");
            }
            this->val += b.val;
            return *this;
        }
        This& subtract(const This& b) {
            if (b.val < 0) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically decrescent! This subtraction will make it increase!");
            }
            debug("this is", *this, " and b is", b, ". difference is ", this->val - b.val);
            this->val -= b.val;
            return *this;
        }
        This& multiply(const This& b) {
            //(a*b)>a => ab-a>0 => a(b-1)>0
            if (this->val * (b.val - 1) > 0) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically decrescent! This multiplication with", b, "will make it increase!");
            }
            this->val *= b.val;
            return *this;
        }
        This& divide(const This& b) {
            //(a/b)>a => a>ab => ab-a<0 => a(b-1)<0
            if (this->val * (b.val - 1) < 0) {
                throw cpp_utils::exceptions::makeInvalidArgumentException(*this, "is monotonically decrescent! This division will make it increase!");
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
            return result+= b;
        }
        friend This operator -(const This& a, const This& b) {
            This result{a};
            return result-= b;
        }
        friend This operator *(const This& a, const This& b) {
            This result{a};
            return result*= b;
        }
        friend This operator /(const This& a, const This& b) {
            This result{a};
            return result/= b;
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
        }
        MemoryConsumption getByteMemoryOccupied() const {
            return MemoryConsumption{sizeof(*this)};
        }
    };

}

#endif