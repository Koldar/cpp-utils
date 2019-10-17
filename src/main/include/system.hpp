#ifndef _SYSTEM_HEADER__
#define _SYSTEM_HEADER__

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <iostream>
#include <iostream>
#include <sstream>
#include "exceptions.hpp"
#include "math.hpp"

namespace cpp_utils {

    namespace internal {

        ::std::string callPyEval(::std::stringstream& ss);

        template <typename FIRST, typename SECOND, typename... OTHERS>
        ::std::string callPyEval(::std::stringstream& ss, const FIRST& first, const SECOND& second, const OTHERS&... args) {
            ss << "--map \"" << first << "\" \"" << second << "\" ";
            return ::cpp_utils::internal::callPyEval(ss, args...);
        }

    }

    int callExternalProgram(const char* format, ...);

    /**
     * @brief call an external program and fetch the `stdout` **only**
     * 
     * @tparam OTHERS args that compose the external program
     * @param output string that will contain the stdout
     * @param args args composing the command to execute
     * @return int exit code 
     */
    template <typename... OTHERS>
    std::string callExternalProgramAndFetchOutput(const OTHERS&... args) {
        auto cmd = scout(args...);

        std::array<char, 128> buffer;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            throw cpp_utils::exceptions::ImpossibleException{"popen() failed!"};
        }
        // if (pipe. != 0) {
        //     throw cpp_utils::exceptions::CommandFailedException{cmd, pipe.exit_code};
        // }

        std::string result{};
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        return result;
    }


    /**
     * @brief check if a program is installed on the system
     * 
     * The function uses `which` to establish that
     * 
     * @param command the command to check its existence
     * @return true 
     * @return false 
     */
    bool isProgramInstalled(const std::string& command);

    /**
     * @brief Call PyEval script to generate templated strings in C++
     * 
     * @pre
     *  @li `pyEval script installed on your system
     * 
     * @code
     *  pyEval("100*log({V}) + {E}", 
     *      "V", 60,
     *      "E", 140
     *  );
     * @endcode
     * 
     * @tparam OTHERS 
     * @param template template string to pass to pyEval
     * @param python flag to pass to pyEval
     * @param args arguments to pass to pyEval
     * @return std::string output of pyEval
     */
    template <typename... OTHERS>
    std::string callPyEval(const std::string& templateString, bool python, const OTHERS&... args) {
        if (!isProgramInstalled("pyeval")) {
            throw cpp_utils::exceptions::ImpossibleException{"pyeval is not installed on your system!"};
        }

        std::stringstream ss;

        ss << "pyeval --string=\"" << templateString << "\" ";
        if (python) {
            ss << "--python ";
        }

        std::string cmd = internal::callPyEval(ss, args...);

        critical("command is ", cmd);
        return callExternalProgramAndFetchOutput(cmd);
    }

    template <typename... OTHERS>
    std::string callPyEvalWithoutEval(const std::string& templateString, const OTHERS&... args) {
        return callPyEval(templateString, false, args...);
    }

    template <typename... OTHERS>
    std::string callPyEvalWithEval(const std::string& templateString, const OTHERS&... args) {
        return callPyEval(templateString, true, args...);
    }

    template <typename OUT, typename... OTHERS>
    OUT callPyEvalAndCastNumberTo(const std::string& templateString, const OTHERS&... args) {
        return parseFromString<OUT>(callPyEval(templateString, true, args...));
    }

};


#endif