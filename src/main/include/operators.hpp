#ifndef _OPERATORS_HEADER__
#define _OPERATORS_HEADER__

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "exceptions.hpp"

namespace cpp_utils {

template <typename X, typename Y>
std::ostream& operator << (std::ostream& ss, const std::pair<X,Y>& p) {
    ss << "<" << p.first << ", " << p.second << ">";
    return ss;
}

template <typename X>
std::ostream& operator << (std::ostream& ss, const std::vector<X>& p) {
    ss << "(size=" << p.size() <<")[";
    for (auto el: p) {
        ss << el << ", ";
    }
    ss << "]";
    return ss;
}

template<typename T>
std::ostream & operator<<(std::ostream& str, const std::unordered_set<T>& v) {
	str << "[ ";
	for (auto it=v.begin(); it != v.end(); ++it) {
		str << *it << " ";
	}
	str << "]";
	return str;
}

template <typename K, typename V>
std::ostream& operator <<(std::ostream& stream, const std::unordered_map<K,V>& map) {
	stream << "{ ";
	for (auto el : map) {
		stream << "[" << el.first << ", " << el.second << "], ";
	}
	stream << " }";
	return stream;
}

/**
 * store a vector instance into a file
 *
 * @pre
 *  @li @c file open with "wb";
 *
 * @param[inout] file the file to write into
 * @param[in] v the vector to save into the file
 */
template<class T>
void saveVectorInFile(std::FILE* file, const std::vector<T>& v) {
	int s = v.size();
	if(std::fwrite(&s, sizeof(s), 1, file) != 1) {
		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
    }
	if(std::fwrite(&v[0], sizeof(T), v.size(), file) != v.size()) {
		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
    }
}

/**
 * Load a vector from a file
 *
 * @pre
 *  @li @c file open with "rb";
 *
 * @param[inout] file the file to read
 * @return a vector instance which has been just read from @c file
 */
template<class T>
std::vector<T> loadVectorFrom(std::FILE* file){
	int s;
	if(std::fread(&s, sizeof(s), 1, file) != 1) {
        throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
    }

	std::vector<T>v(s);

	size_t stuffRead = std::fread(&v[0], sizeof(T), s, file);
	if((int)stuffRead != s) {
		error("we were expecting to read ", s, " but we read", stuffRead, "elements instead");
		throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
	}

	return v; // NVRO
}

}

#endif