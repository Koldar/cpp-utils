#include "MyEnum.hpp"

namespace cpp_utils {

    std::vector<const MyEnum*> MyEnum::VALUES;

    const MyEnum MyEnum::OK = MyEnum{"OK"};
    const MyEnum MyEnum::KO = MyEnum{"KO"};
    const MyEnum MyEnum::BOH = MyEnum{"BOH"};


    const MyEnum& MyEnum::getFirst() {
        return *VALUES[0];
    }

    const std::vector<const MyEnum*>& MyEnum::getValues() {
        return MyEnum::VALUES;
    }

}