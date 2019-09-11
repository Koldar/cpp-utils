/**
 * @file
 *
 * Simple log utility. Can work both on preprocessor time or at compile time
 *
 * All this function works only if:
 * @li the macro @c DEBUG is active OR;
 * @li the macro @c NDEBUG is not active;
 *
 * Otherwise, they will become just empty (or can be easily optimized away)
 *
 * @date Oct 1, 2018
 * @author koldar
 */

#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include "macros.hpp"
#include "file_utils.hpp"

#ifndef QUICK_LOG
#define QUICK_LOG 0
#endif


/**
 * always log an entry via std cio
 *
 * Does no impact performances whatsoever
 *
 * @code
 * 	debug("hello", person->name, "!");
 * @endcode
 *
 * @param[in] level the name of the log level
 * @param[in] ... the entries to put on stream
 */
#define _abstractLog(level, ...) __abstractLog(level, __FILE__, __func__, __LINE__, ## __VA_ARGS__)

template <typename FIRST>
void ___abstractLog(const FIRST& first) {
    std::cerr << first;
}

template <typename FIRST, typename... OTHER>
void ___abstractLog(const FIRST& first, const OTHER&... args) {
    std::cerr << first << " ";
    ___abstractLog(args...);
}

template <typename... OTHER>
void __abstractLog(const char* level, const char* file, const char* func, int lineno, const OTHER&... args) {
    std::cerr 
        << "[" << level << "]" 
        << cpp_utils::getBaseNameAsString(file) 
        << "@" 
        << func 
        << "[" << __LINE__ << "] ";
    ___abstractLog(args...);
    std::cerr << std::endl;
}



#if QUICK_LOG <= 0
/**
 * always log an entry via std cio
 *
 * Does no impact performances whatsoever
 *
 * @code
 * 	debug("hello", person->name, "!");
 * @endcode
 *
 * @param[in] ... the entries to put on stream
 */
#define debug(...) _abstractLog("DEBUG", __VA_ARGS__)
#else
#define debug(...) ;
#endif

#if QUICK_LOG <= 1
/**
 * always log an entry via std cio
 *
 * Does no impact performances whatsoever
 *
 * @code
 * 	finest("hello", person->name, "!");
 * @endcode
 *
 * @param[in] ... the entries to put on stream
 */
#define finest(...) _abstractLog("FINEST", __VA_ARGS__)
#else
#define finest(...) ;
#endif

#if QUICK_LOG <= 2
/**
 * always log an entry via std cio
 *
 * Does no impact performances whatsoever
 *
 * @code
 * 	finer("hello", person->name, "!");
 * @endcode
 *
 * @param[in] ... the entries to put on stream
 */
#define finer(...) _abstractLog("FINER", __VA_ARGS__)
#else
#define finer(...) ;
#endif

#if QUICK_LOG <= 3
/**
 * always log an entry via std cio
 *
 * Does no impact performances whatsoever
 *
 * @code
 * 	debug("hello", person->name, "!");
 * @endcode
 *
 * @param[in] ... the entries to put on stream
 */
#define fine(...) _abstractLog("FINE", __VA_ARGS__)
#else
#define fine(...) ;
#endif

#if QUICK_LOG <= 5
/**
 * always log an entry via std cio
 *
 * Does no impact performances whatsoever
 *
 * @code
 * 	debug("hello", person->name, "!");
 * @endcode
 *
 * @param[in] ... the entries to put on stream
 */
#define info(...) _abstractLog("INFO ", __VA_ARGS__)
#else
#define info(...) ;
#endif

#if QUICK_LOG <= 6
/**
 * always log an entry via std cio
 *
 * Does no impact performances whatsoever
 *
 * @code
 * 	debug("hello", person->name, "!");
 * @endcode
 *
 * @param[in] ... the entries to put on stream
 */
#define warning(...) _abstractLog("WARN ", __VA_ARGS__)
#else
#define warning(...) ;
#endif

#if QUICK_LOG <= 7
/**
 * always log an entry via std cio
 *
 * Does no impact performances whatsoever
 *
 * @code
 * 	debug("hello", person->name, "!");
 * @endcode
 *
 * @param[in] ... the entries to put on stream
 */
#define error(...) _abstractLog("ERROR", __VA_ARGS__)
#else
#define error(...) ;
#endif

#if QUICK_LOG <= 8
/**
 * always log an entry via std cio
 *
 * Does no impact performances whatsoever
 *
 * @code
 * 	debug("hello", person->name, "!");
 * @endcode
 *
 * @param[in] ... the entries to put on stream
 */
#define critical(...) _abstractLog("CRTCL", __VA_ARGS__)
#else
#define critical(...) ;
#endif

/**
 * like ::log but will log only if at runtime the @c expr will evaluate to true
 *
 * Will impact performances (even if by little) even if the log is turned off
 *
 * @code
 * clog(true)("hello ", this->name);
 * @endcode
 *
 * @param[in] expr the expression to be evaluated
 * @param[in] ... input for ::log
 */
#define clog(expr) _clog1(expr)

#define _clog1(expr) if (expr) { _clog2
#define _clog2(...)  debug(__VA_ARGS__); }

/**
 * Condition Statement Log utility allows to perform some previous statement before logging
 *
 * This logging macro is useful when your logging needs some local variable
 *
 * @code
 * cslog(true)(int a = 5)("a is %d", a);
 * @endcode
 *
 * @param[in] expr the condition to check if the log is enabled
 */
#define cslog(expr) _cslog1(expr)
#define _cslog1(expr) if (expr) { _cslog2
#define _cslog2(...) __VA_ARGS__; _cslog3
#define _cslog3(...) debug(__VA_ARGS__); }

#define _LOG_OP(context, index, x) x
#define _LOG_COMB(context, x, y) x << " " << y

#define _debug(...) FOR_EACH(, _LOG_OP, _LOG_COMB, __VA_ARGS__)

#if defined(DEBUG) || !defined(NDEBUG)
    /**
     * Perform some work if debug has been enabled
     *
     *
     */
    #define DO_ON_DEBUG_IF(expr) if (expr)
    #define DO_ON_DEBUG if (true)
#else
    #define DO_ON_DEBUG_IF(expr) if(false) 
    #define DO_ON_DEBUG if (false)
#endif


#endif /* LOG_H_ */
