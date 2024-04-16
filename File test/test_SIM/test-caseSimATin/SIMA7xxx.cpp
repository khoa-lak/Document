#include "SIMA7xxx.h"
#define SerialAT Serial1
CLASS_JSONPWQD simJson;
bool CLASS_SIMA7xxx::Func_SerialSim(char* AT, char* ATCheck) {
  Serial.print("lenh la: ");
  Serial.println(AT);
  SerialAT.println(AT);
  String mesRespond = "";
  uint32_t timeStart = millis();
  uint16_t timeRespond = 6000;
  while (1) {
    if (SerialAT.available() > 0) {
      char inChar = (char)SerialAT.read();
      if ((inChar != '\n') && (inChar != '\r'))
        mesRespond += inChar;
    }
    if (mesRespond.indexOf(ATCheck) >= 0) {
      Serial.print("mes nhan duoc la: ");
      Serial.println(mesRespond);
      return 1;
    }
    if (millis() - timeStart > timeRespond) {
      Serial.print("mes k dung la: ");
      Serial.println(mesRespond);
      Serial.println(ATCheck);
      return 0;
    }
  }
  return 0; //=> khoảng 300ms

  //  String input =  "";
  //  for (int j = 0; j < 10; j++) {
  //    SerialAT.print(AT);
  //    input = SerialAT.readString();
  //    if (input.indexOf(ATCheck) >= 0) {
  //      Serial.print("mes nhan duoc la: ");
  //      Serial.println(input);
  //      return 1;
  //    }
  //  }
  //  Serial.print("mes k duoc la: ");
  //  Serial.println(input);
  //  return 0; => khoản 1s

  //  String input =  "";
  //  for (int j = 0; j < 10; j++) {
  //    SerialAT.print(AT);
  //    while (SerialAT.available()) {
  //      char inChar = (char)SerialAT.read();
  //      if ((inChar != '\n') && (inChar != '\r'))
  //        input += inChar;
  //      if (input.indexOf(ATCheck) >= 0) {
  //        Serial.print("mes nhan duoc la: ");
  //        Serial.println(input);
  //        return 1;
  //      }
  //      else {
  //        Serial.print("mes k duoc la: ");
  //        Serial.println(input);
  //        return 0;
  //      }
  //    }
  //  }
}
void CLASS_SIMA7xxx::Func_SetupSim() {
  Func_ResetVariable();
  return;
}
void CLASS_SIMA7xxx::Func_ResetVariable() {
  Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConfigSim;
  return;
}
bool CLASS_SIMA7xxx::Func_InitSim() {
  bool done = (this ->*Func_RunningConnect)();
  return done;
}
bool CLASS_SIMA7xxx::Func_ConfigSim() {
  uint32_t baud = 0;
  baud = TinyGsmAutoBaud(SerialAT);
  if (!baud) {
    Serial.println(F(" Module does not respond!"));
    delay(500);
  }
  else {
    Serial.println("baud la: " + String(baud));
    SerialAT.begin(baud);
    SerialAT.setTimeout(200);
    Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConnectSim;

  }
  return 0;
}

bool CLASS_SIMA7xxx::Func_ConnectSim() {
  bool AT = 0;
  bool ATCSQ    = 0;    //Query signal quality (FeedBack rssi)
  bool ATCREG   = 0;    //Network registration (Check đã đk mạng chưa)
  bool ATCGREG  = 0;    //EPS Network registration
  bool ATCPSI   = 0;    //Inquiring UE system information
  bool ATCGACT  = 0;    //PDP context activate or deactivate
  AT = Func_SerialSim("AT\r\n", "OK");
  if (AT) {
    ATCSQ = !Func_SerialSim("AT+CSQ\r\n", "+CSQ: 99,99");
    if (ATCSQ) {
      ATCREG = !Func_SerialSim("AT+CREG?\r\n", "+CMEERROR");
      if (ATCREG) {
        ATCGREG = !Func_SerialSim("AT+CGREG?\r\n", "+CMEERROR");
        if (ATCGREG) {
          ATCPSI = !Func_SerialSim("AT+CPSI?\r\n", "+CPSI: NOSERVICE,Online");
          if ( ATCPSI ) {
            ATCGACT = Func_SerialSim("AT+CGACT?\r\n", "+CGACT: 1,1");
            Func_RunningConnect = & CLASS_SIMA7xxx::Func_ConnectMQTT;
            return 0;
          }
        }
      }
    }
  }
  return 0;
}
bool CLASS_SIMA7xxx::Func_ConnectMQTT() {
  char ATCommand[255];
  bool ATCMQTTSTART     = 0;  //Start MQTT service
  bool ATCMQTTACCQ      = 0;  //Acquire a clinent
  bool ATCMQTTWILLTOPIC = 0;  //Set the topic of "will message"
  bool ATCMQTTWILLMSG   = 0;  //Set the message of "will message"
  bool ATCMQTTCONNECT   = 0;  //Connect to MQTT server
  ATCMQTTSTART = Func_SerialSim("AT+CMQTTSTART\r\n", "+CMQTTSTART: 0");
  if (ATCMQTTSTART) {
    sprintf(ATCommand, "AT+CMQTTACCQ=0,\"%s\"\r\n", c_deviceID);
    ATCMQTTACCQ = Func_SerialSim(ATCommand, "OK");
    if (ATCMQTTACCQ) {
      sprintf(ATCommand, "AT+CMQTTWILLTOPIC=0,%d\r\n", strlen(c_topicDeviceID));
      Func_SerialSim(ATCommand, ">");
      sprintf(ATCommand, "%s\r\n", c_topicDeviceID);
      ATCMQTTWILLTOPIC  =  Func_SerialSim(ATCommand, "OK");
      if ( ATCMQTTWILLTOPIC) {
        char* json = simJson.jsonSerStatus(c_mesLost, c_deviceID);
        sprintf(ATCommand, "AT+CMQTTWILLMSG=0,%d,1\r\n", strlen(json));
        Func_SerialSim(ATCommand, ">");
        ATCMQTTWILLMSG = Func_SerialSim(json, "OK");
        delete[] json;
        if (ATCMQTTWILLMSG) {
          sprintf(ATCommand, "AT+CMQTTCONNECT=0,\"%s\",60,1,\"%s\",\"%s\"\r\n", c_host, c_user, c_pass);
          ATCMQTTCONNECT = Func_SerialSim(ATCommand, "+CMQTTCONNECT: 0,0");
          if (ATCMQTTCONNECT) {
            Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConnectTopicDevice;
            return 0;
          }
        }
      }
    }
  }
  Func_RunningConnect = & CLASS_SIMA7xxx::Func_DisConnectMQTT;
  return 0;
}
bool CLASS_SIMA7xxx::Func_ConnectTopicDevice() {
  for (uint8_t i = 0 ; i < 5 ; i++) {
    if (Func_FollowTopicSim(c_topicDeviceID)) {
      Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConnectDone;
      Func_PubStatus(c_mesConnect);
      return 0;
    }
  }
  Serial.println("aydau");
  Func_RunningConnect = & CLASS_SIMA7xxx::Func_DisConnectMQTT;
  return 0;
}
bool CLASS_SIMA7xxx::Func_ConnectDone() {
  return 1;
}
bool CLASS_SIMA7xxx::Func_DisConnectMQTT() {
  bool ATCMQTTDISC = 0;
  bool ATCMQTTREL  = 0;
  bool ATCMQTTSTOP = 0;
  ATCMQTTDISC = Func_SerialSim("AT+CMQTTDISC=0,120\r\n", "+CMQTTDISC: 0,0");
  ATCMQTTREL  = Func_SerialSim("AT+CMQTTREL=0\r\n", "OK");
  ATCMQTTSTOP = Func_SerialSim("AT+CMQTTSTOP\r\n", "+CMQTTSTOP: 0");
  if (ATCMQTTSTOP)
    Func_RunningConnect = & CLASS_SIMA7xxx::Func_ConnectMQTT;
  else {
    Func_ResetSim();
    Func_RunningConnect = & CLASS_SIMA7xxx::Func_ConnectSim;
  }
  return 0;
}
bool CLASS_SIMA7xxx::Func_ResetSim() {
  Serial.println("reset");
  bool done = Func_SerialSim("AT+CRESET\r\n", "OK");
  Func_ResetVariable();
  return 0;
}


bool CLASS_SIMA7xxx::Func_FollowTopic() {
  char *mes = nullptr;
  if (SerialAT.available() > 0)
    mes = Func_SubMesSim();
  if (mes != nullptr) {

    //    if (strstr(mes, "\"message_type\":\"test\""))
    //      Func_RespondNumber(mes);
    delete[] mes;
  }
  //  Serial.print("có mes là: ");
  //  Serial.println(mes);
  return 1;
}
void CLASS_SIMA7xxx::Func_RespondNumber(char* mes) {
  char* json = simJson.jsonGetNumber(mes);
  Func_PubMesSim(json, c_topicPond);
  delete [] json;
  return;
}

bool CLASS_SIMA7xxx::Func_FollowTopicSim(char* topic) {
  char ATCommand [80];
  char ATCommand1[80];
  bool ATCMQTTSUBTOPIC = 0;
  bool ATCMQTTSUB = 0;
  sprintf(ATCommand1, "AT+CMQTTSUBTOPIC=0,%d,2\r\n", strlen(topic));
  sprintf(ATCommand, "%s\r\n", topic);
  Func_SerialSim(ATCommand1, ">");
  ATCMQTTSUBTOPIC = Func_SerialSim(ATCommand, "OK");
  if (ATCMQTTSUBTOPIC) {
    ATCMQTTSUB = Func_SerialSim("AT+CMQTTSUB=0\r\n", "OK");
  }
  return ATCMQTTSUB;
}
bool CLASS_SIMA7xxx::Func_UnFollowTopicSim(char* topic) {
  char ATCommand [80];
  bool ATCMQTTUNSUB = 0;
  sprintf(ATCommand, "AT+CMQTTUNSUB=0,%d,0\r\n", strlen(topic));
  Func_SerialSim(ATCommand, "OK");
  sprintf(ATCommand, "%s\r\n", topic);
  ATCMQTTUNSUB =  Func_SerialSim(ATCommand, "+CMQTTUNSUB: 0,0");
  return ATCMQTTUNSUB;
}

char* CLASS_SIMA7xxx::Func_SubMesSim() {
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
  if (strstr(mes, "CMQTTCONNLOST") || strstr(mes, "*ATREADY:1")) {
    Func_ResetVariable();
    delete[] mes;
    mes = nullptr;
    return;
  }
  char* currentMes = mes;
  while (1) {
    char* posStart = strstr(currentMes, "+CMQTTRXPAYLOAD:");
    if (posStart == NULL)
      break;
    char* posFirst = strchr(posStart, '{');
    if (posFirst == NULL)
      break;
    char* posLast = strstr(currentMes, "+CMQTTRXEND");
    if (posLast == NULL || posLast <= posFirst)
      break;
    uint16_t len = posLast - posFirst ;
    char mesRespond[len + 1];
    strncpy(mesRespond, posFirst, len );
    mesRespond[len] = '\0';
    Serial.print("mes_split: " );
    Serial.println(mesRespond);
    if (simJson.Func_IsJson(mesRespond)) {
      char* json = simJson.jsonGetNumber(mesRespond);
      Func_PubMesSim(json, c_topicPond);
      delete [] json;

    }
    currentMes =  posLast + strlen("+CMQTTRXEND");
    Serial.print("mes_còn lại là: ");
    Serial.println(currentMes);
  }

  delete[] mes;
  mes = nullptr;
  return;
}

void CLASS_SIMA7xxx::Func_PubStatus(char* stt) {
  char* json = simJson.jsonSerStatus(stt, c_topicDeviceID);
  Func_PubMesSim(json, c_topicDeviceID);
  delete[] json;
  return;
}
bool CLASS_SIMA7xxx::Func_PubMesSim(char *mesPub, char *topic) {
  bool ATCMQTTTOPIC = 0;
  bool ATCMQTTPAYLOAD = 0;
  bool ATCMQTTPUB = 0;
  char ATCommand[80];
  sprintf(ATCommand, "AT+CMQTTTOPIC=0,%d\r\n", strlen(topic));
  Func_SerialSim(ATCommand, ">");
  sprintf(ATCommand, "%s\r\n", topic);
  ATCMQTTTOPIC = Func_SerialSim(ATCommand, "OK");
  if (ATCMQTTTOPIC) {
    sprintf(ATCommand, "AT+CMQTTPAYLOAD=0,%d\r\n", strlen(mesPub));
    Func_SerialSim(ATCommand, ">");
    ATCMQTTPAYLOAD = Func_SerialSim(mesPub, "OK");
    if (ATCMQTTPAYLOAD) {
      ATCMQTTPUB = Func_SerialSim("AT+CMQTTPUB=0,2,60\r\n", "+CMQTTPUB");
    }
  }
  if (ATCMQTTPUB)
    return 1;
  Serial.println("PUB K OK");
  Func_ResetVariable();
  return 0;
}
