#include "JsonPWQ.h"
uint16_t JsonPWQ::lengthToAllocation(const char *js) {
  return 2 * strlen(js) + HEADER_LENGTH;
}
bool JsonPWQ::isJon(const char *js) {
  char *js_to_check = new char[strlen(js)];
  strcpy(js_to_check, js);
  DynamicJsonDocument doc(lengthToAllocation(js_to_check));
  DeserializationError error = deserializeJson(doc, js_to_check);
  if (js_to_check != nullptr) {
    delete[] js_to_check;
    js_to_check = nullptr;
  }
  if (error) {
    return false;
  }
  return true;
}
bool JsonPWQ::handleMsgSettingDevice(const char *msg, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR]) {
  DynamicJsonDocument data(lengthToAllocation(msg));
  DeserializationError error = deserializeJson(data, msg);
  if (error) {
    return false;
  }
  JsonObject data_settings = data["data"];
  JsonArray data_ponds = data_settings["ponds"].as<JsonArray>();
  uint8_t numPond = data_ponds.size();
  //GET SETTING PONDS
  for (uint8_t i = 0; i < NUM_POND || i < data_ponds.size(); i++) {
    bool ph_enable = data_ponds[i]["ph_enable"] | false;
    bool salinity_enable = data_ponds[i]["salinity_enable"] | false;
    bool do_enable = data_ponds[i]["do_enable"] | false;
    bool temp_enable = data_ponds[i]["temp_enable"] | false;
    paramSensor[0].enable_each_pond[i] = ph_enable;
    paramSensor[1].enable_each_pond[i] = salinity_enable;
    paramSensor[2].enable_each_pond[i] = do_enable;
    paramSensor[3].enable_each_pond[i] = temp_enable;
    const char *pond_id = data_ponds[i]["pond_id"];
    const char *pond_module_id = data_ponds[i]["module_id"];
    const char *pond_status = data_ponds[i]["status"];
    const char *communication_type = data_ponds[i]["communication_type"];
    if (!pond_id || !pond_module_id) {
      paramPond[i].enable = false;
      continue;
    }
    generalFunction.wrapperPointer(paramPond[i].pond_id, pond_id);
    generalFunction.wrapperPointer(paramPond[i].pond_module_id, pond_module_id);
    generalFunction.wrapperPointer(paramPond[i].communication_type, communication_type);
    if (generalFunction.compareTwoCharArray(pond_status, STATUS_ON) || generalFunction.compareTwoCharArray(pond_status, STATUS_WARNING)) {
      paramPond[i].err = false;
      paramPond[i].enable = true;
    } else if (generalFunction.compareTwoCharArray(pond_status, STATUS_ERROR)) {
      paramPond[i].err = true;
      paramPond[i].enable = true;
    } else if (generalFunction.compareTwoCharArray(pond_status, STATUS_OFF)) {
      paramPond[i].err = false;
      paramPond[i].enable = false;
    }
    paramPond[i].pump_duration = data_ponds[i]["pump_duration"];
  }
  //GET SETTING REAGENTS
  for (JsonObject data_reagent : data_settings["water_quality_params"].as<JsonArray>()) {
    const char *name_reagent = data_reagent["code"];
    if (name_reagent == nullptr) continue;
    bool enable = data_reagent.containsKey("enable") ? data_reagent["enable"].as<bool>() : false;
    uint8_t max_test = data_reagent["reagent"].containsKey("max_test") ? data_reagent["reagent"]["max_test"].as<uint8_t>() : 0;
    uint8_t current_count = data_reagent["reagent"].containsKey("current_count") ? data_reagent["reagent"]["current_count"].as<uint8_t>() : 0;
    for (uint8_t i = 0; i < NUM_REAGENT; i++) {
      if (generalFunction.compareTwoCharArray(name_reagent, paramReagent[i].name_reagent)) {
        Serial.print(name_reagent);
        Serial.print(": ");
        paramReagent[i].enable = enable;
        paramReagent[i].max_test = max_test;
        paramReagent[i].current_count = current_count;
        if (data_reagent["calib_results"].containsKey("leds")) {
          Serial.print("[leds: ");
          JsonArray data_leds = data_reagent["calib_results"]["leds"];
          paramReagent[i].paramCalib.num_index = data_leds.size();
          if (paramReagent[i].paramCalib.leds != nullptr)
            delete[] paramReagent[i].paramCalib.leds;
          paramReagent[i].paramCalib.leds = new uint8_t[paramReagent[i].paramCalib.num_index];
          for (uint8_t j = 0; j < paramReagent[i].paramCalib.num_index; j++) {
            paramReagent[i].paramCalib.leds[j] = data_reagent["calib_results"]["leds"][j];
            Serial.print(String(paramReagent[i].paramCalib.leds[j]) + ", ");
          }
          Serial.print("] - ");
        }
        if (data_reagent["calib_results"].containsKey("coefs")) {
          Serial.print("[coefs: ");
          JsonArray data_coefs = data_reagent["calib_results"]["coefs"];
          if (paramReagent[i].paramCalib.coefs != nullptr)
            delete[] paramReagent[i].paramCalib.coefs;
          paramReagent[i].paramCalib.coefs = new float[paramReagent[i].paramCalib.num_index];
          for (uint8_t j = 0; j < paramReagent[i].paramCalib.num_index; j++) {
            paramReagent[i].paramCalib.coefs[j] = data_reagent["calib_results"]["coefs"][j];
            Serial.print(String(paramReagent[i].paramCalib.coefs[j]) + ", ");
          }
          Serial.print("] - ");
        }
        if (data_reagent["calib_results"].containsKey("inters")) {
          paramReagent[i].paramCalib.inters = data_reagent["calib_results"]["inters"][0];
          Serial.println("[inters: " + String(paramReagent[i].paramCalib.inters) + "]");
        }
        break;
      }
    }
    for (uint8_t i = 0; i < NUM_SENSOR; i++) {
      if (generalFunction.compareTwoCharArray(name_reagent, paramSensor[i].name_sensor)) {
        Serial.print(name_reagent);
        if (enable)
          Serial.println(": true");
        else
          Serial.println(": false");
        paramSensor[i].has_in_machine = enable;
        break;
      }
    }
  }
  return true;
}
void JsonPWQ::handleMsgSettingPond(char *msg, ParamPond paramPond[NUM_POND]) {
  DynamicJsonDocument doc(lengthToAllocation(msg));
  DeserializationError error = deserializeJson(doc, msg);
  if (error) {
    return;
  }
  const char *pond_id = doc["pond_id"];
  for (uint8_t i = 0; i < NUM_POND; i++) {
    if (generalFunction.compareTwoCharArray((char *)pond_id, paramPond[i].pond_id)) {
      generalFunction.wrapperPointer(paramPond[i].sche, msg);
    }
  }
  return;
}
void JsonPWQ::handleMsgMeasureNow(const char *msg, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR], ListReagentNow &listReagentNow) {
  DynamicJsonDocument doc(lengthToAllocation(msg));
  DeserializationError error = deserializeJson(doc, msg);
  if (error) {
    listReagentNow.num_reagent_now = 0;
    return;
  }
  const char *pond_id = doc["payload"]["pondId"];
  const char *test_type = doc["payload"]["test_type"];
  const char *test_id = doc["payload"]["test_id"];
  Serial.println(pond_id);
  Serial.println(test_type);
  Serial.println(test_id);
  if (pond_id) {
    for (uint8_t i = 0; i < NUM_POND; i++)
      if (generalFunction.compareTwoCharArray(pond_id, paramPond[i].pond_id) && paramPond[i].enable)
        listReagentNow.pond = i;
  } else
    listReagentNow.pond = -1;
  if (test_type)
    generalFunction.wrapperPointer(listReagentNow.test_type, test_type);
  if (test_id)
    generalFunction.wrapperPointer(listReagentNow.test_id, test_id);
  listReagentNow.point_index = doc["payload"]["calib_point_index"] | 0;
  uint8_t index = 0;
  Serial.print("list chất: ");
  if (doc["payload"]["params"].is<JsonArray>()) {
    for (const JsonVariant &name : doc["payload"]["params"].as<JsonArray>()) {
      if (!name.is<const char *>()) {
        continue;
      }
      const char *name_reagent = name.as<const char *>();
      for (uint8_t i = 0; i < NUM_REAGENT; i++) {
        if (generalFunction.compareTwoCharArray(name, paramReagent[i].name_reagent) && paramReagent[i].enable) {
          listReagentNow.list_reagent[index] = i;
          index++;
          Serial.print(paramReagent[i].name_reagent);
          Serial.print(" ");
          break;
        }
      }
      for (uint8_t i = 0; i < NUM_SENSOR; i++) {
        if (generalFunction.compareTwoCharArray(name, paramSensor[i].name_sensor) && paramSensor[i].has_in_machine) {
          listReagentNow.list_reagent[index] = NUM_REAGENT + i;
          index++;
          Serial.print(paramSensor[i].name_sensor);
          Serial.print(" ");
          break;
        }
      }
    }
  }
  Serial.println("");
  listReagentNow.num_reagent_now = index;
  return;
}

bool JsonPWQ::handleMsgSchedule(const char *msg, int8_t *data, uint32_t current_time, ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR]) {
  current_time = current_time;
  DynamicJsonDocument doc(lengthToAllocation(msg));
  DeserializationError error = deserializeJson(doc, msg);
  if (error) {
    return false;
  }
  if (!doc["data"].containsKey("schedules") || !doc["data"]["schedules"].is<JsonArray>()) {
    return false;
  }
  constexpr uint32_t TIME_CHECK = 300;  // Time window in seconds
  size_t order = 0;
  Serial.println("time hiện tại: " + String(current_time));
  Serial.println("chất là: ");
  for (JsonObject data_reagent : doc["data"]["schedules"].as<JsonArray>()) {
    const char *name = data_reagent["name"];
    // Serial.println(name);
    if (name == nullptr)
      continue;
    uint8_t index = NUM_REAGENT + NUM_SENSOR;
    for (uint8_t i = 0; i < NUM_REAGENT; i++) {
      if (generalFunction.compareTwoCharArray(name, paramReagent[i].name_reagent) && paramReagent[i].enable) {
        index = i;
        break;
      }
    }
    if (index == NUM_REAGENT + NUM_SENSOR) {
      // Serial.print("vào lấy sensor: ");
      for (uint8_t i = 0; i < NUM_SENSOR; i++) {
        // Serial.print(paramSensor[i].name_sensor);
        if (generalFunction.compareTwoCharArray(name, paramSensor[i].name_sensor) && paramSensor[i].has_in_machine) {
          // Serial.println(": ok");
          index = NUM_REAGENT + i;
          break;
        }
        // Serial.println(": no ok");
      }
    }
    if (index == NUM_REAGENT + NUM_SENSOR) {
      continue;
    }
    if (!data_reagent["time"].containsKey("seconds") || !data_reagent["time"]["seconds"].is<JsonArray>()) {
      continue;
    }
    Serial.print(name);
    Serial.print(" " + String(index) + ": ");
    for (const JsonVariant &time : data_reagent["time"]["seconds"].as<JsonArray>()) {
      if (!time.is<uint32_t>()) {
        continue;
      }
      uint32_t value = time.as<uint32_t>();
      if (current_time < value)
        continue;
      if (current_time >= value && (current_time - value) < TIME_CHECK) {
        Serial.print(" " + String(value) + "-" + String(index) + " ");
        data[order] = index;
        order++;
        break;
      }
    }
    Serial.println("");
  }
  return order > 0;
}

char *JsonPWQ::msgStateMachine(char *title, uint32_t unix_time, uint16_t total_time, float temp, int8_t pond, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR], uint8_t param_queue[][3], uint8_t num_queue, char *test_id, const char *state) {
  uint16_t len = MSG_MAX;
  DynamicJsonDocument doc(len);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "update_state";
  doc["device_id"] = DEVICE_ID;
  if (test_id)
    doc["test_id"] = test_id;
  if (pond != -1)
    doc["pond_id"] = paramPond[pond].pond_id;
  doc["state"] = state;
  JsonObject payload = doc["payload"].to<JsonObject>();
  payload["title"] = title;
  payload["start_time"] = unix_time;
  payload["total_time"] = total_time;
  payload["temperature"] = temp;
  JsonArray statusPond = doc.createNestedArray("ponds");
  for (uint8_t i = 0; i < NUM_POND; i++) {
    if (!paramPond[i].enable && paramPond[i].err_code == 0)
      continue;
    JsonObject listPond = statusPond.createNestedObject();
    listPond["pond_module_id"] = paramPond[i].pond_module_id;
    listPond["pond_id"] = paramPond[i].pond_id;
    JsonArray listErrCode = listPond.createNestedArray("error_code");
    if (paramPond[i].err && paramPond[i].err_code != 0) {
      listErrCode.add(paramPond[i].err_code);
    }
    for (uint8_t j = 0; j < NUM_SENSOR; j++) {
      if (paramSensor[j].enable_each_pond[i] && paramSensor[j].err_each_pond[i] != -1)
        listErrCode.add(paramSensor[j].err_each_pond[i]);
    }
    uint8_t num_err = listErrCode.size();
    if (paramPond[i].enable) {
      if (paramPond[i].err)
        listPond["status"] = STATUS_ERROR;
      else {
        if (num_err == 0)
          listPond["status"] = STATUS_ON;
        else
          listPond["status"] = STATUS_WARNING;
      }
    } else {
      listPond["status"] = STATUS_OFF;
    }
  }
  JsonArray listInfoSensorMachine = doc.createNestedArray("sensor_in_machine");
  for (uint8_t j = 0; j < NUM_SENSOR; j++) {
    if (paramSensor[j].has_in_machine && paramSensor[j].err_in_machine) {
      JsonObject list = listInfoSensorMachine.createNestedObject();
      list["code"] = paramSensor[j].name_sensor;
      list["err_code"] = paramSensor[j].err_code;
    }
  }
  JsonArray listQueue = doc.createNestedArray("working_queue");
  for (uint8_t i = 0; i < num_queue; i++) {
    uint8_t reagent = param_queue[i][0];
    uint8_t pond = param_queue[i][1];
    uint8_t type = param_queue[i][2];
    JsonObject list = listQueue.createNestedObject();
    list["code"] = (reagent >= NUM_REAGENT ? paramSensor[reagent - NUM_REAGENT].name_sensor : paramReagent[reagent].name_reagent);
    list["pond_id"] = (pond == 255 ? NULL : paramPond[pond].pond_id);
    if (type == 1)
      list["type"] = "measure_now_pond_test";
    else if (type == 0)
      list["type"] = "scheduled";
    else if (type == 2)
      list["type"] = "measure_now_non_pond_test";
  }
  JsonObject memory = doc["memory"].to<JsonObject>();
  memory["free_ram"] = paraMem.getFreeRam();
  memory["stack_usage"] = paraMem.getStackUsage();
  memory["heap_usage"] = paraMem.getHeapUsage();
  memory["free_flash"] = paraMem.getFreeFlash();
  // memory["free_block_in_heap"] = paraMem.freeMemoryInHeap();
  // memory["total_sram"] = paraMem.getTotalSram();
  // memory["total_flash"] = paraMem.getTotalFlash();
  char *mes = new char[len];
  serializeJson(doc, mes, len);
  //doc.clear();///////////////note lại
  return mes;
}

char *JsonPWQ::commandRespond(char *cmd_fb) {
  uint16_t len_msg = MSG_MIN;
  DynamicJsonDocument doc(len_msg);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = cmd_fb;
  doc["device_id"] = DEVICE_ID;
  char *msg = new char[len_msg];
  serializeJson(doc, msg, len_msg);
  return msg;
}
char *JsonPWQ::msgStatus(char *state) {
  uint16_t len_msg = MSG_MIN;
  DynamicJsonDocument doc(len_msg);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "update_state";
  doc["device_id"] = DEVICE_ID;
  doc["state"] = state;
  char *mes = new char[len_msg];
  serializeJson(doc, mes, len_msg);
  return mes;
}
char *JsonPWQ::msgStatus() {
  uint16_t len_msg = MSG_MAX;
  DynamicJsonDocument doc(len_msg);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "update_state";
  doc["device_id"] = DEVICE_ID;
  //doc["state"] = stgDevice.current_state;
  // JsonObject memory = doc["memory"].to<JsonObject>();
  // memory["free_ram"] = paraMem.getFreeRam();
  // memory["stack_usage"] = paraMem.getStackUsage();
  // memory["heap_usage"] = paraMem.getHeapUsage();
  // memory["free_block_in_heap"] = paraMem.freeMemoryInHeap();
  // memory["free_flash"] = paraMem.getFreeFlash();
  // memory["total_sram"] = paraMem.getTotalSram();
  // memory["total_flash"] = paraMem.getTotalFlash();
  JsonObject data = doc["data"].to<JsonObject>();

  char *mes = new char[len_msg];
  serializeJson(doc, mes, len_msg);
  return mes;
}
char *JsonPWQ::msgStatusError(char *name_err, int16_t err_code, uint8_t grade_err) {
  uint16_t len_msg = MSG_MIN;
  DynamicJsonDocument doc(len_msg);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "update_state";
  doc["device_id"] = DEVICE_ID;
  doc["state"] = grade_err == 0 ? STATUS_WARNING : STATUS_ERROR;
  doc["err_code"] = err_code;
  doc["err_name"] = name_err;
  char *mes = new char[len_msg];
  serializeJson(doc, mes, len_msg);
  return mes;
}

char *JsonPWQ::msgLogs(char *logs) {
  uint16_t len_msg = MSG_MID;
  DynamicJsonDocument doc(len_msg);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "send_logs";
  doc["device_id"] = DEVICE_ID;
  doc["logs"] = logs;
  char *mes = new char[len_msg];
  serializeJson(doc, mes, len_msg);
  return mes;
}
//for lora
char *JsonPWQ::msgGetValueModule(uint8_t pond, ParamPond paramPond[NUM_POND], ParamSensor paramSensor[NUM_SENSOR]) {
  uint16_t len_msg = MSG_MIN;
  DynamicJsonDocument doc(len_msg);
  doc["module_id"] = paramPond[pond].pond_module_id;
  doc["msg_type"] = "get_value";
  JsonArray payload = doc.createNestedArray("payload");
  for (uint8_t i = 0; i < NUM_SENSOR; i++) {
    JsonObject index = payload.createNestedObject();
    index["name"] = paramSensor[i].name_sensor;
    index["enable"] = paramSensor[i].enable_each_pond[pond];
  }
  char *mes = new char[len_msg];
  serializeJson(doc, mes, len_msg);
  return mes;
}
char *JsonPWQ::handleMsgValueModule(char *msg, uint8_t pond, ParamPond paramPond, ParamSensor paramSensor[NUM_SENSOR]) {
  DynamicJsonDocument doc(lengthToAllocation(msg));
  DeserializationError error = deserializeJson(doc, msg);
  if (error) {
    return nullptr;
  }
  uint16_t len_msg = MSG_MID;
  DynamicJsonDocument data(len_msg);
  data["device_type"] = DEVICE_TYPE;
  data["message_type"] = "measure_completed";
  data["device_id"] = DEVICE_ID;
  data["pond_id"] = paramPond.pond_id;
  data["module_id"] = paramPond.pond_module_id;
  JsonArray water_quality_params = data.createNestedArray("water_quality_params");
  for (JsonVariant payload : doc["payload"].as<JsonArray>()) {
    const char *name_sensor = payload["name"];
    const int16_t err = payload["err_code"] | -1;
    if (err == -1) {
      JsonObject water_quality_param1 = water_quality_params.createNestedObject();
      water_quality_param1["code"] = name_sensor;
      water_quality_param1["source"] = "sensor";
      water_quality_param1["value"] = payload["value"];
    }
    for (uint8_t i = 0; i < NUM_SENSOR; i++)
      if (generalFunction.compareTwoCharArray(name_sensor, paramSensor[i].name_sensor)) {
        paramSensor[i].err_each_pond[pond] = err;
        break;
      }
  }
  char *mes = new char[len_msg];
  serializeJson(data, mes, len_msg);
  return mes;
}
char *JsonPWQ::msgControlPump(const char *module_id, bool enable, uint16_t time) {
  uint16_t len_msg = MSG_MIN;
  DynamicJsonDocument doc(len_msg);
  doc["module_id"] = module_id;
  doc["msg_type"] = "control_pump";
  doc["enable"] = enable;
  doc["time"] = time;
  char *mes = new char[len_msg];
  serializeJson(doc, mes, len_msg);
  return mes;
}
char *JsonPWQ::msgCalibSensorModule(const char *module_id, const char *name_sensor, uint8_t point_index) {
  uint16_t len_msg = MSG_MIN;
  DynamicJsonDocument doc(len_msg);
  doc["module_id"] = module_id;
  doc["msg_type"] = "calib";
  doc["name"] = name_sensor;
  doc["point_index"] = point_index;
  char *mes = new char[len_msg];
  serializeJson(doc, mes, len_msg);
  return mes;
}
char *JsonPWQ::msgInforLed(const char *stage, uint16_t *led_data, SettingDevice stgDevice) {
  uint16_t len_msg = MSG_MIN;
  DynamicJsonDocument doc(len_msg);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "led_values";
  doc["device_id"] = DEVICE_ID;
  doc["stage"] = stage;
  doc["drops"] = stgDevice.current_drop;
  doc["is_calibing"] = generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_CALIB) ? true : false;
  JsonArray data_led = doc.createNestedArray("leds");
  for (uint8_t i = 0; i < 6; i++) {
    data_led.add(led_data[i]);
  }
  char *json = nullptr;
  json = new char[len_msg];
  serializeJson(doc, json, len_msg);
  return json;
}
char *JsonPWQ::msgValueReagent(const char *name_reagent, char *pond_id, float value, uint16_t *led_data, char *test_id, char *test_type, SettingDevice stgDevice) {
  uint16_t len_msg = MSG_MID;
  DynamicJsonDocument doc(len_msg);
  doc["device_type"] = DEVICE_TYPE;
  doc["device_id"] = DEVICE_ID;
  doc["pond_id"] = pond_id;
  doc["test_id"] = test_id;
  doc["test_type"] = test_type;
  if (generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_CALIB)) {
    doc["stage"] = "final";
    doc["raw_value"] = value;
    doc["is_calibing"] = true;
    JsonArray data_led = doc.createNestedArray("leds");
    for (uint8_t i = 0; i < 6; i++) {
      data_led.add(led_data[i]);
    }
  } else if (generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_RUNNING)) {
    doc["message_type"] = "measure_completed";
    JsonArray water_quality_params = doc.createNestedArray("water_quality_params");
    JsonObject water_quality_param1 = water_quality_params.createNestedObject();
    water_quality_param1["code"] = name_reagent;
    water_quality_param1["value"] = value;
    water_quality_param1["source"] = "reagent";
  }
  char *json = nullptr;
  json = new char[len_msg];
  serializeJson(doc, json, len_msg);
  return json;
}
char *JsonPWQ::msgValueSensor(char *pond_id, char *test_id, char *test_type, ParamSensor paramSensor[NUM_SENSOR]) {
  uint16_t len_msg = MSG_MID;
  DynamicJsonDocument doc(len_msg);
  doc["device_type"] = DEVICE_TYPE;
  doc["device_id"] = DEVICE_ID;
  doc["pond_id"] = pond_id;
  doc["test_id"] = test_id;
  doc["test_type"] = test_type;
  doc["message_type"] = "measure_completed";
  JsonArray water_quality_params = doc.createNestedArray("water_quality_params");
  for (uint8_t i = 0; i < NUM_SENSOR; i++) {
    if (!paramSensor[i].has_in_machine || paramSensor[i].err_in_machine)
      continue;
    JsonObject water_quality_param1 = water_quality_params.createNestedObject();
    water_quality_param1["code"] = paramSensor[i].name_sensor;
    water_quality_param1["value"] = paramSensor[i].value;
    water_quality_param1["source"] = "sensor";
  }
  char *json = nullptr;
  json = new char[len_msg];
  serializeJson(doc, json, len_msg);
  return json;
}