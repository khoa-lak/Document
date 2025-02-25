#include "jsonPWQ.h"
ParamPWQ paramPWQ;


char* JsonPWQ::serMesData(float value[], uint16_t r[], uint16_t g[], uint16_t b[], uint8_t num_led) {
  uint16_t len = 1000;
  DynamicJsonDocument doc(len);
  doc["device_type"] = paramPWQ.deviceType;
  doc["message_type"] = "update_data";
  doc["device_id"] = paramPWQ.deviceID;
  doc["name_sensor"] = "TSL2561";
  //JsonObject params = doc.createNestedObject("params");
  JsonArray params = doc.createNestedArray("params");
  for (uint8_t i = 0 ; i < num_led; i++) {
    JsonObject data_led = params.createNestedObject();
    data_led["led"] = i + 1;
    data_led["value"] =value[i];
    data_led["r"] = r[i];
    data_led["g"] = g[i];
    data_led["b"] = b[i];

  }
  char *mes = new char[len];
  serializeJson(doc, mes, len);
  //doc.clear();
  return mes;
}


char* JsonPWQ::serCommandComplete() {
  uint16_t len = lengthMesMin;
  DynamicJsonDocument doc(len);
  doc["device_type"] = paramPWQ.deviceType;
  doc["message_type"] = "command_completed";
  doc["device_id"] = paramPWQ.deviceID;
  char *mes = new char[len];
  serializeJson(doc, mes, len);
  //doc.clear();
  return mes;
}

char* JsonPWQ::serMesSate(char *state) {
  uint16_t len = lengthMesMin;
  DynamicJsonDocument doc(len);
  doc["device_type"] = paramPWQ.deviceType;
  doc["message_type"] = "update_state";
  doc["device_id"] = paramPWQ.deviceID;
  doc["state"] = state;
  char *mes = new char[len];
  serializeJson(doc, mes, len);
  //doc.clear();
  return mes;
}
char* JsonPWQ::serMesLogs(char* mes) {
  uint16_t len = 255;
  DynamicJsonDocument doc(len);
  doc["device_type"] = paramPWQ.deviceType;
  doc["message_type"] = "check_logs";
  doc["device_id"] = paramPWQ.deviceID;
  doc["pay_load"] = mes;
  char *json = nullptr;
  json = new char[len];
  serializeJson(doc, json, len);
  return json;
}
