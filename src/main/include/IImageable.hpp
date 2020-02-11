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
        void _saveImage(ImageExtension extension, const boost::filesystem::path& filename) const;
    public:
        /**
         * @brief save a SVG file
         * 
         * @post
         * @li svg present in the file system
         * 
         * @param others a sequence of elements that are concatenated together in order to form the path where to save the image
         */
        template< typename... OTHER>
        void saveSVG(const OTHER&... others) const {
            boost::filesystem::path p{scout(others...)};
            this->_saveImage(ImageExtension::SVG, p);
        }

        /**
         * @brief save a PNG file
         * 
         * @post
         * @li svg present in the file system
         * 
         * @param others a sequence of elements that are concatenated together in order to form the path where to save the image
         */
        template< typename... OTHER>
        void savePNG(const OTHER&... others) const {
            boost::filesystem::path p{scout(others...)};
            this->_saveImage(ImageExtension::PNG, p);
        }

        /**
         * @brief save a JPEG file
         * 
         * @post
         * @li svg present in the file system
         * 
         * @param others a sequence of elements that are concatenated together in order to form the path where to save the image
         */
        template< typename... OTHER>
        void saveJPEG(const OTHER&... others) const {
            boost::filesystem::path p{scout(others...)};
            this->_saveImage(ImageExtension::JPEG, p);
        }

        /**
         * @brief save a BMP file
         * 
         * @post
         * @li svg present in the file system
         * 
         * @param others a sequence of elements that are concatenated together in order to form the path where to save the image
         */
        template< typename... OTHER>
        void saveBMP(const OTHER&... others) const {
            boost::filesystem::path p{scout(others...)};
            this->_saveImage(ImageExtension::BMP, p);
        }

        /**
         * @brief save a FIG file
         * 
         * @post
         * @li svg present in the file system
         * 
         * @param others a sequence of elements that are concatenated together in order to form the path where to save the image
         */
        template< typename... OTHER>
        void saveFIG(const OTHER&... others) const {
            boost::filesystem::path p{scout(others...)};
            this->_saveImage(ImageExtension::FIG, p);
        }
    };

}

#endif