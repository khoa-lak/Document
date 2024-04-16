#include "PIN_CONFIG.h"
void CLASS_CONFIG::FUNC_CONFIGPIN() {
  pinMode(Led, OUTPUT);
  digitalWrite(Led, 0);
}
