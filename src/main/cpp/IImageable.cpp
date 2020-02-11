#include "IImageable.hpp"
#include "log.hpp"
#include "ppmImage.hpp"
#include "filesystem.hpp"

#include <boost/algorithm/string.hpp>

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
                throw cpp_utils::exceptions::InvalidScenarioException{"image extension", ext};
        }
        return ss;
    }

void IImageable::_saveImage(ImageExtension extension, const boost::filesystem::path& filename) const {
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
            throw exceptions::InvalidScenarioException{"image extension", extension};
    }

    ppm->savePPM(filename);
    callConvert("convert \"", filename.native(), ".ppm\" \"", filename.native(), ".", ext, "\"");
    callRm("rm -f \"", filename.native(), ".ppm\"");
    if (ppm != this) {
        //happens when this is already a ppm (e.g., PPMImage)
        delete ppm;
    }
}

}