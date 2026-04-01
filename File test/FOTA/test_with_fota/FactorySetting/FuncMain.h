#ifndef funcmain_h
#define funcmain_h
#include <EEPROM.h>
#include <Arduino.h>
#include "SimA7xxx.h"
// #include "JsonPWQ.h"
#include "Parameter.h"
class FuncMain {
private:
  SimA7xxx simA7xxx;
  JsonPWQ jsonPWQ;
  bool _first_run;
  uint8_t _jump = 0;
  bool checkSim();
  void classifyFuncRun();
  void handleMsgMQTT(const char *topic, char *msg);
  void handleMsgCommand(char *msg);
  //   void handleMsgSetting(const char *topic, char *msg);
  //   bool handleGetStgPond();
  //   void pubMsgPond(char *msg, uint8_t pond);
  void pubMsgDevice(char *msg);
  //   void pubLogsMQTT(char *msg);
  void handleStateConnected();
  void handleStateRunning();
  //   void handleStateCalib();
  //   void handleStateManual();
public:
  FuncMain();
  ~FuncMain();
  ModuleSetting moduleStg = { MACHINE_STATUS_DISCONNECTED, "", false, false, 0, 0, -1, -1, 0 };
  //err, jack_plug, name_sensor,  err_code, coef, eeprom_addres, raw_value, value;
  ParamSensor paramSensor[NUM_SENSOR] = {
    { false, false, false, "DO", 502, 5, 0, { 1.0, 0 }, { 12, 16 }, { 4.0, 8.0 }, -1 },
    { false, false, false, "TEMP", 503, 5, 0, { 1.0, 0 }, { 20, 24 }, { 0.0, 30.0 }, -1 }
  };
  void setupMain();
  uint8_t runMain();
  void hasMqttMessage(const char *topic, const char *msg);
};
#endif
