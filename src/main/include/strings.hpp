#ifndef _CPP_UTILS_STRINGS_HEADER__
#define _CPP_UTILS_STRINGS_HEADER__

#include <string>
#include <iostream>
#include <sstream>
#include "macros.hpp"
#include "log.hpp"

namespace cpp_utils {

    /**
     * @brief convert a string into its lower case version
     * 
     * @param str 
     * @return std::string 
     */
    std::string toLowercase(const std::string& str);

    /**
     * @brief convert a string into its upper case version
     * 
     * @param str 
     * @return std::string 
     */
    std::string toUppercase(const std::string& str);

    //TODO create a cout version that accept data like sprintf but uses << to generate the values.
    //allowed formats are %s (call <<), %p (obtain pointer of somethign which isn't a pointer, the pointer itself otherwise), %.2f (only for decimals)

    std::ostream& _ojoin(std::ostream& out, const char* sep);

    template <typename FIRST>
    std::ostream& _ojoin(std::ostream& out, const char* sep, const FIRST& f) {
        out << f;
        return out;
    }

    template <typename FIRST, typename ...OTHERS>
    std::ostream& _ojoin(std::ostream& out, const char* sep, const FIRST& f, const OTHERS&... others) {
        out << f << sep;
        return _ojoin(out, sep, others...);
    }

    template <typename ...OTHERS>
    std::ostream& ojoin(std::ostream& out, const char* sep, const OTHERS&... others) {
        return _ojoin(out, sep, others...);
    }


    template <typename SEP>
    std::string _join(std::stringstream& ss, const SEP& sep) {
        debug("in nothing!");
        return ss.str();
    }

    template <typename SEP, typename FIRST>
    std::string _join(std::stringstream& ss, const SEP& sep, const FIRST& first) {
        debug("in base! first is", first);
        ss << first;
        return ss.str();
    }

    template <typename SEP, typename FIRST, typename... OTHERS>
    std::string _join(std::stringstream& ss, const SEP& sep, const FIRST& first, const OTHERS&... args) {
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
    std::string join(const SEP& separator, const OTHERS&... args) {
        debug("call join!");
        std::stringstream ss;
        return _join(ss, separator, args...);
    }

    template <typename SEP, typename CONTAINER>
    std::string joinOn(const SEP& separator, const CONTAINER& c) {
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
     * @brief iteratively perform a "<<" operation over each argument given
     * 
     * @note
     * this version will automatically put a whitespace between 2 parameters (except the first and the last ones!)
     * 
     * @code
     *  swcout("hello", "world!"); //hello world!
     * @endcode
     * 
     * @tparam OTHERs the parameters involved
     * @param args 
     * @return std::string a string obtained by iteratively "<<" the parameters
     */
    template <typename... OTHERs>
    std::string swcout(const OTHERs&... args) {
        return join(" ", args...);
    }

    /**
     * @brief iteratively perform a "<<" operation over each argument given
     * 
     * @tparam OTHERs the parameters involved
     * @param args 
     * @return std::string a string obtained by iteratively "<<" the parameters
     */
    template <typename... OTHERs>
    std::string scout(const OTHERs&... args) {
        return join("", args...);
    }

    template <typename... OTHER>
    std::ostream& owcout(std::ostream& out, const OTHER&... args) {
        return ojoin(out, " ", args...);
    }


    /**
     * @brief a "sprintf" version returning a `string`, not a `char*`
     * 
     * @param format the format of the string
     * @param ... parameters. the parameters will be interpreted as `printf` parameters, **not** as `<<` parameters!
     * @return std::string 
     */
    //TODO add [[deprecated("use swcout instead")]]
    std::string sprintf(const char* format, ...);
    /**
     * @brief a "vsprintf" version returning a `string`, not a `char*`
     * 
     * @param format the format of the string
     * @param ap parameters. the parameters will be interpreted as `printf` parameters, **not** as `<<` parameters!
     * @return std::string 
     */
    //TODO add [[deprecated("use swcout instead")]]
    std::string vsprintf(const char* format, va_list ap);


}

#endif 

