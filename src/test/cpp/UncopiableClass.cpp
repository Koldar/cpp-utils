#include "UncopiableClass.hpp"

UncopiableClass::UncopiableClass(int a): a{a} {

}

UncopiableClass::~UncopiableClass() {

}

std::ostream& operator <<(std::ostream& out, const UncopiableClass& other) {
    out << other.a;
    return out;
}