

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
uint8_t hello[10];
uint16_t sone = 8888;
uint8_t sone1 = sone/100;
uint8_t sone2 = sone - sone1*100;
void setup() {

  Serial.begin(9600);

  inputString.reserve(200);
}

void loop() {
  if (inputString == "1") {
    TachSoNao();
  }

  if (stringComplete) {
    Serial.println(inputString);
    inputString = "";
    stringComplete = false;
  }
}

void TachSoNao() {
  hello[0] = sone1;
  hello[1] = sone2;

  Serial.println("Vaooo " );
  for (int i = 0 ; i < 10; i++) {
    Serial.print(hello[i]);
    Serial.print("-");
  }
  Serial.print(hello[0]*100+hello[1]);
  Serial.println(" " );
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n') {
      inputString += inChar;
    }
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
