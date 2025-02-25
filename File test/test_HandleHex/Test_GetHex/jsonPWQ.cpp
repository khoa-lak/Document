#include "jsonPWQ.h"
ParamPWQ paramPWQ;
void JsonPWQ::derSettingDevice(char *mes, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR], SettingDevice settingDevice) {
  DynamicJsonDocument data(3000);
  DeserializationError error = deserializeJson(data, mes);
  if (error) {
    return ;
  }
  JsonObject data_settings = data["data"];
  settingDevice.tempCeiling = data_settings["cooling_ceiling"];
  settingDevice.tempCeiling = data_settings["cooling_floor"];
  JsonArray data_ponds = data_settings["ponds"].as<JsonArray>();
  uint8_t numPond = data_ponds.size();
  //lấy setting ao
  for (uint8_t i = 0 ; i < numPond && i < data_ponds.size(); i++) {
    if (data_ponds[i]["pond_id"] != nullptr) {
      uint8_t lengthPondId = strlen(data_ponds[i]["pond_id"]) + 1;
      if (paramPond[i].id != nullptr)
        delete[] paramPond[i].id;
      paramPond[i].id = new char[lengthPondId];
      strcpy(paramPond[i].id, data_ponds[i]["pond_id"]);
      paramPond[i].id[lengthPondId - 1] = '\0';
    }
    if (data_ponds[i]["pond_module_id"] != nullptr) {
      uint8_t lengthModulePondId = strlen(data_ponds[i]["pond_module_id"]) + 1;
      if (paramPond[i].moduleId != nullptr)
        delete[] paramPond[i].moduleId;
      paramPond[i].moduleId = new char[lengthModulePondId];
      strcpy(paramPond[i].moduleId, data_ponds[i]["pond_module_id"]);
      paramPond[i].moduleId[lengthModulePondId - 1] = '\0';
    }
    if (data_ponds[i]["pond_name"] != nullptr) {
      uint8_t lengthPondName = strlen(data_ponds[i]["pond_name"]) + 1;
      if (paramPond[i].pondName != nullptr)
        delete[] paramPond[i].pondName;
      paramPond[i].pondName = new char[lengthPondName];
      strcpy(paramPond[i].pondName, data_ponds[i]["pond_name"]);
      paramPond[i].pondName[lengthPondName - 1] = '\0';
    }
    if (strstr(data_ponds[i]["status"] ,  paramPWQ.mesOn) || strstr(data_ponds[i]["status"] ,  paramPWQ.mesWarning)) {
      if (strstr(data_ponds[i]["status"] ,  paramPWQ.mesOn)) {
        paramPond[i].errCode = 0;
        for (uint8_t j = 0; j < NUM_SENSOR; j++) {
          paramSensor[j].errCode[i] = 0;
        }
      }
      paramPond[i].enable = true;
    }
    else if (strstr(data_ponds[i]["status"] ,  paramPWQ.mesOff) || strstr(data_ponds[i]["status"] ,  paramPWQ.mesError)) {
      //if (strstr(data_ponds[i]["status"] ,  paramPWQ.mesOff)) {
      //paramPond[i].errCode = 0;
      //        for (uint8_t j = 0; j < NUM_SENSOR; j++) {
      //          paramPond[i].paramSensor[j].err = 0;
      //        }
      //}
      paramPond[i].enable = false;
    }
    paramPond[i].distance = data_ponds[i]["distance"];
    paramPond[i].diameter = data_ponds[i]["pipe_dimen"];
  }
  //lấy setting reagent
  for (JsonObject data_reagent : data_settings["water_quality_params"].as<JsonArray>()) {
    const char* nameReagent = data_reagent["code"];
    bool enable =  data_reagent["enable"];
    for (uint8_t i = 0; i < NUM_REAGENT; i++) {
      if (strstr(nameReagent, paramReagent[i].nameReagent)) {
        paramReagent[i].enable = enable;
        break;
      }
    }
    for (uint8_t i = 0; i < NUM_SENSOR; i++) {
      if (strstr(nameReagent, paramSensor[i].nameSensor)) {
        paramSensor[i].enable = enable;
        break;
      }
    }
    //    for (uint8_t i = 0; i < NUM_POND; i++) {
    //      if (!paramPond[i].enable)
    //        continue;
    //      for (uint8_t j = 0; j < NUM_SENSOR; j++) {
    //        if (strstr(nameReagent, paramPond[i].paramSensor[j].nameSensor)) {
    //          paramPond[i].paramSensor[j].enable = enable;
    //          break;
    //        }
    //      }
    //    }
  }
  //data.clear();
  return;
}
void JsonPWQ::derSettingPond(char* mes, char*& pondSche) {
  uint8_t order = 0;
  DynamicJsonDocument doc(2000);
  DeserializationError error = deserializeJson(doc, (char*)mes);
  if (error) {
    //    Serial.print("deserializeJson() failed: ");
    //    Serial.println(error.c_str());
    return ;
  }
  uint16_t lengthSche = strlen(mes) + 1;
  if (pondSche != nullptr)
    delete[] pondSche;
  pondSche = new char[lengthSche];
  strcpy(pondSche, mes);
  //doc.clear();
  return;
}
uint8_t JsonPWQ::derSchedule(char* mes, uint8_t* data, char* day, uint32_t currentTime, ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR]) {
  uint8_t order = 0;
  DynamicJsonDocument doc(7000);
  DeserializationError error = deserializeJson(doc, mes);
  if (error) {
    return 0;
  }

  uint8_t timeCheck = 400;
  Serial.println("time hiện tại: " + String(currentTime) + "hôm nay là thứ: " + day);
  Serial.println("chất là: ");
  for (JsonObject data_reagent : doc["data"]["schedules"].as<JsonArray>()) {
    uint8_t orderReagent = 99;
    const char* nameReagent = data_reagent["name"];
    if (nameReagent == nullptr)
      continue;
    for (uint8_t i = 0; i < NUM_REAGENT; i++) {
      if (strstr(nameReagent, paramReagent[i].nameReagent) != nullptr) {
        orderReagent = i;
        break;
      }
    }
    if (orderReagent == 99) {
      for (uint8_t i = 0; i < NUM_SENSOR; i++) {
        if (strstr(nameReagent, paramSensor[i].nameSensor) != nullptr) {
          orderReagent = NUM_REAGENT - 1;
          break;
        }
      }
    }
    if (!paramReagent[orderReagent].enable || orderReagent == 99) {
      orderReagent = 99;
      continue;
    }
    Serial.println(nameReagent);
    Serial.println(orderReagent);
    const char* type = data_reagent["time"]["type"];
    //    if (strstr(type, "daily") != nullptr) {
    for (JsonVariant  data_time : data_reagent["time"]["seconds"].as<JsonArray>()) {
      uint32_t value = data_time;
      if (currentTime < value)
        continue;
      uint32_t delta =  currentTime - value;
      if (delta < timeCheck) {
        Serial.print(" " + String(value) + "-" + String(orderReagent) + " ");
        data[order] = orderReagent;
        order++;
        break;
      }
    }
    //    }
    //    else if (strstr(type, "weekly") != nullptr) {
    //      for (JsonVariant data_days : data_reagent["time"]["days"].as<JsonArray>()) {
    //        const char* d = data_days;
    //        if (strstr(d, day) != nullptr) {
    //          uint32_t tim = data_reagent["time"]["seconds"];
    //          if (currentTime < tim)
    //            continue;
    //          uint32_t delta =  currentTime - tim;
    //          if (delta < timeCheck) {
    //            Serial.print(" " + String(tim) + "-" + String(orderReagent) + " ");
    //            data[order] = orderReagent;
    //            order++;
    //            break;
    //          }
    //        }
    //      }
    //    }
  }
  //doc.clear();
  return order;
}
uint32_t JsonPWQ::derGetTime(char *mes) {
  DynamicJsonDocument doc(255);
  DeserializationError error = deserializeJson(doc, mes);
  if (error) {
    return 0;
  }
  uint32_t value = doc["value"]; // "619"
  //doc.clear();
  return value;
}
uint16_t JsonPWQ::derMeasureNow(char *mes, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR]) {
  DynamicJsonDocument doc(lengthMesMid);
  DeserializationError error = deserializeJson(doc, mes);
  if (error) {
    return 9999;
  }
  const char* nameReagent =  doc["payload"]["param"];
  const char* pondID = doc["payload"]["pondId"];
  for (uint8_t i = 0 ; i < NUM_POND; i++) {
    if (strstr(pondID, paramPond[i].id) && paramPond[i].enable) {
      for (uint8_t j = 0 ; j < NUM_REAGENT; j++) {
        if (strstr(nameReagent, paramReagent[j].nameReagent) && paramReagent[j].enable) {
          uint16_t value = j * 10 + i;
          return value;
        }
      }
      for (uint8_t j = 0 ; j < NUM_SENSOR; j++) {
        if (strstr(nameReagent, paramSensor[j].nameSensor) && paramSensor[j].enable) {
          uint16_t value = (NUM_REAGENT - 1) * 10 + i;
          return value;
        }
      }
    }
  }
  return 9999;
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
char* JsonPWQ::serMesSateErrorCheck(uint16_t errCode) {
  uint16_t len = lengthMesMin;
  DynamicJsonDocument doc(len);
  doc["device_type"] = paramPWQ.deviceType;
  doc["message_type"] = "update_state";
  doc["device_id"] = paramPWQ.deviceID;
  doc["state"] = paramPWQ.mesErrorCheck;
  doc["payload"]["err_code"] = errCode;
  char *mes = new char[len];
  serializeJson(doc, mes, len);
  //doc.clear();
  return mes;
}
char* JsonPWQ::serMesSate(uint16_t errCode) {
  uint16_t len = lengthMesMin;
  DynamicJsonDocument doc(len);
  doc["device_type"] = paramPWQ.deviceType;
  doc["message_type"] = "update_state";
  doc["device_id"] = paramPWQ.deviceID;
  doc["state"] = paramPWQ.mesError;
  doc["payload"]["err_code"] = errCode;
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
char* JsonPWQ::serMesSate(char *title, char *subtitle, uint32_t startTime, uint16_t totalTime, float temp, uint8_t pond, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR], uint16_t *dataQueue, uint8_t numQueue, uint32_t freeMemory, char* testID) {
  uint16_t len = 5000;
  DynamicJsonDocument doc(len);
  doc["free_memory"] = freeMemory;
  doc["device_type"] = paramPWQ.deviceType;
  doc["message_type"] = "update_state";
  doc["test_id"] = testID;
  doc["device_id"] = paramPWQ.deviceID;
  doc["pond_id"] = paramPond[pond].id;
  doc["state"] = paramPWQ.mesRun;
  JsonObject payload = doc["payload"].to<JsonObject>();
  payload["title"] = title;
  payload["subtitle"] = subtitle;
  payload["start_time"] = startTime;
  payload["total_time"] = totalTime;
  payload["temperature"] = temp;
  JsonArray statusPond = doc.createNestedArray("ponds");
  for (uint8_t i = 0 ; i < NUM_POND ; i++) {
    if (!paramPond[i].enable && paramPond[i].errCode == 0)
      continue;
    JsonObject listPond = statusPond.createNestedObject();
    listPond["pond_module_id"] = paramPond[i].moduleId;
    JsonArray listErrCode = listPond.createNestedArray("error_code");
    if (paramPond[i].errCode != 0)
      listErrCode.add(paramPond[i].errCode);
    for (uint8_t j = 0 ; j < NUM_SENSOR ; j++) {
      if (paramSensor[i].errCode[i] != 0)
        listErrCode.add(paramSensor[i].errCode[i]);
    }
    uint8_t numErr  = listErrCode.size();
    if (paramPond[i].enable) {
      if (numErr == 0)
        listPond["status"] = paramPWQ.mesOn;
      else
        listPond["status"] = paramPWQ.mesWarning;
    } else
      listPond["status"] = paramPWQ.mesError;
  }
  JsonObject moduleSensor = doc["module_sensor"].to<JsonObject>();
  JsonArray listErrCodeModule = moduleSensor.createNestedArray("error_code");
  for (uint8_t j = 0 ; j < NUM_SENSOR ; j++) {
    if (paramSensor[j].errCode[NUM_POND] != 0)
      listErrCodeModule.add(paramSensor[j].errCode[NUM_POND]);
  }
  JsonArray listQueue = doc.createNestedArray("working_queue");
  for (uint8_t i = 0 ; i < numQueue ; i++) {
    uint8_t reagent = dataQueue[i] / 100;
    uint8_t pond =  dataQueue[i] / 10 - reagent * 10;
    uint8_t type =  dataQueue[i] - reagent * 100 - pond * 10;
    // Serial.println("chất: " + String(dataQueue[i]) + " " + String(reagent) + " " + String(pond) + " " + String(type));
    if (reagent == 6)
      continue;
    JsonObject list = listQueue.createNestedObject();
    list["code"] = paramReagent[reagent].nameReagent;
    list["pond_id"] = paramPond[pond].id;
    list["pond_name"] = paramPond[pond].pondName;
    if (type == 1)
      list["type"] = "measure_now";
    else if (type == 0)
      list["type"] = "scheduled";
  }
  char *mes = new char[len];
  serializeJson(doc, mes, len);
  //doc.clear();///////////////note lại
  return mes;
}
char* JsonPWQ::serMesData(char *pondID, char* nameReagent, float value,  uint16_t* led_pre, uint16_t* led_af, uint16_t* led_preArray, uint16_t* led_afArray, char* testID) {
  uint16_t len = 3000;
  DynamicJsonDocument doc(len);
  doc["device_type"] = paramPWQ.deviceType;
  doc["message_type"] = "measure_completed";
  doc["device_id"] = paramPWQ.deviceID;
  doc["pond_id"] =  pondID;
  doc["test_id"] = testID;
  JsonArray water_quality_params = doc.createNestedArray("water_quality_params");
  JsonObject water_quality_param1 = water_quality_params.createNestedObject();
  water_quality_param1["code"] = nameReagent;
  water_quality_param1["value"] = value;
  water_quality_param1["source"] = "reagent";
  //  JsonArray data_led = doc.createNestedArray("leds");
  //  JsonObject ledpre = data_led.createNestedObject();
  //  JsonArray ledPreFinal = ledpre["led_pre"].createNestedArray("final");
  //  for (uint8_t i = 0 ; i < 6; i++) {
  //    ledPreFinal.add(led_pre[i]);
  //  }
  //  uint8_t order = 0;
  //  JsonArray ledPreRaw = ledpre["led_pre"].createNestedArray("raw_values");
  //  for (uint8_t i = 0 ; i < 6; i++) {
  //    JsonArray ledPreRawOrder = ledPreRaw.createNestedArray();
  //    for (uint8_t j = 0 ; j < 10; j++) {
  //      ledPreRawOrder.add(led_preArray[order]);
  //      order++;
  //    }
  //  }
  //  JsonObject ledAf = data_led.createNestedObject();
  //  JsonArray ledAfFinal = ledAf["led_af"].createNestedArray("final");
  //  for (uint8_t i = 0 ; i < 6; i++) {
  //    ledAfFinal.add(led_af[i]);
  //  }
  //  order = 0;
  //  JsonArray ledAfRaw = ledAf["led_af"].createNestedArray("raw_values");
  //  for (uint8_t i = 0 ; i < 6; i++) {
  //    JsonArray ledAfRawOrder = ledAfRaw.createNestedArray();
  //    for (uint8_t j = 0 ; j < 10; j++) {
  //      ledAfRawOrder.add(led_afArray[order]);
  //      order++;
  //    }
  //  }
  char *json = nullptr;
  json = new char[len];
  serializeJson(doc, json, len);
  //doc.clear();
  return json;
}
char* JsonPWQ::jsonDataLed(char* nameReagent, float value, uint8_t pond, uint16_t* led_pre, uint16_t* led_af) {
  const uint16_t lenght = 500;
  DynamicJsonDocument doc(lengthMesMin);
  doc["device_type"] = "pwq";
  doc["message_type"] = "measure_completed_test";
  doc["device_id"] = "pwq-b9002e2048fe44e594c51535bf93d770";
  if (pond == 1)
    doc["pond_id"] = "pond-a2880ae527b6449b974bced274e4304c";  //ao nc pha
  else if (pond == 2)
    doc["pond_id"] = "pond-edf1799d6a5142f5aa4c6eb6bef2725b";  //ao nc nha
  doc["gw_name"] = 4;
  JsonArray water_quality_params = doc.createNestedArray("water_quality_params");
  //Serial.print("vị trí: ");
  JsonObject water_quality_param = water_quality_params.createNestedObject();
  water_quality_param["led"] = nameReagent;
  water_quality_param["value"] = value;
  for (uint8_t i = 0 ; i < 6; i++) {
    JsonObject water_quality_param = water_quality_params.createNestedObject();
    water_quality_param["led"] = i + 1;
    water_quality_param["value"] = led_pre[i];
  }
  for (uint8_t i = 0 ; i < 6; i++) {
    JsonObject water_quality_param = water_quality_params.createNestedObject();
    water_quality_param["led"] = i + 7;
    water_quality_param["value"] = led_af[i];
  }


  char *mes = new char[2000];
  serializeJson(doc, mes, 2000);
  //doc.clear();
}

char* JsonPWQ::serMesLoraSensor(char* moduleId) {
  DynamicJsonDocument doc(lengthMesMin);
  doc["pond_module_id"] = moduleId;
  doc["message_type"] = "get_value";
  //  JsonArray payload = doc.createNestedArray("payload");
  //  for (uint8_t i = 0 ; i < NUM_REAGENT ; i++) {
  //    JsonObject params = payload.createNestedObject();
  //    params["code"] = paramPond.paramSensor[i].nameSensor;
  //    params["enable"] = paramPond.paramSensor[i].enable;
  //  }
  char *msg = new char[lengthMesMin];
  serializeJson(doc, msg, lengthMesMin);
  //doc.clear();
  return msg;
}
bool JsonPWQ::derMesReceiveSensor(char* msg, char* moduleId) {
  //  Serial.println("3: ");
  //  Serial.println(msg);
  DynamicJsonDocument doc(lengthMesMid);
  DeserializationError error = deserializeJson(doc, msg);
  if (error) {
    return false;
  }
  if (!strstr(doc["mst"], "gvc")) {
    return false;
  }
  //  for ( JsonObject payload : doc["pl"].as<JsonArray>()) {
  //    const char* nameSensor = payload["code"];
  //    for (uint8_t i = 0; i < NUM_SENSOR ; i++) {
  //      if (strstr(nameSensor, paramPond.paramSensor[i].nameSensor)) {
  //        if (!paramPond.paramSensor[i].enable )
  //          break;
  //        paramPond.paramSensor[i].err = payload["err"];
  //        paramPond.paramSensor[i].value = payload["value"];
  //        paramPond.paramSensor[i].temp = payload["temp"];
  //      }
  //    }
  //  }
  return true;
}
char* JsonPWQ::serMesLoraPump(bool enable, ParamPond paramPond) {
  DynamicJsonDocument doc(lengthMesMin);
  doc["pond_module_id"] = paramPond.moduleId;
  doc["message_type"] = "control_pump";
  doc["enable"] = enable;
  doc["distance"] = paramPond.distance;
  char *msg = new char[lengthMesMin];
  serializeJson(doc, msg, lengthMesMin);
  //doc.clear();
  return msg;
}
bool JsonPWQ::derMesReceivePump(char *msg, ParamPond paramPond) {
  //  Serial.println("3: ");
  //  Serial.println(msg);
  DynamicJsonDocument doc(lengthMesMin);
  DeserializationError error = deserializeJson(doc, msg);
  if (error) {
    return false;
  }
  if (!strstr(doc["pond_module_id"], paramPond.moduleId) || !strstr(doc["message_type"], "control_pump_complete")) {
    return false;
    //doc.clear();
  }
  //doc.clear();
  return true;
}
char* JsonPWQ::serValueSensor(char* mesData, uint8_t module, ParamPond paramPond,  ParamSensor paramSensor[NUM_SENSOR]) {
  DynamicJsonDocument data(1500);
  DeserializationError error = deserializeJson(data, mesData);
  if (error) {
    return nullptr;
  }
  uint16_t len = 1500;
  DynamicJsonDocument doc(len);
  doc["device_type"] = paramPWQ.deviceType;
  doc["message_type"] = "measure_completed";
  doc["device_id"] = paramPWQ.deviceID;
  doc["pond_id"] =  paramPond.id;
  doc["pond_name"] =  paramPond.pondName;
  JsonArray water_quality_params = doc.createNestedArray("water_quality_params");
  Serial.println("module số: " + String(module));
  for ( JsonVariant payload : data["pl"].as<JsonArray>()) {
    const char* mesPayload = payload.as<const char*>();
    char nameSensor[10];
    char nameSource[30];
    uint8_t errSensor = 0;
    char valueTemp[5];
    float value = 0.0f;
    uint8_t order = 0;
    Serial.println("vào lấy giá trị: ");
    if (sscanf(mesPayload, "%9[^/]/%u/%29[^/]/%9s/", nameSensor, &errSensor, nameSource, valueTemp) == 4)
      value = atof(valueTemp);
    Serial.println(nameSensor);
    Serial.println(errSensor);
    Serial.println(nameSource);
    Serial.println(value);
    for (uint8_t i = 0 ; i < NUM_SENSOR; i++) {
      if (!strstr(paramSensor[i].nameSensor, nameSensor))
        continue;
      if (paramSensor[i].enable) {
        if (errSensor == 1) {
          paramSensor[i].errCode[module]  = paramSensor[i].listErrCode[0];
          Serial.println("lỗi: " + String(paramSensor[i].errCode[module]));
          break;
        }
        JsonObject water_quality_param1 = water_quality_params.createNestedObject();
        water_quality_param1["code"] = nameSensor;
        water_quality_param1["source"] = nameSource;
        water_quality_param1["value"] = value;//float(uint16_t(value * 100) / 100);
        break;
      }
    }
  }
  char *json = new char[len];
  serializeJson(doc, json, len);
  return json;
}
char* JsonPWQ::serTurnOnModuleSensor(char* moduleId) {
  DynamicJsonDocument doc(lengthMesMin);
  doc["pond_module_id"] = moduleId;
  doc["message_type"] = "turn_on";
  char *msg = new char[lengthMesMin];
  serializeJson(doc, msg, lengthMesMin);
  //doc.clear();
  return msg;
}
char* JsonPWQ::serLEDWater(uint16_t *dataLED , uint16_t *dataArray) {
  uint16_t len = 2000;
  DynamicJsonDocument doc(len);
  JsonArray data_led = doc.createNestedArray("LED_CAL");
  for (uint8_t i = 0 ; i < 6; i++) {
    JsonObject data_led_led = data_led .createNestedObject();
    data_led_led["led"] = i + 1;
    data_led_led["value"] = dataLED[i];
  }
  uint8_t order = 0;
  JsonArray data_led1 = doc.createNestedArray("LED_ARRAY");
  for (uint8_t i = 0 ; i < 6; i++) {
    JsonObject data_led_led = data_led1.createNestedObject();
    data_led_led["led"] = i + 1;
    JsonArray payload = data_led_led["value"].to<JsonArray>();
    for (uint8_t j = 0 ; j < 10; j++) {
      payload.add(dataArray[order]);
      order++;
    }
  }
  char *msg = new char[len];
  serializeJson(doc, msg, len);
  //doc.clear();
  return msg;
}
char* JsonPWQ::sendValueLed(char* test, char* mission, uint16_t* ledData, uint16_t *dataArray) {
  uint16_t len = 2000;
  DynamicJsonDocument doc(len);
  doc["device_type"] = paramPWQ.deviceType;
  doc["message_type"] = "check_led";
  doc["test_id"] = test;
  doc["device_id"] = paramPWQ.deviceID;
  doc["process"] = mission;
  JsonArray data_led = doc.createNestedArray("leds");
  JsonObject ledpre = data_led.createNestedObject();
  JsonArray ledPreFinal = ledpre.createNestedArray("final");
  for (uint8_t i = 0 ; i < 6; i++) {
    ledPreFinal.add(ledData[i]);
  }
  uint8_t order = 0;
  JsonArray ledPreRaw = ledpre.createNestedArray("raw_values");
  for (uint8_t i = 0 ; i < 6; i++) {
    JsonArray ledPreRawOrder = ledPreRaw.createNestedArray();
    for (uint8_t j = 0 ; j < 10; j++) {
      ledPreRawOrder.add(dataArray[order]);
      order++;
    }
  }
  char *json = nullptr;
  json = new char[len];
  serializeJson(doc, json, len);
  //doc.clear();
  return json;
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
