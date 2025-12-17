#include "Func_Main.h"
class_FuncMain funcMain;
uint32_t ui32_delayLed = millis();
uint32_t ui32_delayProgram = millis();
String inputString = "";
void setup() {
  Serial.begin(9600);
  funcMain.Setup();
}

void loop() {
  funcMain.Running();
  BlinkLed();
}
void BlinkLed() {
  uint8_t ui8_state = digitalRead(Led);
  if (millis() - ui32_delayLed > 1000) {
    digitalWrite(Led, !ui8_state);
    ui32_delayLed = millis();
  }
}
void serialEvent() {
  inputString = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n' && inChar != ' ' && inChar != '\r') {
      inputString += inChar;
    }
  }
  Serial.println(inputString);
}
