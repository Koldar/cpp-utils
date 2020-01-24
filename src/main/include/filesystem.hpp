#ifndef _CPPUTILS_FILESYSTEM_HEADER__
#define _CPPUTILS_FILESYSTEM_HEADER__

#include <boost/filesystem.hpp>

namespace cpp_utils {

    /**
     * @brief rename a file or a directory
     * 
     * @param p the path of either the file or directory to rename
     * @param newName the new name of the file/directory
     */
    void rename(const boost::filesystem::path& p, const std::string& newName);

    /**
     * @brief remove a file
     * 
     * @note
     * do nothing if the file does not exists
     * 
     * @param p the path involved
     * @return true 
     * @return false 
     */
    bool remove(const boost::filesystem::path& p);

    /**
     * @brief a convenience method to check if a path exists in boost
     * 
     * @note
     * I've created it since I never remember how to do it in boost
     * 
     * @param p the path to check
     * @return true exists in the filesystem
     * @return false otherwise
     */
    bool exists(const boost::filesystem::path& p);

    /**
     * @brief a convenience method to check if a path exists in boost
     * 
     * @note
     * I've created it since I never remember how to do it in boost
     * 
     * @param p the path to check
     * @return true exists in the filesystem
     * @return false otherwise
     */
    bool exist(const boost::filesystem::path& p);

    /**
     * @brief check if a path si empty
     * 
     * @param p the path involved
     * @return true 
     * @return false 
     */
    bool isEmpty(const boost::filesystem::path& p);

    /**
     * @brief get the path without the extension
     * 
     * @pre
     *  @li p represents a file, not a directory
     * 
     * @param p the path involved
     * @return boost::filesystem::path 
     */
    boost::filesystem::path withoutExtension(const boost::filesystem::path& p);

    /**
     * @brief replace the extension of the file with a new one
     * 
     * @pre
     *  @li @c p represents a file
     * 
     * @param p the path fo the file
     * @param newExtension new extension of the file
     * @return boost::filesystem::path 
     */
    boost::filesystem::path changeExtension(const boost::filesystem::path& p, const char* newExtension);

    /**
     * @brief get the basename of a path
     * 
     * @code
     *  /tmp/hello/myworld.dat //myworld
     * @endcode
     * 
     * @param p the path involved
     * @return boost::filesystem::path 
     */
    boost::filesystem::path basename(const boost::filesystem::path& p);

    /**
     * @brief get the extension of a path
     * 
     * @code
     *  /tmp/hello/myworld.dat //dat
     * @endcode
     * 
     * @param p the path involved
     * @return boost::filesystem::path 
     */
    boost::filesystem::path extension(const boost::filesystem::path& p);

    /**
     * @brief normalize the path and make it absolute
     * 
     * @code
     *  tmp/hello/.././myFolder /tmp/myFolder
     * @endcode
     * 
     * @param p path to normalize
     * @return boost::filesystem::path 
     */
    boost::filesystem::path absolute(const boost::filesystem::path& p, const boost::filesystem::path& relativeTo = boost::filesystem::current_path());

    /**
     * @brief alias of ::absolute
     * 
     * @param p path to normalize
     * @return boost::filesystem::path 
     */
    boost::filesystem::path normalize(const boost::filesystem::path& p, const boost::filesystem::path& relativeTo = boost::filesystem::current_path());

    boost::filesystem::path _join(boost::filesystem::path& building);

    template <typename FIRST, typename ...OTHER>
    boost::filesystem::path _join(boost::filesystem::path& building, const FIRST& p, const OTHER&... others) {
        building /= p;
        return _join(building, others...);
    }

    template <typename ...OTHER>
    boost::filesystem::path join(const OTHER&... others) {
        boost::filesystem::path result{};
        return _join(result, others...);
    }

}

#endif