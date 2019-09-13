/**
 * @brief defines ASCII color codes
 * 
 * 
 * @see https://stackoverflow.com/a/51850508/1887602
 */

#ifndef _CPP_UTILS_ANSI_COLORS_HEADER__
#define _CPP_UTILS_ANSI_COLORS_HEADER__

namespace cpp_utils {

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& reset(std::basic_ostream<CharT, Traits>& os) {
    return os << "\033[0m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fBlack(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[30m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fRed(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[31m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fGreen(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[32m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fYellow(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[33m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fBlue(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[34m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fMagenta(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[35m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fCyan(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[36m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fWhite(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[37m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fbBlack(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[1m\033[30m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fbRed(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[1m\033[31m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fbGreen(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[1m\033[32m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fbYellow(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[1m\033[33m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fbBlue(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[1m\033[34m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fbMagenta(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[1m\033[35m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fbCyan(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[1m\033[36m";
}

template <typename CharT, typename Traits>
constexpr std::basic_ostream<CharT, Traits>& fbWhite(std::basic_ostream<CharT, Traits> &os) {
    return os << "\033[1m\033[37m";
}

}


#endif