#ifndef _UNCOPIABLECLASS_HEADER__
#define _UNCOPIABLECLASS_HEADER__

#include <iostream>
#include <ostream>

class UncopiableClass {
public:
    int a;
public:
    UncopiableClass(int a);
    virtual ~UncopiableClass();
    UncopiableClass(const UncopiableClass& other) = delete;
    UncopiableClass& operator =(const UncopiableClass& other) = delete;
    friend std::ostream& operator <<(std::ostream& out, const UncopiableClass& other);
};

#endif