#ifndef SIMA7xxx_h
#define SIMA7xxx_h
#include <Arduino.h>
#include <EEPROM.h>   //Sim chiem 400byte (30 ->430)
#include "JSONPWQD.h"
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
class CLASS_SIMA7xxx {
  private:
    char* c_setting ;
    char c_meanow[500] = {};
    bool (CLASS_SIMA7xxx::*Func_RunningConnect)();
    bool Func_SerialSim(char* AT, char* ATCheck);
    bool Func_ConnectSim();
    bool Func_ConnectMQTT();
    bool Func_DisConnectMQTT();
    bool Func_ConnectTopicDevice();
    bool Func_ConnectDone();
    void Func_ResetVariable();
    void Func_RespondNumber(char* mes);
    void Func_UpdateCommand(char* mes);
    void Func_UpdateTime(char* mes);
    void Func_SplitString(char*& mes);
    bool Func_SubMes(char* topic);
    bool Func_UnSubMes(char* topic);
    bool Func_FollowTopicSim(char* topic);
    bool Func_UnFollowTopicSim(char* topic);
  public:
    uint8_t ui8_timegetsetting = 0;
    float f_currentTime = 99999.0;
    uint32_t ui32_basetime = 0;
    uint8_t ui8_commandRun = 0;
    char *c_host = "tcp://mqtt.iotoom.com:1883";
    char *c_user = "lab";
    char *c_pass = "IotoomVN";
    char *c_deviceID          = "pwq-b9002e2048fe44e594c51535bf93d770";
    char *c_topicDeviceID     = "devices/pwq-b9002e2048fe44e594c51535bf93d770";
    char *c_topicPond      = "ponds/pond-d7fd097fc29c49058067a909abdf4999";
    char *c_mesLost       = "disconnected";
    char *c_mesConnect    = "connected";
    char *c_mesRun        = "running";
    char *c_mesOff        = "off";
    char *c_mesCommand    = "command_completed";
    char *c_mesError      = "error";
    bool b_newsetting = 0;
    void Func_SetupSim();
    bool Func_ConfigSim();
    bool Func_InitSim();
    bool Func_FollowTopic();
    bool Func_ConditionSim(bool off);
    bool Func_PubData(char* pondID, uint8_t gw, float value[]);
    void Func_PubStatus(char* stt);
    void Func_PubRequest();


    bool Func_ResetSim();

    char* Func_SubMesSim();
    bool Func_PubMesSim(char *mesPub, char *topic);

    bool Func_IsRun();
    char* Func_GetMesSetting();
    int16_t Func_CurrentTime();
};
#endif
