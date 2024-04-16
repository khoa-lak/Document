#include "JSONPWQD.h"

bool CLASS_JSONPWQD::Func_IsJson(char *mes) {
  char mesjson[2000] = {0};
  strcpy(mesjson, mes);
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, (char*)mesjson);
  doc.clear();
  if (error) {
    Serial.println("not Json");
    return 0;
  }
  Serial.println("Json ok");
  return 1;
}
