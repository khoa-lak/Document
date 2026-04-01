#include "FuncMain.h"
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(SENSOR_TEMP);
DallasTemperature sensorTemp(&oneWire);
FuncMain *g_funcMainPtr = nullptr;
// Free function for MQTT callback
void mqttMsgHandler(const char *topic, const char *msg) {
  if (g_funcMainPtr) {
    g_funcMainPtr->hasMqttMessage(topic, msg);
  }
}
FuncMain::FuncMain() {
  g_funcMainPtr = this;
  simA7xxx.setMqttMsgCallback(mqttMsgHandler);
}
FuncMain::~FuncMain() {
}
void FuncMain::setupMain() {
  simA7xxx.setupSim();
  return;
}
uint8_t FuncMain::runMain() {
  if (!GeneralFunction::compareTwoCharArray(moduleStg.current_state, MACHINE_STATUS_MANUAL))
    if (checkSim()) {
      //  handleStateRunning();
      classifyFuncRun();
    }
  return _jump;
}
bool FuncMain::checkSim() {
  if (simA7xxx.initSim()) {
    if (GeneralFunction::compareTwoCharArray(moduleStg.current_state, MACHINE_STATUS_DISCONNECTED)) {
      GeneralFunction::wrapperStatus(moduleStg.current_state, MACHINE_STATUS_CONNECTED);
    }
    simA7xxx.followMsgAnyTopic();
    return true;
  } else {
    if (!GeneralFunction::compareTwoCharArray(moduleStg.current_state, MACHINE_STATUS_DISCONNECTED))
      GeneralFunction::wrapperStatus(moduleStg.current_state, MACHINE_STATUS_DISCONNECTED);
    _first_run = true;
    return false;
  }
}
void FuncMain::hasMqttMessage(const char *topic, const char *msg_input) {
  if (msg_input) {
    uint16_t length_msg = strlen(msg_input);
    if (length_msg >= MAX_MSG_SIZE) {
      Serial.println(F("Msg too long"));
      return;
    }
    static char msg[MAX_MSG_SIZE];
    GeneralFunction::removeWhitespace(msg_input, msg, length_msg + 1);
    if (msg[0] != '0') {
      handleMsgMQTT(topic, msg);
    }
  }
  return;
}
void FuncMain::handleMsgMQTT(const char *topic, char *msg) {
  if (msg != "") {
    //  if (jsonPWQ.isJon(msg)) {
    if (strstr(msg, MSG_TYPE_CMD) && strstr(msg, DEVICE_ID)) {
      Serial.println(F("RECV MQTT CMD:"));
      Serial.print("  - ");
      Serial.println(msg);
      handleMsgCommand(msg);
    }
    // else if (strstr(msg, MSG_TYPE_STG)) {
    //   Serial.println(F("RECV MQTT STG:"));
    //   Serial.print("  - ");
    //   Serial.println(msg);
    //   handleMsgSetting(topic, msg);
    // }
    // }
  }
  return;
}
void FuncMain::handleMsgCommand(char *msg) {
  bool is_feedback = false;
  if (strstr(msg, CMD_ON)) {
    is_feedback = true;
    GeneralFunction::wrapperStatus(moduleStg.current_state, MACHINE_STATUS_RUNNING);
  } else if (strstr(msg, CMD_OFF)) {
    is_feedback = true;
    GeneralFunction::wrapperStatus(moduleStg.current_state, MACHINE_STATUS_CONNECTED);
  } else if (strstr(msg, CMD_CALIB_ON)) {
    is_feedback = true;
    GeneralFunction::wrapperStatus(moduleStg.current_state, MACHINE_STATUS_CALIB);
  } else if (strstr(msg, CMD_UPLOAD_CODE)) {
    is_feedback = true;
    _jump = 1;
    simA7xxx.downloadToFlash(0x0801D000);
  } else if (strstr(msg, CMD_FACTORY_SETTING)) {
    _jump = 2;
    is_feedback = true;
  } else if (strstr(msg, CMD_WHILE)) {
    while (true)
      ;
  }

  char *js_cplt = is_feedback ? jsonPWQ.commandRespond(CMD_COMPLETED) : jsonPWQ.commandRespond(CMD_DENIED);
  if (js_cplt) {
    pubMsgDevice(js_cplt);
  }
  return;
}
// void FuncMain::handleMsgSetting(const char *topic, char *msg) {
//   // if (strstr(topic, TOPIC_DEVICE)) {
//   //   jsonPWQ.handleMsgSettingDevice((char *)msg, moduleStg, paramPond, paramReagent, paramSensor);
//   // } else if (strstr(topic, "ponds/")) {
//   //   jsonPWQ.handleMsgSettingPond((char *)msg, paramPond);
//   // }
//   return;
// }
// bool FuncMain::handleGetStgPond() {
//   // static uint8_t pond_current = 0;
//   // if (pond_current < NUM_POND) {
//   //   // Serial.print("pond: " + String(pond_current) + ": ");
//   //   if (paramPond[pond_current].pond_id != "") {
//   //     simA7xxx.subTopicPond(paramPond[pond_current].pond_id);
//   //     // Serial.println(paramPond[pond_current].pond_id);
//   //   }
//   //   pond_current++;
//   //   return false;
//   // } else {
//   //   pond_current = 0;
//   //   return true;
//   // }
//   return false;
// }
void FuncMain::pubMsgDevice(char *msg) {
  // Serial.print("in js: ");
  // Serial.println(msg);
  simA7xxx.pubStatus(msg);
  return;
}
// void FuncMain::pubMsgPond(char *msg, uint8_t pond) {
//   // Serial.print("in js: ");
//   // Serial.println(msg);
//   //simA7xxx.pubMsgMQTT(msg, paramPond[pond].pond_id);
//   return;
// }
// void FuncMain::pubLogsMQTT(char *msg) {
//   // char *js_logs = jsonPWQ.msgLogs(msg);
//   // // Serial.print("logs: ");
//   // // Serial.println(js_logs);
//   // if (js_logs) {
//   //   //simA7xxx.pubLogs(js_logs);
//   // }
//   return;
// }
void FuncMain::classifyFuncRun() {
  static char last_state[32] = "";
  if (last_state[0] == '\0') {
    GeneralFunction::copyString(last_state, MACHINE_STATUS_DISCONNECTED, sizeof(last_state));
  }
  if (!GeneralFunction::compareTwoCharArray(moduleStg.current_state, last_state)) {
    GeneralFunction::wrapperStatus(last_state, moduleStg.current_state);
    char *js_state = jsonPWQ.msgStatus((char *)moduleStg.current_state);
    if (js_state) {
      pubMsgDevice(js_state);
    }
  }
  if (GeneralFunction::compareTwoCharArray(moduleStg.current_state, MACHINE_STATUS_RUNNING)) {
    handleStateRunning();
  }
  //  else if (GeneralFunction::compareTwoCharArray(moduleStg.current_state, MACHINE_STATUS_CALIB)) {
  //   handleStateCalib();
  //}
  else if (GeneralFunction::compareTwoCharArray(moduleStg.current_state, MACHINE_STATUS_CONNECTED)) {
    handleStateConnected();
  }
  //else if (GeneralFunction::compareTwoCharArray(moduleStg.current_state, MACHINE_STATUS_MANUAL)) {
  //   handleStateManual();
  // }
  return;
}
void FuncMain::handleStateRunning() {
  static uint32_t timer_print = 0;
  if (timer_print == 0 || millis() - timer_print > 30000) {
    Serial.print("firmware: ");
    Serial.println(FIRM_WARE);
    Serial.print("device id: ");
    Serial.println(DEVICE_ID);
    timer_print = millis();
  }
  return;
}
// void FuncMain::handleStateCalib() {
//   return;
// }
void FuncMain::handleStateConnected() {
  if (_first_run) {
    GeneralFunction::wrapperStatus(moduleStg.current_state, MACHINE_STATUS_RUNNING);
  }
  // else {
  //   rstVariables();
  // }
  return;
}
// void FuncMain::handleStateManual() {
//   // if (runManual()) {
//   //   char *js_cplt = jsonPWQ.commandRespond(CMD_MANUAL_CPLT);
//   //   if (js_cplt) {
//   //     pubMsgDevice(js_cplt);
//   //   }
//   //   controlManual.cmd_code = -1;
//   //   // GeneralFunction::wrapperStatus(moduleStg.current_state, MACHINE_STATUS_CONNECTED);
//   // }
//   return;
// }