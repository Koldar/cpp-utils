#include "serializers.hpp"
#include <boost/filesystem.hpp>
#include "file_utils.hpp"

namespace cpp_utils::serializers {

void saveInFile(std::FILE* f, const int& v) {
    //TODO here valgrind says that there is an unitiialized write, but I can't solve it (btw: it'ìs not true)
	if (std::fwrite(&v, sizeof(v), 1, f) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{cpp_utils::recoverFilename(f)};
	}
}

void saveInFile(std::FILE* f, const char& v) {
	if (std::fwrite(&v, sizeof(v), 1, f) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{cpp_utils::recoverFilename(f)};
	}
}

void saveInFile(std::FILE* f, const unsigned int& v) {
	if (std::fwrite(&v, sizeof(v), 1, f) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{cpp_utils::recoverFilename(f)};
	}
}

void saveInFile(std::FILE* f, const std::string& v) {
    //string size
    auto length = v.size();
    if (std::fwrite(&length, sizeof(length), 1, f) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{cpp_utils::recoverFilename(f)};
	}
    //actual string
	if (std::fwrite(v.c_str(), length, 1, f) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{cpp_utils::recoverFilename(f)};
	}
}

int& loadFromFile(FILE* f, int& result) {
    if (std::fread(&result, sizeof(result), 1, f) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{cpp_utils::recoverFilename(f)};
    }
    return result;
}

char& loadFromFile(FILE* f, char& result) {
    if (std::fread(&result, sizeof(result), 1, f) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{cpp_utils::recoverFilename(f)};
    }
    return result;
}

unsigned int& loadFromFile(FILE* f, unsigned int& result) {
    if (std::fread(&result, sizeof(result), 1, f) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{cpp_utils::recoverFilename(f)};
    }
    return result;
}

std::string& loadFromFile(FILE* f, std::string& result) {
    //string size
    size_t length = 0;
    if (std::fread(&length, sizeof(length), 1, f) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{cpp_utils::recoverFilename(f)};
    }
    //actual string
    char* content = new char[length + 1];
    //set 0 padding
    content[length] = 0;
    if (std::fread(&result, sizeof(char) * length, 1, f) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{cpp_utils::recoverFilename(f)};
    }
    result = std::string{content};
    return result;
}

}