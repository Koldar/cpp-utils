#ifndef _PPMIMAGE_HEADER__
#define _PPMIMAGE_HEADER__

#include "wrapped_number.hpp"
#include "IImageable.hpp"
#include <fstream>
#include <iostream>

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
public:
	color_t() : red{0}, green{0}, blue{0} {

	} 
	color_t(const wrapped_uchar& r, const wrapped_uchar& g, const wrapped_uchar& b) : red{r}, green{g}, blue{b} {
		
	}
	color_t(const color_t& other) : red{other.red}, green{other.green}, blue{other.blue} {
		
	}
	virtual ~color_t() {
		
	}
	color_t& operator=(const color_t& other) {
		this->red = other.red;
		this->green = other.green;
		this->blue = other.blue;
		return *this;
	}
public:
	wrapped_uchar getRed() const {
		return this->red;
	}
	wrapped_uchar getGreen() const {
		return this->green;
	}
	wrapped_uchar getBlue() const {
		return this->blue;
	}
public:
	color_t merge(const color_t& other) const {
		color_t result{*this};
		return result.merge(other);
	}
	color_t& merge(const color_t& other) {
		wrapped_uchar minRed;
		wrapped_uchar maxRed;
		wrapped_uchar minGreen;
		wrapped_uchar maxGreen;
		wrapped_uchar minBlue;
		wrapped_uchar maxBlue;
		if (this->red > other.red) {
			minRed = other.red;
			maxRed = this->red;
		} else {
			minRed = this->red;
			maxRed = other.red;
		}

		if (this->green > other.green) {
			minGreen = other.green;
			maxGreen = this->green;
		} else {
			minGreen = this->green;
			maxGreen = other.green;
		}

		if (this->blue > other.blue) {
			minBlue = other.blue;
			maxBlue = this->blue;
		} else {
			minBlue = this->blue;
			maxBlue = other.blue;
		}

		this->red = minRed + (maxRed - minRed)/2;
		this->green = minGreen + (maxGreen - minGreen)/2;
		this->blue = minBlue + (maxBlue - minBlue)/2;

		return *this;
	}
private:
	wrapped_uchar red;
	wrapped_uchar green;
	wrapped_uchar blue;
};

std::ostream& operator << (std::ostream& out, const color_t& c);

class IImageable;
class PPMImage : public IImageable {
	friend PPMImage operator +(const PPMImage& a, const PPMImage& b);
	friend PPMImage operator +(const PPMImage& a, const color_t& b);
private:
	size_t width;
	size_t height;
	color_t* image;
public:
	PPMImage(const size_t& width, const size_t& height, color_t color) : PPMImage{width, height} {
		this->setAllPixels(color);
	}
	PPMImage(const size_t& width, const size_t& height) : width{width}, height{height}, image{nullptr} {
		this->image = new color_t[this->totalPixels()];
	}
	PPMImage(const PPMImage& other);
	PPMImage(PPMImage&& other);
	PPMImage(const char* filename);
	~PPMImage() {
		this->deleteImage();
	}
	PPMImage& operator =(const PPMImage& other);
	PPMImage& operator =(PPMImage&& other);
protected:
	virtual const PPMImage* getPPM() const;
public:
	PPMImage& operator +=(const PPMImage& other);
	PPMImage& operator +=(const color_t& other);
public:
	color_t getPixel(size_t x, size_t y) const;
	void setPixel(size_t x, size_t y, const color_t& color);
	void setAllPixels(const color_t& color);
	void mergePixel(size_t x, size_t y, const color_t& color);
	void merge(const PPMImage& other);
	PPMImage& mean(const PPMImage& other);
	void savePPM(const std::string& filename) const;
	size_t totalPixels() const {
		return this->width * this->height;
	}
private:
	/**
	 * @brief generate a file in the CWD representing a PPM image
	 * 
	 * @post
	 *  @li ppm image inside CWD
	 * 
	 * @param filename name of the filename. This string will be appended to the ".ppm" extension
	 */
	void generatePPM(const std::string& filename) const;
	static void checkDimensions(const PPMImage& a, const PPMImage& b);
	void deleteImage();
	};

PPMImage operator +(const PPMImage& a, const PPMImage& b);
PPMImage operator +(const PPMImage& a, const color_t& b);

}

#endif