#include <Stepper.h>
#include "PIN_CONFIG.h"
CLASS_CONFIG configPin;
String inputString = "";
uint8_t currentStir = 0;
uint32_t delayLed;
uint32_t timStir;
uint32_t pwmStir;
void (*pWhichStir)();
void blinkLed() {
  uint8_t en = digitalRead(led);
  if (millis() - delayLed > 200) {
    // IWatchdog.reload();
    digitalWrite(led, !en);
    delayLed = millis();
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
void setup() {
  Serial.begin(9600);
  configPin.FUNC_CONFIGPIN();
  pWhichStir = &inttial;
}

void loop() {
  if (inputString.indexOf("clear") >= 0) {
    configPin.FUNC_CONFIGPIN();
    inputString = "";
    Serial.println("===============");
  }
  else if (inputString.indexOf("stir1") >= 0) {
    digitalWrite(stir1, HIGH);
  }
  else if (inputString.indexOf("stir2") >= 0) {
    digitalWrite(stir2, HIGH);
  }
  else if (inputString.indexOf("stir3") >= 0) {
    digitalWrite(stir3, HIGH);
  }
  else if (inputString.indexOf("stir4") >= 0) {
    digitalWrite(stir4, HIGH);
  }
  else if (inputString.indexOf("stir5") >= 0) {
    digitalWrite(stir5, HIGH);
  }
  else if (inputString.indexOf("auto") >= 0) {
    pWhichStir();
  }
  pWhichStir();
  blinkLed();
}
void inttial() {
  if (millis() > timStir) {
    timStir = millis() + 6000;
    pwmStir = micros();
    pWhichStir = &runStir;
  }
}
void runStir() {
  uint8_t pin;
  uint8_t type;
  if (currentStir == 0) {
    pin = stir1;
    type = 1;
  }
  else if (currentStir == 1) {
    pin = stir2;
    type = 1;
  }
  else if (currentStir == 2) {
    pin = stir3;
    type = 1;
  }
  else if (currentStir == 3) {
    pin = stir4;
    type = 1;
  }
  else if (currentStir == 4) {
    pin = stir5;
    type = 1;
  }
  if (motorStir(pin , type)) {
    currentStir++;
    if (currentStir > 4)
      currentStir = 0;
    pWhichStir = &inttial;
    timStir = millis() + 2000;
  }
}
bool motorStir(uint8_t pin , uint8_t type) {
  if (millis() < timStir) {
    if (type == 0) {
      uint16_t timeCompare = 50;
      uint8_t en = digitalRead(pin);
      if (en)
        timeCompare = 50;
      if ((micros() - pwmStir) >= timeCompare) {
        digitalWrite(pin , !en);
        pwmStir = micros();
        return false;
      }
    }
    else if (type == 1)
      digitalWrite(pin, HIGH);
  }
  else {
    configPin.FUNC_CONFIGPIN();
    return true;
  }
  return false;
}
