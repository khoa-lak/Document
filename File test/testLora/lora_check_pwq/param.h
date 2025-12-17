#ifndef param_h
#define param_h
#include <Arduino.h>
#define NUM_POND    8 
#define NUM_REAGENT 7
#define NUM_SENSOR  5
#define NUM_CALIB   5

#define API_DEVICE            "https://apis.iotoom.com/devices/photo_water_quality/settings"
#define API_GET_TIME          "https://apis.iotoom.com/get_time"
#define HOST                  "tcp://mqtt.iotoom.com:8883"
#define USER                  "lab"
#define PASSWORD              "IotoomVN"
#define DEVICE_TYPE           "pwq"
#define DEVICE_ID             "pwq-4effc60db7b648619981a57e1e846fc2" //"pwq-b9002e2048fe44e594c51535bf93d770";
#define TOPIC_DEVICE          "devices/pwq-4effc60db7b648619981a57e1e846fc2"
#define BASE64                "cHdxLTRlZmZjNjBkYjdiNjQ4NjE5OTgxYTU3ZTFlODQ2ZmMy" //6fc2
#define MSG_DISCONNECTED      "DISCONNECTED"
#define MSG_CONNECTED         "CONNECTED"
#define MSG_WARNING           "WARNING"
#define MSG_RUN               "RUNNING"
#define MSG_CALIB             "CALIB"
#define MSG_ERROR             "ERROR"
#define MSG_ERROR_CHECK       "ERROR_CHECK"
#define MSG_ON                "ON"
#define MSG_OFF               "OFF"

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
  uint8_t maxTest;
  uint8_t currentCount;
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
  uint8_t flowRate;
  uint16_t timePump;
  uint16_t errCode;
  //float temp;
  //ParamSensor paramSensor[NUM_SENSOR];
};
struct SettingCalib {
  char* nameReagent;
  uint8_t amount;
  float* coefs;
  float* inters;
  uint8_t* numLed;
};
//class ParamPWQ {
//  public:
//    char* API_DEVICE         = "https://apis.iotoom.com/devices/photo_water_quality/settings";
//    char* API_GET_TIME        = "https://apis.iotoom.com/get_time";
//    char* HOST          = "tcp://mqtt.iotoom.com:8883";
//    char* USER          = "lab";
//    char* PASSWORD          = "IotoomVN";
//    char* DEVICE_TYPE        = "pwq";
//    char* DEVICE_ID          = "pwq-4effc60db7b648619981a57e1e846fc2"; //"pwq-b9002e2048fe44e594c51535bf93d770";
//    char* TOPIC_DEVICE     = "devices/pwq-4effc60db7b648619981a57e1e846fc2";
//    char* BASE64            = "cHdxLTRlZmZjNjBkYjdiNjQ4NjE5OTgxYTU3ZTFlODQ2ZmMy"; //6fc2
//    char* MSG_DISCONNECTED           = "DISCONNECTED";
//    char* MSG_CONNECTED        = "CONNECTED";
//    char* MSG_WARNING        = "WARNING";
//    char* MSG_RUN            = "RUNNING";
//    char* MSG_CALIB_ON        = "CALIB_START";
//    char* MSG_CALIB_OFF      = "CALIB_STOP";
//    char* MSG_MEASURE_NOW     = "measure_now_ok";
//    char* MSG_ERROR          = "ERROR";
//    char* MSG_ERROR_CHECK     = "ERROR_CHECK";
//    //char* pumpID            = "pond-module-c2c37ceb25ac44baae32088f94881918";
//    char* MSG_ON             = "ON";
//    char* MSG_OFF            = "OFF";
//
//
//};
#endif
