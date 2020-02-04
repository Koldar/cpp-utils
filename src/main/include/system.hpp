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
#include "imemory.hpp"
#include "operators.hpp"
#include "strings.hpp"

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
     * @brief check if a program is installed on the system
     * 
     * The function uses `which` to establish that
     * 
     * @param command the command to check its existence
     * @return true 
     * @return false 
     */
    bool isProgramInstalled(const std::string& command);

    [[deprecated("does not fully work")]]
    FILE* popenRead(const char* command, const char* mode);

    [[deprecated("does not fully work")]]
    int pclose(FILE* fileDescriptor);

    /**
     * @brief the the ram used by a process
     * 
     * @param processId the process involved
     * @return MemoryConsumption bytes of used ram
     */
    MemoryConsumption getProcessUsedRAM(pid_t processId);

    /**
     * @brief ram use din the system
     * 
     * @return MemoryConsumption bytes of used ram
     */
    MemoryConsumption getSystemRAMUsed();

    /**
     * @brief the pid of the current process
     * 
     * @return pid_t the pid of he current process
     */
    pid_t getCurrentPID();

    size_t getPIDNumberOpened();

    /**
     * @brief get the open files this process has
     * 
     * @note
     * this function will return **all** the file descriptor open, included some notable ones:
     * @li "." (the current working directory);
     * @li ".." (the parent fo the current working directory)
     * @li "/dev/pts/<number>", the `stdin`;
     * @li "/dev/pts/<number>", the `stdout`;
     * @li "/dev/pts/<number>", the `stderr`;
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
        debug("pid opened are", getPIDNumberOpened());

        std::array<char, 128> buffer;
        //std::unique_ptr<FILE, decltype(&pclose)> pipe{popen(cmd.c_str(), "r"), pclose};
        critical("executing", cmd);
        errno = 0;
        FILE* pipe = ::popen(cmd.c_str(), "r");
        if (pipe == nullptr) {
            log_error("failed executing cmd \"", cmd.c_str(), "\"");
            log_error("It is possible that fork has failed. errno is ", errno, "description: ", strerror(errno));
			log_error("AFAIK there are either 2 issues: ");
			log_error("1) there is left literally no memory. Consider buying new RAM or creating a swap");
			log_error("2) overcommit is prevented (see https://stackoverflow.com/a/15623171/1887602). Or with root do \" echo 1 > /proc/sys/vm/overcommit_memory\"");
			log_error("3) the child process didn't exit before running out of memory. Happens if you're running a big script taking lots of memory");
            throw cpp_utils::exceptions::makeImpossibleException("popen() failed! While executing ", cmd);
        }

        std::string result{};
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

        int exitStatus = ::pclose(pipe);

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
        debug("pid opened are", getPIDNumberOpened());
        debug("open files are ", getOpenFileDescriptors());
        critical("executing", cmd);
        
        errno = 0;
        FILE* pipe = popen(cmd.c_str(), "r");
        if (pipe == nullptr) {
            log_error("failed executing cmd \"", cmd.c_str(), "\"");
            log_error("It is possible that fork has failed. errno is ", errno, "description: ", strerror(errno));
			log_error("AFAIK there are either 2 issues: ");
			log_error("1) there is left literally no memory. Consider buying new RAM or creating a swap");
			log_error("2) overcommit is prevented (see https://stackoverflow.com/a/15623171/1887602). Or with root do \" echo 1 > /proc/sys/vm/overcommit_memory\"");
			log_error("3) the child process didn't exit before running out of memory. Happens if you're running a big script taking lots of memory");
            throw cpp_utils::exceptions::ImpossibleException{"popen() failed! While executing %s", cmd};
        }

        std::string result{};
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

        ::pclose(pipe);

        return result;
    }


    /**
     * @brief Call an external program, but before calling it, it check if it is present in the system
     * 
     * @note
     * the function will fail if the system call will receive an exit code which is not 0
     * 
     * @param programNameToCheck the name of the external program you rely on (e.g., dot, valgrind)
     * @param versionYouHave useful for external developer. By using this value you can tell them which version of the program you've been using all along
     * @param others variadic list of elements that will form the command you will execute
     * @return int exist code. Usualluy 0
     */
    template <typename ...OTHERS>
    int callExternalProgramSafe(const char* programNameToCheck, const char* versionYouHave, const OTHERS&... others) {
        auto cmd = scout(others...);
        if (!isProgramInstalled(programNameToCheck)) {
            throw cpp_utils::exceptions::CommandNotFoundException{programNameToCheck, versionYouHave, ""};
        }

        return callExternalProgram("%s", cmd.c_str());
    }

    template <typename ...OTHER>
    std::string callExternalProgramSafeAndFetchOutput(const char* programNameToCheck, const char* versionYouHave, const OTHER&... others) {
        auto cmd = scout(others...);
        if (!isProgramInstalled(programNameToCheck)) {
            throw cpp_utils::exceptions::CommandNotFoundException{programNameToCheck, versionYouHave, ""};
        }

        return callExternalProgramAndFetchOutput(cmd);
    }

    template <typename ...OTHER>
    int callFFMPEG(const OTHER&... other) {
        return callExternalProgramSafe("ffmpeg", "4.1.4", other...);
    }

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

        debug("command is ", cmd);
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