#ifndef funcmain_h
#define funcmain_h
#include "SimA7xxx.h"
#include "JsonPWQ.h"
class FuncMain {
private:
  SimA7xxx simA7xxx;
  JsonPWQ jsonPWQ;
  bool _first_run;
  bool checkSim();
  void classifyFuncRun();
  void handleMsgMQTT(const char *topic, char *msg);
  void handleMsgCommand(char *msg);
  void handleMsgSetting(const char *topic, char *msg);
  bool handleGetStgPond();
  void pubMsgPond(char *msg, uint8_t pond);
  void pubMsgDevice(char *msg);
  void pubLogsMQTT(char *msg);
  void handleStateConnected();
  void handleStateRunning();
  void handleStateCalib();
  void handleStateManual();
public:
  FuncMain();
  ~FuncMain();
  SettingDevice stgDevice = { MACHINE_STATUS_RUNNING, false, false, false, true, { 0, 0 }, { 0, 0 }, -1, -1, -1, -1, 27.0, 0 };
  ListReagentNow listReagentNow = { 0, -1, { -1 }, "", "", 0 };
  void setupMain();
  void runMain();
  void hasMqttMessage(const char *topic, const char *msg);
};
#endif
