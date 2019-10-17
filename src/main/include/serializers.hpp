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
 * By using `saveToFile` I can cover all these use cases.
 * 
 * Preconditions
 * =============
 * 
 * All `saveToFile` and `loadFromFile` functions needs to be positioned in `cpp_utils::serializers`
 * 
 */

#ifndef _CPP_UTILS_SERIALIZERS_HEADER__
#define _CPP_UTILS_SERIALIZERS_HEADER__

#include <cstdio>
#include <string>
#include <vector>
#include "exceptions.hpp"

namespace cpp_utils::serializers {

    void saveToFile(std::FILE* f, const bool& v);

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
    void saveToFile(std::FILE* f, const int& v);

    void saveToFile(std::FILE* f, const char& v);

    void saveToFile(std::FILE* f, const unsigned int& v);

    void saveToFile(std::FILE* f, const size_t& v);

    void saveToFile(std::FILE* f, const std::string& v);

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
    void saveToFile(std::FILE* file, const std::vector<EL>& v) {
        int s = v.size();
        if(std::fwrite(&s, sizeof(s), 1, file) != 1) {
            throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
        }
        if(std::fwrite(&v[0], sizeof(EL), v.size(), file) != v.size()) {
            throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
        }
    }

    bool& loadFromFile(FILE* f, bool& result);

    int& loadFromFile(FILE* f, int& result);

    char& loadFromFile(FILE* f, char& result);

    unsigned int& loadFromFile(FILE* f, unsigned int& result);

    size_t& loadFromFile(FILE* f, size_t& result);

    std::string& loadFromFile(FILE* f, std::string& result);

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

}

#endif