#include "imemory.hpp"
#include "exceptions.hpp"

namespace cpp_utils {

MemoryConsumption MemoryConsumption::to(MemoryConsumptionEnum target_unit) const {
    if (this->unit == target_unit) {
        return *this;
    } else if (this->unit < target_unit) {
        MemoryConsumption result{*this};
        while (result.unit < target_unit) {
            result.value /= 1024;
            result.unit = static_cast<MemoryConsumptionEnum>(static_cast<int>(result.unit) + 1);
        }
        return result;
    } else if (this->unit > target_unit) {
        MemoryConsumption result{*this};
        while (result.unit > target_unit) {
            result.value *= 1024;
            result.unit = static_cast<MemoryConsumptionEnum>(static_cast<int>(result.unit) - 1);
        }
        result.unit = target_unit;
        return result;
    } else {
        throw exceptions::ImpossibleException{};
    }
}

bool MemoryConsumption::isCompliantWith(const MemoryConsumption& other) const {
    return (this->unit == other.unit);
}

MemoryConsumption& MemoryConsumption::operator +=(const MemoryConsumption& other) {
    this->value += other.to(this->unit).value;
    return *this;
}

MemoryConsumption& MemoryConsumption::operator -=(const MemoryConsumption& other) {
    this->value -= other.to(this->unit).value;
    return *this;
}

bool operator == (const MemoryConsumption& a, const MemoryConsumption& b) {
    if (a.unit == b.unit) {
        return a.value == b.value;
    } else {
        return a.to(MemoryConsumptionEnum::BYTE) == b.to(MemoryConsumptionEnum::BYTE);
    }
}

bool operator != (const MemoryConsumption& a, const MemoryConsumption& b) {
    return !(a == b);
}

bool operator > (const MemoryConsumption& a, const MemoryConsumption& b) {
    if (a.unit == b.unit) {
        return a.value > b.value;
    } else {
        return a.to(MemoryConsumptionEnum::BYTE) > b.to(MemoryConsumptionEnum::BYTE);
    }

}

bool operator >= (const MemoryConsumption& a, const MemoryConsumption& b) {
    if (a.unit == b.unit) {
        return a.value >= b.value;
    } else {
        return a.to(MemoryConsumptionEnum::BYTE) >= b.to(MemoryConsumptionEnum::BYTE);
    }

}

bool operator < (const MemoryConsumption& a, const MemoryConsumption& b) {
    if (a.unit == b.unit) {
        return a.value < b.value;
    } else {
        return a.to(MemoryConsumptionEnum::BYTE) < b.to(MemoryConsumptionEnum::BYTE);
    }

}

bool operator <= (const MemoryConsumption& a, const MemoryConsumption& b) {
    if (a.unit == b.unit) {
        return a.value <= b.value;
    } else {
        return a.to(MemoryConsumptionEnum::BYTE) <= b.to(MemoryConsumptionEnum::BYTE);
    }

}

MemoryConsumption operator + (const MemoryConsumption& a, const MemoryConsumption& b) {
        MemoryConsumption result{a};
        result += b;
        return result;
}

MemoryConsumption operator - (const MemoryConsumption& a, const MemoryConsumption& b) {
        MemoryConsumption result{a};
        result -= b;
        return result;
}

}