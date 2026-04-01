#include "JsonPWQ.h"
bool JsonPWQ::isJson(const char* js) {
  static StaticJsonDocument<256> doc;
  doc.clear();
  DeserializationError error = deserializeJson(doc, js);
  return !error;
}
void JsonPWQ::handleMsgSettingDevice(char* msg, ModuleSetting& moduleStg, ParamSensor paramSensor[NUM_SENSOR]) {
  DynamicJsonDocument data(MAX_MSG_SIZE);
  DeserializationError error = deserializeJson(data, msg);
  if (error) {
    Serial.print(F("JSON Setting Failed: "));
    Serial.println(error.c_str());
    return;
  }
  JsonObject data_settings = data["data"]["pond"];
  moduleStg.time_pump = data_settings["pump_duration"];
  const char* pond_id = data_settings["pond_id"];
  GeneralFunction::copyString(moduleStg.pond_id, pond_id, MAX_ID_LEN);
  paramSensor[0].enable = data_settings["do_enable"];
  paramSensor[0].interval_read = data_settings["do_interval"];
  paramSensor[1].enable = data_settings["temp_enable"];
  paramSensor[1].interval_read = data_settings["temp_interval"];
  return;
}
bool JsonPWQ::handleMsgTurnPump(char* msg, ModuleSetting& moduleStg) {
  static StaticJsonDocument<MAX_MSG_SIZE> doc;  // Dùng Static để an toàn Stack hoặc Dynamic nếu cần
  doc.clear();
  DeserializationError error = deserializeJson(doc, msg);
  if (error) {
    return false;
  }
  const char* pond_id = doc["pond_id"];
  const char* module_id = doc["module_id"];
  Serial.println(pond_id);
  Serial.println(module_id);
  if (!GeneralFunction::compareTwoCharArray(moduleStg.pond_id, pond_id) || !GeneralFunction::compareTwoCharArray(DEVICE_ID, module_id))
    return false;
  moduleStg.is_pump_run = doc["control_pump"];
  moduleStg.pump_start_time = millis();
  return true;
}
char* JsonPWQ::msgCMDCplt(const char* msg_type) {
  static StaticJsonDocument<MAX_MSG_SIZE> doc;
  doc.clear();
  doc["device_id"] = DEVICE_ID;
  doc["msg_type"] = msg_type;
  //doc["command"] = CMD_CPLT;
  memset(_txBuffer, 0, MAX_MSG_SIZE);
  serializeJson(doc, _txBuffer, MAX_MSG_SIZE);
  return _txBuffer;
}
char* JsonPWQ::msgCMDDenied(const char* msg_type) {
  static StaticJsonDocument<MAX_MSG_SIZE> doc;
  doc.clear();
  doc["device_id"] = DEVICE_ID;
  doc["msg_type"] = msg_type;
  doc["command"] = CMD_DENIED;
  memset(_txBuffer, 0, MAX_MSG_SIZE);
  serializeJson(doc, _txBuffer, MAX_MSG_SIZE);
  return _txBuffer;
}
char* JsonPWQ::msgStateError(uint16_t error_code) {
  static StaticJsonDocument<MAX_MSG_SIZE> doc;
  doc.clear();
  doc["device_id"] = DEVICE_ID;
  doc["msg_type"] = "error";
  doc["err_code"] = error_code;
  memset(_txBuffer, 0, MAX_MSG_SIZE);
  serializeJson(doc, _txBuffer, MAX_MSG_SIZE);
  return _txBuffer;
}
char* JsonPWQ::msgResultCalib(const char* state) {
  static StaticJsonDocument<MAX_MSG_SIZE> doc;
  doc.clear();
  doc["device_id"] = DEVICE_ID;
  doc["msg_type"] = "calib";
  doc["result"] = state;
  memset(_txBuffer, 0, MAX_MSG_SIZE);
  serializeJson(doc, _txBuffer, MAX_MSG_SIZE);
  return _txBuffer;
}
char* JsonPWQ::msgStateMachine(ModuleSetting moduleStg, ParamSensor paramSensor[NUM_SENSOR]) {
  const uint16_t len = MSG_MAX;
  DynamicJsonDocument doc(len);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "update_state";
  doc["device_id"] = DEVICE_ID;
  doc["pond_id"] = moduleStg.pond_id;
  doc["state"] = moduleStg.current_state;
  doc["title"] = "title";
  JsonObject payload = doc["payload"].to<JsonObject>();
  JsonArray statusSensor = payload.createNestedArray("sensors");
  for (uint8_t i = 0; i < NUM_SENSOR; i++) {
    if (!paramSensor[i].name_sensor)
      continue;
    JsonObject list = statusSensor.createNestedObject();
    list["code"] = paramSensor[i].name_sensor;
    list["enable"] = paramSensor[i].enable;
    list["err"] = paramSensor[i].err;
    list["plug"] = paramSensor[i].jack_plug;
  }
  JsonObject statusPump = payload["pumps"].to<JsonObject>();
  statusPump["plug"] = moduleStg.jack_pump_plug;
  statusPump["time"] = moduleStg.time_pump;
  // JsonObject memory = doc["memory"].to<JsonObject>();
  // memory["free_ram"] = paraMem.getFreeRam();
  // memory["stack_usage"] = paraMem.getStackUsage();
  // memory["heap_usage"] = paraMem.getHeapUsage();
  // memory["largest_block"] = paraMem.getLargestFreeBlock();
  serializeJson(doc, _sharedBufferMax, len);
  return _sharedBufferMax;
}
char* JsonPWQ::msgStatus(char* state) {
  const uint16_t len = MSG_MIN;
  DynamicJsonDocument doc(len);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = "update_state";
  doc["device_id"] = DEVICE_ID;
  doc["state"] = state;
  serializeJson(doc, _sharedBufferMin, len);
  return _sharedBufferMin;
}
char* JsonPWQ::msgValueSensor(char* pond_id, ParamSensor paramSensor) {
  if (!paramSensor.enable || paramSensor.err || !paramSensor.jack_plug)
    return nullptr;
  const uint16_t len = MSG_MID;
  DynamicJsonDocument doc(len);
  doc["device_type"] = DEVICE_TYPE;
  doc["device_id"] = DEVICE_ID;
  doc["pond_id"] = pond_id;
  doc["message_type"] = "measure_completed";
  JsonObject water_quality_param = doc["water_quality_params"].to<JsonObject>();
  water_quality_param["param"] = paramSensor.name_sensor;
  water_quality_param["value"] = paramSensor.value;
  water_quality_param["source"] = "sensor";
  serializeJson(doc, _sharedBufferMid, len);
  return _sharedBufferMid;
}
char* JsonPWQ::commandRespond(char* cmd_fb) {
  const uint16_t len = MSG_MIN;
  DynamicJsonDocument doc(len);
  doc["device_type"] = DEVICE_TYPE;
  doc["message_type"] = cmd_fb;
  doc["device_id"] = DEVICE_ID;
  serializeJson(doc, _sharedBufferMin, len);
  return _sharedBufferMin;
}