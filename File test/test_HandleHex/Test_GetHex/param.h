#ifndef param_h
#define param_h
#include <Arduino.h>
#define NUM_POND    4
#define NUM_REAGENT 7
#define NUM_SENSOR  5
struct ParamReagent {
  bool enable;
  bool isSensor;
  char* nameReagent;
  uint8_t  amount;
  uint8_t* pos;
  uint8_t* method;
  float* drops;
  float  mlWater;
  float  dilutingRate;
  float  reversionRate;
  uint16_t* delayMeasure;
  uint8_t* checkReagent;
  bool cleanVim;
};
struct ParamSensor {
  bool enable;
  bool err;
  uint16_t* listErrCode;
  uint16_t errCode[NUM_POND + 1];
  char* nameSensor;

};
struct ParamPond {
  bool enable;
  bool subMqtt;
  char* pondName;
  char* moduleId;
  char* id;
  char* sche;
  uint8_t distance;
  uint8_t diameter;
  uint16_t errCode;
  //float temp;
  //ParamSensor paramSensor[NUM_SENSOR];
};
struct SettingDevice {
  float tempCeiling;
  float tempFloor;
  float mlDrop;
  float speedPumpPond;
  float speedPumpPurity;
  float speedPumpVim;
  float speedPumpDis;
};
class ParamPWQ {
  public:
    char* api_file_hex      = "https://s3.ap-southeast-1.amazonaws.com/iotoom.com/hardware/test_OTA.txt";
    char* apiGettime        = "https://apis.iotoom.com/get_time";
    char* hostMQTT          = "tcp://mqtt.iotoom.com:8883";
    char* userMQTT          = "lab";
    char* passMQTT          = "IotoomVN";
    char* deviceType        = "OTA";
    char* deviceID          = "upload_tu_xa"; //"pwq-b9002e2048fe44e594c51535bf93d770";
    char* topicDeviceID     = "devices/upload_tu_xa";
    char* mesLost           = "DISCONNECTED";
    char* mesConnect        = "CONNECTED";
    char* mesWarning        = "WARNING";
    char* mesRun            = "RUNNING";
    char* mesMeasureNow     = "measure_now_ok";
    char* mesError          = "ERROR";
    char* mesErrorCheck     = "ERROR_CHECK";
    char* mesOn             = "ON";
    char* mesOff            = "OFF";


};
#endif
