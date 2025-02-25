#ifndef simA7600_h
#define simA7600_h
#include <Arduino.h>
#include "jsonPWQ.h"
#include "time_unix.h"
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
class SimA7600 {
  private:
    bool (SimA7600::*P_PubMes)(char *mesPub, char *topic);
    uint8_t timesFail = 0;
    uint8_t timesErr = 0;
    uint8_t timesPub = 0;
    uint32_t timeWairRST = millis();
    uint32_t timeWaitInit = millis();

    bool serialSim(char* AT, char* ATCheck, uint8_t type);
    void resetVariable();
    bool (SimA7600::*pConnect)();
    bool initConfigSim();
    bool initConnectSim();
    bool initConnectMQTT();
    bool initConnectMapping();
    bool initConnectTopicDevice();
    bool initConnectDone();
    bool initDisConnectMQTT();

    void mesDevice(char* mes);
    void mesPond(char* mes);
    char* subMesSim();
    void splitString(char*& mes);
    bool followTopicSim(char* topic);
    bool unFollowTopicSim(char* topic);
    bool pubMesSim(char *mesPub, char *topic);
    bool resetSim();
  public:
    bool cmdRun = false;
    void setupSim();
    bool initSim();
    bool followTopic();
    bool pubStatus(char* mes);
    bool pubData(char* mes, uint8_t type);
    bool pubLogs(char* mes);


};
#endif
