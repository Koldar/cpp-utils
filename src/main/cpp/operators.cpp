#include "operators.hpp"

namespace std {

    ::std::ostream& operator <<(::std::ostream& ss, const boost::filesystem::path& p) {
		ss << p.string();
		return ss;
	}

}