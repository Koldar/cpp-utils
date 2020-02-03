#include "MyEnum.hpp"

namespace cpp_utils {

    std::vector<const AbstractEnum<MyEnum>*> MyEnum::VALUES{};

    const MyEnum MyEnum::OK = MyEnum{"OK"};
    const MyEnum MyEnum::KO = MyEnum{"KO"};
    const MyEnum MyEnum::BOH = MyEnum{"BOH"};

}