#include "JSONPWQD.h"

bool CLASS_JSONPWQD::Func_IsJson(char *mes) {
  char mesjson[2000] = {0};
  strcpy(mesjson, mes);
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, (char*)mesjson);
  doc.clear();
  if (error) {
    Serial.println("not Json");
    return 0;
  }
  Serial.println("Json ok");
  doc.clear();
  return 1;
}

char* CLASS_JSONPWQD::jsonGetNumber(char* mes) {
  uint8_t len = strlen(mes) + 100;
  DynamicJsonDocument doc(len);
  DeserializationError error = deserializeJson(doc, (char*)mes);
  if (error)
    return jsonFault();
  uint32_t num = doc["command"];
  DynamicJsonDocument doc1(len);
  doc1["message_type"] = "test";
  doc1["command"] = num;
  char *mesRespond = new char[len];
  serializeJson(doc1, mesRespond, len);
  doc1.clear();
  doc.clear();
  return mesRespond;
}
char* CLASS_JSONPWQD::jsonSerStatus(char *state, char *deviceID) {
  const uint8_t lenght = 1000;
  DynamicJsonDocument doc(lenght);
  doc["device_type"] = "photo_water_quality";
  doc["message_type"] = "update_state";
  doc["device_id"] = deviceID;
  doc["state"] = state;
  char *mes = new char[lenght];
  serializeJson(doc, mes, lenght);
  doc.clear();
  return mes;
}
char* CLASS_JSONPWQD::jsonFault() {
  const uint8_t lenght = 1000;
  DynamicJsonDocument doc(lenght);
  doc["message_type"] = "update_state";
  doc["state"] = "json fault";
  char *mes = new char[lenght];
  serializeJson(doc, mes, lenght);
  doc.clear();
  return mes;
}
