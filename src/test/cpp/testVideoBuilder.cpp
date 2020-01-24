#include "catch.hpp"

#include "VideoBuilder.hpp"
#include "filesystem.hpp"

using namespace cpp_utils;

SCENARIO("test video builder", "[VideoBuilder]") {

    VideoBuilder builder{};

    function_t<int, boost::filesystem::path> mapper = [&](int id) { return scout("./iteration_", id, ".bmp.bmp");};
    builder
        .addImagesThat(IntInterval{0, 6, true, false}, mapper)
        .buildVideo("./output.mp4")
        .setDuration(0.20)
        .buildVideo("./output-fast.mp4")
        ;
    

    REQUIRE(cpp_utils::exists("./output.mp4"));
    REQUIRE(cpp_utils::exists("./output-fast.mp4"));
}