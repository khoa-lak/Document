#ifndef SIMA7xxx_h
#define SIMA7xxx_h
#include "jsonPWQ.h"
#define TINY_GSM_MODEM_SIM7600
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#define MAX_MSG_SIZE 8191  //4096
typedef void (*MqttMsgCallback)(const char *topic, const char *msg);
typedef void (*DataCallback)(uint8_t *data, size_t len);//fota
class SimA7xxx {
private:
  JsonPWQ json;
  static SimA7xxx *instance;
  MqttMsgCallback mqttMsgCallback = nullptr;
  static void staticCallback(char *topic, uint8_t *payload, unsigned int length);
  char *c_setting;
  char c_meanow[500] = {};
  bool (SimA7xxx::*pRunningConnect)();

  // void Func_UpdateCommand(char *mes);
  // void Func_UpdateTime(char *mes);
  // bool Func_SubMes(char *topic);
  // bool Func_UnSubMes(char *topic);

  void mqttCallback(char *topic, byte *payload, unsigned int length);
  void rstVariable();
  bool configSim();
  bool connectSim();
  bool connectMQTT();
  bool disConnectMQTT();
  bool subTopicDevice();
  bool connectDone();
  bool followTopic(char *topic);
  bool unfollowTopic(char *topic);
  void pubMsg(char *msg, char *topic);

public:
  void setMqttMsgCallback(MqttMsgCallback cb);
  void setupSim();
  bool initSim();
  bool resetSim();
  void followMsgAnyTopic();
  void pubLogs(char *logs);
  void pubMsgMQTT(char *stt, char *topic);
  void pubStatus(char *stt);
  void subTopicPond(char *pond_id);

  bool initalTube;
  char *getDate();
  uint32_t getUnix();
  uint32_t getCurrentTime(uint32_t unixTime);
};
#endif