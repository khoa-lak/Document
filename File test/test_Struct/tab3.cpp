#include "tab3.h"
Tab1 tab1;
Tab2 tab2;
void Tab3::func_println() {
  tab1.func_println(struct3[0]);
  tab2.func_println(struct3[0]);
   Serial.println("print 3");
  Serial.println(struct3[0].nameS);
  Serial.println(struct3[0].val1);
}
void Tab3::func1() {
  tab1.func(struct3[0]);
}
void Tab3::func2() {
  tab2.func(struct3[0]);
}
