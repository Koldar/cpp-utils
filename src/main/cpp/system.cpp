#include "system.hpp"

#include "exceptions.hpp"
#include "log.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "configurations.hpp"
#include "math.hpp"

namespace cpp_utils {

	namespace internal {

		::std::string callPyEval(::std::stringstream& ss) {
            return ss.str();
        }

	}

	MemoryConsumption getProcessUsedRAM(pid_t processId) {
		std::ifstream f;
		f.open(scout("/proc/", processId, "/status"));
		std::string variableName;
		while (f >> variableName) {
			// https://stackoverflow.com/a/22326766/1887602
			// https://stackoverflow.com/a/64422/1887602
			// I've opened system monitor and I noticed that the process ram shown there is the same of RssAnon
			if (boost::starts_with(variableName, "RssAnon")) {
				std::string variableValue;
				std::string unit;
				f >> variableValue >> unit;
				auto kilobytes = parseFromString<size_t>(variableValue);
				f.close();
				return MemoryConsumption(kilobytes, MemoryConsumptionEnum::KILOBYTE).to(MemoryConsumptionEnum::BYTE);
			}
		}
		f.close();
		throw cpp_utils::exceptions::ImpossibleException{"VmRSS not found in status file!"};
	}

	MemoryConsumption getSystemRAMUsed() {
		std::ifstream f;
		f.open("/proc/meminfo");

		bool totalFound = false;
		bool freeFound = false;
		bool shmemFound = false;
		bool buffersFound = false;
		bool sreclaimableFound = false;
		bool cachedFound = false;
		size_t memtotal;
		size_t memfree;
		size_t membuffers;
		size_t memshmem;
		size_t memsreclaimable;
		size_t memcached;

		std::string variableName;
		while (f >> variableName) {
			if (boost::starts_with(variableName, "MemTotal")) {
				std::string variableValue, unit;
				f >> variableValue >> unit;
				memtotal = parseFromString<size_t>(variableValue);
				totalFound = true;
			}
			if (boost::starts_with(variableName, "MemFree")) {
				std::string variableValue, unit;
				f >> variableValue >> unit;
				memfree = parseFromString<size_t>(variableValue);
				freeFound = true;
			}
			if (boost::starts_with(variableName, "Shmem")) {
				std::string variableValue, unit;
				f >> variableValue >> unit;
				memshmem = parseFromString<size_t>(variableValue);
				shmemFound = true;
			}
			if (boost::starts_with(variableName, "Buffers")) {
				std::string variableValue, unit;
				f >> variableValue >> unit;
				membuffers = parseFromString<size_t>(variableValue);
				buffersFound = true;
			}
			if (boost::starts_with(variableName, "Cached")) {
				std::string variableValue, unit;
				f >> variableValue >> unit;
				memcached = parseFromString<size_t>(variableValue);
				cachedFound = true;
			}
			if (boost::starts_with(variableName, "SReclaimable")) {
				std::string variableValue, unit;
				f >> variableValue >> unit;
				memsreclaimable = parseFromString<size_t>(variableValue);
				sreclaimableFound = true;
			}

			// see "man free"
			if (totalFound && freeFound && shmemFound && buffersFound && sreclaimableFound && cachedFound) {
				size_t memShared = memshmem;
				size_t memUsed = memtotal - memfree - membuffers - memcached;
				f.close();
				return MemoryConsumption(memUsed + memShared, MemoryConsumptionEnum::KILOBYTE).to(MemoryConsumptionEnum::BYTE);
			}
		}
		f.close();
		throw cpp_utils::exceptions::ImpossibleException{"MemTotal or MemFree not found in status file!"};
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
		debug("reading link ", boost::filesystem::absolute(link.string()).c_str());
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