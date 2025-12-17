#include "lora_master.h"
RH_RF95 rf95(nss_lora, di0_lora);
JsonPWQ jsonLora;
void LoraMaster::setUp() {
  loraErr = 0;
  timesErr = 0;
  SPI.setMISO(mis_lora);
  SPI.setMOSI(mos_lora);
  SPI.setSCLK(sck_lora);
  if (!rf95.init()) {
    loraErr = ERR_INIT;
    return;
  }
  pHandleMes = &LoraMaster::sendMes;
  digitalWrite(rst_lora, 1);
  return;
}
bool LoraMaster::checkLoraError() {
  uint16_t timWait = 8000;
  if (millis() - waitLora > timWait) {
    timesErr++;
    if (timesErr >= 3) {
      loraErr = ERR_SEND_RECEIVE;
      timesErr = 0;
      return true;
    }
    waitLora = millis();
    pHandleMes = &LoraMaster::sendMes;
    return false;
  }
  return false;
}

bool LoraMaster::runProgram(bool enable, const char* moduleId, uint16_t timePump) {
  Serial.println(moduleId);
  Serial.println(timePump);
  if (loraErr == ERR_INIT) return false;
  char* mesSend = jsonLora.serMesLoraPump(enable, moduleId, timePump);
  char* mesRespond = nullptr;
  bool result = processMessage(mesSend, mesRespond);
  if (result && mesRespond) {
    result = jsonLora.derMesReceivePump(mesRespond, moduleId);
  }
  if (mesSend != nullptr)
    delete[] mesSend;
  if (mesRespond != nullptr)
    delete[] mesRespond;
  return result;
}
char* LoraMaster::runValueSensor(const char* moduleId) {
  if (loraErr == ERR_INIT || !moduleId) return nullptr;
  char* mesSend = jsonLora.serMesLoraSensor(moduleId);
  char* mesRespond = nullptr;
  bool result = processMessage(mesSend, mesRespond);
  if (result && mesRespond) {
    char* temp = new char[strlen(mesRespond) + 1];
    strcpy(temp, mesRespond);
    if (jsonLora.derMesReceiveSensor(temp, moduleId)) {
      delete[] mesSend;
      delete[] temp;
      return mesRespond; // Trả về mesRespond nếu thành công
    }
    if (temp != nullptr)
      delete[] temp;
  }
  if (mesSend != nullptr)
    delete[] mesSend;
  if (mesRespond != nullptr)
    delete[] mesRespond;
  return nullptr;
}
void LoraMaster::modulePondReady() {
  if (loraErr == ERR_INIT ) return ;
  char* mesSend = jsonLora.serTurnOnModuleSensor("commom");
  if (mesSend != nullptr) {
    uint8_t msg_send[RH_RF95_MAX_MESSAGE_LEN] = {0};
    size_t len = strlen(mesSend);
    memcpy(msg_send, mesSend, len > RH_RF95_MAX_MESSAGE_LEN ? RH_RF95_MAX_MESSAGE_LEN : len);
    Serial.println((char*)msg_send);
    rf95.send(msg_send, RH_RF95_MAX_MESSAGE_LEN);
    rf95.waitPacketSent();
    delete[] mesSend;
    delay(200);
  }
  return ;
}
bool  LoraMaster::setUpModuleSensor(const char* moduleId) {
  if (loraErr == ERR_INIT || !moduleId) return false;
  char* mesSend = jsonLora.serTurnOnModuleSensor(moduleId);
  char* mesRespond = nullptr;
  bool result  = processMessage(mesSend, mesRespond);
  //  if (result && mesRespond) {
  //    result = (strstr(mesRespond, moduleId) && strstr(mesRespond, "module_on"));
  //  }
  if (mesSend != nullptr)
    delete[] mesSend;
  if (mesRespond != nullptr)
    delete[] mesRespond;
  return result;
}
bool LoraMaster::processMessage(char* mesSend, char*& mesRespond) {
  waitLora = millis();
  timesErr = 0;
  bool done = false;
  while (!done && !checkLoraError()) {
    done = (this->*pHandleMes)(mesSend, mesRespond);
  }
  return done && !loraErr;
}
bool LoraMaster::sendMes(char* mesSend, char*& mesRespond) {
  if (!mesSend) return false;
  uint8_t msg_send[RH_RF95_MAX_MESSAGE_LEN] = {0};
  size_t len = strlen(mesSend);
  memcpy(msg_send, mesSend, len > RH_RF95_MAX_MESSAGE_LEN ? RH_RF95_MAX_MESSAGE_LEN : len);
  Serial.println((char*)msg_send);
  rf95.send(msg_send, RH_RF95_MAX_MESSAGE_LEN);
  rf95.waitPacketSent();
  pHandleMes = &LoraMaster::waitMes;
  return false; 
}
bool LoraMaster::waitMes(char* mesSend, char*& mesRespond) {
  uint32_t waitReceive = millis() + 1500;
  while (millis()  < waitReceive) {
    if (rf95.available())
    {
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      if (rf95.recv(buf, &len))
      {
        //Serial.println((char*)buf);
        if (mesRespond != nullptr)
          delete[] mesRespond;
        mesRespond = new char[len + 1];
        memcpy(mesRespond, buf, len);
        mesRespond[len] = '\0';
        Serial.println(mesRespond);
        pHandleMes = &LoraMaster::sendMes;
        return true;
      }
    }
  }
  pHandleMes = &LoraMaster::sendMes;
  return false;
}
