#include "Color.hpp"

namespace cpp_utils {

    const color_t color_t::BLACK = color_t{wrapped_uchar{0}, wrapped_uchar{0}, wrapped_uchar{0}};
    const color_t color_t::WHITE = color_t{wrapped_uchar{255}, wrapped_uchar{255}, wrapped_uchar{255}};

    const color_t color_t::RED = color_t{wrapped_uchar{255}, wrapped_uchar{0}, wrapped_uchar{0}};
    const color_t color_t::GREEN = color_t{wrapped_uchar{0}, wrapped_uchar{255}, wrapped_uchar{0}};
    const color_t color_t::BLUE = color_t{wrapped_uchar{0}, wrapped_uchar{0}, wrapped_uchar{255}};

    const color_t color_t::CYAN = color_t{wrapped_uchar{0}, wrapped_uchar{255}, wrapped_uchar{255}};

    color_t::color_t() : red{0}, green{0}, blue{0} {

    } 

    color_t::color_t(const wrapped_uchar& r, const wrapped_uchar& g, const wrapped_uchar& b) : red{r}, green{g}, blue{b} {
        
    }

    color_t::color_t(const color_t& other) : red{other.red}, green{other.green}, blue{other.blue} {
        
    }

    color_t::~color_t() {
        
    }

    color_t& color_t::operator=(const color_t& other) {
        this->red = other.red;
        this->green = other.green;
        this->blue = other.blue;
        return *this;
    }

    wrapped_uchar color_t::getRed() const {
        return this->red;
    }

    wrapped_uchar color_t::getGreen() const {
        return this->green;
    }

    wrapped_uchar color_t::getBlue() const {
        return this->blue;
    }

    color_t color_t::merge(const color_t& other) const {
        color_t result{*this};
        return result.merge(other);
    }

    color_t& color_t::merge(const color_t& other) {
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

    std::ostream& operator << (std::ostream& out, const color_t& c) {
        out << "R=" << static_cast<int>(c.red.getUnderlyingValue()) << " G=" << static_cast<int>(c.green.getUnderlyingValue()) << " B=" << static_cast<int>(c.blue.getUnderlyingValue());
        return out;
    };

}