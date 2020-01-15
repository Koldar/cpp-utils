#include "AbstractEnum.hpp"

namespace cpp_utils {

    static int nextId = 0;

    AbstractEnum::AbstractEnum(const std::string& name): index{AbstractEnum::getNextId()}, name{name} {

    }

    AbstractEnum::~AbstractEnum() {

    }

    AbstractEnum::AbstractEnum(const AbstractEnum::This& o): index{o.index}, name{o.name} {

    }
    
    AbstractEnum::AbstractEnum(AbstractEnum::This&& o): index{o.index}, name{o.name} {

    }
    
    AbstractEnum::This& AbstractEnum::operator =(const AbstractEnum::This& o) {
        this->index = o.index;
        this->name = o.name;
        return *this;
    }
    
    AbstractEnum::This& AbstractEnum::operator = (AbstractEnum::This&& o) {
        this->index = o.index;
        this->name = o.name;
        return *this;
    }
    
    int AbstractEnum::getNextId() {
        nextId += 1;
        return nextId - 1;
    }

    int AbstractEnum::getIndex() const {
        return this->index;
    }

    std::string AbstractEnum::getName() const {
        return this->name;
    }

    bool operator ==(const AbstractEnum::This& a, const AbstractEnum::This& b) {
        return a.index == b.index;
    }

    bool operator !=(const AbstractEnum::This& a, const AbstractEnum::This& b) {
        return a.index != b.index;
    }

    std::ostream& operator <<(std::ostream& out, const AbstractEnum::This& a) {
        out << a.name;
        return out;
    }
}