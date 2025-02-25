#include "FuncMain.h"
FuncMain funcMain;
String inputString = "";
void setup() {
  Serial.begin(9600);
  funcMain.mainSetup();
}

void loop() {
  funcMain.mainRunning();
}
//void serialEvent() {
//  inputString = "";
//  while (Serial.available()) {
//    char inChar = (char)Serial.read();
//    if (inChar != '\n' && inChar != ' ' && inChar != '\r') {
//      inputString += inChar;
//    }
//  }
//  Serial.println(inputString);
//}
