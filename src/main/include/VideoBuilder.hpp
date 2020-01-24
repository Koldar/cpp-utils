#ifndef _CPPUTILS_VIDEOBUILDER_HEADER__
#define _CPPUTILS_VIDEOBUILDER_HEADER__

#include "functional.hpp"
#include "vectorplus.hpp"
#include "Interval.hpp"

namespace cpp_utils {

    /**
     * @brief A small utility which uses ffmpeg program to build a video from a set of images
     * 
     */
    class VideoBuilder {
    public:
        using This = VideoBuilder;
    private:
        vectorplus<boost::filesystem::path> imageNames;
        /**
         * @brief the framerate the output video will have
         * 
         * @see https://ffmpeg.org/ffmpeg.html#Video-Options
         */
        double outputFramerate;
        double duration;
    public:
        VideoBuilder();
        virtual ~VideoBuilder();
        VideoBuilder(const This& o) = default;
        VideoBuilder(This&& o) = default;
        This& operator = (const This& o) = default;
        This& operator = (This&& o) = default;
    public:
        /**
         * @brief how much time a single image will be displayed in the video
         * 
         * @param d the new duration
         * @return VideoBuilder& this
         */
        VideoBuilder& setDuration(double d);

        /**
         * @brief 
         * 
         * @param fr the framerate the output video will have
         * @return VideoBuilder& this
         * @see https://ffmpeg.org/ffmpeg.html#Video-Options
         */
        VideoBuilder& setOutputFramerate(double fr);

        /**
         * @brief append the given image as a frame at the end of the video
         * 
         * @note
         * do nothing if the image does not exists
         * 
         * @param filename name of image to add
         * @return VideoBuilder& this
         */
        VideoBuilder& addImage(const boost::filesystem::path& filename);

        /**
         * @brief append a range of images as frames at the end of the video
         * 
         * @note
         * do nothing if some images do not exist
         * 
         * @param range range where to fetch numbers from. It represents the order of the frames as well
         * @param mapper a function that maps the values from the range into a the images filenames.
         * @return VideoBuilder& this
         */
        VideoBuilder& addImagesThat(const IntInterval& range, const function_t<int, boost::filesystem::path>& mapper);

        /**
         * @brief like ::addImagesThat but stops after an image is not found
         * 
         * 
         * @param range range where to fetch numbers from. It represents the order of the frames as well
         * @param mapper a function that maps the values from the range into a the images filenames.
         * @return VideoBuilder& this
         */
        VideoBuilder& addImagesUntilNoFound(const IntInterval& range, const function_t<int, boost::filesystem::path>& mapper);

        /**
         * @brief concretely build an video
         * 
         * This operation will directly call ffmpeg. COnsider a CPU intensive operation that can take quite a while!
         * 
         * @param path the output mp4 file (without extension)
         */
        VideoBuilder& buildVideo(const boost::filesystem::path& path) const;
    private:
        /**
         * @brief generate a Concat Demuxer
         * 
         * It's a file like these:
         * @code
         *  file '/path/to/dog.png'
         *  duration 5
         *  file '/path/to/cat.png'
         *  duration 1
         *  file '/path/to/rat.png'
         *  duration 3
         *  file '/path/to/tapeworm.png'
         *  duration 2
         *  file '/path/to/tapeworm.png'
         * @endcode
         * 
         * @see https://trac.ffmpeg.org/wiki/Slideshow
         * @see https://ffmpeg.org/ffmpeg-formats.html#concat
         * 
         * @param filename basename of the demuxer file
         * @param duration time an image needs to stay display in the video
         * @return the path of the generated concat demuxer
         */
        boost::filesystem::path generateConcatDemuxer(const std::string& basename, double duration) const;
    };

}

#endif