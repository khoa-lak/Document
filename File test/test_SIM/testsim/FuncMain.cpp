#include "FuncMain.h"
FuncMain *_funcMainPtr = nullptr;
void mqttMsgHandler(const char *topic, const char *msg) {
  if (_funcMainPtr) {
    _funcMainPtr->hasMqttMessage(topic, msg);
  }
}
FuncMain::FuncMain() {
  _funcMainPtr = this;
  simA7xxx.setMqttMsgCallback(mqttMsgHandler);
}
FuncMain::~FuncMain() {
}
void FuncMain::setupMain() {
  simA7xxx.setupSim();
  rstVariables();
  rstPointer();
  return;
}
void FuncMain::rstVariables() {
  _first_run = true;
  return;
}
void FuncMain::rstPointer() {
  return;
}
void FuncMain::runMain() {
  if (checkSim()) {
    classifyFuncRun();
  }
  return;
}
bool FuncMain::checkSim() {
  if (simA7xxx.initSim()) {
    if (generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_DISCONNECTED)) {
      generalFunction.wrapperStatus(stgDevice.current_state, STATUS_CONNECTED);
    }
    simA7xxx.followMsgAnyTopic();
    return true;
  } else {
    if (!generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_DISCONNECTED))
      generalFunction.wrapperStatus(stgDevice.current_state, STATUS_DISCONNECTED);
    _first_run = true;
    return false;
  }
}
void FuncMain::hasMqttMessage(const char *topic, const char *msg_input) {
  if (msg_input) {
    uint16_t length_msg = strlen(msg_input);
    char msg[length_msg] = { '\0' };
    generalFunction.removeWhitespace(msg_input, msg, length_msg);
    if (msg[0] != '0') {
      Serial.println("RECV MSG MQTT:");
      Serial.print("  - ");
      Serial.println(topic);
      Serial.print("  - ");
      Serial.println(msg);
      handleMsgMQTT(topic, msg);
    }
  }
  return;
}
void FuncMain::handleMsgMQTT(const char *topic, const char *msg) {
  if (msg != nullptr) {
    if (jsonPWQ.isJon(msg)) {
      if (strstr(msg, MSG_TYPE_CMD) && strstr(msg, DEVICE_ID))
        handleMsgCommand(msg);
      else if (strstr(msg, MSG_TYPE_STG))
        handleMsgSetting(topic, msg);
    }
  }
  return;
}
void FuncMain::handleMsgCommand(const char *msg) {
  bool is_feedback = false;
  if (strstr(msg, CMD_ON)) {
    is_feedback = true;
    generalFunction.wrapperStatus(stgDevice.current_state, STATUS_RUNNING);
  } else if (strstr(msg, CMD_OFF)) {
    is_feedback = true;
    generalFunction.wrapperStatus(stgDevice.current_state, STATUS_CONNECTED);
  } else if (strstr(msg, CMD_MEASURE_NOW)) {
    if (!generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_RUNNING))
      return;
    jsonPWQ.handleMsgMeasureNow(msg, paramPond, paramReagent, paramSensor, listReagentNow);
    if (listReagentNow.num_reagent_now)
      is_feedback = true;
  } else if (strstr(msg, CMD_CALIB_ON)) {
    is_feedback = true;
    // if (!generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_CALIB))
    //   setCoefInter(0);
    generalFunction.wrapperStatus(stgDevice.current_state, STATUS_CALIB);
  } else if (strstr(msg, CMD_CALIB_OFF)) {
    is_feedback = true;
    // if (generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_CALIB))
    //   setCoefInter();
    generalFunction.wrapperStatus(stgDevice.current_state, STATUS_CONNECTED);
  } else if (strstr(msg, CMD_INIT_TUBE)) {
    // HANDLE INIT TUBE
  } else if (strstr(msg, CMD_UPDATE_STG_NOW)) {
    // HANDLE UPDATE SETTING NOW
  }
  char *js_cplt = is_feedback ? jsonPWQ.commandRespond(CMD_COMPLETED) : jsonPWQ.commandRespond(CMD_DENIED);
  if (js_cplt) {
    pubMsgDevice(js_cplt);
    generalFunction.deletePointer(js_cplt);
  }
  return;
}
void FuncMain::handleMsgSetting(const char *topic, const char *msg) {
  if (strstr(topic, TOPIC_DEVICE)) {
    if (strstr(msg, MSG_TYPE_STG))
      jsonPWQ.handleMsgSettingDevice((char *)msg, paramPond, paramReagent, paramSensor);
  } else if (strstr(topic, "ponds/")) {
    jsonPWQ.handleMsgSettingPond((char *)msg, paramPond);
  }
  return;
}
bool FuncMain::handleGetStgPond() {
  static uint8_t pond_current = 0;
  if (pond_current < NUM_POND) {
    Serial.print("pond: " + String(pond_current) + ": ");
    if (paramPond[pond_current].pond_id) {
      simA7xxx.subTopicPond(paramPond[pond_current].pond_id);
      Serial.println(paramPond[pond_current].pond_id);
    }
    pond_current++;
    return false;
  } else {
    pond_current = 0;
    return true;
  }
  return false;
}
void FuncMain::pubMsgDevice(char *msg) {
  simA7xxx.pubStatus(msg);
  return;
}
void FuncMain::pubMsgPond(char *msg, uint8_t pond) {
  simA7xxx.pubMsgMQTT(msg, paramPond[pond].pond_id);
  return;
}
void FuncMain::pubLogsMQTT(char *msg) {
  char *js_logs = jsonPWQ.msgLogs(msg);
  if (js_logs) {
    simA7xxx.pubLogs(js_logs);
    generalFunction.deletePointer(js_logs);
  }
  return;
}
void FuncMain::classifyFuncRun() {
  const static char *last_state = STATUS_DISCONNECTED;
  if (!generalFunction.compareTwoCharArray(stgDevice.current_state, last_state)) {
    generalFunction.wrapperStatus(last_state, stgDevice.current_state);
    char *js_state = jsonPWQ.msgStatus((char *)stgDevice.current_state);
    if (js_state) {
      pubMsgDevice(js_state);
      generalFunction.deletePointer(js_state);
    }
  }
  if (generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_RUNNING)) {
    handleStateRunning();
  } else if (generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_CALIB)) {
    handleStateCalib();
  } else if (generalFunction.compareTwoCharArray(stgDevice.current_state, STATUS_CONNECTED)) {
    handleStateConnected();
  }
  return;
}
void FuncMain::handleStateRunning() {
  static uint32_t timer = 0;
  if (timer == 0 || millis() - timer > 10000) {
    Serial.println("máy đang running");
    timer = millis();
  }
  return;
}
void FuncMain::handleStateCalib() {
  static uint32_t timer = 0;
  if (timer == 0 || millis() - timer > 10000) {
    Serial.println("máy đang calib");
    timer = millis();
  }
  return;
}
void FuncMain::handleStateConnected() {
  static uint32_t timer_run_first = millis();
  if (_first_run) {
    if (millis() - timer_run_first > 1000) {
      if (handleGetStgPond()) {
        generalFunction.wrapperStatus(stgDevice.current_state, STATUS_RUNNING);
        _first_run = false;
      }
      timer_run_first = millis();
    }
  } else {
    timer_run_first = millis();
    rstPointer();
    rstVariables();
  }
  return;
}
