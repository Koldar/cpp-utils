#ifndef _OPERATORS_HEADER__
#define _OPERATORS_HEADER__

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "exceptions.hpp"

//define the overloading of operators in the namespace of its argument
namespace std {

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

}

#endif