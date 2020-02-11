#ifndef _PPMIMAGE_HEADER__
#define _PPMIMAGE_HEADER__

#include "IImageable.hpp"
#include "Color.hpp"
#include <fstream>
#include <iostream>

namespace cpp_utils {

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
		virtual ~PPMImage() {
			this->deleteImage();
		}
		PPMImage& operator =(const PPMImage& other);
		PPMImage& operator =(PPMImage&& other);
	protected:
		/**
		 * @brief return this very PPM
		 * 
		 * @attention
		 * this function will return this pointer, that will be modified!!!!
		 * 
		 * @return PPMImage* **this** pointer, as modificable
		 */
		virtual PPMImage* getPPM() const;
	public:
		PPMImage& operator +=(const PPMImage& other);
		PPMImage& operator +=(const color_t& other);
	public:
		/**
		 * @brief check if the image is valid
		 * 
		 * An image is valid if it has width and height positive non null and has at least 2 colors
		 * 
		 * @return true 
		 * @return false 
		 */
		bool isValid() const;
		color_t getPixel(size_t x, size_t y) const;
		void setPixel(size_t x, size_t y, const color_t& color);
		/**
		 * @brief Set the Rectangle Pixel object
		 * 
		 * @pre
		 *  @li \f$ top < bottom \f$;
		 *  @li  \f$ left < right \f$;
		 * 
		 * @param top inclusive
		 * @param left inclusive
		 * @param bottom excluded
		 * @param right excluded
		 */
		void setRectanglePixel(size_t top, size_t left, size_t bottom, size_t right, const color_t& color, bool bottomIncluded = false, bool rightIncluded = false);
		void setHorizontalLine(size_t y, size_t left, size_t right, const color_t& color, bool rightIncluded = false);
		void setVerticalLine(size_t x, size_t top, size_t bottom, const color_t& color, bool bottomIncluded = false);
		void setAllPixels(const color_t& color);
		void mergePixel(size_t x, size_t y, const color_t& color);
		void merge(const PPMImage& other);
		PPMImage& mean(const PPMImage& other);
		/**
		 * @brief save the image on the file system
		 * 
		 * @param filename path of the image
		 */
		void savePPM(const boost::filesystem::path& filename) const;
		size_t totalPixels() const {
			return this->width * this->height;
		}
		size_t getWidth() const;
		size_t getHeight() const;
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