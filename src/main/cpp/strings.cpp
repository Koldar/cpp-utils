#include "strings.hpp"
#include <cstdarg>
#include <cstdio>
#include <vector>

namespace cpp_utils {

std::string sprintf(const char* format, ...) {
    char buffer[1000];
    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, 1000, format, ap);
    va_end(ap);

    return std::string{buffer};
}

std::string vsprintf(const char* format, va_list ap) {
    char buffer[1000];
    vsnprintf(buffer, 1000, format, ap);

    return std::string{buffer};
}

}