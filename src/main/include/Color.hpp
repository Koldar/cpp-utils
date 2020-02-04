#ifndef _CPPUTILS_COLOR_HEADER__
#define _CPPUTILS_COLOR_HEADER__

#include "wrapped_number.hpp"

namespace cpp_utils {

    typedef wrapped_number<unsigned char, 0, 255, int> wrapped_uchar;

	class color_t {
	public:
		friend std::ostream& operator << (std::ostream& out, const color_t& c);
		friend bool operator ==(const color_t& a, const color_t& b);
		friend bool operator !=(const color_t& a, const color_t& b);
	public:
		static const color_t LIGHT_RED;
		static const color_t RED;
		static const color_t DARK_RED;

		static const color_t LIGHT_GREEN;
		static const color_t GREEN;
		static const color_t DARK_GREEN;

		static const color_t LIGHT_BLUE;
		static const color_t BLUE;
		static const color_t DARK_BLUE;

		static const color_t LIGHT_ORANGE;
		static const color_t ORANGE;
		static const color_t DARK_ORANGE;

		static const color_t LIGHT_YELLOW;
		static const color_t YELLOW;
		static const color_t DARK_YELLOW;

		static const color_t LIGHT_CYAN;
		static const color_t CYAN;
		static const color_t DARK_CYAN;

		static const color_t LIGHT_PURPLE;
		static const color_t PURPLE;
		static const color_t DARK_PURPLE;

		static const color_t LIGHT_VIOLET;
		static const color_t VIOLET;
		static const color_t DARK_VIOLET;

		static const color_t LIGHT_GRASS;
		static const color_t GRASS;
		static const color_t DARK_GRASS;

		static const color_t LIGHT_SKY;
		static const color_t SKY;
		static const color_t DARK_SKY;

		static const color_t LIGHT_PINKISH;
		static const color_t PINKISH;
		static const color_t DARK_PINKISH;

		static const color_t LIGHT_BROWN;
		static const color_t BROWN;
		static const color_t DARK_BROWN;
		
		static const color_t LIGHT_GREY;
		static const color_t GREY;
		static const color_t DARK_GREY;

		static const color_t BLACK;
		static const color_t WHITE;
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

		/**
		 * @brief make a color lighter
		 * 
		 * @param scale a value between 0 and 1. for instance 0.2 means make the color lighter by 20%
		 * @return color_t& new color
		 */
		color_t lighter(double scale) const;

		/**
		 * @brief make a color lighter
		 * 
		 * @param scale a value between 0 and 1. for instance 0.2 means make the color lighter by 20%
		 * @return color_t& this
		 */
		color_t& lighter(double scale);

		/**
		 * @brief make a color darker
		 * 
		 * @param scale a value between 0 and 1. for instance 0.2 means make the color darker by 20%
		 * @return color_t& new color
		 */
		color_t darker(double scale) const;

		/**
		 * @brief make a color darker
		 * 
		 * @param scale a value between 0 and 1. for instance 0.2 means make the color darker by 20%
		 * @return color_t& this
		 */
		color_t& darker(double scale);

		/**
		 * @brief multiply each component with the given multiplier
		 * 
		 * @code
		 * 	color_t{100,10,20}.scale(1.2) //120, 12, 25
		 * @endcode
		 * 
		 * @param scale multipler. if the multiplication exceeds 255, it is truncated to 255
		 * @return color_t a new color
		 */
		color_t scale(double scale) const;
		/**
		 * @brief multiply each component with the given multiplier
		 * 
		 * @code
		 * 	color_t{100,10,20}.scale(1.2) //120, 12, 25
		 * @endcode
		 * 
		 * @param scale multipler. if the multiplication exceeds 255, it is truncated to 255
		 * @return color_t this
		 */
		color_t& scale(double scale);
		/**
		 * @brief invert each component fo the color
		 * 
		 * @code
		 *  {255,123,6}.invert() //0,132,249
		 * @endcode
		 * 
		 * @return color_t a new color
		 */
		color_t invert() const;

		/**
		 * @brief invert each component fo the color
		 * 
		 * @code
		 *  {255,123,6}.invert() //0,132,249
		 * @endcode
		 * 
		 * @return color_t this
		 */
		color_t& invert();

		/**
		 * @brief compute a new color based on the minimum of each components of the colors
		 * 
		 * @code
		 *  {255,123,6}.min(6, 123, 127) //6,123,6
		 * @endcode
		 * 
		 * @param other the other colors involved
		 * @return color_t new color
		 */
		color_t min(const color_t& other) const;

		/**
		 * @brief compute a new color based on the minimum of each components of the colors
		 * 
		 * @code
		 *  {255,123,6}.min(6, 123, 127) //6,123,6
		 * @endcode
		 * 
		 * @param other the other colors involved
		 * @return color_t this
		 */
		color_t& min(const color_t& other);

		/**
		 * @brief compute a new color based on the maximum of each components of the colors
		 * 
		 * @code
		 *  {255,123,6}.max(6, 123, 127) //255,123,127
		 * @endcode
		 * 
		 * @param other the other colors involved
		 * @return color_t new color
		 */
		color_t max(const color_t& other) const;

		/**
		 * @brief compute a new color based on the maximum of each components of the colors
		 * 
		 * @code
		 *  {255,123,6}.max(6, 123, 127) //255,123,127
		 * @endcode
		 * 
		 * @param other the other colors involved
		 * @return color_t this
		 */
		color_t& max(const color_t& other);

		color_t lerp(const color_t& other) const;
		color_t& lerp(const color_t& other);

		/**
		 * @brief perform a lerp between 2 colors
		 * 
		 * 
		 * 
		 * @param other 
		 * @return color_t 
		 */
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