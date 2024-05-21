// test.h

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef A2DD_H
#define A2DD_H

int cpp_entry(void);

namespace foo {
    int bob();

    namespace bar {
        int bob2(void);
    }
}

class A2DD
{
  int gx;
  int gy;

public:
  A2DD(int x,int y);
  int getSum();

};

#endif

#ifdef	__cplusplus
}
#endif
