#ifndef _STRINGS_HEADER__
#define _STRINGS_HEADER__

#include <string>
#include <iostream>
#include <sstream>
#include "macros.hpp"

namespace cpp_utils {

template <typename FIRST>
std::string _scout(std::stringstream& ss, const FIRST& s) {
    ss << s;
    return ss.str();
}

template <typename FIRST, typename... OTHERs>
std::string _scout(std::stringstream& ss, const FIRST& s, OTHERs... args) {
    ss << s;
    return _scout(ss, args...);
}

/**
 * @brief iteratively perform a "<<" operation over each argument given
 * 
 * @tparam OTHERs the parameters involved
 * @param args 
 * @return std::string a string obtained by iteratively "<<" the parameters
 */
template <typename... OTHERs>
std::string scout(OTHERs... args) {
    std::stringstream ss;
    return _scout(ss, args...);
}

template <typename FIRST>
std::string _swcout(std::stringstream& ss, const FIRST& s) {
    ss << s;
    return ss.str();
}

template <typename FIRST, typename... OTHERs>
std::string _swcout(std::stringstream& ss, const FIRST& s, OTHERs... args) {
    ss << s << " ";
    return _swcout(ss, args...);
}

/**
 * @brief iteratively perform a "<<" operation over each argument given
 * 
 * @note
 * this version will automatically put a whitespace between 2 parameters (except the first and the last ones!)
 * 
 * @tparam OTHERs the parameters involved
 * @param args 
 * @return std::string a string obtained by iteratively "<<" the parameters
 */
template <typename... OTHERs>
std::string swcout(OTHERs... args) {
    std::stringstream ss;
    return _swcout(ss, args...);
}


/**
 * @brief a "sprintf" version returning a `string`, not a `char*`
 * 
 * @param format the format of the string
 * @param ... parameters. the parameters will be interpreted as `printf` parameters, **not** as `<<` parameters!
 * @return std::string 
 */
std::string sprintf(const char* format, ...);
/**
 * @brief a "vsprintf" version returning a `string`, not a `char*`
 * 
 * @param format the format of the string
 * @param ap parameters. the parameters will be interpreted as `printf` parameters, **not** as `<<` parameters!
 * @return std::string 
 */
std::string vsprintf(const char* format, va_list ap);


}

#endif 

