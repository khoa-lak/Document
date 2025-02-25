#ifndef param_h
#define param_h
#include <Arduino.h>
class ParamPWQ {
  public:
    char* apiGettime        = "https://apis.iotoom.com/get_time";
    char* hostMQTT          = "tcp://mqtt.iotoom.com:8883";
    char* userMQTT          = "lab";
    char* passMQTT          = "IotoomVN";
    char* deviceType        = "sensor";
    char* deviceID          = "sensor-cluster";
    char* topicDeviceID     = "devices/sensor-cluster";
    char* topicBH1750       = "devices/sensor-cluster/BH1750";
    char* topicTCS34725     = "devices/sensor-cluster/TCS34725";
    char* topicTEMT6000     = "devices/sensor-cluster/TEMT6000";
    char* mesLost           = "DISCONNECTED";
    char* mesConnect        = "CONNECTED";
    char* mesWarning        = "WARNING";
    char* mesRun            = "RUNNING";
};
#endif
