#ifndef _OPERATORS_HEADER__
#define _OPERATORS_HEADER__

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <boost/functional/hash.hpp>
#include <functional>

#include "exceptions.hpp"

//define the overloading of operators in the namespace of its argument
namespace std {

	// ####################################################################
	// <<
	// ####################################################################

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

	namespace operator_lt_lt_implementation {
		template<::std::size_t...> struct seq{};

		template<::std::size_t N, ::std::size_t... Is>
		struct gen_seq : gen_seq<N-1, N-1, Is...>{};

		template<::std::size_t... Is>
		struct gen_seq<0, Is...> : seq<Is...>{};

		template<class Ch, class Tr, class Tuple, ::std::size_t... Is>
		void print_tuple(::std::basic_ostream<Ch,Tr>& os, Tuple const& t, seq<Is...>){
			using swallow = int[];
			(void)swallow{0, (void(os << (Is == 0? "" : ", ") << ::std::get<Is>(t)), 0)...};
		}
	}

	template <typename... ARGS>
	::std::ostream& operator <<(::std::ostream& ss, const ::std::tuple<ARGS...>& tuple) {
		//see https://stackoverflow.com/a/6245777/1887602
		ss << "<";
		print_tuple(ss, tuple, ::std::operator_lt_lt_implementation::gen_seq<sizeof...(ARGS)>());
		ss << ">";
		return ss;
	}

	// ####################################################################
	// hash
	// ####################################################################

	template <typename T1, typename T2>
	struct hash<pair<T1, T2>> {
		size_t operator() (const pair<T1, T2>& node) const {
			size_t seed = 0;
			boost::hash_combine(seed, node.first);
			boost::hash_combine(seed, node.second);
			return seed;
		}
	};

	template <typename T0>
	struct hash<tuple<T0>> {
		size_t operator() (const std::tuple<T0>& t) const {
			size_t seed = 0;

			boost::hash_combine(seed, std::get<0>(t));
			return seed;
		}
	};

	template <typename T0, typename T1>
	struct hash<tuple<T0, T1>> {
		size_t operator() (const std::tuple<T0, T1>& t) const {
			size_t seed = 0;

			boost::hash_combine(seed, std::get<0>(t));
			boost::hash_combine(seed, std::get<1>(t));
			return seed;
		}
	};

	template <typename T0, typename T1, typename T2>
	struct hash<tuple<T0, T1, T2>> {
		size_t operator() (const std::tuple<T0, T1, T2>& t) const {
			size_t seed = 0;

			boost::hash_combine(seed, std::get<0>(t));
			boost::hash_combine(seed, std::get<1>(t));
			boost::hash_combine(seed, std::get<2>(t));
			return seed;
		}
	};
}


#endif
