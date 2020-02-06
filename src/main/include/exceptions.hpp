#ifndef _EXCEPTION_HEADER__
#define _EXCEPTION_HEADER__

#include <boost/stacktrace.hpp>
#include <boost/filesystem.hpp>
#include <exception>
#include <string>
#include <cstdarg>
#include "strings.hpp"
#include "log.hpp"
#include "file_utils.hpp"

namespace cpp_utils::exceptions {

    /**
     * @brief super class of all the exceptions
     * 
     */
    class AbstractException: public std::exception {
    public:
        typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;
    protected:
        std::string message;
        boost::stacktrace::stacktrace stacktrace;
        mutable std::string wholeMessage;
    public:
        AbstractException(): message{}, stacktrace{boost::stacktrace::stacktrace()}, wholeMessage{} {
        }
        AbstractException(const std::string& str): message{str}, stacktrace{boost::stacktrace::stacktrace()}, wholeMessage{} {
        }
        template <typename... OTHERS>
        AbstractException(const OTHERS&... others) : message{swcout(others...)}, stacktrace{boost::stacktrace::stacktrace()}, wholeMessage{} {
        }
        virtual const char* what() const throw() {
            if (this->wholeMessage.empty()) {
                this->wholeMessage = scout(this->message, "\n", this->stacktrace);
            }
            return this->wholeMessage.c_str();
        }
        const std::string& getMessage() const throw() {
            return this->message;
        }
    protected:
        void updateMessage(const char* cstr) {
            std::stringstream ss;
            ss << std::string{cstr} << "\n" << this->stacktrace;
            this->message = ss.str();
        }
        void updateMessage(const std::string& str) {
            std::stringstream ss;
            ss << str << "\n" << this->stacktrace;
            this->message = ss.str();
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
        ImpossibleException() : AbstractException{"The impossible has sadly happened!"} {

        }
        template <typename... OTHERS>
        ImpossibleException(const OTHERS&... others) : AbstractException{others...} {
        }
    };

    /**
     * @brief create an impossible exception
     * 
     * @code
     * throw makeImpossibleException("the number", 3, "is invalid!");
     * @endcode
     * 
     * @tparam OTHERS 
     * @param types message to put in the exception
     * @return ImpossibleException instance of the impossible exception
     */
    template <typename ...OTHERS>
    ImpossibleException makeImpossibleException(const OTHERS&... types) {
        return ImpossibleException{cpp_utils::scout(types...).c_str()};
    }

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
        InvalidScenarioException(const std::string& description, const TYPE& t) : AbstractException{"No matched scenario for", description, "for value ", t, ", which type is ", typeid(t).name()}, t{t} {
        }
    private:
        TYPE t;
    };

    /**
     * @brief convenience method to avoid explicitling writing the name of the type
     * 
     * @code
     *     makeInvalidScenarioException(5);
     * @endcode
     * 
     * @param t the type which is not within our expected scenarios
     * @return InvalidScenarioException<TYPE> an exception
     */
    template <typename TYPE>
    [[deprecated]]
    InvalidScenarioException<TYPE> makeInvalidScenarioException(TYPE t) {
        return InvalidScenarioException<TYPE>(t);
    }

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
        InvalidPairScenarioException(const T1& t1, const T2& t2) : AbstractException{"No matched scenario for value", t1, "-", t2}, t1{t1}, t2{t2} {
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
        InvalidStateException(const SELF& self): AbstractException{self, "is in an invalid state"}, self{self} {
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
        InvalidFormatException(const T& container, const U& element): AbstractException{"within container \"", container, "\" the element \"", element, "\" cause a format invalidation!"} {
        }
    };

    /**
     * @brief Exception thrown when you expected a well specific value, but you got something else
     * 
     */
    class ExpectedValueException: public AbstractException {
    public:
        template <typename X, typename Y>
        ExpectedValueException(const X& expected, const Y& actual): AbstractException{"expected \"", expected, "\", but got \"", actual, "\""} {

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
        ElementNotFoundException(const ITEM& item, const CONTAINER& container) : AbstractException{"Item \"", item, "\" not present in container \"", container, "\""} {
        }
    };

    /**
     * @brief Exception to throw if you still need to implement a feature
     * 
     */
    class NotYetImplementedException: public AbstractException {
    public:
        NotYetImplementedException(const std::string& c): AbstractException{"Feature", c, "still needs to be implemented"} {

        }
    };

    /**
     * @brief exception to throw when a function cannot operate correctly with the the parameters given
     * 
     */
    class InvalidArgumentException: public AbstractException {
    public:
        template <typename... OTHER>
        InvalidArgumentException(const OTHER&... other): AbstractException{other...} {

        } 
    };

    class OperationFailedException: public AbstractException {
    public:
        OperationFailedException(const std::string& operation): AbstractException{"Operation", operation, "failed"}, operation{operation} {
        }
    private:
        std::string operation;	
    };

    class CommandFailedException : public AbstractException {
    public:
        CommandFailedException(const std::string& command, int exitCode): AbstractException{"Command \"", command, "\" failed with exit status ", exitCode}, command{command}, exitCode{exitCode} {
        }
    private:
        std::string command;
        int exitCode;
    };

    /**
     * @brief thrown when you're trying to call an external program via ::system, but the program does not exist
     * 
     */
    class CommandNotFoundException : public AbstractException {
    public:
        CommandNotFoundException(const std::string& command, const std::string& version, const std::string& howToInstall): 
            AbstractException{"Command \"", command, "\" is not installed on your system. I've installed it via ", howToInstall, " and in my setup the version was ", version}, 
            command{command}, version{version}, howToInstall{howToInstall} {
        }

        CommandNotFoundException(const std::string& command, const std::string& howToInstall): 
            AbstractException{"Command \"", command, "\" is not installed on your system. I've installed it via ", howToInstall}, 
            command{command}, version{""}, howToInstall{howToInstall} {
        }
    private:
        std::string command;
        std::string version;
        std::string howToInstall;
    };

    class FileOpeningException: public AbstractException {
    private:
        std::string filename;
    public:
        FileOpeningException(const boost::filesystem::path& p): AbstractException{"Couldn't open file \"", p.native(), "\". Maybe it doesn't exist?"}, filename{p.native()} {
        }
        FileOpeningException(const std::string& filename): AbstractException{"Couldn't open file \"", filename, "\". Maybe it doesn't exist?"}, filename{filename} {
        }
        FileOpeningException(const char* filename): AbstractException{"Couldn't open file \"", filename, "\". Maybe it doesn't exist?"}, filename{filename} {
        }
        FileOpeningException(FILE* f): AbstractException{"Couldn't open file \"", recoverFilename(f), "\". Maybe it doesn't exist?"}, filename{recoverFilename(f)} {
        }
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
        NumericalOperationException(const char* operation, T value): AbstractException{"Aborting since performing the operation", operation, "will lead to a wrong numerical conversion of", value, "!"} {

        }

        template<typename T1, typename T2>
        NumericalOperationException(const char* operation, T1 value1, T2 value2): AbstractException{"Aborting since performing the operation", value1, operation, value2, "will surely lead to a wrong result!"} {

        }
    };

    /**
     * @brief an exception to use in general case. Use the message format to customize the message of the exception
     * 
     */
    class GenericException: public AbstractException {
    public:
        template <typename... OTHER>
        GenericException(const OTHER&... other) : AbstractException{other...} {
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
        EmptyObjectException(const CONTAINER& container) : AbstractException{"Container", container, "is actually empty! Cannot fetch data from it!"}, container{container} {
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
        AbstractMethodCalledException(const char* methodName) : AbstractException{"Cannot execute method \"", methodName, "\". The developer says it is abstract!"}, methodName{methodName} {
        }
    private:
        const std::string methodName;
    };

}

#endif