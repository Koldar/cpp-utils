#ifndef _IMEMORY_HEADER__
#define _IMEMORY_HEADER__

#include <cstdlib>
#include <iostream>

namespace cpp_utils {


enum class MemoryConsumptionEnum {
    BYTE = 0,
    KILOBYTE = 1,
    MEGABYTE = 2,
    GIGABYTE = 3
};

std::ostream& operator <<(std::ostream& out, const MemoryConsumptionEnum& mce);

class MemoryConsumption {
public:
    friend bool operator == (const MemoryConsumption& a, const MemoryConsumption& b);
    friend bool operator != (const MemoryConsumption& a, const MemoryConsumption& b);
    friend bool operator > (const MemoryConsumption& a, const MemoryConsumption& b);
    friend bool operator < (const MemoryConsumption& a, const MemoryConsumption& b);
    friend bool operator >= (const MemoryConsumption& a, const MemoryConsumption& b);
    friend bool operator <= (const MemoryConsumption& a, const MemoryConsumption& b);
    friend MemoryConsumption operator + (const MemoryConsumption& a, const MemoryConsumption& b);
    friend MemoryConsumption operator - (const MemoryConsumption& a, const MemoryConsumption& b);
    friend std::ostream& operator <<(std::ostream& out, const MemoryConsumption& mc);
private:
    size_t value;
    MemoryConsumptionEnum unit;
private:
    bool isCompliantWith(const MemoryConsumption& other) const;
public:
    /**
     * @brief construct an object repèresenting a consumption of 0 bytes
     * 
     */
    MemoryConsumption(): value{0}, unit{MemoryConsumptionEnum::BYTE} {

    }
    /**
     * @brief construct an object representing a consumption of @c value bytes
     * 
     * @note
     * implicit constructor to allow promotion from integers to MemoryConsumption
     * 
     * @tparam T type of @c value
     * @param value number of bytes
     */
    template <typename T>
    MemoryConsumption(T value): value{value}, unit{MemoryConsumptionEnum::BYTE} {

    }
    MemoryConsumption(size_t value, MemoryConsumptionEnum unit) : value{value}, unit{unit} {

    }
    MemoryConsumption to(MemoryConsumptionEnum unit) const;

    MemoryConsumption& operator +=(const MemoryConsumption& other);
    MemoryConsumption& operator -=(const MemoryConsumption& other);
    
    explicit operator size_t() const {
        return this->value;
    }
    explicit operator unsigned int() const {
        return this->value;
    }
};



class IMemorable {
public:
    virtual MemoryConsumption getByteMemoryOccupied() const = 0;
};

}

#endif