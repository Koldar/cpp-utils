/**
 * @file optional.hpp
 * @author Massimo Bono (you@domain.com)
 * @brief contains the optional class
 * @version 0.1
 * @date 2020-02-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _CPPUTILS_OPTIONAL_HEADER__
#define _CPPUTILS_OPTIONAL_HEADER__

#include "ICleanable.hpp"
#include "imemory.hpp"

#include "assertions.hpp"

namespace cpp_utils::datastructures {

    /**
     * @brief a wrapper that contains or not a value
     * 
     * The instance is the owner of the object: put in another way, the `T` instance is stored **by value**!
     * 
     * If you want to use this class, there are several assumptions you need to make:
     * In order to be used, the wrapped class needs to have an empty constructor
     * 
     * @tparam T the type to store
     */
    template <typename T>
    class optional: public ICleanable, public IMemorable {
    public:
        using This = optional<T>;
    private:
        bool present;
        T value;
    public:
        /*implicit*/ optional(const T& v): value{v}, present{true} {

        }
        optional(): value{}, present{false} {

        }
        optional(const This& o): value{o.value}, present{o.present} {

        }
        optional(This&& o): value{std::move(o.value)}, present{o.present} {

        }
        virtual ~optional() {

        }
        This& operator = (const This& o) {
            this->value = o.value;
            this->present = o.present;
            return *this;
        }
        This& operator =(This&& o) {
            this->value = std::move(o.value);
            this->present = o.present;
            return *this;
        }
    public:
        /**
         * @brief check if an element is present in the structure
         * 
         * @return true if the element is valid
         * @return false otherwise
         */
        bool isPresent() const {
            return this->present;
        }
        /**
         * @brief check if an element is absent in the structure
         * 
         * @return true if the element is not valid
         * @return false otherwise
         */
        bool isAbsent() const {
            return !this->present;
        }
        T& getOrElse(T& o) {
            return this->present ? this->value : o;
        }
        const T& getOrElse(const T& o) const {
            return this->present ? this->value : o;
        }
        /**
         * @brief get the element within the structure
         * 
         * @pre
         *  @ li element is valid
         * 
         * @return T& the element in the structure
         */
        T& get() {
            assertHappens(this->present);
            return this->value;
        }
        /**
         * @brief get the element within the structure
         * 
         * @pre
         *  @ li element is valid
         * 
         * @return const T& the element in the structure
         */
        const T& get() const {
            assertHappens(this->present);
            return this->value;
        }
        explicit operator T() const {
            return this->value;
        }
    public:
        /**
         * @brief make the element in the structure be invalid
         * 
         */
        void cleanup() {
            this->present = false;
        }
        MemoryConsumption getByteMemoryOccupied() const {
            return sizeof(*this);
        }
    public:
        friend bool operator ==(const This& a, const This& b) {
            if (!a.present && !b.present) {
                return true;
            }
            if (a.present && b.present) {
                return a.value = b.value;
            }
            return false;
        }
        friend bool operator !=(const This& a, const This& b) {
            if (a.present != b.present) {
                return true;
            }
            if (a.present) {
                return a.value != b.value;
            } else {
                return false;
            }
        }
        friend std::ostream& operator <<(std::ostream& out, const This& a) {
            if (a.present) {
                return out << a.value;
            } else {
                return out << "<none>";
            }
        }

    };

}

#endif