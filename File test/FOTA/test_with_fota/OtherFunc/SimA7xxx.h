#ifndef SIMA7xxx_h
#define SIMA7xxx_h
#include "jsonPWQ.h"
#define TINY_GSM_MODEM_SIM7600
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#define MAX_MSG_SIZE 6144  //4096
typedef void (*MqttMsgCallback)(const char *topic, const char *msg);
class SimA7xxx {
private:
  JsonPWQ json;
  static SimA7xxx *instance;
  MqttMsgCallback mqttMsgCallback = nullptr;
  static void staticCallback(char *topic, uint8_t *payload, unsigned int length);
  char _rxBuffer[MAX_MSG_SIZE + 1]; 
  char _dateBuffer[32];
  uint32_t _lastSyncUnix = 0;   // Thời điểm Unix lần cuối sync thành công
  uint32_t _lastSyncMillis = 0; // Thời điểm millis() lúc sync
  bool (SimA7xxx::*pRunningConnect)();

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
  char *getDate();
  uint32_t getUnix();
  uint32_t getCurrentTime(uint32_t unixTime);
};
#endif