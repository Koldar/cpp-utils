#include "safe_inf_uint.hpp"

namespace cpp_utils {

safe_inf_uint const safe_inf_uint::ZERO = safe_inf_uint{0U};
safe_inf_uint const safe_inf_uint::MIN = safe_inf_uint{0U};
safe_inf_uint const safe_inf_uint::MAX = safe_inf_uint{UINT64_MAX - 1};
safe_inf_uint const safe_inf_uint::INFTY = safe_inf_uint{UINT64_MAX};

std::ostream& operator <<(std::ostream& out, const safe_inf_uint& n) {
    if (n.isInfinity()) {
        out << "+inf";
    } else {
        out << n.value;
    }
    return out;
}

safe_inf_uint operator +(const safe_inf_uint& a, const safe_inf_uint& b) {
    safe_inf_uint result{a};
    result += b;
    return result;
}

safe_inf_uint operator -(const safe_inf_uint& a, const safe_inf_uint& b) {
    safe_inf_uint result{a};
    result -= b;
    return result;
}

safe_inf_uint operator *(const safe_inf_uint& a, const safe_inf_uint& b) {
    safe_inf_uint result{a};
    result *= b;
    return result;
}

safe_inf_uint operator /(const safe_inf_uint& a, const safe_inf_uint& b) {
    safe_inf_uint result{a};
    result /= b;
    return result;
}

bool operator <(const safe_inf_uint& a, const safe_inf_uint& b) {
    if (a.isInfinity() && b.isInfinity()) {
        throw cpp_utils::exceptions::NumericalOperationException{"<", a, b};
    }
    return a.value < b.value;
}

bool operator <=(const safe_inf_uint& a, const safe_inf_uint& b) {
    if (a.isInfinity() && b.isInfinity()) {
        throw cpp_utils::exceptions::NumericalOperationException{"<=", a, b};
    }
    return a.value <= b.value;
}

bool operator >(const safe_inf_uint& a, const safe_inf_uint& b) {
    if (a.isInfinity() && b.isInfinity()) {
        throw cpp_utils::exceptions::NumericalOperationException{">", a, b};
    }
    return a.value > b.value;
}

bool operator >=(const safe_inf_uint& a, const safe_inf_uint& b) {
    if (a.isInfinity() && b.isInfinity()) {
        throw cpp_utils::exceptions::NumericalOperationException{">=", a, b};
    }
    return a.value > b.value;
}

bool operator ==(const safe_inf_uint& a, const safe_inf_uint& b) {
    return a.value == b.value;
}

bool operator !=(const safe_inf_uint& a, const safe_inf_uint& b) {
    return a.value != b.value;
}

}