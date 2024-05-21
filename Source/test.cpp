// A2DD.cpp
#include "test.hpp"

A2DD::A2DD(int x,int y)
{
    gx = x;
    gy = y;
}

int A2DD::getSum()
{
    return gx + gy;
}

namespace foo {
    int bob(void) {
        return 1;
    }

    namespace bar {
        int bob2(void) {
            return 1;
        }
    }
}

int cpp_entry(void) {

    foo::bar::bob2();

    return foo::bob();
}
