#ifndef tab3_h
#define tab3_h
#include "struct.h"
#include "tab1.h"
#include "tab2.h"

class Tab3 {
  public:
    Struct1 struct3[2] =
    {
      {nullptr, 1},
      {nullptr, 2},
    };
    void func_println();
    void func1();
    void func2();
};
#endif
