#ifndef _CPPUTILS_MYENUM_HEADER__
#define _CPPUTILS_MYENUM_HEADER__

#include "AbstractEnum.hpp"

namespace cpp_utils {

    

    struct MyEnum: public AbstractEnum<MyEnum> {
    private:
        static std::vector<const MyEnum*> VALUES;
    private:
        MyEnum(const std::string& name): AbstractEnum{name, VALUES} {

        }
    public:
        static const MyEnum& getFirst();
        static const std::vector<const MyEnum*>& getValues();
    public:
        static const MyEnum OK;
        static const MyEnum KO;
        static const MyEnum BOH;
    };
}

#endif