#include "JsonPWQ.h"


bool JsonPWQ::isJon(char *js) {
  if (js == nullptr) return false;
  DynamicJsonDocument doc(JSON_LARGE_BUFFER_SIZE);
  DeserializationError error = deserializeJson(doc, js);
  return !error;
}
bool JsonPWQ::handleMsgSettingDevice(char *msg, SettingDevice stgDevice, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR]) {
  DynamicJsonDocument data(JSON_LARGE_BUFFER_SIZE);

  // 3. Deserialize với Filter
  // DeserializationOption::Filter(filter) là chìa khóa để tiết kiệm RAM
  DeserializationError error = deserializeJson(data, msg);
  if (error) {
    Serial.print(F("JSON Setting Failed: "));
    Serial.println(error.c_str());
    return false;
  }
  JsonObject data_settings = data["data"];
  stgDevice.amount_clean_water[0] = data_settings["clean_water"]["max"];
  stgDevice.amount_clean_water[1] = data_settings["clean_water"]["remaining"];
  stgDevice.amount_vim[0] = data_settings["clean_water"]["max"];
  stgDevice.amount_vim[1] = data_settings["clean_water"]["remaining"];
  JsonArray data_ponds = data_settings["ponds"].as<JsonArray>();
  uint8_t numPond = data_ponds.size();
  //GET SETTING PONDS
  for (uint8_t i = 0; i < NUM_POND && i < data_ponds.size(); i++) {
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
    const char *module_type = data_ponds[i]["module_type"];
    if (!pond_id || !pond_module_id) {
      paramPond[i].enable = false;
      continue;
    }
    GeneralFunction::copyString(paramPond[i].pond_id, pond_id, MAX_ID_LEN);
    GeneralFunction::copyString(paramPond[i].pond_module_id, pond_module_id, MAX_ID_LEN);
    GeneralFunction::copyString(paramPond[i].module_type, module_type, MAX_MTYPE_LEN);
    // GeneralFunction::wrapperPointer(paramPond[i].pond_id, pond_id);
    // GeneralFunction::wrapperPointer(paramPond[i].pond_module_id, pond_module_id);
    // GeneralFunction::wrapperPointer(paramPond[i].module_type, module_type);
    if (GeneralFunction::compareTwoCharArray(pond_status, POND_STATUS_ON) || GeneralFunction::compareTwoCharArray(pond_status, POND_STATUS_WARNING)) {
      paramPond[i].err = false;
      paramPond[i].enable = true;
    } else if (GeneralFunction::compareTwoCharArray(pond_status, POND_STATUS_ERROR)) {
      paramPond[i].err = true;
      paramPond[i].enable = true;
    } else if (GeneralFunction::compareTwoCharArray(pond_status, POND_STATUS_OFF)) {
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
    for (uint8_t i = 0; i < NUM_REAGENT; i++) {
      if (GeneralFunction::compareTwoCharArray(name_reagent, paramReagent[i].name_reagent)) {
        // Serial.print(name_reagent);
        // Serial.print(": ");
        paramReagent[i].enable = enable;
        if (data_reagent["reagent"].containsKey("vials")) {
          // Serial.println("vials: ");
          uint8_t order_vial = 0;
          for (JsonObject data_vials : data_reagent["reagent"]["vials"].as<JsonArray>()) {
            paramReagent[i].pos[order_vial] = data_vials["position"];
            paramReagent[i].max_ml[order_vial] = data_vials["max_ml"];
            paramReagent[i].remaining[order_vial] = data_vials["remaining"];
            // Serial.print(order_vial);
            // Serial.print(", ");
            // Serial.print(paramReagent[i].pos[order_vial]);
            // Serial.print(", ");
            // Serial.print(paramReagent[i].max_ml[order_vial]);
            // Serial.print(", ");
            // Serial.println(paramReagent[i].remaining[order_vial]);
            order_vial++;
          }
        }
        if (data_reagent["calib_results"].containsKey("leds")) {
          // Serial.print("[leds: ");
          JsonArray data_leds = data_reagent["calib_results"]["leds"];
          paramReagent[i].paramCalib.num_index = data_leds.size();
          if (paramReagent[i].paramCalib.num_index > MAX_CALIB_POINTS)
            paramReagent[i].paramCalib.num_index = MAX_CALIB_POINTS;
          for (uint8_t j = 0; j < paramReagent[i].paramCalib.num_index; j++) {
            paramReagent[i].paramCalib.leds[j] = data_reagent["calib_results"]["leds"][j];
            // Serial.print(String(paramReagent[i].paramCalib.leds[j]) + ", ");
          }
          // Serial.print("] - ");
        }
        if (data_reagent["calib_results"].containsKey("coefs")) {
          // Serial.print("[coefs: ");
          JsonArray data_coefs = data_reagent["calib_results"]["coefs"];
          uint8_t count = data_coefs.size();
          if (count > MAX_CALIB_POINTS) count = MAX_CALIB_POINTS;
          for (uint8_t j = 0; j < count; j++) {
            paramReagent[i].paramCalib.coefs[j] = data_coefs[j];
            // Serial.print(String(paramReagent[i].paramCalib.coefs[j]) + ", ");
          }
          // Serial.print("] - ");
        }
        if (data_reagent["calib_results"].containsKey("inters")) {
          paramReagent[i].paramCalib.inters = data_reagent["calib_results"]["inters"][0];
          // Serial.println("[inters: " + String(paramReagent[i].paramCalib.inters) + "]");
        }
        break;
      }
    }
    for (uint8_t i = 0; i < NUM_SENSOR; i++) {
      if (GeneralFunction::compareTwoCharArray(name_reagent, paramSensor[i].name_sensor)) {
        // Serial.print(name_reagent);
        // if (enable)
        //   Serial.println(": true");
        // else
        //   Serial.println(": false");
        paramSensor[i].has_in_machine = enable;
        break;
      }
    }
  }
  return true;
}
void JsonPWQ::handleMsgSettingPond(char *msg, ParamPond paramPond[NUM_POND]) {
  // static StaticJsonDocument<3072> doc;
  DynamicJsonDocument data(MSG_MAX);
  DeserializationError error = deserializeJson(data, msg);
  if (error) return;
  const char *pond_id = data["pond_id"];
  for (uint8_t i = 0; i < NUM_POND; i++) {
    if (GeneralFunction::compareTwoCharArray((char *)pond_id, paramPond[i].pond_id)) {
      GeneralFunction::copyString(paramPond[i].sche, msg, MAX_SCHE_LEN);
    }
  }
  return;
}
void JsonPWQ::handleMsgManual(char *msg, ControlManual &controlManual) {
  DynamicJsonDocument doc(MSG_MIN);
  DeserializationError error = deserializeJson(doc, msg);
  if (error) {
    controlManual.cmd_code = -1;
    return;
  }
  controlManual.cmd_code = doc["payload"]["cmd_manual"];
  controlManual.time = doc["payload"]["time"];
  controlManual.is_run = doc["payload"]["run"];
  //Serial.println("manual: " + String(controlManual.cmd_code) + " " + String(controlManual.time));
  return;
}
void JsonPWQ::handleMsgMeasureNow(char *msg, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR], ListReagentNow &listReagentNow) {
 
  DynamicJsonDocument doc(MSG_MID);
  DeserializationError error = deserializeJson(doc, msg);
  if (error) {
    listReagentNow.num_reagent_now = 0;
    return;
  }
  const char *pond_id = doc["payload"]["pondId"];
  const char *test_type = doc["payload"]["test_type"];
  const char *test_id = doc["payload"]["test_id"];
  // Serial.println(pond_id);
  // Serial.println(test_type);
  // Serial.println(test_id);
  if (pond_id) {
    for (uint8_t i = 0; i < NUM_POND; i++)
      if (GeneralFunction::compareTwoCharArray(pond_id, paramPond[i].pond_id) && paramPond[i].enable)
        listReagentNow.pond = i;
  } else
    listReagentNow.pond = -1;
  if (test_type)
    GeneralFunction::copyString(listReagentNow.test_type, test_type, MAX_TEST_STR_LEN);
  else
    listReagentNow.test_type[0] = '\0';
  if (test_id)
    GeneralFunction::copyString(listReagentNow.test_id, test_id, MAX_TEST_STR_LEN);
  else
    listReagentNow.test_id[0] = '\0';
  listReagentNow.point_index = doc["payload"]["calib_point_index"] | 0;
  uint8_t index = 0;
  //Serial.print("list chất: ");
  if (doc["payload"]["params"].is<JsonArray>()) {
    for (const JsonVariant &name : doc["payload"]["params"].as<JsonArray>()) {
      if (!name.is<const char *>()) {
        continue;
      }
      const char *name_reagent = name.as<const char *>();
      for (uint8_t i = 0; i < NUM_REAGENT; i++) {
        if (GeneralFunction::compareTwoCharArray(name, paramReagent[i].name_reagent) && paramReagent[i].enable) {
          listReagentNow.list_reagent[index] = i;
          index++;
          // Serial.print(paramReagent[i].name_reagent);
          // Serial.print(" ");
          break;
        }
      }
      for (uint8_t i = 0; i < NUM_SENSOR; i++) {
        if (GeneralFunction::compareTwoCharArray(name, paramSensor[i].name_sensor) && paramSensor[i].has_in_machine) {
          listReagentNow.list_reagent[index] = NUM_REAGENT + i;
          index++;
          // Serial.print(paramSensor[i].name_sensor);
          // Serial.print(" ");
          break;
        }
      }
    }
  }
  // Serial.println("");
  listReagentNow.num_reagent_now = index;
  return;
}

bool JsonPWQ::handleMsgSchedule(char *msg, int8_t *data, uint32_t current_time, ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR]) {
  DynamicJsonDocument doc(MSG_MAX);
  DeserializationError error = deserializeJson(doc, msg);
  if (error) return false;
  if (!doc["data"].containsKey("schedules") || !doc["data"]["schedules"].is<JsonArray>()) {
    return false;
  }
  constexpr uint32_t TIME_CHECK = 300;  // Time window in seconds
  size_t order = 0;
  Serial.println("chất là: ");
  for (JsonObject data_reagent : doc["data"]["schedules"].as<JsonArray>()) {
    const char *name = data_reagent["name"];
    // Serial.println(name);
    if (name == nullptr)
      continue;
    uint8_t index = NUM_REAGENT + NUM_SENSOR;
    for (uint8_t i = 0; i < NUM_REAGENT; i++) {
      if (GeneralFunction::compareTwoCharArray(name, paramReagent[i].name_reagent) && paramReagent[i].enable) {
        index = i;
        break;
      }
    }
    if (index == NUM_REAGENT + NUM_SENSOR) {
      // Serial.print("vào lấy sensor: ");
      for (uint8_t i = 0; i < NUM_SENSOR; i++) {
        // Serial.print(paramSensor[i].name_sensor);
        if (GeneralFunction::compareTwoCharArray(name, paramSensor[i].name_sensor) && paramSensor[i].has_in_machine) {
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
    Serial.print(" ");
    for (const JsonVariant &time : data_reagent["time"]["seconds"].as<JsonArray>()) {
      if (!time.is<uint32_t>()) {
        continue;
      }
      uint32_t value = time.as<uint32_t>();
      if (current_time < value)
        continue;
      if (current_time >= value && (current_time - value) < TIME_CHECK) {
        Serial.print(value);
        data[order] = index;
        order++;
        break;
      }
    }
    Serial.println("");
  }
  return order > 0;
}

char *JsonPWQ::msgStateMachine(char *title, uint32_t unix_time, uint16_t total_time, float temp, int8_t pond, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR], uint8_t param_queue[][3], uint8_t num_queue, char *test_id, char *state) {
  const uint16_t len = MSG_MAX;
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
    listPond["module_type"] = paramPond[i].module_type;
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
        listPond["status"] = POND_STATUS_ERROR;
      else {
        if (num_err == 0)
          listPond["status"] = POND_STATUS_ON;
        else
          listPond["status"] = POND_STATUS_WARNING;
      }
    } else {
      listPond["status"] = POND_STATUS_OFF;
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
  // JsonObject memory = doc["memory"].to<JsonObject>();
  // memory["free_ram"] = paraMem.getFreeRam();
  // memory["stack_usage"] = paraMem.getStackUsage();
  // memory["heap_usage"] = paraMem.getHeapUsage();
  // memory["largest_block"] = paraMem.getLargestFreeBlock();
  serializeJson(doc, _sharedBufferMax, len);
  return _sharedBufferMax;
}

char *JsonPWQ::commandRespond(char *cmd_fb) {
  const uint16_t len = MSG_MIN;
  DynamicJsonDocument doc(len);

  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = cmd_fb;
  doc["device_id"] = DEVICE_ID;
  serializeJson(doc, _sharedBufferMin, len);
  return _sharedBufferMin;
}
char *JsonPWQ::msgStatus(char *state) {
  const uint16_t len = MSG_MIN;
  DynamicJsonDocument doc(len);

  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "update_state";
  doc["device_id"] = DEVICE_ID;
  doc["state"] = state;
  serializeJson(doc, _sharedBufferMin, len);
  return _sharedBufferMin;
}
char *JsonPWQ::msgStatusError(char *name_err, int16_t err_code, uint8_t grade_err) {
  const uint16_t len = MSG_MIN;
  DynamicJsonDocument doc(len);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "update_state";
  doc["device_id"] = DEVICE_ID;
  doc["state"] = grade_err == 0 ? MACHINE_STATUS_WARNING : MACHINE_STATUS_ERROR;
  JsonObject payload = doc["payload"].to<JsonObject>();
  payload["err_code"] = err_code;
  payload["err_name"] = name_err;
  serializeJson(doc, _sharedBufferMin, len);
  return _sharedBufferMin;
}
char *JsonPWQ::msgLogs(char *logs) {
  const uint16_t len = MSG_MID;
  // DynamicJsonDocument doc(len);
  DynamicJsonDocument doc(len);

  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "send_logs";
  doc["device_id"] = DEVICE_ID;
  doc["logs"] = logs;
  serializeJson(doc, _sharedBufferMid, len);
  return _sharedBufferMid;
}
//for lora
char *JsonPWQ::msgGetValueModule(uint8_t pond, ParamPond paramPond[NUM_POND], ParamSensor paramSensor[NUM_SENSOR]) {
  const uint16_t len = MSG_MID;
  DynamicJsonDocument doc(len);

  doc["module_id"] = paramPond[pond].pond_module_id;
  doc["msg_type"] = "get_value";
  JsonArray payload = doc.createNestedArray("payload");
  for (uint8_t i = 0; i < NUM_SENSOR; i++) {
    JsonObject index = payload.createNestedObject();
    index["name"] = paramSensor[i].name_sensor;
    index["enable"] = paramSensor[i].enable_each_pond[pond];
  }
  serializeJson(doc, _sharedBufferMin, len);
  return _sharedBufferMin;
}
char *JsonPWQ::handleMsgValueModule(char *msg, uint8_t pond, ParamPond paramPond, ParamSensor paramSensor[NUM_SENSOR]) {
  DynamicJsonDocument doc(MSG_MIN);
  DeserializationError error = deserializeJson(doc, msg);
  if (error) return nullptr;
  const uint16_t len = MSG_MID;
  DynamicJsonDocument data(len);
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
      water_quality_param1["param"] = name_sensor;
      water_quality_param1["source"] = "sensor";
      water_quality_param1["value"] = payload["value"];
    }
    for (uint8_t i = 0; i < NUM_SENSOR; i++)
      if (GeneralFunction::compareTwoCharArray(name_sensor, paramSensor[i].name_sensor)) {
        paramSensor[i].err_each_pond[pond] = err;
        break;
      }
  }
  serializeJson(data, _sharedBufferMid, len);
  return _sharedBufferMid;
}
char *JsonPWQ::msgControlPump(const char *module_id, bool enable, uint16_t time) {
  const uint16_t len = MSG_MIN;
  DynamicJsonDocument doc(len);

  doc["module_id"] = module_id;
  doc["msg_type"] = "control_pump";
  doc["enable"] = enable;
  doc["time"] = time;
  serializeJson(doc, _sharedBufferMin, len);
  return _sharedBufferMin;
}
char *JsonPWQ::msgCalibSensorModule(const char *module_id, const char *name_sensor, uint8_t point_index) {
  const uint16_t len = MSG_MIN;
  DynamicJsonDocument doc(len);

  doc["module_id"] = module_id;
  doc["msg_type"] = "calib";
  doc["name"] = name_sensor;
  doc["point_index"] = point_index;
  serializeJson(doc, _sharedBufferMin, len);
  return _sharedBufferMin;
}
char *JsonPWQ::msgInforLed(const char *stage, uint16_t *led_data, SettingDevice stgDevice) {
  const uint16_t len = MSG_MID;
  DynamicJsonDocument doc(len);

  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "led_values";
  doc["device_id"] = DEVICE_ID;
  doc["stage"] = stage;
  doc["drops"] = stgDevice.current_drop;
  doc["is_calibing"] = GeneralFunction::compareTwoCharArray(stgDevice.current_state, MACHINE_STATUS_CALIB) ? true : false;
  JsonArray data_led = doc.createNestedArray("leds");
  for (uint8_t i = 0; i < 6; i++) {
    data_led.add(led_data[i]);
  }
  serializeJson(doc, _sharedBufferMin, len);
  return _sharedBufferMin;
}
char *JsonPWQ::msgValueReagent(const char *name_reagent, char *pond_id, float value, uint16_t *led_data, char *test_id, char *test_type, SettingDevice stgDevice) {
  const uint16_t len = MSG_MID;
  DynamicJsonDocument doc(len);

  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "measure_completed";
  doc["device_id"] = DEVICE_ID;
  doc["pond_id"] = pond_id;
  doc["test_id"] = test_id;
  doc["test_type"] = test_type;
  if (GeneralFunction::compareTwoCharArray(stgDevice.current_state, MACHINE_STATUS_CALIB)) {
    doc["param"] = name_reagent;
    doc["stage"] = "final";
    doc["raw_value"] = value;
    doc["is_calibing"] = true;
    JsonArray data_led = doc.createNestedArray("leds");
    for (uint8_t i = 0; i < 6; i++) {
      data_led.add(led_data[i]);
    }
  } else if (GeneralFunction::compareTwoCharArray(stgDevice.current_state, MACHINE_STATUS_RUNNING)) {
    JsonArray water_quality_params = doc.createNestedArray("water_quality_params");
    JsonObject water_quality_param1 = water_quality_params.createNestedObject();
    water_quality_param1["param"] = name_reagent;
    water_quality_param1["value"] = value;
    water_quality_param1["source"] = "reagent";
  }
  serializeJson(doc, _sharedBufferMid, len);
  return _sharedBufferMid;
}
char *JsonPWQ::msgValueSensor(char *pond_id, char *test_id, char *test_type, ParamSensor paramSensor[NUM_SENSOR]) {
  const uint16_t len = MSG_MID;
  DynamicJsonDocument doc(len);
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
    water_quality_param1["param"] = paramSensor[i].name_sensor;
    water_quality_param1["value"] = paramSensor[i].value;
    water_quality_param1["source"] = "sensor";
  }
  serializeJson(doc, _sharedBufferMid, len);
  return _sharedBufferMid;
}
char *JsonPWQ::msgInforReagent(const char *reagent, uint8_t pos, float ml_used) {
  const uint16_t len = MSG_MID;
  DynamicJsonDocument doc(len);
  doc["device_type"] = DEVICE_TYPE;
  doc["device_id"] = DEVICE_ID;
  doc["message_type"] = "update_reagent";
  JsonObject reagent_params = doc.createNestedObject("reagent_params");
  reagent_params["param"] = reagent;
  reagent_params["vial_position"] = pos;
  reagent_params["ml_used"] = ml_used;
  serializeJson(doc, _sharedBufferMid, len);
  return _sharedBufferMid;
}
char *JsonPWQ::msgInforVimWater(uint8_t type, float ml_used) {
  const uint16_t len = MSG_MID;
  DynamicJsonDocument doc(len);
  doc["device_type"] = DEVICE_TYPE;
  doc["device_id"] = DEVICE_ID;
  doc["message_type"] = "update_reagent";
  JsonObject object_params = doc.createNestedObject("reagent_params");
  object_params["param"] = (type == 0 ? "clean_water" : "vim");
  object_params["ml_used"] = ml_used;
  serializeJson(doc, _sharedBufferMid, len);
  return _sharedBufferMid;
}