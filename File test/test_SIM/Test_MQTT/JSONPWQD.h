#ifndef JSONPWQD_h
#define JSONPWQD_h
#include <Arduino.h>
#include <ArduinoJson.h>
#include "PWQDSTRUCT.h"
const uint8_t g_ui8NumParam = 10;
class CLASS_JSONPWQD {
  private:
  public:
    bool Func_IsJson(char *mes);
    bool Func_DeserializeMeasureNow(char setting[], s_pond* pond);
    bool Func_DeserializeSetting(char setting[], s_pond* pond);
    float Func_DeserializeTime(char *mesrespond);
    bool Func_DeserializeStatus(char *stt);
    char* Func_SerializeData(char *deviceID, char *pondID , uint8_t gw, float value[]);
    char* Func_SerializeStatus(char *state, char *deviceID, uint16_t error);
    char* Func_SerializeCommand(char *command, char *deviceID);
    char* Func_SerializeRequestTime(char *deviceID);
};
#endif
