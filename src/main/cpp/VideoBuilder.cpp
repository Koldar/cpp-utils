#include "VideoBuilder.hpp"

#include <iostream>
#include <fstream>
#include "filesystem.hpp"
#include "system.hpp"

namespace cpp_utils {

    VideoBuilder::VideoBuilder(): imageNames{}, duration{1.0}, inputFramerate{25}, outputFramerate{25}, removeImages{false}, audioFile{} {

    }
    VideoBuilder::~VideoBuilder() {

    }

    VideoBuilder& VideoBuilder::setAudio(const boost::filesystem::path& audio) {
        this->audioFile = audio;
        return *this;
    }
    
    VideoBuilder& VideoBuilder::setDuration(double d) {
        this->duration = d;
        return *this;
    }

    VideoBuilder& VideoBuilder::setInputFramerate(double fr) {
        this->inputFramerate = fr;
        return *this;
    }

    VideoBuilder& VideoBuilder::setOutputFramerate(double fr) {
        this->outputFramerate = fr;
        return *this;
    }
   
    VideoBuilder& VideoBuilder::addImage(const boost::filesystem::path& filename) {
        if (boost::filesystem::exists(filename)) {
            this->imageNames.add(filename);
        }
        return *this;
    }
    
    VideoBuilder& VideoBuilder::addImagesThat(const IntInterval& range, const function_t<int, boost::filesystem::path>& mapper) {
        for (int i=range.getLB(); i<range.getUB(); ++i) {
            auto filename = mapper(i);
            this->addImage(filename);
        }
        return *this;
    }

    VideoBuilder& VideoBuilder::addImagesUntilNoFound(const IntInterval& range, const function_t<int, boost::filesystem::path>& mapper) {
        for (int i=range.getLB(); i<range.getUB(); ++i) {
            auto filename = mapper(i);
            if (!cpp_utils::filesystem::exists(filename)) {
                break;
            }
            this->addImage(filename);
        }
        return *this;
    }

    VideoBuilder& VideoBuilder::setRemoveImages(bool remove) {
        this->removeImages = remove;
        return *this;
    }
   
    VideoBuilder& VideoBuilder::buildVideo(const boost::filesystem::path& path) const {
        if (this->imageNames.isEmpty()) {
            critical("no images to build ", path, "! Ignoring");
            return const_cast<VideoBuilder&>(*this);
        }
        //see https://trac.ffmpeg.org/wiki/ChangingFrameRate
        critical("duration is", this->duration);
        critical("the video will be based on", this->imageNames.size(), "images!");
        auto demuxerPath = this->generateConcatDemuxer(cpp_utils::filesystem::basename(path).native(), this->duration);

        boost::filesystem::path tmpMp4{scout("/tmp/", cpp_utils::filesystem::basename(path).native(), ".mp4")};
        auto absolutePath = cpp_utils::filesystem::absolute(path);
        auto outputPath = cpp_utils::filesystem::changeExtension(absolutePath, "mp4");

        boost::filesystem::path tmp;
        if (!this->audioFile.empty()) {
            //if the audio is present, we need to call ffmpeg twice, meaning that we need a temproary file. Otherwise we can generate
            //the mp4 directly in the outputPath
            tmp = tmpMp4;
        } else {
            tmp = outputPath;
        }

        auto output = cpp_utils::callExternalProgramSafeAndFetchOutput("ffmpeg", "4.1.4", "ffmpeg -threads auto -y -safe 0 -r ", this->inputFramerate, " -f concat -i \"", demuxerPath.native(), "\" -c:v libx264 -vf fps=", this->outputFramerate, " -pix_fmt yuv420p -preset veryfast \"", tmp.native(), "\"");

        //adds the audio file
        if (!this->audioFile.empty()) {
            //we have saved the file in tmp. add the audio
            critical("adding audio");
            callFFMPEG("ffmpeg -y -i \"", tmp.native(), "\" -i \"", this->audioFile.native(), "\" -shortest -c:v copy -map 0:v:0 -map 1:a:0 -c:a aac -b:a 192k \"", outputPath.native(), "\"");

            //delete tmp file
            cpp_utils::filesystem::remove(tmpMp4);
        }

        //delete demuxer
        cpp_utils::filesystem::remove(demuxerPath);

        if (this->removeImages) {
            for (auto image : this->imageNames) {
                cpp_utils::filesystem::remove(image);
            }
        }

        return const_cast<VideoBuilder&>(*this);
    }
    
    boost::filesystem::path VideoBuilder::generateConcatDemuxer(const std::string& basename, double duration) const {
        std::ofstream concatDemuxer;
        //boost::filesystem::path demuxerPath{scout("/tmp/", basename, "concatDemuxer.txt")};
        boost::filesystem::path demuxerPath{scout(basename, ".concatDemuxer")};

        debug("trying to open", demuxerPath);
        concatDemuxer.open(demuxerPath.native(), std::ios::trunc);
        concatDemuxer << "ffconcat version 1.0" << "\n";
        concatDemuxer << "# total of " << this->imageNames.size() << "images" << "\n";
        for (auto p : this->imageNames) {
            concatDemuxer << "file " << cpp_utils::filesystem::absolute(p) << "\n";
            concatDemuxer << "duration " << duration << "\n";
        }
        //repeat the last file
        auto lastImageName = this->imageNames.getTail();
        concatDemuxer << "file " << cpp_utils::filesystem::absolute(lastImageName) << "\n";
    
        //close file
        concatDemuxer.close();
        return demuxerPath;
    }

}