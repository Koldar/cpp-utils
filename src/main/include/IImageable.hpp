#ifndef _CPP_UTILS_IMAGEABLE_HEADER__
#define _CPP_UTILS_IMAGEABLE_HEADER__

#include <string>
#include <cstdarg>
#include "system.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace cpp_utils {

    /**
     * @brief an image extension which can be created with IImageable interface
     * 
     */
    enum class ImageExtension {
        SVG,
        PNG,
        JPEG,
        BMP,
        PPM,
        FIG
    };

    std::ostream& operator <<(std::ostream& ss, const ImageExtension& ext);

    class PPMImage;
    /**
     * @brief represents a concept which can be converted into an image
     * 
     */
    class IImageable {
    protected:
        virtual const PPMImage* getPPM() const = 0;
    protected:
        void _saveImage(ImageExtension extension, const char* filename) const;
    public:
        void saveSVG(const std::string& filename, ...) const;
        void saveSVG(const char* filename, ...) const;

        void savePNG(const std::string& filename, ...) const;
        void savePNG(const char* filename, ...) const;

        void saveJPEG(const std::string& filename, ...) const;
        void saveJPEG(const char* filename, ...) const;

        void saveBMP(const std::string& filename, ...) const;
        void saveBMP(const char* filename, ...) const;

        void saveFIG(const std::string& filename, ...) const;
        void saveFIG(const char* filename, ...) const;
    public:
        void vsaveImage(ImageExtension extension, const char* filename, va_list ap) const;
        void saveImage(ImageExtension extension, const char* filename, ...) const;
    };

}

#endif