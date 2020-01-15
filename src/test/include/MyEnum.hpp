#ifndef _CPPUTILS_MYENUM_HEADER__
#define _CPPUTILS_MYENUM_HEADER__

#include "AbstractEnum.hpp"

namespace cpp_utils {

    struct MyEnum: public AbstractEnum {
    private:
        MyEnum(const std::string& name): AbstractEnum{name} {

        }
    public:
        static const MyEnum OK;
        static const MyEnum KO;
        static const MyEnum BOH;
    };

}

#endif