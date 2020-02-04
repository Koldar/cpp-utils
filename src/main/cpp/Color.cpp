#include "Color.hpp"

#include "log.hpp"

namespace cpp_utils {

    const color_t color_t::LIGHT_RED = RED.lighter(0.2);
    const color_t color_t::RED = color_t{wrapped_uchar{255}, wrapped_uchar{0}, wrapped_uchar{0}};
    const color_t color_t::DARK_RED = RED.darker(0.2);

    const color_t color_t::LIGHT_GREEN = GREEN.lighter(0.2);
    const color_t color_t::GREEN = color_t{wrapped_uchar{0}, wrapped_uchar{255}, wrapped_uchar{0}};
    const color_t color_t::DARK_GREEN = GREEN.darker(0.2);

    const color_t color_t::LIGHT_BLUE = BLUE.lighter(0.2);
    const color_t color_t::BLUE = color_t{wrapped_uchar{0}, wrapped_uchar{0}, wrapped_uchar{255}};
    const color_t color_t::DARK_BLUE = BLUE.darker(0.2);

    const color_t color_t::LIGHT_CYAN = CYAN.lighter(0.2);
    const color_t color_t::CYAN = color_t{wrapped_uchar{0}, wrapped_uchar{255}, wrapped_uchar{255}};
    const color_t color_t::DARK_CYAN = CYAN.darker(0.2);

    const color_t color_t::LIGHT_VIOLET = VIOLET.lighter(0.2);
    const color_t color_t::VIOLET = color_t{wrapped_uchar{255}, wrapped_uchar{0}, wrapped_uchar{255}};
    const color_t color_t::DARK_VIOLET = VIOLET.darker(0.2);

    const color_t color_t::LIGHT_SKY = SKY.lighter(0.2);
    const color_t color_t::SKY = color_t{wrapped_uchar{0}, wrapped_uchar{128}, wrapped_uchar{255}};
    const color_t color_t::DARK_SKY = SKY.darker(0.2);

    const color_t color_t::LIGHT_GRASS = GRASS.lighter(0.2);
    const color_t color_t::GRASS = color_t{wrapped_uchar{0}, wrapped_uchar{255}, wrapped_uchar{128}};
    const color_t color_t::DARK_GRASS = GRASS.darker(0.2);
    
    const color_t color_t::LIGHT_PURPLE = PURPLE.lighter(0.2);
    const color_t color_t::PURPLE = color_t{wrapped_uchar{128}, wrapped_uchar{0}, wrapped_uchar{255}};
    const color_t color_t::DARK_PURPLE = PURPLE.darker(0.2);

    const color_t color_t::LIGHT_PINKISH = PINKISH.lighter(0.2);
    const color_t color_t::PINKISH = color_t{wrapped_uchar{255}, wrapped_uchar{0}, wrapped_uchar{128}};
    const color_t color_t::DARK_PINKISH = PINKISH.darker(0.2);
    
    const color_t color_t::LIGHT_YELLOW = YELLOW.lighter(0.2);
    const color_t color_t::YELLOW = color_t{wrapped_uchar{255}, wrapped_uchar{255}, wrapped_uchar{0}};
    const color_t color_t::DARK_YELLOW = YELLOW.darker(0.2);
    
    const color_t color_t::LIGHT_ORANGE = ORANGE.lighter(0.2);
    const color_t color_t::ORANGE = color_t{wrapped_uchar{255}, wrapped_uchar{128}, wrapped_uchar{0}};
    const color_t color_t::DARK_ORANGE = ORANGE.darker(0.2);
    
    const color_t color_t::LIGHT_GREY = GREY.lighter(0.2);
    const color_t color_t::GREY = color_t{wrapped_uchar{128}, wrapped_uchar{128}, wrapped_uchar{128}};
    const color_t color_t::DARK_GREY = GREY.darker(0.2);

    const color_t color_t::BLACK = color_t{wrapped_uchar{0}, wrapped_uchar{0}, wrapped_uchar{0}};
    const color_t color_t::WHITE = color_t{wrapped_uchar{255}, wrapped_uchar{255}, wrapped_uchar{255}};

    const color_t color_t::LIGHT_BROWN = BROWN.lighter(0.2);
    const color_t color_t::BROWN = color_t{wrapped_uchar{102}, wrapped_uchar{51}, wrapped_uchar{0}};
    const color_t color_t::DARK_BROWN = BROWN.darker(0.2);

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

    color_t color_t::lighter(double scale) const {
        color_t result{*this};
        return result.lighter(scale);
    }

    color_t& color_t::lighter(double scale) {
        color_t diff = color_t{wrapped_uchar{255} - this->red, wrapped_uchar{255} - this->green, wrapped_uchar{255} - this->blue};
        diff = diff.scale(scale);
        diff = color_t{diff.red + this->red, diff.green + this->green, diff.blue + this->blue};
        this->red = diff.red;
        this->green = diff.green;
        this->blue = diff.blue;
        return *this;
    }

    color_t color_t::darker(double scale) const {
        color_t result{*this};
        return result.darker(scale);
    }

    color_t& color_t::darker(double scale) {
        return this->scale(1. - scale);
    }

    color_t color_t::scale(double scale) const {
        color_t result{*this};
        return result.scale(scale);
    }
	
    color_t& color_t::scale(double scale) {
        this->red *= scale;
        this->green *= scale;
        this->blue *= scale;
        return *this;
    }

    color_t color_t::invert() const {
        color_t result{*this};

        return result.invert();
    }

    color_t& color_t::invert() {
        this->red = wrapped_uchar{255} - this->red;
        this->green = wrapped_uchar{255} - this->green;
        this->blue = wrapped_uchar{255} - this->blue;

        return *this;
    }

    color_t color_t::min(const color_t& other) const {
        color_t result{*this};
        return result.min(other);
    }

    color_t& color_t::min(const color_t& other) {
        this->red = std::min(this->red, other.red);
        this->green = std::min(this->green, other.green);
        this->blue = std::min(this->blue, other.blue);
        return *this;
    }

    color_t color_t::max(const color_t& other) const {
        color_t result{*this};
        return result.max(other);
    }

    color_t& color_t::max(const color_t& other) {
        this->red = std::max(this->red, other.red);
        this->green = std::max(this->green, other.green);
        this->blue = std::max(this->blue, other.blue);
        return *this;
    }

    color_t color_t::lerp(const color_t& other) const {
        color_t result{*this};
        return result.lerp(other);
    }

	color_t& color_t::lerp(const color_t& other) {
        this->red = (this->red + other.red)/2;
        this->green = (this->green + other.green)/2;
        this->blue = (this->blue + other.blue)/2;

        return *this;
    }

    color_t color_t::merge(const color_t& other) const {
        color_t result{*this};
        return result.merge(other);
    }

    color_t& color_t::merge(const color_t& other) {
        return this->lerp(other);
        //TODO remove
        // wrapped_uchar minRed;
        // wrapped_uchar maxRed;
        // wrapped_uchar minGreen;
        // wrapped_uchar maxGreen;
        // wrapped_uchar minBlue;
        // wrapped_uchar maxBlue;
        // if (this->red > other.red) {
        //     minRed = other.red;
        //     maxRed = this->red;
        // } else {
        //     minRed = this->red;
        //     maxRed = other.red;
        // }

        // if (this->green > other.green) {
        //     minGreen = other.green;
        //     maxGreen = this->green;
        // } else {
        //     minGreen = this->green;
        //     maxGreen = other.green;
        // }

        // if (this->blue > other.blue) {
        //     minBlue = other.blue;
        //     maxBlue = this->blue;
        // } else {
        //     minBlue = this->blue;
        //     maxBlue = other.blue;
        // }

        // this->red = minRed + (maxRed - minRed)/2;
        // this->green = minGreen + (maxGreen - minGreen)/2;
        // this->blue = minBlue + (maxBlue - minBlue)/2;

        // return *this;
    }

    std::ostream& operator << (std::ostream& out, const color_t& c) {
        out << "R=" << static_cast<int>(c.red.getUnderlyingValue()) << " G=" << static_cast<int>(c.green.getUnderlyingValue()) << " B=" << static_cast<int>(c.blue.getUnderlyingValue());
        return out;
    };

    bool operator ==(const color_t& a, const color_t& b) {
        return a.red == b.red && a.green == b.green && a.blue == b.blue;
    }

	bool operator !=(const color_t& a, const color_t& b) {
        return a.red != b.red || a.green != b.green || a.blue != b.blue;
    }

}