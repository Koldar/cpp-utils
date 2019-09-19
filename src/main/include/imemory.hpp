#ifndef _IMEMORY_HEADER__
#define _IMEMORY_HEADER__

#include <cstdlib>
#include <iostream>

namespace cpp_utils {

/**
 * @brief Available measurement units
 * 
 */
enum class MemoryConsumptionEnum {
    ///the unit is Byte (B)
    BYTE = 0,
    ///the unit is KiloByte (KB)
    KILOBYTE = 1,
    ///the unit is MegaByte (MB)
    MEGABYTE = 2,
    ///the unit is GigaByte (GB)
    GIGABYTE = 3
};

std::ostream& operator <<(std::ostream& out, const MemoryConsumptionEnum& mce);

/**
 * @brief a number representing a space
 * 
 * For example 30MB or 1B
 */
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
    /**
     * @brief number of unit (e.g., 100)
     * 
     */
    size_t value;
    /**
     * @brief the unit of the number value (e.g. MB)
     * 
     */
    MemoryConsumptionEnum unit;
private:
    /**
     * @brief check if the units of 2 MemoryConsumption match
     * 
     * @param other the other object to check against
     * @return true if the units are the same
     * @return false otherwise
     */
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
    /**
     * @brief construct a object by specifying both value and unit
     * 
     * @param value the value fo the space consumption
     * @param unit the measurement unit of value
     */
    MemoryConsumption(size_t value, MemoryConsumptionEnum unit) : value{value}, unit{unit} {

    }
    /**
     * @brief convert a space consumption in another measurement unit
     * 
     * @param unit another measurement unit
     * @return MemoryConsumption another instance of MemoryConsumption
     */
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