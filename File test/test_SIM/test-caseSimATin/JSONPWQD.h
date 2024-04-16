#ifndef JSONPWQD_h
#define JSONPWQD_h
#include <Arduino.h>
#include <ArduinoJson.h>
const uint8_t g_ui8NumParam = 10;
class CLASS_JSONPWQD {
  private:
  public:
    bool Func_IsJson(char *mes);
    char* jsonFault();
    char* jsonGetNumber(char* mes);
    char* jsonSerStatus(char *state, char *deviceID);

};
#endif
