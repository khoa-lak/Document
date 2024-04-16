#ifndef SIMA7xxx_h
#define SIMA7xxx_h
#include <Arduino.h>
#include <EEPROM.h>   //sim chiem 400byte (30 ->430)
#include "JSONPWQD.h"
#include "PWQDSTRUCT.h"
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
class CLASS_SIMA7xxx {
  private:
    char c_setting[1000] = {};
    char c_meanow[500] = {};
    bool (CLASS_SIMA7xxx::*Func_RunningConnect)();
    bool (CLASS_SIMA7xxx::*Func_RunningFollowTopic)();
    bool Func_SerialSIM(char* AT, char* ATCheck);
    bool Func_CheckSIM();
    bool Func_CheckMQTT();
    bool Func_ConnectDone();
    void Func_ResetVariable();
    void Func_UpdateSetting(char* mes);
    void Func_UpdateCommand(char* mes);
    void Func_UpdateTime(char* mes);
    void Func_SplitString(char*& mes);
    bool Func_FollowTopic1();
    bool Func_FollowTopic2();
    bool Func_SubMes(char* topic);
    bool Func_UnSubMes(char* topic);
  public:
    float f_currentTime = 99999.0;
    uint32_t ui32_basetime = 0;
    uint8_t ui8_command = 0;
    char *c_host = "tcp://mqtt.iotoom.com:1883";
    char *c_user = "lab";
    char *c_pass = "IotoomVN";
    char *c_deviceID          = "pwq-b9002e2048fe44e594c51535bf93d770";
    char *c_topicDeviceID     = "devices/pwq-b9002e2048fe44e594c51535bf93d770";
    char *c_prexPond      = "ponds/";
    char *c_topicPond      = "ponds/pond-d7fd097fc29c49058067a909abdf4999";
    char *c_mesLost       = "disconnected";
    char *c_mesConnect    = "connected";
    char *c_mesRun        = "running";
    char *c_mesOff        = "off";
    char *c_mesError      = "error";
    char *c_mesCommand    = "command_completed";
    
    bool b_newsetting = 0;
    bool Func_ConfigSIM();
    bool Func_ConnectSIM();
    bool Func_ConditionSIM(bool off);
    bool Func_PubData(char* pondID, uint8_t gw, float value[]);
    void Func_PubStatus(char* stt, uint16_t error);
    void Func_PubRequest();
    bool Func_FollowTopic();
    bool Func_DisConnectSIM();
    bool Func_ResetSIM();
    bool Func_FollowTopicSIM(char* topic);
    bool Func_UnFollowTopicSIM(char* topic);
    char* Func_SubMesSIM();
    bool Func_PubMesSIM(char *mesPub, char *topic);
    bool Func_ConnectSim();
    bool Func_IsRun();
    char* Func_GetMesSetting();
    int16_t Func_CurrentTime();
};
#endif
