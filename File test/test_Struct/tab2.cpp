#include "tab2.h"

void Tab2::func_println(Struct1 struct2) {
  Serial.println("print 2");
  Serial.println(struct2.nameS);
  Serial.println(struct2.val1);

}
void Tab2::func(Struct1 struct2) {
  char txt[50]  = "cccccccccccccccccccccc1";
  if (struct2.nameS != nullptr)
    delete [] struct2.nameS;
  struct2.nameS = new char[50];
  strcpy(struct2.nameS, txt);
  struct2.val1 = 30;
}
