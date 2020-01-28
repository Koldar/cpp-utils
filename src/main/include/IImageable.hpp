#ifndef _CPP_UTILS_IMAGEABLE_HEADER__
#define _CPP_UTILS_IMAGEABLE_HEADER__

#include <string>
#include <cstdarg>
#include <iostream>

#include <boost/filesystem.hpp>

#include "system.hpp"
#include "exceptions.hpp"


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
    public:
        /**
         * @brief generate a PPM image that represents the concept
         * 
         * The image is modifiable because maybe you want to edit it before saving it into a fil format (e.g., bmp)
         * 
         * @return PPMImage* an image. This pointer needs to be manually freed (or you can wrap it in a smart pointer)
         */
        virtual PPMImage* getPPM() const = 0;
    protected:
        void _saveImage(ImageExtension extension, const char* filename) const;
    public:
        void saveSVG(const std::string& filename, ...) const;
        void saveSVG(const char* filename, ...) const;
        /**
         * @brief save a SVG file
         * 
         * @post
         * @li svg present in the file system
         * 
         * @param p the path where to save it. If the path has extension SVG, we will not duplicate such extension
         */
        void saveSVG(const boost::filesystem::path& p) const;

        void savePNG(const std::string& filename, ...) const;
        void savePNG(const char* filename, ...) const;
        /**
         * @brief save a SVG file
         * 
         * @post
         * @li svg present in the file system
         * 
         * @param p the path where to save it. If the path has extension SVG, we will not duplicate such extension
         */
        void savePNG(const boost::filesystem::path& p) const;

        void saveJPEG(const std::string& filename, ...) const;
        void saveJPEG(const char* filename, ...) const;
        /**
         * @brief save a SVG file
         * 
         * @post
         * @li svg present in the file system
         * 
         * @param p the path where to save it. If the path has extension SVG, we will not duplicate such extension
         */
        void saveJPEG(const boost::filesystem::path& p) const;

        void saveBMP(const std::string& filename, ...) const;
        void saveBMP(const char* filename, ...) const;
        /**
         * @brief save a SVG file
         * 
         * @post
         * @li svg present in the file system
         * 
         * @param p the path where to save it. If the path has extension SVG, we will not duplicate such extension
         */
        void saveBMP(const boost::filesystem::path& p) const;

        void saveFIG(const std::string& filename, ...) const;
        void saveFIG(const char* filename, ...) const;
        /**
         * @brief save a SVG file
         * 
         * @post
         * @li svg present in the file system
         * 
         * @param p the path where to save it. If the path has extension SVG, we will not duplicate such extension
         */
        void saveFIG(const boost::filesystem::path& p) const;
    public:
        void vsaveImage(ImageExtension extension, const char* filename, va_list ap) const;
        void saveImage(ImageExtension extension, const char* filename, ...) const;
    };

}

#endif