#ifndef jsonPWQ_h
#define jsonPWQ_h
#include "param.h"
#include <ArduinoJson.h>
#define lengthMesMin 255
#define lengthMesMid 1000
#define lengthMesMax 7000
const uint8_t g_ui8NumParam = 10;

class JsonPWQ {
  public:
    char* serCommandComplete();
    char* serMesSate(char *state);
    char* serMesLogs(char* mes);
    char* serMesData(float value[], uint16_t r[], uint16_t g[], uint16_t b[], uint8_t num_led);
};
#endif
