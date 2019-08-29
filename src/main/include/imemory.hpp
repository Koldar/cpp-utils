#ifndef _IMEMORY_HEADER__
#define _IMEMORY_HEADER__

#include <cstdlib>

namespace cpp_utils {


enum class MemoryConsumptionEnum {
    BYTE = 0,
    KILOBYTE = 1,
    MEGABYTE = 2,
    GIGABYTE = 3
};

class MemoryConsumption {
    friend bool operator == (const MemoryConsumption& a, const MemoryConsumption& b);
    friend bool operator != (const MemoryConsumption& a, const MemoryConsumption& b);
    friend bool operator > (const MemoryConsumption& a, const MemoryConsumption& b);
    friend bool operator < (const MemoryConsumption& a, const MemoryConsumption& b);
    friend bool operator >= (const MemoryConsumption& a, const MemoryConsumption& b);
    friend bool operator <= (const MemoryConsumption& a, const MemoryConsumption& b);
    friend MemoryConsumption operator + (const MemoryConsumption& a, const MemoryConsumption& b);
    friend MemoryConsumption operator - (const MemoryConsumption& a, const MemoryConsumption& b);
private:
    size_t value;
    MemoryConsumptionEnum unit;
private:
    bool isCompliantWith(const MemoryConsumption& other) const;
public:
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