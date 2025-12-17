#ifndef lora_master_h
#define lora_master_h
#include "pin_config.h"
#include "jsonPWQ.h"
#include <SPI.h>
#include <RH_RF95.h>

#define ERR_INIT            501
#define ERR_SEND_RECEIVE    502

class LoraMaster {
  private:
    uint32_t waitLora = millis();
    uint8_t timesErr = 0;
    bool (LoraMaster::*pHandleMes)(char* mesSend, char *&mesRespond);
    bool processMessage(char* mesSend, char*& mesRespond);
    bool waitMes(char* mesSend, char *&mesRespond);
    bool sendMes(char* mesSend, char *&mesRespond);
    bool checkLoraError();
  public:
    uint16_t loraErr = 0;
    bool bitRun = false;
    void setUp();
    bool runProgram(bool enable, const char* moduleId, uint16_t timePump);
    char* runValueSensor(const char* moduleId);
    bool  setUpModuleSensor(const char* moduleId);
    char* getValueModuleSensor();
    void modulePondReady();
};
#endif
