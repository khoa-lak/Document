#include <Base64.h>
#include "simA7600.h"
#define SerialAT Serial1
JsonPWQ jsonSim;
UnixStamp unixTime;
ParamPWQ paramSim;
bool SimA7600::serialSim(char* AT, char* ATCheck, uint8_t type) {
  //  Serial.print("lenh la: ");
  //  Serial.println(AT);
  SerialAT.println(AT);
  String input_string = "";
  uint32_t timeStart = millis();
  uint16_t timeRespond = 6000;
  while (millis() - timeStart < timeRespond) {
    if (SerialAT.available() > 0) {
      input_string = SerialAT.readString();
      input_string.replace(" ", "");
      if (input_string.indexOf(ATCheck) >= 0) {
        Serial.print("mes nhan duoc la: ");
        Serial.println(input_string);
        Serial.println("===============");
        Serial.println(ATCheck);
        return 1;
      }
      else {
        if (type == 1) {
          Serial.print("mes nhan duoc la111111111: ");
          Serial.println(input_string);
          Serial.println("===============");
          Serial.println(ATCheck);
          return 0;
        }
        else {
          Serial.print("mes loi: ");
          Serial.println(input_string);
          Serial.println("===============");
          Serial.println(ATCheck);
        }
      }
    }
  }
  return 0;
}
void SimA7600::setupSim() {
  P_PubMes = &SimA7600::pubMesSim;
  pConnect = &SimA7600::initConfigSim;
  simErr = 0;
  timesErr = 0;
  timesPub = 0;
  return;
}
void SimA7600::resetVariable() {
  P_PubMes = &SimA7600::pubMesSim;
  pConnect = &SimA7600::initConfigSim;
  return;
}
bool SimA7600::initSim() {
  bool done = (this ->*pConnect)();
  return done;
}
bool SimA7600::initConfigSim() {
  uint32_t baud = 0;
  baud = TinyGsmAutoBaud(SerialAT);
  if (!baud) {
    Serial.println("lỗi sim baudrate");
    delay(1000);
  }
  else {
    SerialAT.begin(baud);
    SerialAT.setTimeout(150);
    timesFail = 0;
    pConnect = &SimA7600::initConnectSim;
  }
  return 0;
}
bool SimA7600::initConnectSim() {
  bool AT = 0;
  bool ATCSQ    = 0;    //Query signal quality (FeedBack rssi)
  bool ATCREG   = 0;    //Network registration (Check đã đk mạng chưa)
  bool ATCGREG  = 0;    //EPS Network registration
  bool ATCPSI   = 0;    //Inquiring UE system information
  bool ATCGACT  = 0;    //PDP context activate or deactivate
  AT = serialSim("AT\r\n", "OK", 0);
  if (AT) {
    ATCSQ = !serialSim("AT+CSQ\r\n", "+CSQ:99,99", 1);
    if (ATCSQ) {
      ATCREG = !serialSim("AT+CREG?\r\n", "+CMEERROR", 1);
      if (ATCREG) {
        ATCGREG = !serialSim("AT+CGREG?\r\n", "+CMEERROR", 1);
        if (ATCGREG) {
          ATCPSI = !serialSim("AT+CPSI?\r\n", "+CPSI:NOSERVICE,Online", 1);
          if ( ATCPSI ) {
            ATCGACT = serialSim("AT+CGACT?\r\n", "+CGACT:1,1", 0);
            pConnect = & SimA7600::initConnectMQTT;
            timesFail = 0;
            return 0;
          }
        }
      }
    }
  }
  return 0;
}
bool SimA7600::initConnectMQTT() {
  char ATCommand[255];
  bool ATCMQTTSTART     = 0;  //Start MQTT service
  bool ATCMQTTACCQ      = 0;  //Acquire a clinent
  bool ATCMQTTWILLTOPIC = 0;  //Set the topic of "will message"
  bool ATCMQTTWILLMSG   = 0;  //Set the message of "will message"
  bool ATCMQTTCONNECT   = 0;  //Connect to MQTT server
  ATCMQTTSTART = serialSim("AT+CMQTTSTART\r\n", "+CMQTTSTART:0", 0);
  if (ATCMQTTSTART) {
    sprintf(ATCommand, "AT+CMQTTACCQ=0,\"%s\",1\r\n", paramSim.deviceID);
    ATCMQTTACCQ = serialSim(ATCommand, "OK", 0);
    if (ATCMQTTACCQ) {
      sprintf(ATCommand, "AT+CMQTTWILLTOPIC=0,%d\r\n", strlen(paramSim.topicDeviceID));
      serialSim(ATCommand, ">", 0);
      sprintf(ATCommand, "%s\r\n", paramSim.topicDeviceID);
      ATCMQTTWILLTOPIC  =  serialSim(ATCommand, "OK", 0);
      if ( ATCMQTTWILLTOPIC) {
        char* json = jsonSim.serMesSate(paramSim.mesLost);
        sprintf(ATCommand, "AT+CMQTTWILLMSG=0,%d,1\r\n", strlen(json));
        serialSim(ATCommand, ">", 0);
        ATCMQTTWILLMSG = serialSim(json, "OK", 0);
        delete[] json;
        if (ATCMQTTWILLMSG) {
          sprintf(ATCommand, "AT+CMQTTCONNECT=0,\"%s\",60,1,\"%s\",\"%s\"\r\n", paramSim.hostMQTT, paramSim.userMQTT, paramSim.passMQTT);
          ATCMQTTCONNECT = serialSim(ATCommand, "+CMQTTCONNECT:0,0", 0); //
          if (ATCMQTTCONNECT) {
            pConnect = &SimA7600::initConnectMapping;
            return 0;
          }
        }
      }
    }
  }
  pConnect = &SimA7600::initDisConnectMQTT;
  return 0;
}
bool SimA7600::initConnectMapping() {
  //  if (processMapping())
  pConnect = &SimA7600::initConnectTopicDevice;
  return 0;
}
bool SimA7600::initConnectTopicDevice() {
  for (uint8_t i = 0 ; i < 5 ; i++) {
    if (followTopicSim(paramSim.topicDeviceID)) {
      char* json = jsonSim.serMesSate(paramSim.mesRun);
      pubStatus(json);
      delete[] json;
      timesFail = 0;
      cmdRun = true;
      pConnect = & SimA7600::initConnectDone;
      return 0;
    }
  }
  pConnect = & SimA7600::initDisConnectMQTT;
  return 0;
}
bool SimA7600::initConnectDone() {
  return 1;
}
bool SimA7600::initDisConnectMQTT() {
  bool ATCMQTTDISC = 0;
  bool ATCMQTTREL  = 0;
  bool ATCMQTTSTOP = 0;
  ATCMQTTDISC = serialSim("AT+CMQTTDISC=0,120\r\n", "+CMQTTDISC:0,0", 0);
  ATCMQTTREL  = serialSim("AT+CMQTTREL=0\r\n", "OK", 0);
  ATCMQTTSTOP = serialSim("AT+CMQTTSTOP\r\n", "+CMQTTSTOP:0", 0);
  if (ATCMQTTSTOP)
    pConnect = &SimA7600::initConnectMQTT;
  else {
    resetSim();
    pConnect = &SimA7600::initConnectSim;
  }
  return 0;
}
bool SimA7600::resetSim() {
  serialSim("AT+CRESET\r\n", "OK", 0);
  delay(20000);
  return 0;
}
bool SimA7600::runCMD() {
  return cmdRun;
}
void SimA7600::controlCMDRun(bool cmd) {
  cmdRun = cmd;
  char* json = nullptr;
  if (cmd)
    json = jsonSim.serMesSate(paramSim.mesRun);
  else
    json = jsonSim.serMesSate(paramSim.mesConnect);
  if (json != nullptr) {
    pubStatus(json);
    delete[] json;
  }
  return ;
}
bool SimA7600::followTopic() {
  char *mes = nullptr;
  if (SerialAT.available() > 0)
    mes = subMesSim();
  if (mes != nullptr) {
    if (strstr(mes, "CMQTTPUB")) {
      delete[] mes;
      return 0;
    }
    else if (strstr(mes, "devices/")) {
      mesDevice(mes);
    }
    //    else if (strstr(mes, "ponds/")) {
    //      mesPond(mes);
    //    }
    delete[] mes;
  }
  return 0;
}
void SimA7600::mesDevice(char* mes) {
  splitString(mes);
  if (strstr(mes, "\"command\":")) {
    Serial.println(mes);
    if (strstr(mes, "\"command\":\"on\"")) {
      char* js = jsonSim.serCommandComplete();
      pubStatus(js);
      delete[] js;
      char* json = jsonSim.serMesSate(paramSim.mesRun);
      pubStatus(json);
      delete[] json;
      cmdRun = true;
    }
    else if (strstr(mes, "\"command\":\"off\"")) {
      char* js = jsonSim.serCommandComplete();
      pubStatus(js);
      delete[] js;
      char* json = jsonSim.serMesSate(paramSim.mesConnect);
      pubStatus(json);
      delete[] json;
      cmdRun = false;
    }
    else if (strstr(mes, "\"command\":\"measure_now\"")) {
      if (!cmdRun)
        return;
      reagentNow = jsonSim.derMeasureNow(mes, paramPond, paramReagent, paramSensor);
      if (reagentNow != 9999) {
        char* js = jsonSim.serCommandComplete();
        pubStatus(js);
        delete[] js;
      }
    }
    else if (strstr(mes, "\"command\":\"init_tube\"")) {
      char* js = jsonSim.serCommandComplete();
      pubStatus(js);
      delete[] js;
      initalTube = true;
    }
    else if (strstr(mes, "\"command\":\"update_setting_now\"")) {
      char* js = jsonSim.serCommandComplete();
      pubStatus(js);
      delete[] js;
      getSetting = true;
    }
  }
  //  else if (strstr(mes, "\"message_type\":\"settings\"")) {
  //    mappingSettingDevice(mes);
  //  }
  return;
}
void SimA7600::mesPond(char* mes) {
  splitString(mes);
  if (strstr(mes, "\"message_type\":\"settings\"")) {
    for (uint8_t i = 0; i < NUM_POND; i++) {
      if (strstr(mes, paramPond[i].id)) {
        if (paramPond[i].sche != nullptr)
          delete[] paramPond[i].sche;
        paramPond[i].sche = new char[strlen(mes) + 1];
        strcpy(paramPond[i].sche, mes);
        break;
      }
    }
  }
  return;
}
char* SimA7600::subMesSim() {
  String input_string = "";
  uint32_t timeStart = millis();
  uint16_t timeRespond = 6000;
  while (millis() - timeStart < timeRespond) {
    if (SerialAT.available() > 0) {
      input_string += SerialAT.readString();
      input_string.replace(" ", "");
    }
    if (input_string.indexOf("+CMQTTRXEND:0") >= 0) {
      uint16_t len = input_string.length();
      char* mes = new char[len + 1];
      input_string.toCharArray( mes, len);
      mes[len] = '\0';
      return mes;
    } else if (input_string.indexOf("ATREADY") >= 0 || input_string.indexOf("CMQTTCONNLOST") >= 0) {
      resetVariable();
      return nullptr;
    }
  }
  return nullptr;
}
void SimA7600::splitString(char*& mes) {
  char* ref1 = "\r\n+CMQTTRXPAYLOAD";
  char* ref2 = "\r\n+CMQTTRXEND";
  char* posFirst = strstr(mes, ref1);
  while ( char *match = strstr(posFirst, ref1)) {
    char *lineBreak = strstr(match + 2, "\r\n");
    if (lineBreak != NULL) {
      size_t len = lineBreak - match + 2  ;
      memmove(match, lineBreak + 2, strlen(lineBreak - 2));
    }
  }
  char* posLast = strstr(posFirst, ref2);
  uint16_t len = strlen(posFirst) - strlen(posLast);
  char mes_temp[len + 1];
  strncpy(mes_temp, posFirst, len);
  mes_temp[len] = '\0';
  if (strchr(mes, '{')) {
    if (mes != nullptr) {
      delete[] mes;
    }
    mes = new char[len + 1];
    strncpy(mes, mes_temp, len);
    mes[len] = '\0';
    //    Serial.print("json: ");
    //    Serial.println(mes);
  }
  else {
    uint16_t decoded_len = Base64.decodedLength(mes_temp, sizeof(mes_temp));
    char mes_decode[decoded_len];
    Base64.decode(mes_decode, mes_temp, sizeof(mes_temp));
    if (mes != nullptr) {
      delete[] mes;
    }
    mes = new char[decoded_len + 1];
    strncpy(mes, mes_decode, decoded_len);
    mes[decoded_len] = '\0';
    //    Serial.print("base64: ");
    //    Serial.println(mes);
  }
  return;
}

bool SimA7600::followTopicSim(char* topic) {
  char ATCommand [80];
  char ATCommand1[80];
  bool ATCMQTTSUBTOPIC = 0;
  bool ATCMQTTSUB = 0;
  sprintf(ATCommand1, "AT+CMQTTSUBTOPIC=0,%d,0\r\n", strlen(topic));
  sprintf(ATCommand, "%s\r\n", topic);
  //Serial.println(ATCommand);
  serialSim(ATCommand1, ">", 0);
  ATCMQTTSUBTOPIC = serialSim(ATCommand, "OK", 0);
  if (ATCMQTTSUBTOPIC) {
    ATCMQTTSUB = serialSim("AT+CMQTTSUB=0\r\n", "OK", 0);
  }
  return ATCMQTTSUB;
}
bool SimA7600::unFollowTopicSim(char* topic) {
  char ATCommand [80];
  bool ATCMQTTUNSUB = 0;
  sprintf(ATCommand, "AT+CMQTTUNSUB=0,%d,0\r\n", strlen(topic));
  serialSim(ATCommand, "OK", 0);
  sprintf(ATCommand, "%s\r\n", topic);
  ATCMQTTUNSUB =  serialSim(ATCommand, "+CMQTTUNSUB:0,0", 0);
  return ATCMQTTUNSUB;
}
bool SimA7600::pubStatus(char* mes) {
  bool done = (this->*P_PubMes)(mes, paramSim.topicDeviceID);
  return done;
}

bool SimA7600::pubData(char* mes, char* pondID) {
  char topic[255] = {'\0'};
  sprintf(topic, "ponds/%s", pondID);
  bool done = (this->*P_PubMes)(mes, topic);
  return done;
}
bool SimA7600::pubLogs(char* mes) {
  char topic[255] = {'\0'};
  sprintf(topic, "%s/check_logs", paramSim.topicDeviceID);
  char* js = jsonSim.serMesLogs(mes);
  bool done = (this->*P_PubMes)(mes, topic);
  delete[] js;
  return done;
}
bool SimA7600::pubMesSim(char *mesPub, char *topic) {
  Serial.print("in tại topic: ");
  Serial.println(topic);
  Serial.println(mesPub);
  bool ATCMQTTTOPIC = 0;
  bool ATCMQTTPAYLOAD = 0;
  bool ATCMQTTPUB = 0;
  char ATCommand[80];
  sprintf(ATCommand, "AT+CMQTTTOPIC=0,%d\r\n", strlen(topic));
  if (serialSim(ATCommand, ">", 0)) {
    sprintf(ATCommand, "%s\r\n", topic);
    ATCMQTTTOPIC = serialSim(ATCommand, "OK", 0);
    if (ATCMQTTTOPIC) {
      sprintf(ATCommand, "AT+CMQTTPAYLOAD=0,%d\r\n", strlen(mesPub));
      serialSim(ATCommand, ">", 0);
      ATCMQTTPAYLOAD = serialSim(mesPub, "OK", 0);
      if (ATCMQTTPAYLOAD) {
        ATCMQTTPUB = serialSim("AT+CMQTTPUB=0,2,60\r\n", "+CMQTTPUB", 0);
      }
    }
    //Serial.println("times err: " + String(timesPub));
    if (ATCMQTTPUB) {
      timesPub = 0;
      return 1;
    }
  }
  Serial.println("PUB K OK");
  timesPub++;
  if (timesPub >= 5) {
    timesPub = 0;
    resetVariable();
  }
  return 0;
  //return 1;
}

//http
char* SimA7600::connectHTTP(char* url, uint8_t mode) {
  bool ATHTTPINIT     = 0;
  bool ATHTTPPARA     = 0;
  bool ATHTTPPARA_AU  = 1;
  bool ATHTTPACTION   = 0;
  bool ATHTTPREAD     = 0;
  bool ATHTTPTERM     = 0;
  ATHTTPINIT = serialSim("AT+HTTPINIT\r\n", "OK", 0);
  if (ATHTTPINIT) {
    char ATCommand[255];
    sprintf(ATCommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", url);
    ATHTTPPARA = serialSim(ATCommand, "AT+HTTPPARA", 0);
    if ( ATHTTPPARA) {
      //      if (mode) {
      //        sprintf(ATCommand, "AT+HTTPPARA=\"USERDATA\",\"Authorization: Bearer %s\"\r\n", paramSim.base64);
      //        //sprintf(ATCommand, "AT+HTTPPARA=\"USERDATA\",\"Authorization: Bearer cHdxLWI5MDAyZTIwNDhmZTQ0ZTU5NGM1MTUzNWJmOTNkNzcw\"\r\n");  //"USERDATA","Authorization: Bearer cHdxLWI5MDAyZTIwNDhmZTQ0ZTU5NGM1MTUzNWJmOTNkNzcw"
      //        ATHTTPPARA_AU  = serialSim(ATCommand, "AT+HTTPPARA", 0);
      //      }
      if (ATHTTPPARA_AU) {
        ATHTTPACTION = serialSim("AT+HTTPACTION=0\r\n", "+HTTPACTION:", 0);
        //delay(100);
        //        Serial.println("vô:");
        if (ATHTTPACTION) {
          uint32_t timeStart = millis();
          uint16_t timeRespond = 2000;
          //          uint16_t MAX_STRING_LENGTH = 5000;
          //          char in_come[MAX_STRING_LENGTH] = {0};
          String input_string = "";
          //          int count = 0;
          SerialAT.println("AT+HTTPREAD=0,5000\r\n");
          while (millis() - timeStart < timeRespond) {
            while (SerialAT.available() > 0) {
              input_string = SerialAT.readString();
              input_string.replace(" ", "");
              //              in_come[count] = SerialAT.read();
              //              count++;
            }
            if (input_string.indexOf("+HTTPREAD:0") >= 0) {
              break;
            }
            //            if ( strstr(in_come, "+HTTPREAD:0"))
            //              break;
          }
          //          Serial.println("input_string: ");
          //          Serial.println(input_string);
          Serial.println(input_string);
          pubLogs((char*)input_string.c_str());
          char *pos = strchr(input_string.c_str(), '{');
          if (pos == nullptr)
            return nullptr;
          char *last = strstr(pos, "+HTTPREAD:0");
          if (last == nullptr)
            return nullptr;
          uint16_t len = last - pos - 2;
          char* mes = new char[len];
          strncpy(mes, pos, len);
          mes[len - 1] = '\0';
          ATHTTPTERM  = serialSim("AT+HTTPTERM\r\n", "OK", 0);
          return mes;
        }
      }
    }
  }
  ATHTTPTERM  = serialSim("AT+HTTPTERM\r\n", "OK", 0);
  return nullptr;
}
void SimA7600::removeSubstringHTTP(char*& mes, char *sub) {
  char* match = mes;
  size_t subLen = strlen(sub);
  //while ( char *match = strstr(mes, sub)) {
  while ((match = strstr(match, sub)) != nullptr) {
    char *lineBreak = strstr(match + 2, "\r\n");
    if (lineBreak != nullptr) {
      size_t len = lineBreak - match + 2  ;
      memmove(match, lineBreak + 2, strlen(lineBreak - 2));
    }
    else {
      size_t remainingLen = strlen(match + subLen);
      memmove(match, match + subLen, remainingLen);
    }
  }
  return;
}
bool SimA7600::getFileHex() {
  char* sub = "\r\n+HTTPREAD:";
  char* mes = connectHTTP(paramSim.api_file_hex, 0);
  if (mes == nullptr) {
    return 0;
  }
  removeSubstringHTTP(mes, sub);
  Serial.println(mes);
  delete[] mes;
  return 1;
}
