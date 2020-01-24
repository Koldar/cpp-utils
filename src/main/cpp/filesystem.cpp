#include "filesystem.hpp"

#include <cstdio>

#include "strings.hpp"

namespace cpp_utils {

    void rename(const boost::filesystem::path& p, const std::string& newName) {
        auto newPath = p.parent_path();
        newPath /= p.filename();
        boost::filesystem::rename(p, newPath);
    }

    bool remove(const boost::filesystem::path& p) {
        if (cpp_utils::exists(p)) {
            std::remove(p.native().c_str());
            return true;
        }
        return false;
    }

    bool exists(const boost::filesystem::path& p) {
        return boost::filesystem::exists(p);
    }

    bool exist(const boost::filesystem::path& p) {
        return cpp_utils::exists(p);
    }

    bool isEmpty(const boost::filesystem::path& p) {
        return p.empty();
    }

    boost::filesystem::path withoutExtension(const boost::filesystem::path& p) {
        boost::filesystem::path result{p.parent_path()};
        result /= p.stem();
        return result;
    }

    boost::filesystem::path changeExtension(const boost::filesystem::path& p, const char* newExtension) {
        return withoutExtension(p).concat(scout(".", newExtension));
    }

    boost::filesystem::path basename(const boost::filesystem::path& p) {
        return p.stem();
    }

    boost::filesystem::path extension(const boost::filesystem::path& p) {
        return p.extension();
    }

    boost::filesystem::path absolute(const boost::filesystem::path& p, const boost::filesystem::path& relativeTo) {
        return boost::filesystem::weakly_canonical(p);
    }

    boost::filesystem::path normalize(const boost::filesystem::path& p, const boost::filesystem::path& relativeTo) {
        return boost::filesystem::weakly_canonical(p);
    }

    boost::filesystem::path _join(boost::filesystem::path& building) {
        return building;
    }

}