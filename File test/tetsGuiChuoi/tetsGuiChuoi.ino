void setup() {
  Serial.begin(9600);

}

void loop() {
  bool K = testChuoi("ABC");
  if (K) {
    Serial.println("111");
    SA76xx_CallSerial("TOP");
  }
  delay(500);
}

bool testChuoi(char* R) {
  String T = "ABCZYX";
  uint8_t len = T.length();
  char OP[50];
  T.toCharArray(OP, len);
  Serial.println("22222222");
  Serial.println(R);
  Serial.println(OP);
  if (strstr(OP, R)) {
    return true;
  } else {
    return false;
  }
}


void  SA76xx_CallSerial(String AT) {
  String g_inputString = "";
  uint8_t i = 0;
  uint8_t index = AT.length();
  Serial.println(AT);
  delay(1000);
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if ((inChar != '\n') && (inChar != '\r')) {
      g_inputString += inChar;
    }
  }
  uint8_t len = g_inputString.length()+1;
  char input[len];
  g_inputString.toCharArray(input,len);
}
