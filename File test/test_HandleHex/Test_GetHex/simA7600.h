//"{\"device_id\":\"pwq-b9002e2048fe44e594c51535bf93d770\",\"message_type\":\"settings\",\"data\":{\"farm_id\":\"farm-330e1ac4715847958a73204decc7c8b5\",\"gateways\":[{\"name\":1,\"pond_id\":\"pond-edf1799d6a5142f5aa4c6eb6bef2725b\",\"schedules\":{\"PH\":[0,480,720],\"NH3\":[0,480,720],\"NO2\":[0,480,720],\"KH\":[0,480,720],\"MG\":[0,480,720],\"CA\":[0,480,720]}},{\"name\":2,\"pond_id\":\"pond-edf1799d6a5142f5aa4c6eb6bef2725c\",\"schedules\":{\"PH\":[0,200,900],\"NH3\":[0,150,1400],\"NO2\":[0,480,1200],\"KH\":[0,10,1400],\"MG\":[0,80,500],\"CA\":[0,90,300]}}]}}"
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
    bool cmdRun = false;
    uint8_t timesFail = 0;
    uint8_t timesErr = 0;
    uint8_t timesPub = 0;
    uint32_t timeWairRST = millis();
    uint32_t timeWaitInit = millis();


    //bool serialSim(char* AT, char* ATCheck, uint8_t type);
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
    char* connectHTTP(char* url, uint8_t mode);
    void removeSubstringHTTP(char*& mes, char *sub);
    bool resetSim();

  public:
    bool getSetting = false;
    bool initalTube = false;
    uint16_t reagentNow = 9999;
    uint16_t simErr = 0;
    void setupSim();
    bool initSim();
    bool followTopic();
    bool processMapping();
    bool pubStatus(char* mes);
    bool pubData(char* mes, char* pondID);
    bool pubLogs(char* mes);
    bool runCMD();
    void controlCMDRun(bool cmd);
    bool getFileHex();
    ParamReagent paramReagent[NUM_REAGENT] = {
      {false, false, "KH",   1, (uint8_t[]){4},            (uint8_t[]){1},         (float[]){4.0},                   3.0    , 1.0  , 1.0,  (uint16_t[]){0},       (uint8_t[]){2},           false},     //KH
      {false, false, "NO2",  1, (uint8_t[]){5},            (uint8_t[]){0},         (float[]){2.0},                 3.0    , 0.33 , 1.0,  (uint16_t[]){90},      (uint8_t[]){2},           false},     //NO2-moni //trươc đợi 60s
      {false, false, "NH3",  2, (uint8_t[]){6, 7},         (uint8_t[]){0, 0},      (float[]){2.4, 2.4},            3.0    , 0.5  , 1.0,  (uint16_t[]){0, 180},  (uint8_t[]){0, 2},        false},     //NH3
      {false, false, "CA",   3, (uint8_t[]){10, 11, 18},   (uint8_t[]){0, 0, 1},   (float[]){2.4, 0.6, 4.0},       3.0    , 1.0  , 1.0,  (uint16_t[]){0, 0, 0}, (uint8_t[]){0, 1, 2},     false},    //CA
      {false, false, "MG",   3, (uint8_t[]){8, 9, 18},     (uint8_t[]){0, 0, 1},   (float[]){4.0, 1.0, 4.0},       2.5   , 1.0  , 1.0,  (uint16_t[]){0, 0, 0}, (uint8_t[]){0, 1, 2},     false},     //MG
      {false, false, "CL",   1, (uint8_t[]){12},           (uint8_t[]){0},         (float[]){1.0},                 3.5    , 1.0  , 1.0,  (uint16_t[]){0},       (uint8_t[]){2},           false},     //CL
      //{false, false, "ACIDITY",   3, (uint8_t[]){13, 14, 15},   (uint8_t[]){0, 0, 0},   (float[]){3.0, 3.0, 3.0},       3.0    , 1.0  , 1.0,  (uint16_t[]){0, 0, 300}, (uint8_t[]){0, 0, 2},     false},   //FE
      {true, true , "SENSOR",          0,  nullptr,           nullptr,         nullptr,                   0, 0, 0,  (uint16_t[]){0}, nullptr,   false},     //sensor
      //      {false, false, "H2S",  3, (uint8_t[]){8, 9, 10},     (uint8_t[]){0, 0, 0},   (float[]){4.0, 4.0, 4.0},       3.0    , 1.0  , 1.0,  (uint16_t[]){0, 180, 40},   false},       //H2S
      //      {false, false, "FE",   1, (uint8_t[]){11},           (uint8_t[]){0, 0, 0},   (float[]){3.0},                 3.0    , 1.0  , 1.0,  (uint16_t[]){180},          false},       //FE
      //      {false, false, "NO3",  2, (uint8_t[]){12, 13},       (uint8_t[]){0, 0},      (float[]){6.0, 6.0},            3.0    , 0.5  , 1.0,  (uint16_t[]){0, 300},       false},       //NO3
    };
    ParamPond paramPond[NUM_POND] = {
      { false, false, nullptr, nullptr, nullptr, nullptr, 0, 0, 0},
      { false, false, nullptr, nullptr, nullptr, nullptr, 0, 0, 0},
      { false, false, nullptr, nullptr, nullptr, nullptr, 0, 0, 0},
      { false, false, nullptr, nullptr, nullptr, nullptr, 0, 0, 0}
    };
    ParamSensor paramSensor[NUM_SENSOR] = {
      {false, false, (uint16_t[]){503}, {0, 0, 0, 0, 0}, "DO" },
      {false, false, (uint16_t[]){504}, {0, 0, 0, 0, 0}, "PH"},
      {false, false, (uint16_t[]){505}, {0, 0, 0, 0, 0}, "SALINITY"},
      {false, false, (uint16_t[]){506}, {0, 0, 0, 0, 0}, "K"},
      {false, false, (uint16_t[]){507}, {0, 0, 0, 0, 0}, "TEMP"}
    };
    //, {{false, false, "DO" , 0.0, 0.0, 503}, {false, false, "SALINITY", 0.0, 0.0, 504}, {false, false, "TEMP", 0.0, 0.0, 505}}
    SettingDevice settingDevice = {26.0, 22.0, 0.038, 100, 100, 100, 100};
};
#endif
