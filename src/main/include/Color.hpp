#ifndef _CPPUTILS_COLOR_HEADER__
#define _CPPUTILS_COLOR_HEADER__

#include "wrapped_number.hpp"

namespace cpp_utils {

    typedef wrapped_number<unsigned char, 0, 255> wrapped_uchar;

	class color_t {
	public:
		friend std::ostream& operator << (std::ostream& out, const color_t& c);
	public:
		static const color_t BLACK;
		static const color_t WHITE;
		static const color_t RED;
		static const color_t GREEN;
		static const color_t BLUE;
		static const color_t CYAN;
	public:
		color_t();
		color_t(const wrapped_uchar& r, const wrapped_uchar& g, const wrapped_uchar& b);
		color_t(const color_t& other);
		virtual ~color_t();
		color_t& operator=(const color_t& other);
	public:
		wrapped_uchar getRed() const;
		wrapped_uchar getGreen() const;
		wrapped_uchar getBlue() const;
	public:
		color_t merge(const color_t& other) const;
		color_t& merge(const color_t& other);
	private:
		wrapped_uchar red;
		wrapped_uchar green;
		wrapped_uchar blue;
	};

	std::ostream& operator << (std::ostream& out, const color_t& c);

}

#endif 