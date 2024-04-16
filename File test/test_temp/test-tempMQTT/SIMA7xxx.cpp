//Đã mapping được -> remove, tính toán mapping khỏi nhầm lẫn ao, lấy setting gửi quy trinh
#include "SIMA7xxx.h"
#define SerialAT Serial1
CLASS_JSONPWQD simJson;
bool CLASS_SIMA7xxx::Func_SerialSIM(char* AT, char* ATCheck) {

  for (int i = 0; i < 10; i++) {
    SerialAT.print(AT);
    String input = SerialAT.readString();
    Serial.println(input);
    if (input.indexOf(ATCheck) >= 0) {
      return 1;
    } else
      return 0;
  }
  return 0;
}
void CLASS_SIMA7xxx::Func_ResetVariable() {
  Func_RunningConnect = &CLASS_SIMA7xxx::Func_CheckSIM;
  Func_RunningFollowTopic = &CLASS_SIMA7xxx::Func_FollowTopic1;
  return;
}
bool CLASS_SIMA7xxx::Func_ConfigSIM() {
  uint32_t baud = 0;
  baud = TinyGsmAutoBaud(SerialAT);
  if (!baud) {
    Serial.println(F(" Module does not respond!"));
    return 0;
  } else {
    SerialAT.begin(baud);
    SerialAT.setTimeout(150);
    Func_ResetVariable();
    return 1;
  }
}
bool CLASS_SIMA7xxx::Func_ConnectSim() {
  bool done = (this ->*Func_RunningConnect)();
  return done;
}
bool CLASS_SIMA7xxx::Func_CheckSIM() {
  bool AT = 0;
  bool ATCSQ    = 0;    //Query signal quality (FeedBack rssi)
  bool ATCREG   = 0;    //Network registration (Check đã đk mạng chưa)
  bool ATCGREG  = 0;    //EPS Network registration
  bool ATCPSI   = 0;    //Inquiring UE system information
  bool ATCGACT  = 0;    //PDP context activate or deactivate
  AT = Func_SerialSIM("AT\r\n", "OK");
  if (AT) {
    ATCSQ = !Func_SerialSIM("AT+CSQ\r\n", "+CSQ: 99,99");
    if (ATCSQ) {
      ATCREG = !Func_SerialSIM("AT+CREG?\r\n", "+CMEERROR");
      if (ATCREG) {
        ATCGREG = !Func_SerialSIM("AT+CGREG?\r\n", "+CMEERROR");
        if (ATCGREG) {
          ATCPSI = !Func_SerialSIM("AT+CPSI?\r\n", "+CPSI: NOSERVICE,Online");
          if ( ATCPSI ) {
            ATCGACT = Func_SerialSIM("AT+CGACT?\r\n", "+CGACT: 1,1");
            Func_RunningConnect = & CLASS_SIMA7xxx::Func_CheckMQTT;
          }
        }
      }
    }
  }
  return 0;
}
bool CLASS_SIMA7xxx::Func_CheckMQTT() {
  char ATCommand[255];
  bool ATCMQTTSTART     = 0;  //Start MQTT service
  bool ATCMQTTACCQ      = 0;  //Acquire a clinent
  bool ATCMQTTWILLTOPIC = 0;  //Set the topic of "will message"
  bool ATCMQTTWILLMSG   = 0;  //Set the message of "will message"
  bool ATCMQTTCONNECT   = 0;  //Connect to MQTT server
  ATCMQTTSTART = Func_SerialSIM("AT+CMQTTSTART\r\n", "+CMQTTSTART: 0");
  if (ATCMQTTSTART) {
    sprintf(ATCommand, "AT+CMQTTACCQ=0,\"%s\"\r\n", c_deviceID);
    ATCMQTTACCQ = Func_SerialSIM(ATCommand, "OK");
    if (ATCMQTTACCQ) {
      sprintf(ATCommand, "AT+CMQTTWILLTOPIC=0,%d\r\n", strlen(c_topicDeviceID));
      Func_SerialSIM(ATCommand, "OK");
      sprintf(ATCommand, "%s\r\n", c_topicDeviceID);
      ATCMQTTWILLTOPIC  =  Func_SerialSIM(ATCommand, "OK");
      if ( ATCMQTTWILLTOPIC) {
        //char* json = simJson.Func_SerializeStatus(c_mesLost, c_deviceID, 0);
        char* json = "aaaaaaaa";
        sprintf(ATCommand, "AT+CMQTTWILLMSG=0,%d,1\r\n", strlen(json));
        Func_SerialSIM(ATCommand, "OK");
        ATCMQTTWILLMSG = Func_SerialSIM(json, "OK");
        //Serial.println(json);
        delete[] json;
        if (ATCMQTTWILLMSG) {
          sprintf(ATCommand, "AT+CMQTTCONNECT=0,\"%s\",1,1,\"%s\",\"%s\"\r\n", c_host, c_user, c_pass);
          ATCMQTTCONNECT = Func_SerialSIM(ATCommand, "OK");
          if (ATCMQTTCONNECT) {
            Func_RunningConnect = & CLASS_SIMA7xxx::Func_ConnectDone;
            return 0;
          }
        }
      }
    }
  }
  Func_RunningConnect = & CLASS_SIMA7xxx::Func_DisConnectSIM;
  return 0;
}
bool CLASS_SIMA7xxx::Func_ConnectDone() {
  return 1;
}
bool CLASS_SIMA7xxx::Func_DisConnectSIM() {
  bool ATCMQTTDISC = 0;
  bool ATCMQTTREL  = 0;
  bool ATCMQTTSTOP = 0;
  ATCMQTTDISC = Func_SerialSIM("AT+CMQTTDISC=0,120\r\n", "+CMQTTDISC: 0,0");
  ATCMQTTREL  = Func_SerialSIM("AT+CMQTTREL=0\r\n", "OK");
  ATCMQTTSTOP = Func_SerialSIM("AT+CMQTTSTOP\r\n", "+CMQTTSTOP: 0");
  if (ATCMQTTSTOP)
    Func_RunningConnect = & CLASS_SIMA7xxx::Func_CheckMQTT;
  else {
    Func_ResetSIM();
    Func_RunningConnect = & CLASS_SIMA7xxx::Func_CheckSIM;
  }
  return 0;
}
bool CLASS_SIMA7xxx::Func_ResetSIM() {
  bool done = Func_SerialSIM("AT+CRESET\r\n", "OK");
  delay(4000);
  if (done) {
    return 1;
  }
  return 0;
}
bool CLASS_SIMA7xxx::Func_FollowTopic() {
  bool done = (this ->*Func_RunningFollowTopic)();
  return done;
}
bool CLASS_SIMA7xxx::Func_FollowTopic1() {
  bool done = Func_FollowTopicSIM(c_topicDeviceID);
  if (!done)
    Func_ResetVariable();
  else {
    b_newsetting = 1;
    Func_RunningFollowTopic = &CLASS_SIMA7xxx::Func_FollowTopic2;
  }
  return 0;
}
bool CLASS_SIMA7xxx::Func_FollowTopic2() {
  char *mes = nullptr;
  if (SerialAT.available() > 0)
    mes = Func_SubMesSIM();
  if (mes != nullptr) {
    Serial.print("mes nhận được là: ");
    Serial.println(mes);
    if (strstr(mes, "\"message_type\":\"command\""))
      Func_UpdateCommand(mes);
    else if (strstr(mes, "\"message_type\":\"settings\""))
      Func_UpdateSetting(mes);
    else if (strstr(mes, "\"message_type\":\"respond_time\""))
      Func_UpdateTime(mes);

    delete[] mes;
  }
  return 1;
}
void CLASS_SIMA7xxx::Func_UpdateSetting(char* mes) {
  Serial.println("update setting");
  memset(c_setting, 0, sizeof(c_setting));
  strcpy(c_setting, mes);
  b_newsetting = 1;
  Serial.println(c_setting);
  return;
}
void CLASS_SIMA7xxx::Func_UpdateCommand(char* mes) {
  Serial.print("update command: ");
  Serial.println(mes);
  char* commandrespond = nullptr;
  if (strstr(mes, "\"command\":\"off\"")) {
    ui8_command = 0;
    commandrespond = new char[4];
    strcpy(commandrespond, "off");
  }
  else if (strstr(mes, "\"command\":\"on\"")) {
    ui8_command = 1;
    commandrespond = new char[3];
    strcpy(commandrespond, "on");
  }
  else if (strstr(mes, "\"command\":\"measure_now\"")) {
    Serial.print("mesnow update: ");
    memset(c_meanow, 0, sizeof(c_meanow));
    strcpy(c_meanow, mes);
    Serial.println(c_meanow);
    ui8_command = 2;
    b_newsetting = 1;
    commandrespond = new char[13];
    strcpy(commandrespond, "measure_now");
  }
  if (commandrespond != nullptr) {
    //char* json = simJson.Func_SerializeCommand(commandrespond, c_deviceID);
    char* json = "bbbbbbbbbbb";
    Func_PubMesSIM(json, c_topicDeviceID);
    delete[] json;
    delete[] commandrespond;
  }
  return;
}
void CLASS_SIMA7xxx::Func_UpdateTime(char* mes) {
  Serial.print("update time: ");
  Serial.println(mes);
  //float value = simJson.Func_DeserializeTime( mes);
  float value = 9.9;
  if (value != 99999.0) {
    f_currentTime = value ;
    ui32_basetime = millis();
  }
  Serial.print("time: " + String(f_currentTime));
  return;
}

bool CLASS_SIMA7xxx::Func_FollowTopicSIM(char* topic) {
  char ATCommand [80];
  char ATCommand1[80];
  bool ATCMQTTSUBTOPIC = 0;
  bool ATCMQTTSUB = 0;
  sprintf(ATCommand1, "AT+CMQTTSUBTOPIC=0,%d,1\r\n", strlen(topic));
  sprintf(ATCommand, "%s\r\n", topic);
  Func_SerialSIM(ATCommand1, "OK");
  ATCMQTTSUBTOPIC = Func_SerialSIM(ATCommand, "OK");
  if (ATCMQTTSUBTOPIC) {
    ATCMQTTSUB = Func_SerialSIM("AT+CMQTTSUB=0\r\n", "+CMQTTSUB: 0,0");
  }
  return ATCMQTTSUB;
}
bool CLASS_SIMA7xxx::Func_UnFollowTopicSIM(char* topic) {
  char ATCommand [80];
  bool ATCMQTTUNSUB = 0;
  sprintf(ATCommand, "AT+CMQTTUNSUB=0,%d,0\r\n", strlen(topic));
  Func_SerialSIM(ATCommand, "OK");
  sprintf(ATCommand, "%s\r\n", topic);
  ATCMQTTUNSUB =  Func_SerialSIM(ATCommand, "+CMQTTUNSUB: 0,0");
  return ATCMQTTUNSUB;
}
bool CLASS_SIMA7xxx::Func_SubMes(char* topic) {
  char ATCommand [80];
  bool ATSUBMES = 0;
  sprintf(ATCommand, "AT+CMQTTSUB=0,%d,1\r\n", strlen(topic));
  Func_SerialSIM(ATCommand, "OK");
  sprintf(ATCommand, "%s\r\n", topic);
  ATSUBMES =  Func_SerialSIM(ATCommand, "+CMQTTSUB: 0,0");
  return ATSUBMES;
}
bool CLASS_SIMA7xxx::Func_UnSubMes(char* topic) {
  char ATCommand [80];
  bool ATUNSUBMES = 0;
  sprintf(ATCommand, "AT+CMQTTUNSUB=0,%d,0\r\n", strlen(topic));
  Func_SerialSIM(ATCommand, "OK");
  sprintf(ATCommand, "%s\r\n", topic);
  ATUNSUBMES =  Func_SerialSIM(ATCommand, "+CMQTTUNSUB: 0,0");
  return ATUNSUBMES;
}
char* CLASS_SIMA7xxx::Func_SubMesSIM() {
  String input_string = SerialAT.readString();
  input_string.replace(" ", "");
  uint16_t len = input_string.length() + 1;
  char* mes = new char[len];
  input_string.toCharArray( mes, len);
  Serial.println((char*)mes);
  Func_SplitString(mes);
  return mes;
}
void CLASS_SIMA7xxx::Func_SplitString(char*& mes) {
  if (strstr(mes, "CMQTTCONNLOST")) {
    Func_ResetVariable();
    mes = nullptr;
    return;
  }
  while (1) {
    char* posStart = strstr(mes, "+CMQTTRXPAYLOAD:");
    if (posStart == NULL)
      break;
    char* posFirst = strchr(posStart, '{');
    if (posFirst == NULL)
      break;
    char* posLast = strstr(mes, "+CMQTTRXEND");
    if (posLast == NULL || posLast <= posFirst)
      break;
    uint16_t len = posLast - posFirst ;
    char* mesRespond = new char[len];
    mesRespond[len] = '\0';
    strncpy(mesRespond, posFirst, len );
    if (mes != nullptr)
      delete[] mes;
    mes = new char[len];
    strcpy(mes, mesRespond);
    Serial.print("mes_split: " + String(len) + " ");
    Serial.println(mes);
    delete[] mesRespond;
    if (simJson.Func_IsJson(mes)) {
      return;
    }
    else {
      strncpy(mes, posLast + strlen("+CMQTTRXEND"), len );
    }
  }
  if (mes != NULL) {
    delete[] mes;
  }
  mes = nullptr;
  return;
}
bool CLASS_SIMA7xxx::Func_PubData(char* pondID, uint8_t gw, float value[]) {
  //  char* json = simJson.Func_SerializeData(c_deviceID, pondID, gw, value);
  //  char* topicPond = new char[strlen(pondID) + strlen(c_prexPond)];
  //  strcpy(topicPond, c_prexPond);
  //  strcat(topicPond, pondID);
  //  Func_PubMesSIM(json, topicPond);
  //  Serial.println(json);
  //  Serial.println(topicPond);
  //  delete [] json;
  //  delete [] topicPond;
  return 1;
}
void CLASS_SIMA7xxx::Func_PubStatus(char* stt, uint16_t error) {
  //  char* json = simJson.Func_SerializeStatus(stt, c_deviceID, error);
  //  Func_PubMesSIM(json, c_topicDeviceID);
  //  delete[] json;
  return;
}
void CLASS_SIMA7xxx::Func_PubRequest() {
  //  char* json = simJson.Func_SerializeRequestTime(c_deviceID);
  //  Func_PubMesSIM(json, c_topicDeviceID);
  //  delete[] json;
  return;
}
bool CLASS_SIMA7xxx::Func_PubMesSIM(char *mesPub, char *topic) {
  bool ATCMQTTTOPIC = 0;
  bool ATCMQTTPAYLOAD = 0;
  bool ATCMQTTPUB = 0;
  char ATCommand[80];
  sprintf(ATCommand, "AT+CMQTTTOPIC=0,%d\r\n", strlen(topic));
  Func_SerialSIM(ATCommand, "OK");
  sprintf(ATCommand, "%s\r\n", topic);
  ATCMQTTTOPIC = Func_SerialSIM(ATCommand, "OK");
  if (ATCMQTTTOPIC) {
    sprintf(ATCommand, "AT+CMQTTPAYLOAD=0,%d\r\n", strlen(mesPub));
    Func_SerialSIM(ATCommand, "OK");
    ATCMQTTPAYLOAD = Func_SerialSIM(mesPub, "OK");
    if (ATCMQTTPAYLOAD) {
      ATCMQTTPUB = Func_SerialSIM("AT+CMQTTPUB=0,1,60\r\n", "OK");
    }
  }
  if (ATCMQTTPUB)
    return 1;
  Serial.println("PUB K OK");
  while (!Func_ResetSIM());
  Func_ResetVariable();
  return 0;
}
char* CLASS_SIMA7xxx::Func_GetMesSetting() {
  Serial.print("lay setting: " + String(ui8_command) + " ");
  if (ui8_command == 1) {
    Serial.println(c_setting);
    if (strlen(c_setting) > 0) {
      uint16_t len = strlen(c_setting);
      char* mesres = new char[len];
      strcpy(mesres, c_setting);
      return mesres;
    }
  }
  else if (ui8_command == 2) {
    Serial.println(c_meanow);
    if (strlen(c_meanow) > 0) {
      uint16_t len = strlen(c_meanow);
      char* mesres = new char[len];
      strcpy(mesres, c_meanow);
      return mesres;
    }
  }
  return nullptr;
}
