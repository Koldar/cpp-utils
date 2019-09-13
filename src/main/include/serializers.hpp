/**
 * @file
 * @brief set of functions to save and load elements
 * 
 * Rationale
 * =========
 * 
 * I've decided not to create an interface "ISerliazable" because everything can be serialized:
 * not only classes or structs (which might exploit the ineritance of ISerializable) but also
 * classes which I cannot change (e.g., std::vector) or primitive types (e.g., int).
 * 
 * By using `saveInFile` I can cover all these use cases.
 * 
 * Preconditions
 * =============
 * 
 * All `saveInFile` and `loadFromFile` functions needs to be positioned in `cpp_utils::serializers`
 * 
 */

#ifndef _CPP_UTILS_SERIALIZERS_HEADER__
#define _CPP_UTILS_SERIALIZERS_HEADER__

#include <boost/filesystem.hpp>

namespace cpp_utils::serializers {

/**
 * @brief generic code for a generic value. Put all its bytes inside the file
 * 
 * @pre
 *  @li @c file open with "wb";
 * 
 * @tparam T type to save
 * @param[inout] f file to update
 * @param[in] v object to store
 */
void saveInFile(std::FILE* f, const int& v) {
    //TODO here valgrind says that there is an unitiialized write, but I can't solve it (btw: it'ìs not true)
	if (std::fwrite(&v, sizeof(v), 1, f) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
	}
}

void saveInFile(std::FILE* f, const char& v) {
	if (std::fwrite(&v, sizeof(v), 1, f) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
	}
}

void saveInFile(std::FILE* f, const unsigned int& v) {
	if (std::fwrite(&v, sizeof(v), 1, f) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
	}
}

void saveInFile(std::FILE* f, const std::string& v) {
    //string size
    auto length = v.size();
    if (std::fwrite(&length, sizeof(length), 1, f) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
	}
    //actual string
	if (std::fwrite(v.c_str(), length, 1, f) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
	}
}

/**
 * store a vector instance into a file
 *
 * @pre
 *  @li @c file open with "wb";
 *
 * @tparam EL items of the vector
 * @param[inout] file the file to write into
 * @param[in] v the vector to save into the file
 */
template <typename EL>
void saveInFile(std::FILE* file, const std::vector<EL>& v) {
	int s = v.size();
	if(std::fwrite(&s, sizeof(s), 1, file) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
    }
	if(std::fwrite(&v[0], sizeof(EL), v.size(), file) != v.size()) {
		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
    }
}

int& loadFromFile(FILE* f, int& result) {
    if (std::fread(&result, sizeof(result), 1, f) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
    }
    return result;
}

char& loadFromFile(FILE* f, char& result) {
    if (std::fread(&result, sizeof(result), 1, f) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
    }
    return result;
}

unsigned int& loadFromFile(FILE* f, unsigned int& result) {
    if (std::fread(&result, sizeof(result), 1, f) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
    }
    return result;
}

std::string& loadFromFile(FILE* f, std::string& result) {
    //string size
    size_t length = 0;
    if (std::fread(&length, sizeof(length), 1, f) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
    }
    //actual string
    char* content = new char[length + 1];
    //set 0 padding
    content[length] = 0;
    if (std::fread(&result, sizeof(char) * length, 1, f) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
    }
    result = std::string{content};
    return result;
}

template <typename EL>
std::vector<EL>& loadFromFile(FILE* f, std::vector<EL>& result) {
    int s;
    if(std::fread(&s, sizeof(s), 1, f) != 1) {
        error("error while reading vector size. Sizeof(EL)", sizeof(s));
        throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
    }
    result.resize(s);

    size_t stuffRead = std::fread(&result[0], sizeof(EL), s, f);
    if((int)stuffRead != s) {
        error("we were expecting to read ", s, " but we read", stuffRead, "elements instead");
        throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
    }

    return result; // NVRO
}

// // template <typename T>
// // T loadFromFile(const boost::filesystem::path& p) {
// //     FILE* f = fopen(p.native().c_str(), "rb");
// //     if (f == nullptr ) {
// //         throw cpp_utils::exceptions::FileOpeningException{p};
// //     }
// //     T result = loadFromFile<T>(f);
// //     fclose(f);
// //     return result;
// // }

// // /**
// //  * @brief generic function method to load an element from a file
// //  * 
// //  * @pre
// //  *  @li @c file open with "rb";
// //  * @post
// //  *  @li cursor in @c file moved
// //  * 
// //  * The function will just read sizeof(T) bytes from the stream
// //  * 
// //  * @tparam T the type of object to load
// //  * @param file the file we will read
// //  * @return T the element read
// //  */
// // template <typename T>
// // T loadFromFile(std::FILE* file) {
// //     T result;
// //     if (std::fread(&result, sizeof(result), 1, file) != 1) {
// //         throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
// //     }
// //     return result;
// // }

// /**
//  * Load a vector from a file
//  *
//  * @pre
//  *  @li @c file open with "rb";
//  *
//  * @param[inout] file the file to read
//  * @return a vector instance which has been just read from @c file
//  */
// template<typename EL>
// std::vector<EL> loadVectorFrom(std::FILE* file) {
// 	int s;
// 	if(std::fread(&s, sizeof(s), 1, file) != 1) {
//         throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
//     }
// 	std::vector<EL> v(s);

// 	size_t stuffRead = std::fread(&v[0], sizeof(EL), s, file);
// 	if((int)stuffRead != s) {
// 		error("we were expecting to read ", s, " but we read", stuffRead, "elements instead");
// 		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
// 	}

// 	return v; // NVRO
// }

}

#endif