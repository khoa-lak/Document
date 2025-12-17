#include "SIMA7xxx.h"
#define SerialAT Serial1
CLASS_JSONPWQD simJson;
bool CLASS_SIMA7xxx::Func_SerialSim(char* AT, char* ATCheck, uint8_t type) {
  Serial.print("lenh la: ");
  Serial.println(AT);
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
      } else {
        if (type == 1) {
          Serial.print("mes nhan duoc la111111111: ");
          Serial.println(input_string);
          Serial.println("===============");
          Serial.println(ATCheck);
          return 0;
        } else {
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
void CLASS_SIMA7xxx::Func_SetupSim() {
  Func_ResetVariable();
  return;
}
void CLASS_SIMA7xxx::Func_ResetVariable() {
  Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConfigSim;
  return;
}
bool CLASS_SIMA7xxx::Func_InitSim() {
  bool done = (this->*Func_RunningConnect)();
  return done;
}
bool CLASS_SIMA7xxx::Func_ConfigSim() {
  uint32_t baud = 0;
  baud = TinyGsmAutoBaud(SerialAT);
  if (!baud) {
    Serial.println(F(" Module does not respond!"));
    delay(500);
  } else {
    Serial.println("baud la: " + String(baud));
    SerialAT.begin(baud);
    SerialAT.setTimeout(200);
    Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConnectSim;
  }
  return 0;
}

bool CLASS_SIMA7xxx::Func_ConnectSim() {
  bool AT = 0;
  bool ATCSQ = 0;    //Query signal quality (FeedBack rssi)
  bool ATCREG = 0;   //Network registration (Check đã đk mạng chưa)
  bool ATCGREG = 0;  //EPS Network registration
  bool ATCPSI = 0;   //Inquiring UE system information
  bool ATCGACT = 0;  //PDP context activate or deactivate
  AT = Func_SerialSim("AT\r\n", "OK", 0);
  if (AT) {
    ATCSQ = !Func_SerialSim("AT+CSQ\r\n", "+CSQ:99,99", 1);
    if (ATCSQ) {
      ATCREG = !Func_SerialSim("AT+CREG?\r\n", "+CMEERROR", 1);
      if (ATCREG) {
        ATCGREG = !Func_SerialSim("AT+CGREG?\r\n", "+CMEERROR", 1);
        if (ATCGREG) {
          ATCPSI = !Func_SerialSim("AT+CPSI?\r\n", "+CPSI:NOSERVICE,Online", 1);
          if (ATCPSI) {
            ATCGACT = Func_SerialSim("AT+CGACT?\r\n", "+CGACT:1,1", 0);
            Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConnectMQTT;

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
  bool ATCMQTTSTART = 0;      //Start MQTT service
  bool ATCMQTTACCQ = 0;       //Acquire a clinent
  bool ATCMQTTWILLTOPIC = 0;  //Set the topic of "will message"
  bool ATCMQTTWILLMSG = 0;    //Set the message of "will message"
  bool ATCMQTTCONNECT = 0;    //Connect to MQTT server
  ATCMQTTSTART = Func_SerialSim("AT+CMQTTSTART\r\n", "+CMQTTSTART:0", 0);
  if (ATCMQTTSTART) {
    sprintf(ATCommand, "AT+CMQTTACCQ=0,\"%s\",1\r\n", c_deviceID);
    ATCMQTTACCQ = Func_SerialSim(ATCommand, "OK", 0);
    if (ATCMQTTACCQ) {
      sprintf(ATCommand, "AT+CMQTTWILLTOPIC=0,%d\r\n", strlen(c_topicDeviceID));
      Func_SerialSim(ATCommand, ">", 0);
      sprintf(ATCommand, "%s\r\n", c_topicDeviceID);
      ATCMQTTWILLTOPIC = Func_SerialSim(ATCommand, "OK", 0);
      if (ATCMQTTWILLTOPIC) {
        char* json = simJson.jsonSerStatus("DISCONNECTED", c_deviceID);
        sprintf(ATCommand, "AT+CMQTTWILLMSG=0,%d,1\r\n", strlen(json));
        Func_SerialSim(ATCommand, ">", 0);
        ATCMQTTWILLMSG = Func_SerialSim(json, "OK", 0);
        delete[] json;
        if (ATCMQTTWILLMSG) {
          sprintf(ATCommand, "AT+CMQTTCONNECT=0,\"%s\",60,1,\"%s\",\"%s\"\r\n", c_host, c_user, c_pass);
          ATCMQTTCONNECT = Func_SerialSim(ATCommand, "+CMQTTCONNECT:0,0", 0);  //
          if (ATCMQTTCONNECT) {
            Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConnectTopicDevice;
            return 0;
          }
        }
      }
    }
  }
  Func_RunningConnect = &CLASS_SIMA7xxx::Func_DisConnectMQTT;
  return 0;
}
bool CLASS_SIMA7xxx::Func_ConnectTopicDevice() {
  for (uint8_t i = 0; i < 5; i++) {
    if (Func_FollowTopicSim(c_topicDeviceID)) {
      Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConnectDone;
      Func_PubStatus(c_mesConnect);
      return 0;
    }
  }
  Serial.println("aydau");
  //  / Func_RunningConnect = & CLASS_SIMA7xxx::Func_DisConnectMQTT;
  return 0;
}
bool CLASS_SIMA7xxx::Func_ConnectDone() {
  return 1;
}
bool CLASS_SIMA7xxx::Func_DisConnectMQTT() {
  bool ATCMQTTDISC = 0;
  bool ATCMQTTREL = 0;
  bool ATCMQTTSTOP = 0;
  ATCMQTTDISC = Func_SerialSim("AT+CMQTTDISC=0,120\r\n", "+CMQTTDISC: 0,0", 0);
  ATCMQTTREL = Func_SerialSim("AT+CMQTTREL=0\r\n", "OK", 0);
  ATCMQTTSTOP = Func_SerialSim("AT+CMQTTSTOP\r\n", "+CMQTTSTOP: 0", 0);
  if (ATCMQTTSTOP)
    Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConnectMQTT;
  else {
    Func_ResetSim();
    Func_RunningConnect = &CLASS_SIMA7xxx::Func_ConnectSim;
  }
  return 0;
}
bool CLASS_SIMA7xxx::Func_ResetSim() {
  //  Serial.println("reset");
  //  bool done = Func_Func_SerialSim("AT+CRESET\r\n", "OK");
  //  Func_ResetVariable();
  return 0;
}


bool CLASS_SIMA7xxx::Func_FollowTopic() {
  // uint32_t timer_delay = millis();
  // //while (millis() - timer_delay < 2000) {
  // char* mes = nullptr;
  // if (SerialAT.available())
  //   mes = Func_SubMesSim();
  char buffer[2048];  // Buffer đủ lớn cho tin nhắn 1183 bytes
  uint16_t index = 0;

  // Đọc hết dữ liệu từ SerialAT
  unsigned long startTime = millis();
  while (millis() - startTime < 5000 && index < sizeof(buffer) - 1) {
    while (SerialAT.available()) {
      buffer[index++] = SerialAT.read();
    }
    // delay(5);
  }
  buffer[index] = '\0';  // Kết thúc chuỗi

  if (index == 0) {
    return 0;  // Không có dữ liệu
  }

  // Sao chép vào bộ nhớ động
  uint16_t len = index + 1;
  char* mes = new char[len];
  memcpy(mes, buffer, len);
  Serial.print("Received message (");
  Serial.print(index);
  Serial.print(" bytes): ");
  Serial.println(mes);

  // Xử lý message
  Func_SplitString(mes);
  if (mes != nullptr) {
    Serial.print("có mes là: ");
    Serial.println(mes);
    //    if (strstr(mes, "\"message_type\":\"test\""))
    //      Func_RespondNumber(mes);
    delete[] mes;
  }
  //}
  return 1;
}
void CLASS_SIMA7xxx::Func_RespondNumber(char* mes) {
  char* json = simJson.jsonGetNumber(mes);
  Func_PubMesSim(json, c_topicPond);
  delete[] json;
  return;
}

bool CLASS_SIMA7xxx::Func_FollowTopicSim(char* topic) {
  char ATCommand[80];
  char ATCommand1[80];
  bool ATCMQTTSUBTOPIC = 0;
  bool ATCMQTTSUB = 0;
  sprintf(ATCommand1, "AT+CMQTTSUBTOPIC=0,%d,2\r\n", strlen(topic));
  sprintf(ATCommand, "%s\r\n", topic);
  Func_SerialSim(ATCommand1, ">", 0);
  ATCMQTTSUBTOPIC = Func_SerialSim(ATCommand, "OK", 0);
  if (ATCMQTTSUBTOPIC) {
    ATCMQTTSUB = Func_SerialSim("AT+CMQTTSUB=0\r\n", "OK", 0);
  }
  return ATCMQTTSUB;
  return false;
}
bool CLASS_SIMA7xxx::Func_UnFollowTopicSim(char* topic) {
  //  char ATCommand [80];
  //  bool ATCMQTTUNSUB = 0;
  //  sprintf(ATCommand, "AT+CMQTTUNSUB=0,%d,0\r\n", strlen(topic));
  //  Func_Func_SerialSim(ATCommand, "OK");
  //  sprintf(ATCommand, "%s\r\n", topic);
  //  ATCMQTTUNSUB =  Func_Func_SerialSim(ATCommand, "+CMQTTUNSUB: 0,0");
  //  return ATCMQTTUNSUB;
  return false;
}

char* CLASS_SIMA7xxx::Func_SubMesSim() {
  // String input_string = "";
  // uint32_t timer_delay = millis();
  // while (SerialAT.available()) {
  //   input_string += SerialAT.readString();
  // }
  // input_string.replace(" ", "");
  // uint16_t len = input_string.length() + 1;
  // char* mes = new char[len];
  // input_string.toCharArray(mes, len);
  // Serial.println((char*)mes);
  // Func_SplitString(mes);
  // return mes;
  char buffer[1200];  // Buffer đủ lớn cho tin nhắn 1183 bytes
  uint16_t index = 0;

  // Đọc hết dữ liệu từ SerialAT
  unsigned long startTime = millis();
  while (millis() - startTime < 5000 && index < sizeof(buffer) - 1) {
    if (SerialAT.available()) {
      buffer[index++] = SerialAT.read();
    }
  }
  buffer[index] = '\0';  // Kết thúc chuỗi

  if (index == 0) {
    return nullptr;  // Không có dữ liệu
  }

  // Sao chép vào bộ nhớ động
  uint16_t len = index + 1;
  char* mes = new char[len];
  memcpy(mes, buffer, len);
  Serial.print("Received message (");
  Serial.print(index);
  Serial.print(" bytes): ");
  Serial.println(mes);

  // Xử lý message
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
    uint16_t len = posLast - posFirst;
    char mesRespond[len + 1];
    strncpy(mesRespond, posFirst, len);
    mesRespond[len] = '\0';
    Serial.print("mes_split: ");
    Serial.println(mesRespond);
    if (simJson.Func_IsJson(mesRespond)) {
      char* json = simJson.jsonGetNumber(mesRespond);
      Func_PubMesSim(json, c_topicPond);
      delete[] json;
    }
    currentMes = posLast + strlen("+CMQTTRXEND");
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
bool CLASS_SIMA7xxx::Func_PubMesSim(char* mesPub, char* topic) {
  //  bool ATCMQTTTOPIC = 0;
  //  bool ATCMQTTPAYLOAD = 0;
  //  bool ATCMQTTPUB = 0;
  //  char ATCommand[80];
  //  sprintf(ATCommand, "AT+CMQTTTOPIC=0,%d\r\n", strlen(topic));
  //  Func_Func_SerialSim(ATCommand, ">");
  //  sprintf(ATCommand, "%s\r\n", topic);
  //  ATCMQTTTOPIC = Func_Func_SerialSim(ATCommand, "OK");
  //  if (ATCMQTTTOPIC) {
  //    sprintf(ATCommand, "AT+CMQTTPAYLOAD=0,%d\r\n", strlen(mesPub));
  //    Func_Func_SerialSim(ATCommand, ">");
  //    ATCMQTTPAYLOAD = Func_Func_SerialSim(mesPub, "OK");
  //    if (ATCMQTTPAYLOAD) {
  //      ATCMQTTPUB = Func_Func_SerialSim("AT+CMQTTPUB=0,2,60\r\n", "+CMQTTPUB");
  //    }
  //  }
  //  if (ATCMQTTPUB)
  //    return 1;
  //  Serial.println("PUB K OK");
  //  Func_ResetVariable();
  return 0;
}
