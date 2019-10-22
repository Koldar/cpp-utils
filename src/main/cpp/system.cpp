#include "system.hpp"

#include "exceptions.hpp"
#include "log.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>
#include "configurations.hpp"

namespace cpp_utils {

	namespace internal {

		::std::string callPyEval(::std::stringstream& ss) {
            return ss.str();
        }

	}

	pid_t getCurrentPID() {
        return ::getpid();
    }


	std::vector<boost::filesystem::path> getOpenFileDescriptors() {
		DIR *dp;
		std::vector<boost::filesystem::path> result{};

		std::string filename = scout("/proc/", getCurrentPID(), "/fd/");
		dp = opendir(filename.c_str());
		if (dp != NULL) {
			struct dirent *ep;
			while (ep = readdir(dp)) {

				std::string subFileInvolved{filename + ep->d_name};
				if (boost::filesystem::is_symlink(subFileInvolved)) {
					result.push_back(getLinkTargetName(subFileInvolved));
				} else {
					result.push_back(subFileInvolved);
				}
				
			}
			closedir(dp);
		} else {
			throw cpp_utils::exceptions::FileOpeningException{filename};
		}

		return result;
	}

	boost::filesystem::path getLinkTargetName(const boost::filesystem::path& link) {
		char buffer[BUFFER_SIZE];
		info("reading link ", boost::filesystem::absolute(link.string()).c_str());
		ssize_t bytesWritten = ::readlink(boost::filesystem::absolute(link.string()).c_str(), &buffer[0], BUFFER_SIZE);

		if (bytesWritten < 0) {
			error("reason", strerror(errno));
			throw cpp_utils::exceptions::ImpossibleException{"readlink error!"};
		}
		if (bytesWritten >= BUFFER_SIZE) {
			throw cpp_utils::exceptions::ImpossibleException{"buffer completely full!"};
		}
		//append null termination since readLinkDoesn't do it
		buffer[bytesWritten] = '\0';

		return buffer;
	}


	int callExternalProgram(const char* format, ...) {
		char buffer[1000];
		va_list va;
		va_start(va, format);
		vsnprintf(buffer, 1000, format, va);
		va_end(va);

		info("command to execute ", buffer);
		int exitCode = system(buffer);
		if (exitCode != 0) {
			throw exceptions::CommandFailedException{std::string{buffer}, exitCode};
		}
		return exitCode;
	}

	bool isProgramInstalled(const std::string& command) {
		std::string output = callUnsafeExternalProgramAndFetchOutput("which ", command);
		return output.size() > 0;
	}

}