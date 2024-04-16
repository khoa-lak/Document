#include "FuncEeprom.h"
String inputString = "";
bool  stringComplete = false;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (inputString == "G") {
    Serial.println("Du Lieu Khi Gui La");
    FuncSendData();
  }
  if (inputString == "N") {
    Serial.println("Du Lieu Khi Nhan La");
    FuncTakeData();
  }

  if (stringComplete) {
    Serial.println(inputString);
    inputString = "";
    stringComplete = false;
  }
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
