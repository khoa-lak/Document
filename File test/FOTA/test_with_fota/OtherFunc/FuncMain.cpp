#include "FuncMain.h"
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
}
void FuncMain::runMain() {
  if (!GeneralFunction::compareTwoCharArray(stgDevice.current_state, MACHINE_STATUS_MANUAL))
    if (checkSim()) {
      handleStateRunning();
      //classifyFuncRun();
    }
  return;
}
bool FuncMain::checkSim() {
  if (simA7xxx.initSim()) {
    if (GeneralFunction::compareTwoCharArray(stgDevice.current_state, MACHINE_STATUS_DISCONNECTED)) {
      GeneralFunction::wrapperStatus(stgDevice.current_state, MACHINE_STATUS_CONNECTED);
    }
    simA7xxx.followMsgAnyTopic();
    return true;
  } else {
    if (!GeneralFunction::compareTwoCharArray(stgDevice.current_state, MACHINE_STATUS_DISCONNECTED))
      GeneralFunction::wrapperStatus(stgDevice.current_state, MACHINE_STATUS_DISCONNECTED);
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
  return;
}

void FuncMain::handleStateRunning() {
  static uint32_t timer_print = 0;
  if (timer_print == 0 || millis() - timer_print > 10000) {
    Serial.print("firmware: ");
    Serial.println(FIRM_WARE);
    Serial.print("device id: ");
    Serial.println(DEVICE_ID);
    timer_print = millis();
  }
  return;
}
