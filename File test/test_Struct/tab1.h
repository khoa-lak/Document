#ifndef tab1_h
#define tab1_h
#include "struct.h"
class Tab1 {
  public:
    void func_println(Struct1 &struct2);
    //    {
    //      Serial.println("print 2");
    //      //Serial.println(val);
    //      Serial.println(struct1[0].nameS);
    //      Serial.println(struct1[0].val1);
    //      Serial.println(struct1[1].nameS);
    //      Serial.println(struct1[1].val1);
    //    }
    void func(Struct1 &struct2);
    //    {
    //      char txt[50]  = "bbbbbbbbbbbbbbbb1";
    //      if (struct1[0].nameS != nullptr)
    //        delete [] struct1[0].nameS;
    //      struct1[0].nameS = new char[50];
    //      strcpy(struct1[0].nameS, txt);
    //      struct1[0].val1 = 20;
    //      char txt1[50]  = "bbbbbbbbbbbbbb2";
    //      if (struct1[1].nameS != nullptr)
    //        delete [] struct1[1].nameS;
    //      struct1[1].nameS = new char[50];
    //      strcpy(struct1[1].nameS, txt1);
    //      struct1[1].val1 = 21;
    //    }

};
#endif
