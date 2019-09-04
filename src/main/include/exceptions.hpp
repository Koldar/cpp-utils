#ifndef _EXCEPTION_HEADER__
#define _EXCEPTION_HEADER__

#include <exception>
#include <string>
#include <cstdarg>
#include "strings.hpp"
#include "log.hpp"

namespace cpp_utils::exceptions {

/**
 * @brief macro inserting a constructor accepting a string
 * 
 * @param[in] exception the name of the expection
 */
#define ADD_STRING_CONSTRUCTOR(expection) \
    expection(const char* format, ...) : AbstractException{} { \
        char buffer[1000]; \
        va_list ap; \
        va_start(ap, format); \
        vsnprintf(buffer, 1000, format, ap); \
        va_end(ap); \
        \
        this->message = std::string{buffer}; \
    } \

/**
 * @brief super class of all the exceptions
 * 
 */
class AbstractException: public std::exception {
protected:
    std::string message;
public:
    ADD_STRING_CONSTRUCTOR(AbstractException)
    AbstractException(const std::string& str): message{str}{
        
    }
    AbstractException(const char* format, va_list ap): message{} {
        char buffer[1000];
        vsnprintf(buffer, 1000, format, ap);
        this->message = std::string{buffer};
    }
    AbstractException(): message{} {

    }
    virtual const char* what() const throw() {
        return this->getMessage().c_str();
    }
    const std::string& getMessage() const throw() {
        return this->message;
    }
};

// list of available exception

/**
 * @brief Exception representing something that the developer didn't accounted for.
 * 
 * Usually it means that the developer didn't think you could end up in that situation
 * 
 */
class ImpossibleException: public AbstractException {
public:
    ImpossibleException() : AbstractException{} {

    }
    ADD_STRING_CONSTRUCTOR(ImpossibleException)
};

/**
 * @brief exception to throw when you have listed several scenarios depending to a single parameter and the parameter you have is not compliant with any scenario you're expecting
 * 
 * For example in a `switch case` statement depending on a variable `int x` you're expecting either 1,2 or 3 but you got 4.
 * 
 * @tparam TYPE type of the parameter you're depending on
 */
template <typename TYPE>
class InvalidScenarioException: public AbstractException {
public:
    InvalidScenarioException(TYPE t) : AbstractException{}, t{t} {
        this->message = cpp_utils::sprintf("No matched scenario for value %s", t);
    }
private:
    TYPE t;
};

/**
 * @brief like InvalidScenarioException but here the scenario is uniquely indentified by 2 parameters, not one
 * 
 * @tparam T1 the type of the first parameter
 * @tparam T2 the type of the second parameter
 */
template <typename T1, typename T2>
class InvalidPairScenarioException: public AbstractException {
private:
    ///first value
    T1 t1;
    ///second value
    T2 t2;
public:
    /**
     * @brief Construct a new Invalid Pair Scenario Exception object
     * 
     * @param t1 the in valid value of the first parameter
     * @param t2 the invalid value of the second parameter
     */
    InvalidPairScenarioException(T1 t1, T2 t2) : AbstractException{}, t1{t1}, t2{t2} {
        this->message = cpp_utils::sprintf("No matched scenario for value %s-%s", t1, t2);
    }
};

/**
 * @brief exception to throw when you can't perform the operation because **self** is in an invalid state
 * 
 */
template<typename SELF>
class InvalidStateException: public AbstractException {
private:
    const SELF& self;
public:
    InvalidStateException(const SELF& self): AbstractException{}, self{self} {
        this->message = cpp_utils::sprintf("%s is in an invalid state", self);
    }
};

/**
 * @brief exception thrown when an element inside a bigger concept goes against some sort of format compliance
 * 
 * For example a file may start with "55" but we got "54". The file is "T" while "54" is U.
 * 
 * @tparam T concept invalidated
 * @tparam U element which cause the format invalidation
 */
template <typename T, typename U>
class InvalidFormatException: public AbstractException {
public:
    InvalidFormatException(T container, U element): AbstractException{} {
        this->message = cpp_utils::sprintf("in %s the element %s cause a format invalidation!", container, element);
    }
};

/**
 * @brief exception to throw when the operation needs to have at least one element in a container, but the container is actually empty
 * 
 * @tparam ITEM type of the element inside the container
 * @tparam CONTAINER container involved
 */
template <typename ITEM, typename CONTAINER>
class ElementNotFoundException: public AbstractException {
public:
    ElementNotFoundException(const ITEM& item, const CONTAINER& container) : AbstractException{} {
        this->message = cpp_utils::scout("Item ", item, " not present in container ", container);
    }
};

class InvalidArgumentException: public AbstractException {
public:
    InvalidArgumentException(): AbstractException{} {
        this->message = cpp_utils::scout("Invalid argument exception");
    }
    InvalidArgumentException(const char* format, ...) : AbstractException{} {
        char buffer[1000];
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, 1000, format, ap);
        va_end(ap);

        this->message = std::string{buffer};
    }
};

class OperationFailedException: public AbstractException {
public:
	OperationFailedException(const char* operation): AbstractException{}, operation{operation} {
		this->message = cpp_utils::sprintf("Operation %s failed", operation);
	}
private:
	const char* operation;	
};

class CommandFailedException : public AbstractException {
public:
    CommandFailedException(const std::string& command, int exitCode): AbstractException{}, command{command}, exitCode{exitCode} {
        this->message = cpp_utils::scout("Command \"", command, "\" failed with exit status ", exitCode);
    }
private:
    std::string command;
    int exitCode;
};

class FileOpeningException: public AbstractException {
public:
    FileOpeningException(const std::string& filename): AbstractException{}, filename{filename} {
        this->message = cpp_utils::sprintf("Couldn't open file \"%s\". Maybe it doesn't exist?", filename);
    }
    FileOpeningException(const char* filename): AbstractException{}, filename{filename} {
        this->message = cpp_utils::sprintf("Couldn't open file \"%s\". Maybe it doesn't exist?", filename);
    }
private:
    std::string filename;
};

/**
 * @brief thrown when you are sure that performing a numeric operation will lead to a wrong result
 * 
 * For example:
 *  * division by 0;
 *  * overflow sum, subtract or multiplication;
 *  * conversions that will for sure fail;
 * 
 */
class NumericalOperationException: public AbstractException {
public:
    template<typename T>
    NumericalOperationException(const char* operation, T value): AbstractException{swcout("Aborting since performing the operation", operation, "will lead to a wrong numerical conversion of", value, "!")} {

    }

    template<typename T1, typename T2>
    NumericalOperationException(const char* operation, T1 value1, T2 value2): AbstractException{swcout("Aborting since performing the operation", value1, operation, value2, "will surely lead to a wrong result!")} {

    }
};

/**
 * @brief an exception to use in general case. Use the message format to customize the message of the exception
 * 
 */
class GenericException: public AbstractException {
public:
    GenericException(const char* format, ...) : AbstractException{} {
        char buffer[1000];
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, 1000, format, ap);
        va_end(ap);

        this->message = std::string{buffer};
    }
};

/**
 * @brief Exception to throw when we're trying to fetch an element from an empty container
 * 
 * @tparam CONTAINER 
 */
template <typename CONTAINER>
class EmptyObjectException: public AbstractException {
public:
    EmptyObjectException(const CONTAINER& container) : AbstractException{}, container{container} {
        this->message = cpp_utils::sprintf("Container %s is actually empty! Cannot fetch data from it!", container);
    }
private:
    const CONTAINER& container;
};

/**
 * @brief called when you attempted to call an abstract method without implementation
 * 
 */
class AbstractMethodCalledException: public AbstractException {
public:
    AbstractMethodCalledException(const char* methodName) : AbstractException{}, methodName{methodName} {
        this->message = cpp_utils::sprintf("Cannot execute method \"%s\". The developer says it is abstract!", methodName);
    }
private:
    const std::string methodName;
};

}

#endif