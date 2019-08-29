/*
 * file_utils.cpp
 *
 *  Created on: Oct 1, 2018
 *      Author: koldar
 */


#include "file_utils.hpp"
#include "log.hpp"
#include <cstdio>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "exceptions.hpp"

namespace cpp_utils {

size_t getBytesOfFile(const std::string& name) {
	struct stat result;
	int statResult = stat(name.c_str(), &result);
	if (statResult != 0) {
		throw exceptions::GenericException{"stat on %s returned error!", name.c_str()};
	}

	return result.st_size;
}

bool isFileExists(const std::string& name) {
//	if (FILE *file = fopen(name.c_str(), "rb")) {
//		fclose(file);
//		return true;
//	} else {
//		return false;
//	}
	std::ifstream f(name.c_str());
	return f.good();
}

std::string getBaseNameAsString(const std::string& filepath) {
	return std::string{getBaseNameAsString(filepath)};
}

std::string getBaseNameAsString(const char* filepath) {
	return std::string{getBaseName(filepath)};
}

const char* getBaseName(const std::string& filepath) {
	return getBaseName(filepath.c_str());
}

const char* getBaseName(const char* filepath) {
	//the last slash we have detected
	const char* tmp = filepath;
	const char* lastSlashDetected = &filepath[0];
	while(*tmp != 0) {
		if (*tmp == '/') {
			lastSlashDetected = tmp;
		}
		++tmp;
	}

	return lastSlashDetected == filepath ? filepath : lastSlashDetected + 1;

}

}