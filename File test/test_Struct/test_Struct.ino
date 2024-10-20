#include "tab3.h"
Tab3 tab3;
void setup() {
  Serial.begin(9600);
}

void loop() {

  tab3.func1();
  tab3.func_println();
  Serial.println("============");
  tab3.func2();
  tab3.func_println();
  Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++");
  delay(5000);
}
