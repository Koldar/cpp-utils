/**
 * @file system.hpp
 * @author Massimo Bono
 * @brief Contains several methosd to interact with the underlying operating system (paths excluded)
 * @version 0.1
 * @date 2019-10-22
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _CPP_UTILS_SYSTEM_HEADER__
#define _CPP_UTILS_SYSTEM_HEADER__

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <iostream>
#include <iostream>
#include <sstream>
#include <cerrno>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <boost/filesystem.hpp>
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


    /**
     * @brief the pid of the current process
     * 
     * @return pid_t the pid of he current process
     */
    pid_t getCurrentPID();

    /**
     * @brief get the open files this process has
     * 
     * @note
     * this function will return **all** the file descriptor open, included some notable ones:
     * @li "." (the current working directory);
     * @li ".." (the parent fo the current working directory)
     * @li "/dev/pts/1", the `stdin`;
     * @li "/dev/pts/1", the `stdout`;
     * @li "/dev/pts/1", the `stderr`;
     * @li process open files list
     * 
     * Internally, we require to read a file inside `fd` folder. Such file is included in the return value as well.
     * So expect at least 6 files open
     * 
     * @return std::vector<boost::filesystem::path> vector containing all the ope file descriptors
     */
    std::vector<boost::filesystem::path> getOpenFileDescriptors();

    /**
     * @brief get the link target
     * 
     * @pre
     *  @li link is a link filename
     * 
     * @param link the link whose target we need to retrieve
     * @return std::string the target
     */
    boost::filesystem::path getLinkTargetName(const boost::filesystem::path& link);

    int callExternalProgram(const char* format, ...);

    /**
     * @brief call an external program and fetch the `stdout` **only**
     * 
     * @tparam OTHERS args that compose the external program
     * @param output string that will contain the stdout
     * @param args args composing the command to execute
     * @return std::string stdout
     */
    template <typename... OTHERS>
    std::string callExternalProgramAndFetchOutput(const OTHERS&... args) {
        auto cmd = scout(args...);

        // FILE* file = popen("ls", "r");
        // // use fscanf to read:
        // char buffer[100];
        // fscanf(file, "%100s", buffer);
        // pclose(file);

        std::array<char, 128> buffer;
        //std::unique_ptr<FILE, decltype(&pclose)> pipe{popen(cmd.c_str(), "r"), pclose};
        errno = 0;
        FILE* pipe = popen(cmd.c_str(), "r");
        if (pipe == nullptr) {
            error("failed executing cmd \"", cmd.c_str(), "\"");
            error("reason: ", strerror(errno));
            throw cpp_utils::exceptions::ImpossibleException{"popen() failed! While executing %s", cmd};
        }

        std::string result{};
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

        int exitStatus = pclose(pipe);

        if (exitStatus != 0) {
            throw cpp_utils::exceptions::CommandFailedException{cmd, exitStatus};
        }

        return result;
    }

    /**
     * @brief call an external program and fetch the `stdout` **only**
     * 
     * @attention
     * This function won't check process exit code!!!
     * 
     * @tparam OTHERS args that compose the external program
     * @param output string that will contain the stdout
     * @param args args composing the command to execute
     * @return std::string stdout
     */
    template <typename... OTHERS>
    std::string callUnsafeExternalProgramAndFetchOutput(const OTHERS&... args) {
        auto cmd = scout(args...);

        // FILE* file = popen("ls", "r");
        // // use fscanf to read:
        // char buffer[100];
        // fscanf(file, "%100s", buffer);
        // pclose(file);

        std::array<char, 128> buffer;
        //std::unique_ptr<FILE, decltype(&pclose)> pipe{popen(cmd.c_str(), "r"), pclose};
        errno = 0;
        FILE* pipe = popen(cmd.c_str(), "r");
        if (pipe == nullptr) {
            error("failed executing cmd \"", cmd.c_str(), "\"");
            error("reason: ", strerror(errno));
            throw cpp_utils::exceptions::ImpossibleException{"popen() failed! While executing %s", cmd};
        }

        std::string result{};
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

        pclose(pipe);

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

        ss << "pyeval --string=\'" << templateString << "\' ";
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