#ifndef _STRINGS_HEADER__
#define _STRINGS_HEADER__

#include <string>
#include <iostream>
#include <sstream>
#include "macros.hpp"
#include "log.hpp"

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

//TODO swcout and scout can be written as join!
template <typename FIRST, typename... OTHERs>
std::string _swcout(std::stringstream& ss, const FIRST& s, OTHERs... args) {
    ss << s << " ";
    return _swcout(ss, args...);
}

//TODO create a cout version that accept data like sprintf but uses << to generate the values.
//allowed formats are %s (call <<), %p (obtain pointer of somethign which isn't a pointer, the pointer itself otherwise), %.2f (only for decimals)

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

template <typename SEP>
std::string _join(std::stringstream& ss, SEP sep) {
    debug("in nothing!");
    return ss.str();
}

template <typename SEP, typename FIRST>
std::string _join(std::stringstream& ss, SEP sep, FIRST first) {
    debug("in base! first is", first);
    ss << first;
    return ss.str();
}

template <typename SEP, typename FIRST, typename... OTHERS>
std::string _join(std::stringstream& ss, SEP sep, FIRST first, OTHERS... args) {
    debug("in recursion! first is", first);
    ss << first << sep;
    return _join(ss, sep, args...);
}

/**
 * @brief join strings by adding a separator between them
 * 
 * @code
 *  cpp_utils::join("-", "a", "b", "c"); //a-b-c
 * @endcode
 * 
 * 
 * @tparam OTHERS types of all the arguments to concatenate. If strings are not provided, they will be converted to strings with `<<`
 * @param separator the string (or something else) that will be added between each `args`. If this is not a string, it will be converted with `<<`
 * @param args the stuff to concatenate
 * @return std::string string representation
 */
template <typename SEP, typename... OTHERS>
std::string join(SEP separator, OTHERS... args) {
    debug("call join!");
    std::stringstream ss;
    return _join(ss, separator, args...);
}

template <typename SEP, typename CONTAINER>
std::string joinOn(SEP& separator, CONTAINER& c) {
    std::stringstream ss;
    bool first = true;
    for (auto x : c) {
        if (first) {
            ss << x;
            first = false;
        } else {
            ss << separator << x;
        }
    }
    return ss.str();
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

