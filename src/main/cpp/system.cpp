#include "system.hpp"

#include "exceptions.hpp"
#include "log.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace cpp_utils {

	namespace internal {

		::std::string callPyEval(::std::stringstream& ss) {
            return ss.str();
        }

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
		std::string output = callExternalProgramAndFetchOutput("which ", command);
		return output.size() > 0;
	}

}