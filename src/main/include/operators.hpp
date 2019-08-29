#ifndef _OPERATORS_HEADER__
#define _OPERATORS_HEADER__

#include <string>
#include <iostream>
#include <sstream>

namespace cpp_utils {

template <typename X, typename Y>
std::ostream& operator << (std::ostream& ss, const std::pair<X,Y>& p) {
    ss << "<" << p.first << ", " << p.second << ">";
    return ss;
}

}

#endif