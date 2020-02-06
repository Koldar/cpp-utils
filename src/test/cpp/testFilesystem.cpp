#include "catch.hpp"
#include "filesystem.hpp"

SCENARIO("test filesystem") {

    REQUIRE(cpp_utils::filesystem::exists(boost::filesystem::path{"/tmp/"}));
    REQUIRE(!cpp_utils::filesystem::exists(boost::filesystem::path{"/tmp354658/"}));

    REQUIRE(cpp_utils::filesystem::absolute(boost::filesystem::path{"/tmp/./../home"}) == "/home");
    REQUIRE(cpp_utils::filesystem::normalize(boost::filesystem::path{"/tmp/./../home"}) == "/home");

    REQUIRE(cpp_utils::filesystem::join("/tmp", ".", "..", "home") == "/tmp/./../home");

    boost::filesystem::path path{"/tmp/./../home"};
    REQUIRE(cpp_utils::filesystem::absolute(path) == "/home");
}