#include "catch.hpp"
#include "filesystem.hpp"

SCENARIO("test filesystem") {

    REQUIRE(cpp_utils::exists(boost::filesystem::path{"/tmp/"}));
    REQUIRE(!cpp_utils::exists(boost::filesystem::path{"/tmp354658/"}));

    REQUIRE(cpp_utils::absolute(boost::filesystem::path{"/tmp/./../home"}) == "/home");
    REQUIRE(cpp_utils::normalize(boost::filesystem::path{"/tmp/./../home"}) == "/home");

    REQUIRE(cpp_utils::join("/tmp", ".", "..", "home") == "/tmp/./../home");

    boost::filesystem::path path{"/tmp/./../home"};
    REQUIRE(cpp_utils::absolute(path) == "/home");
}