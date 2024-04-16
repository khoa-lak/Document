void setup() {
  Serial.begin(9600);
}

void loop() {
  char* mes = "+CMQTTRXSTART: 0,44,203+CMQTTRXTOPIC: 0,44device/swqd-930a3da167fa4ac2a28c0389fc17c89b+CMQTTRXPAYLOAD: 0,203{\"device\": \"water_quality_f9s7dga09sdfas0df\", \"message_type\": \"settings\", \"timestamp\": 1634534678, \"data\": {\"command\": \"start\", \"status\": \"on\", \"properties\": {\"interval\": 4, \"qty\": 4, \"params\": [\"kH\"]}}}+CMQTTRXEND: 0+CMQTTRXSTART: 0,42,201+CMQTTRXTOPIC: 0,42pond/pond-d7fd097fc29c49058067a909abdf4999+CMQTTRXPAYLOAD: 0,201{\"pond\": \"water_quality_f9s7dga09sdfas0df\", \"message_type\": \"settings\", \"timestamp\": 1634534678, \"data\": {\"command\": \"start\", \"status\": \"on\", \"properties\": {\"interval\": 4, \"qty\": 4, \"params\": [\"kH\"]}}}+CMQTTRXEND: 0";
  char* mesRespond;
  char nameDevice[] = "swqd-930a3da167fa4ac2a28c0389fc17c89b";
  char topicDevice[] = "device/";
  char finalString[strlen(topicDevice) + strlen(nameDevice) + 1];
  strcpy(finalString, topicDevice);
  strcat(finalString, nameDevice);
  char* condition = "pond/pond-d7fd097fc29c49058067a909abdf4999";
  Serial.println("Mes truoc khi cat: ");
  Serial.println(mes);
  Serial.println("topic device: ");
  Serial.println(finalString);
  bool success = Func_SplitString(mes, finalString , &mesRespond);
  if (success) {
    Serial.println("Mes da duoc xu ly: ");
    Serial.println(mesRespond);
  } else {
    Serial.println("Khong tim thay chuoi can cat.");
  }
  delay(2000);
}

bool Func_SplitString(char* mes, char* condition, char** mesRespond) {
  char* posCondition = strstr(mes, condition);
  if (posCondition == NULL) {
    return false;
  }
  char* posFirst = strstr(mes, "+CMQTTRXTOPIC");
  char* posTem = posFirst;
  while (posTem != NULL) {
    posTem = strstr(posTem + 1, "+CMQTTRXTOPIC");
    if (posTem != NULL && strlen(posTem) > strlen(posCondition)) {
      posFirst = posTem;
    } else {
      break;
    }
  }
  char* posSecond = strstr(posFirst, "+CMQTTRXEND");
  if (posSecond != NULL && strlen(posSecond) > strlen(posCondition)) {
    return false;
  }
  //  Serial.println(posFirst);
  //  Serial.println(posSecond);
  uint16_t len = posSecond - posFirst;
  //  Serial.println(len);
  char mesRespondTemp[len + 1];
  strncpy( mesRespondTemp, posFirst, len);
  mesRespondTemp[len] = '\0';
  char* posFirst1 = strchr(mesRespondTemp, '{');
  if (posFirst1 == NULL) {
    return false;
  }
  char* posLast1 = strrchr(mesRespondTemp, '}');
  if (posLast1 == NULL || posLast1 <= posFirst1) {
    return false;
  }
  uint16_t len1 = posLast1 - posFirst1 + 1;
  char mesRespondTemp1[len1 + 1];
  strncpy( mesRespondTemp1, posFirst1, len1);
  mesRespondTemp1[len1] = '\0';
  *mesRespond = mesRespondTemp1;
  //sprintf(*mesRespond, "%s\r\n", mesRespondTemp1);
  return true;
}
