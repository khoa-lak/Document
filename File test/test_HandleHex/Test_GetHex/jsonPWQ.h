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
        void derSettingDevice(char* mes, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR], SettingDevice settingDevice);
        void derSettingPond(char *mes, char*& pondSche);
        uint8_t derSchedule(char* mes, uint8_t* data, char* day, uint32_t currentTime, ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR]);
        uint32_t derGetTime(char *mes);
        uint16_t derMeasureNow(char *mes, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR]);
        char* serCommandComplete();
        char* serMesSate(char *state);
        char* serMesSate(char *title, char *subtitle, uint32_t startTime, uint16_t totalTime, float temp, uint8_t pond, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR], uint16_t *dataQueue, uint8_t numQueue, uint32_t freeMemory, char* testID);
        char* serMesSate(uint16_t errCode);
        char* serMesSateErrorCheck(uint16_t errCode);
        char* serMesData(char *pondID, char* nameReagent, float value,  uint16_t* led_pre, uint16_t* led_af, uint16_t* led_preArray, uint16_t* led_afArray, char* testID);
        char* jsonDataLed(char* nameReagent, float value, uint8_t pond, uint16_t* led_pre, uint16_t* led_af);
        char* serLEDWater(uint16_t *dataLED , uint16_t *dataArray);
        char* serMesLoraSensor(char* moduleId);
        bool derMesReceiveSensor(char* msg, char* moduleId);
        char* serMesLoraPump(bool enable, ParamPond paramPond);
        bool derMesReceivePump(char *msg, ParamPond paramPond);
        char* serTurnOnModuleSensor(char* moduleId);
        char* serValueSensor(char* mesData, uint8_t module, ParamPond paramPond, ParamSensor paramSensor[NUM_SENSOR]);
        char* sendValueLed(char* test, char* mission, uint16_t* ledData, uint16_t *dataArray);
        char* serMesLogs(char* mes);
};
#endif
