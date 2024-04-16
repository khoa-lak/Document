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
  return 1;
}
bool CLASS_JSONPWQD::Func_DeserializeMeasureNow(char setting[], s_pond* pond) {
  uint16_t len = 2000;
  char json[len] = "";
  strcpy(json, setting);
  DynamicJsonDocument doc(2000);
  DeserializationError error = deserializeJson(doc, (char*)json);
  if (error) {
    //    Serial.print(F("deserializeJson1() failed: "));
    //    Serial.println(error.f_str());
    doc.clear();
    return false;
  }
  for (uint8_t i = 0; i < s_amountPond; i++) {
    pond[i].active = 0;
    for (uint8_t j = 0; j < s_amountReagent; j++) {
      pond[i].reagentPond[j].active = 0;
      if (pond[i].reagentPond[j].timeMea != nullptr) {
        delete[] pond[i].reagentPond[j].timeMea;
      }
      pond[i].reagentPond[j].timeMea = nullptr;
    }
  }
  //  memset(&pond->active, 0, sizeof(uint8_t) * s_amountPond);
  //  memset(&(pond->reagentPond->active), 0, sizeof(uint8_t) * s_amountPond * s_amountReagent);
  //  JsonArray payloadArray = doc["payload"].as<JsonArray>();
  //  for (JsonVariant data_payload : payloadArray) {
  //    JsonObject data = data_payload.as<JsonObject>();
  //    uint8_t data_gateway = data["name"].as<uint8_t>();
  //    const char* data_pondid = data["pond_id"].as<const char*>();
  //    for (uint8_t i = 0; i < s_amountPond; i++) {
  //      if (pond[i].gw == data_gateway ) {
  //        if (data_pondid != NULL) {
  //          pond[i].active = 1;
  //          if (pond[i].pondID != nullptr) {
  //            delete[] pond[i].pondID;
  //            pond[i].pondID = nullptr;
  //          }
  //          size_t len = strlen(data_pondid) + 1;
  //          pond[i].pondID = new char[len];
  //          strncpy(pond[i].pondID, data_pondid, len);
  //          pond[i].pondID[len - 1] = '\0';
  //          Serial.print("ao đo là: ");
  //          Serial.println(pond[i].pondID);
  //          JsonArray paramArray = data["param"].as<JsonArray>();
  //          for (JsonVariant param : paramArray) {
  //            const char* data_param_name = param.as<const char*>();
  //            Serial.println(data_param_name);
  //            for (uint8_t j = 0; j < s_amountReagent; j++) {
  //              if (strcmp(data_param_name, s_listreagent[j]) == 0) {
  //                pond[i].reagentPond[j].active = 1;
  //                Serial.println("json có");
  //                break;
  //              }
  //            }
  //          }
  //          break;
  //        }
  //      }
  //    }
  //  }
  const char* payload_pondId = doc["payload"]["pondId"]; // "pond-d7fd097fc29c49058067a909abdf4999"
  const char* payload_param = doc["payload"]["param"]; // "PH"
  pond[0].active = 1;
  if (pond[0].pondID != nullptr) {
    delete[] pond[0].pondID;
    pond[0].pondID = nullptr;
  }
  size_t len1 = strlen(payload_pondId) + 1;
  pond[0].pondID = new char[len1];
  strncpy(pond[0].pondID, payload_pondId, len1);
  pond[0].pondID[len1 - 1] = '\0';
  Serial.print("ao đo là: ");
  Serial.println(pond[0].pondID);
  Serial.println(payload_pondId);
  for (uint8_t j = 0; j < s_amountReagent; j++) {
    if (strcmp( payload_param, s_listreagent[j]) == 0) {
      pond[0].reagentPond[j].active = 1;
      Serial.println("json có");
      break;
    }
  }
  doc.clear();
  return 1;
}
bool CLASS_JSONPWQD::Func_DeserializeSetting(char setting[], s_pond * pond) {
  uint16_t len = 2000;
  char json[len] = "";
  strcpy(json, setting);
  DynamicJsonDocument doc(2000);
  DeserializationError error = deserializeJson(doc, (char*)json);
  if (error) {
    //    Serial.print(F("deserializeJson1() failed: "));
    //    Serial.println(error.f_str());
    doc.clear();
    return false;
  }
  for (uint8_t i = 0; i < s_amountPond; i++) {
    pond[i].active = 0;
    for (uint8_t j = 0; j < s_amountReagent; j++) {
      pond[i].reagentPond[j].active = 0;
      if (pond[i].reagentPond[j].timeMea != nullptr) {
        delete[] pond[i].reagentPond[j].timeMea;
      }
      pond[i].reagentPond[j].timeMea = nullptr;
    }
  }
  Serial.print("json okkk setting: ");
  JsonObject data = doc["data"];
  for (JsonObject data_gateway : data["gateways"].as<JsonArray>()) {
    uint8_t data_gateway_name = data_gateway["name"].as<uint8_t>();
    const char* pond_id = data_gateway["pond_id"].as<const char*>();
    if (pond_id != nullptr) {
      Serial.print("ao đo là: ");
      Serial.print(pond_id);
      Serial.println(" thuoc cong: " + String(data_gateway_name));
      for (uint8_t i = 0; i < s_amountPond; i++) {
        if (pond[i].gw == data_gateway_name) {
          pond[i].active = 1;
          if (pond[i].pondID != nullptr) {
            delete[] pond[i].pondID;
            pond[i].pondID = nullptr;
          }
          size_t len = strlen(pond_id);
          pond[i].pondID = new char[len];
          strcpy(pond[i].pondID, pond_id);
          for (JsonObject data_schedule : data["schedules"].as<JsonArray>()) {
            const char* data_schedule_name = data_schedule["name"].as<const char*>(); // "CA", "DO", "KH", "PH"
            for (uint8_t j = 0; j < s_amountReagent; j++) {
              if (strcmp(data_schedule_name, s_listreagent[j]) == 0) {
                JsonArray data_schedule_time = data_schedule["time"];
                size_t numElements = data_schedule_time.size();
                if (numElements > 0) {
                  pond[i].reagentPond[j].active = 1;
                  if (pond[i].reagentPond[j].timeMea != nullptr) {
                    delete[] pond[i].reagentPond[j].timeMea;
                    pond[i].reagentPond[j].timeMea = nullptr;
                  }
                  pond[i].reagentPond[j].timeMea = new uint16_t[numElements + 1];
                  Serial.print(data_schedule_name);
                  Serial.print(" time: " + String(numElements) + " ");
                  for (uint8_t k = 0; k < numElements; k++) {
                    int timeMea = data_schedule_time[k];
                    pond[i].reagentPond[j].timeMea[k] = timeMea;
                    Serial.print(pond[i].reagentPond[j].timeMea[k]);
                    Serial.print(" ");
                  }
                  Serial.println(" ");
                  pond[i].reagentPond[j].timeMea[numElements] = 65534;
                } else {
                  pond[i].reagentPond[j].active = 0;
                  if (pond[i].reagentPond[j].timeMea != nullptr) {
                    delete[] pond[i].reagentPond[j].timeMea;
                    pond[i].reagentPond[j].timeMea = nullptr;
                  }
                }
                break;
              }
            }
          }
          break;
        }
      }
    }
    else {
      for (uint8_t i = 0; i < s_amountPond; i++) {
        if (pond[i].gw == data_gateway_name) {
          pond[i].active = 0;
          if (pond[i].pondID != nullptr) {
            delete[] pond[i].pondID;
          }
          pond[i].pondID = nullptr;
          for (uint8_t j = 0; j < s_amountReagent; j++) {
            pond[i].reagentPond[j].active = 0;
            if (pond[i].reagentPond[j].timeMea != nullptr) {
              delete[] pond[i].reagentPond[j].timeMea;
            }
            pond[i].reagentPond[j].timeMea = nullptr;
          }
          break;
        }
      }
    }
  }
  doc.clear();
  return true;
}
float CLASS_JSONPWQD::Func_DeserializeTime(char *mesrespond) {
  DynamicJsonDocument doc(255);
  DeserializationError error = deserializeJson(doc, mesrespond);
  if (error) {
    return 99999.0;
  }
  float value = 99999.0;
  value = doc["value"].as<float>(); // "619"
  doc.clear();
  return value;
}
char* CLASS_JSONPWQD::Func_SerializeData(char *deviceID, char *pondID , uint8_t gw, float value[]) {
  DynamicJsonDocument doc(1000);
  doc["culture_animal"] = "whiteleg_shrimp";
  doc["device_type"] = "photo_water_qualit";
  doc["message_type"] = "updated_params";
  doc["device_id"] = deviceID;
  doc["pond_id"] = pondID;
  doc["gw_id"] = gw;
  JsonArray water_quality_params = doc.createNestedArray("water_quality_params");
  for (uint8_t i = 0 ; i < s_amountReagent; i++) {
    if (value[i] != 99999.0) {
      JsonObject water_quality_param = water_quality_params.createNestedObject();
      water_quality_param["code"] = s_listreagent[i];
      water_quality_param["value"] = value[i];
    }
  }
  char *mes = new char[1000];
  serializeJson(doc, mes, 1000);
  doc.clear();
  return mes;
}
char* CLASS_JSONPWQD::Func_SerializeStatus(char *state, char *deviceID, uint16_t error) {
  Serial.println("json stt nè");
  Serial.println(state);
  Serial.println(deviceID);
  DynamicJsonDocument doc(255);
  doc["device_type"] = "photo_water_quality";
  doc["message_type"] = "updated_state";
  doc["state"] = state;
  doc["device_id"] = deviceID;
  if (error != 0)
    doc["error_code"] = error;
  uint8_t len = measureJson(doc) + 1;
  char *json = new char[len];
  String output1 = "";
  serializeJson(doc, output1);
  serializeJson(doc, json, len);
  doc.clear();
  Serial.println(output1);
  Serial.println(json);
  return json;
}
char* CLASS_JSONPWQD::Func_SerializeCommand(char *command, char *deviceID) {
  Serial.println("command");
  DynamicJsonDocument doc(255);
  doc["device_type"] = "photo_water_quality";
  doc["message_type"] = "command_complete";
  doc["device_id"] = deviceID;
  doc["command"] = command;
  uint8_t len = measureJson(doc) + 1;
  char *json = new char[len];
  serializeJson(doc, json, len);
  doc.clear();
  Serial.println(json);
  return json;
}
char* CLASS_JSONPWQD::Func_SerializeRequestTime(char *deviceID) {
  Serial.println("request time");
  DynamicJsonDocument doc(255);
  doc["device_type"] = "photo_water_quality";
  doc["message_type"] = "request_time";
  doc["device_id"] = deviceID;
  uint8_t len = measureJson(doc) + 1;
  char *json = new char[len];
  serializeJson(doc, json, len);
  doc.clear();
  Serial.println(json);
  return json;
}
