#include "IImageable.hpp"
#include "log.hpp"
#include "ppmImage.hpp"

namespace cpp_utils {

    std::ostream& operator <<(std::ostream& ss, const ImageExtension& ext) {
        switch (ext){
            case ImageExtension::SVG: { ss << "svg"; break; }
            case ImageExtension::PNG: { ss << "png"; break; }
            case ImageExtension::JPEG: { ss << "jpeg"; break; }
            case ImageExtension::BMP: { ss << "bmp"; break; }
            case ImageExtension::PPM: { ss << "ppm"; break; }
            case ImageExtension::FIG: { ss << "fig"; break; }
            default:
                throw cpp_utils::exceptions::makeInvalidScenarioException(ext);
        }
        return ss;
    }

void IImageable::_saveImage(ImageExtension extension, const char* filename) const {
    PPMImage* ppm = this->getPPM();
    const char* ext;
    switch (extension) {
        case ImageExtension::SVG: ext="svg"; break;
        case ImageExtension::PNG: ext="png"; break;
        case ImageExtension::JPEG: ext="jpeg"; break;
        case ImageExtension::BMP: ext="bmp"; break;
        case ImageExtension::PPM: ppm->savePPM(filename); return;
        case ImageExtension::FIG: ext="fig"; break;
        default:
            throw exceptions::InvalidScenarioException<ImageExtension>{extension};
    }

    ppm->savePPM(filename);
    callExternalProgram("convert %s.ppm %s.%s", filename, filename, ext);
    callExternalProgram("rm -fv %s.ppm", filename);
    if (ppm != this) {
        //happens when this is already a ppm (e.g., PPMImage)
        delete ppm;
    }
}

void IImageable::saveSVG(const std::string& filename, ...) const {
    va_list ap;
    va_start(ap, filename);
    this->vsaveImage(ImageExtension::SVG, filename.c_str(), ap);
    va_end(ap);
}
void IImageable::saveSVG(const char* filename, ...) const {
    va_list ap;
    va_start(ap, filename);
    this->vsaveImage(ImageExtension::SVG, filename, ap);
    va_end(ap);
}

void IImageable::savePNG(const std::string& filename, ...) const {
    va_list ap;
    va_start(ap, filename);
    this->vsaveImage(ImageExtension::PNG, filename.c_str(), ap);
    va_end(ap);
}
void IImageable::savePNG(const char* filename, ...) const {
    va_list ap;
    va_start(ap, filename);
    this->vsaveImage(ImageExtension::PNG, filename, ap);
    va_end(ap);
}
void IImageable::saveJPEG(const std::string& filename, ...) const {
    va_list ap;
    va_start(ap, filename);
    this->vsaveImage(ImageExtension::JPEG, filename.c_str(), ap);
    va_end(ap);
}
void IImageable::saveJPEG(const char* filename, ...) const {
    va_list ap;
    va_start(ap, filename);
    this->vsaveImage(ImageExtension::JPEG, filename, ap);
    va_end(ap);
}
void IImageable::saveBMP(const std::string& filename, ...) const {
    va_list ap;
    va_start(ap, filename);
    this->vsaveImage(ImageExtension::BMP, filename.c_str(), ap);
    va_end(ap);
}
void IImageable::saveBMP(const char* filename, ...) const {
    va_list ap;
    va_start(ap, filename);
    this->vsaveImage(ImageExtension::BMP, filename, ap);
    va_end(ap);
}
void IImageable::saveFIG(const std::string& filename, ...) const {
    va_list ap;
    va_start(ap, filename);
    this->vsaveImage(ImageExtension::FIG, filename.c_str(), ap);
    va_end(ap);
}
void IImageable::saveFIG(const char* filename, ...) const {
    va_list ap;
    va_start(ap, filename);
    this->vsaveImage(ImageExtension::FIG, filename, ap);
    va_end(ap);
}

void IImageable::vsaveImage(ImageExtension extension, const char* filename, va_list ap) const {
    char buffer[1000];
    vsnprintf(buffer, 1000, filename, ap);
    info("vsaveImage buffer is\"", buffer, "\"");

    this->_saveImage(extension, buffer);
}
void IImageable::saveImage(ImageExtension extension, const char* filename, ...) const {
    char buffer[1000];
    va_list ap;
    va_start(ap, filename);
    vsnprintf(buffer, 1000, filename, ap);
    va_end(ap);
    info("saveImage buffer is\"", buffer, "\"");

    this->_saveImage(extension, buffer);
}

}