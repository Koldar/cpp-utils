#include "ppmImage.hpp"
#include "exceptions.hpp"
#include "system.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <cstdlib>
#include <cstring>
#include "log.hpp"


namespace cpp_utils {

PPMImage::PPMImage(const PPMImage& other) : width{other.width}, height{other.height}, image{nullptr} {
	this->image = new color_t[this->width * this->height];
	memcpy(this->image, other.image, sizeof(color_t) * this->width * this->height);
}

PPMImage::PPMImage(PPMImage&& other): width{other.width}, height{other.height}, image{other.image} {
	
}

PPMImage::PPMImage(const char* filename) {
	std::ifstream f;
	f.open(filename, std::ios::in);
	
	std::string p3;
	int width;
	int height;
	int maxVal;
	f >> p3 >> width >> height >> maxVal;

	if (p3 != std::string{"P3"}) {
		throw exceptions::ImpossibleException{"expected P3 but header was %s", p3};
	}
	if (maxVal != 255) {
		throw exceptions::ImpossibleException{"expected max depth of 255, but got %d!", maxVal};
	}
	this->width = width;
	this->height = height;
	this->image = new color_t[this->width * this->height];
	unsigned char red, green, blue;
	debug("width is", this->width, " height is", this->height);
	for (size_t y=0; y<this->height; ++y) {
		for (size_t x=0; x<this->width; ++x) {
			f >> red >> green >> blue;
			//debug("x=", x, "y=", y);
			this->setPixel(x, y, color_t{red, green, blue});
		}
	}

	f.close();
}

PPMImage& PPMImage::operator =(const PPMImage& other) {
	this->deleteImage();
	this->width = other.width;
	this->height = other.height;
	this->image = new color_t[this->width * this->height];
	memcpy(this->image, other.image, sizeof(color_t) * this->width * this->height);
	
	return *this;
}

PPMImage& PPMImage::operator =(PPMImage&& other) {
	PPMImage::checkDimensions(*this, other);
	this->image = other.image;
	return *this;
}

PPMImage& PPMImage::operator +=(const PPMImage& other) {
	PPMImage::checkDimensions(*this, other);
	for (auto y=0; y<this->height; ++y) {
		for (auto x=0; x<this->width; ++x) {
			this->mergePixel(x, y, other.getPixel(x, y));
		}
	}
	return *this;
}

PPMImage& PPMImage::operator +=(const color_t& other) {
	for (auto y=0; y<this->height; ++y) {
		for (auto x=0; x<this->width; ++x) {
			this->mergePixel(x, y, other);
		}
	}
	return *this;
}

bool PPMImage::isValid() const {
	if (this->width == 0) {
		log_error("width is 0");
		return false;
	}
	if (this->height == 0) {
		log_error("height is 0");
		return false;
	}
	color_t c1, c2;
	bool c1Found = false;
	bool c2Found = false;
	for (auto y=0; y<this->height; ++y) {
		for (auto x=0; x<this->width; ++x) {
			if (!c1Found) {
				c1 = this->getPixel(x, y);
				c1Found = true;
			} else if (!c2Found && (this->getPixel(x, y) != c1)) {
				return true;
			} 
		}
	}

	log_error("there is only one color and it is", c1);
	return false;
}

color_t PPMImage::getPixel(size_t x, size_t y) const {
	return this->image[y*this->width + x];
}

void PPMImage::setPixel(size_t x, size_t y, const color_t& color) {
	//debug("trying to set y=", y, " x=", x, "total is y=", this->height, "x=", this->width);
	this->image[y*this->width + x] = color;
}

void PPMImage::setRectanglePixel(size_t top, size_t left, size_t bottom, size_t right, const color_t& color, bool bottomIncluded, bool rightIncluded) {
	bottom += bottomIncluded ? 1 : 0;
	right += rightIncluded ? 1 : 0;
	for (auto y=top; y<bottom; ++y) {
		for (auto x=left; x<right; ++x) {
			this->setPixel(x, y, color);
		}
	}
}

void PPMImage::setHorizontalLine(size_t y, size_t left, size_t right, const color_t& color, bool rightIncluded) {
	this->setRectanglePixel(y, left, y, right, color, true, rightIncluded);
}

void PPMImage::setVerticalLine(size_t x, size_t top, size_t bottom, const color_t& color, bool bottomIncluded) {
	this->setRectanglePixel(top, x, bottom, x, color, bottomIncluded, true);
}

void PPMImage::setAllPixels(const color_t& color) {
	for (auto y=0; y<this->height; ++y) {
		for (auto x=0; x<this->width; ++x) {
			this->setPixel(x, y, color);
		}
	}
}

void PPMImage::mergePixel(size_t x, size_t y, const color_t& color) {
	this->setPixel(x, y, color.merge(this->getPixel(x, y)));
}

void PPMImage::merge(const PPMImage& other) {
	PPMImage::checkDimensions(*this, other);
	for (auto y=0; y<this->height; ++y) {
		for (auto x=0; x<this->width; ++x) {
			this->mergePixel(x, y, other.getPixel(x, y));
		}
	}
}

PPMImage& PPMImage::mean(const PPMImage& other) {
	this->merge(other);
	return *this;
}

void PPMImage::generatePPM(const std::string& filename) const {
	std::ofstream ofs;
	std::stringstream ss;
	ss << filename << ".ppm";
	debug("generating PPM image", ss.str());
	ofs.open(ss.str(), std::ofstream::out | std::ofstream::trunc);
	//header
	ofs << "P3\n";
	//width & height
	ofs << this->width << " " << this->height << "\n";
	// Maximum value for each color
	ofs << 255 << "\n";
	//LIST OF RGB TRIPLETS: from top left to bottom right, going by image row
	for (auto y=0; y<this->height; ++y) {
		for (auto x=0; x<this->width; ++x) {
			//critical("write y=", y, "/", this->height, "x=", x, "/", this->width, ": ", this->getPixel(x,y), (y*this->width + x), "/", this->totalPixels(), "LINE: ", static_cast<int>(this->getPixel(x,y).getRed()), " ", static_cast<int>(this->getPixel(x,y).getGreen()), " ", static_cast<int>(this->getPixel(x,y).getBlue()));
			ofs << static_cast<int>(this->getPixel(x,y).getRed()) << " " << static_cast<int>(this->getPixel(x,y).getGreen()) << " " << static_cast<int>(this->getPixel(x,y).getBlue());
			if ((x+1)<this->width)
			{
				ofs << " ";
			}
		}
		ofs << " ";
	}
	ofs.close();
}

PPMImage* PPMImage::getPPM() const {
	return const_cast<PPMImage*>(this);
}

void PPMImage::savePPM(const boost::filesystem::path& filename) const {
	//create ppm
	this->generatePPM(filename.native());
}

size_t PPMImage::getWidth() const {
	return this->width;
}

size_t PPMImage::getHeight() const {
	return this->height;
}

void PPMImage::checkDimensions(const PPMImage& a, const PPMImage& b) {
	if (a.width != b.width) {
		throw exceptions::InvalidPairScenarioException{a.width, b.width};
	}
	if (a.height != b.height) {
		throw  exceptions::InvalidPairScenarioException{a.width, b.width};
	}
}

void PPMImage::deleteImage() {
	if (this->image != nullptr) {
		delete[] this->image;
		this->image = nullptr;
	}
}

PPMImage operator +(const PPMImage& a, const PPMImage& b) {
	PPMImage result{a};
	result += b;
	return result;
}
PPMImage operator +(const PPMImage& a, const color_t& b) {
	PPMImage result{a};
	result += b;
	return result;
}

}