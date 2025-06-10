#ifndef DEFINITION_VARIABLE_H
#define DEFINITION_VARIABLE_H

#include <stdint.h>
#define NUM_POND    4
#define NUM_REAGENT 7
#define NUM_SENSOR  5
#define NUM_CALIB   5

#define API_DEVICE            "https://apis.iotoom.com/devices/photo_water_quality/settings"
#define API_GET_TIME          "https://apis.iotoom.com/get_time"
#define HOST                  "tcp://mqtt.iotoom.com:8883"
#define USER                  "lab"
#define PASSWORD              "IotoomVN"
#define DEVICE_TYPE           "pwq"
#define DEVICE_ID             "pwq-4effc60db7b648619981a57e1e846fc2"
#define TOPIC_DEVICE          "devices/pwq-4effc60db7b648619981a57e1e846fc2"
#define BASE64                "cHdxLTRlZmZjNjBkYjdiNjQ4NjE5OTgxYTU3ZTFlODQ2ZmMy"
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
    uint8_t amount;
    uint8_t* pos;
    uint8_t* method;
    float* drops;
    float mlWater;
    float dilutingRate;
    float reversionRate;
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
    uint8_t flowRate;
    uint16_t timePump;
    uint16_t errCode;
};

struct SettingCalib {
    char* nameReagent;
    uint8_t amount;
    float* coefs;
    float* inters;
    uint8_t* numLed;
};

#endif