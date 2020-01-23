#ifndef _CPPUTILS_LOGNUMBERLISTENER_HEADER__
#define _CPPUTILS_LOGNUMBERLISTENER_HEADER__

#include "NumberListener.hpp"

#include "log.hpp"

namespace cpp_utils {

    /**
     * @brief ::NumberListener that simply logs out the change performed
     * 
     * @tparam NUMBER type of the number to log
     */
    template <typename NUMBER>
    class LogNumberListener: public NumberListener<NUMBER> {
    public:
        typedef LogNumberListener<NUMBER> This;
        typedef NumberListener<NUMBER> Super;
    private:
        /**
         * @brief log level to output. either 0,1,2,3,5,6,7,8
         * 
         */
        int logLevel;
        /**
         * @brief what the number whose changes we are outputting represents
         * 
         */
        const std::string& what;
    public:
        /**
         * @brief Construct a new Log Number Listener object
         * 
         * @param what what the number actually represents
         * @param logLevel log level to output. either 0,1,2,3,5,6,7,8
         */
        LogNumberListener(const std::string& what, int logLevel): Super{}, logLevel{logLevel}, what{what} {

        }
        virtual ~LogNumberListener() = default;
        LogNumberListener(const This& o) = default;
        LogNumberListener(This&& o) = default;
        This& operator=(const This& o) = default;
        This& operator=(This&& o) = default;
    public:
        virtual void cleanup() {

        }
        virtual void onNumberIncreased(const NUMBER& oldValue, const NUMBER& newValue) {
            switch (logLevel) {
                case 0: debug("number", what, "increased from ", oldValue, "to", newValue, "!"); break;
                case 1: finest("number", what, "increased from ", oldValue, "to", newValue, "!"); break;
                case 2: finer("number", what, "increased from ", oldValue, "to", newValue, "!"); break;
                case 3: fine("number", what, "increased from ", oldValue, "to", newValue, "!"); break;
                case 5: info("number", what, "increased from ", oldValue, "to", newValue, "!"); break;
                case 6: warning("number", what, "increased from ", oldValue, "to", newValue, "!"); break;
                case 7: log_error("number", what, "increased from ", oldValue, "to", newValue, "!"); break;
                case 8: critical("number", what, "increased from ", oldValue, "to", newValue, "!"); break;
                default:
                    throw cpp_utils::exceptions::InvalidArgumentException("invalid logLevel", logLevel);
            }
        }

        virtual void onNumberDecreased(const NUMBER& oldValue, const NUMBER& newValue) {
            switch (logLevel) {
                case 0: debug("number", what, "decreased from ", oldValue, "to", newValue, "!"); break;
                case 1: finest("number", what, "decreased from ", oldValue, "to", newValue, "!"); break;
                case 2: finer("number", what, "decreased from ", oldValue, "to", newValue, "!"); break;
                case 3: fine("number", what, "decreased from ", oldValue, "to", newValue, "!"); break;
                case 5: info("number", what, "decreased from ", oldValue, "to", newValue, "!"); break;
                case 6: warning("number", what, "decreased from ", oldValue, "to", newValue, "!"); break;
                case 7: log_error("number", what, "decreased from ", oldValue, "to", newValue, "!"); break;
                case 8: critical("number", what, "decreased from ", oldValue, "to", newValue, "!"); break;
                default:
                    throw cpp_utils::exceptions::InvalidArgumentException("invalid logLevel", logLevel);
            }
        }
    };

}

#endif