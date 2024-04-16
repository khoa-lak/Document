#define SerialAT Serial1
char prexHTTP[] = "https://be.iotoom.com/ponds/";
char pondID[] = "pond-d7fd097fc29c49058067a909abdf4999";
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  uint8_t len1 = strlen(prexHTTP);
  uint8_t len2 = strlen(pondID);
  char URL[len1 + len2] = {0};
  char ATcomand[100] = {0};

  if ( Func_SerialSIM("AT+HTTPINIT\r\n", "OK")) {
    sprintf(ATCommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", URL);
    if (Func_SerialSIM(ATCommand, "OK")) {
      if ( Func_SerialSIM("AT+HTTPACTION=0\r\n", "OK")) {
        if ( Func_SerialSIM("AT+HTTPHEAD\r\n", "OK")) {
          
        }
      }
    }
  }

}

bool CLASS_SIMA7xxx::Func_SerialSIM(char* AT, char* ATCheck) {
  String inputString = "";
  SerialAT.println(AT);
  delay(2000);
  while (SerialAT.available()) {
    char inChar = (char)SerialAT.read();
    if ((inChar != '\n') && (inChar != '\r')) {
      inputString += inChar;
    }
  }
  uint8_t len = inputString.length() + 1;
  char input[len];
  inputString.toCharArray(input, len);
  Serial.println((char*)input);
  if (strstr(input, ATCheck))
  {
    return 1;
  } else {
    return 0;
  }
}
