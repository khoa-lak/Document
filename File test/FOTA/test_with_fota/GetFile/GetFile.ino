#include <Arduino.h>
#include "FuncMain.h"
FuncMain funcMain;
uint32_t interval_blink = millis();
void setup() {
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);
  funcMain.setupMain();
}
void loop() {
  funcMain.runMain();
  blinkLed();
  if (Serial.available() > 0) {
    String inputString = "";
    while (Serial.available()) {
      char inChar = (char)Serial.read();
      inputString += inChar;
      if (inChar == '\n') {
        Serial.println(inputString);
      }
    }
  }
}
void blinkLed() {
  if (millis() - interval_blink > 200) {
    digitalWrite(PC13, !digitalRead(PC13));
    interval_blink = millis();
  }
}