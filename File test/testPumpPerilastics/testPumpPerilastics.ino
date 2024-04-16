/*
   factory: Vmax = 300rpm => 1v:200ms
   reality: 1v:165ms => Vmax = 363.63636 rpm
   suggest: 1v:326ms => Vmax = 184 rpm
   ===========================================
   calib bơm: V = 184rpm( 10v = 0.74  ml, 20v  = 0.74+ 0.6 , 0.68 + 0.76, 30v =0.94+0.86+ 0.2, 0.68+0.76+0.62) - dự tính 10v = 0.73 ml => 1v = 0.073ml
   Sau khi đo 10 lần 5ml, 10 lần 10ml, 10 lần 20ml thì chọn:
    + 1v = 1600 xung = 0.073ml
    + giá trị ml nhỏ nhất là 1/2v = 800 xung = 0.0365ml
*/

#include "CONFIGPIN.h"
#include "SENSORCOLOR.h"
CLASS_CONFIGPIN mainCONFIGPIN;
CLASS_SSC mainSSC;
uint16_t pulsePerRound = 3200;
float mlPerRound = 0.073;
float mlDropReagent = 0.035;
float mlWater = 3.0;
float dropPH = 2.4;
float dropNH3 = 1.8;
uint32_t pulseMeasure = 0;
uint32_t ui32_delayStep = micros();
uint32_t ui32_delayLed = micros();
String inputString = "";
void (*runningFunc) ();
void setup() {
  Serial.begin(9600);
  mainSSC.SCFunc_Setup();
  mainCONFIGPIN.Func_ConfigPin();
  runningFunc = &FuncReset;
}
void FuncReset() {
  digitalWrite(CFPin_EnStepPump, 0);
  pulseMeasure = 0;
  return;
}
void loop() {
  BlinkLed();
  //  tuanhoan();
  //  if (inputString == "v") {
  //    vongquay(1);
  //  }
  if (inputString == "deba1") {
    Serial.println("đề ba 1");
    pulseMeasure = 4000;
    inputString = "";
    runningFunc = &FuncReagent;
  }
  else if (inputString == "deba2") {
    Serial.println("đề ba 2");
    pulseMeasure = 4000;
    inputString = "";
    runningFunc = &FuncReagent2;
  }
  else if (inputString == "t1") {
    float mlCurrent = mlDropReagent * dropPH;
    float amountRound = mlCurrent / mlPerRound;
    pulseMeasure = amountRound * pulsePerRound;
    Serial.println("số pulse của thuốc thử 1: " + String(pulseMeasure));
    inputString = "";
    runningFunc = &FuncReagent;
  }
  else if (inputString == "t2") {
    float mlCurrent = mlDropReagent * dropNH3;
    float amountRound = mlCurrent / mlPerRound;
    pulseMeasure = amountRound * pulsePerRound;
    Serial.println("số pulse của thuốc thử 2: " + String(pulseMeasure));
    inputString = "";
    runningFunc = &FuncReagent2;
  }
  else if (inputString == "outt1") {
    float mlCurrent = mlDropReagent * dropPH;
    float amountRound = mlCurrent / mlPerRound;
    pulseMeasure = (amountRound * pulsePerRound)  + 1000;
    Serial.println("số pulse của out thuốc thử 1: " + String(pulseMeasure));
    inputString = "";
    runningFunc = &FuncReagent2;
  }
  else if (inputString == "outt2") {
    float mlCurrent = mlDropReagent * dropPH;
    float amountRound = mlCurrent / mlPerRound;
    pulseMeasure = (amountRound * pulsePerRound)  + 1000;
    Serial.println("số pulse của out thuốc thử 2: " + String(pulseMeasure));
    inputString = "";
    runningFunc = &FuncReagent;
  }
  else if (inputString == "water1") {
    float amountRound = mlWater / mlPerRound;
    pulseMeasure = amountRound * pulsePerRound;
    Serial.println("số pulse của water 1: " + String(pulseMeasure));
    inputString = "";
    runningFunc = &FuncReagent;
  }
  else if (inputString == "water2") {
    float amountRound = mlWater / mlPerRound;
    pulseMeasure = amountRound * pulsePerRound;
    Serial.println("số pulse của water 2: " + String(pulseMeasure));
    inputString = "";
    runningFunc = &FuncReagent2;
  }
  else if (inputString == "outwater1") {
    float amountRound = mlWater / mlPerRound;
    pulseMeasure = (amountRound * pulsePerRound) + 1000;
    Serial.println("số pulse của out water 1: " + String(pulseMeasure));
    inputString = "";
    runningFunc = &FuncReagent2;
  }
  else if (inputString == "outwater2") {
    float amountRound = mlWater / mlPerRound;
    pulseMeasure = (amountRound * pulsePerRound) + 1000;
    Serial.println("số pulse của out water 2: " + String(pulseMeasure));
    inputString = "";
    runningFunc = &FuncReagent;
  }
  else if (inputString == "mix1") {
    Serial.println("Trộn thuốc 1");
    pumpmix(0);
    inputString = "";
  }
  else if (inputString == "mix2") {
    Serial.println("Trộn thuốc 2");
    pumpmix(1);
    inputString = "";
  }
  else if (inputString == "rua1") {
    Serial.println("rửa 1");
    pumpclean(0);
    inputString = "";
  }
  else if (inputString == "rua2") {
    Serial.println("rửa 2");
    pumpclean(1);
    inputString = "";
  }
  else if (inputString == "m1" || inputString == "m2" || inputString == "m3") {
    Serial.println("Đo cảm biến");
    Func_Color();
    inputString = "";
  }
  else if (inputString == "x") {
    Serial.println("cancel");
    inputString = "";
    runningFunc = &FuncReset;
  }
  else if (inputString == "t" || inputString == "n") {
    tuanhoan();
    return;
  } else if (inputString == "out") {
    Serial.println("bơm xả");
    pumpout();
    inputString = "";
  }
  runningFunc();

}
void FuncReagent() {
  digitalWrite(CFPin_EnStepPump, 1);
  if ( Func_RunStep(0, 100))
    pulseMeasure --;
  if (pulseMeasure  <= 0) {
    runningFunc = &FuncReset;
    Serial.println("done");
  }
}
void FuncReagent2() {
  digitalWrite(CFPin_EnStepPump, 1);
  if (Func_RunStep(1, 100))
    pulseMeasure --;
  if (pulseMeasure  <= 0) {
    runningFunc = &FuncReset;
    Serial.println("done");
  }
}
bool Func_RunStep(uint8_t dir, uint16_t spe) {
  uint8_t state = digitalRead(CFPin_PulseStep);
  digitalWrite(CFPin_DirStep, dir);
  if (abs(int64_t(micros() - ui32_delayStep)) > spe) {
    digitalWrite(CFPin_PulseStep, !state);
    ui32_delayStep = micros();
    return 1;
  }
  return 0;
}
void Func_Color() {
  if (inputString == "m2" || inputString == "m3" ) {
    digitalWrite(CFPin_Stir, 0);
    delay(10000);
    digitalWrite(CFPin_Stir, 1);
    if (inputString == "m3")
      return;
  }
  digitalWrite(CFPin_LedSensor, 0);
  delay(500);
  float RGBme[4] = {0};
  uint8_t solan = 5;
  mainSSC.SCFunc_Running(RGBme, 1);
  digitalWrite(CFPin_LedSensor, 1);
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
void BlinkLed() {
  uint8_t state13 = digitalRead(PC13);
  if (millis() - ui32_delayLed > 1000) {
    digitalWrite(PC13, !state13);
    ui32_delayLed = millis();
  }
}
void pumpmix(uint8_t dir) {
  digitalWrite(CFPin_EnStepPump, 1);
  digitalWrite(CFPin_DirStep, dir);
  for (uint32_t i = 0 ; i < 25000 ; i ++) { //68.5
    digitalWrite(CFPin_PulseStep, 0);
    delayMicroseconds(100);
    digitalWrite(CFPin_PulseStep, 1);
    delayMicroseconds(100);
  }
  delay(2000);
  digitalWrite(CFPin_DirStep, !dir);
  for (uint32_t i = 0 ; i < 26500 ; i ++) { //68.5
    digitalWrite(CFPin_PulseStep, 0);
    delayMicroseconds(100);
    digitalWrite(CFPin_PulseStep, 1);
    delayMicroseconds(100);
  }
  digitalWrite(CFPin_EnStepPump, 0);
}
void pumpout() {
  digitalWrite(CFPin_PumpInTube, 0);
  delay(10000);
  digitalWrite(CFPin_PumpInTube, 1);
}
void pumpclean(uint8_t dir) {
  digitalWrite(CFPin_EnStepPump, 1);
  digitalWrite(CFPin_DirStep, dir);
  for (uint32_t i = 0 ; i < 30000 ; i ++) { //68.5
    digitalWrite(CFPin_PulseStep, 0);
    delayMicroseconds(100);
    digitalWrite(CFPin_PulseStep, 1);
    delayMicroseconds(100);
  }
  delay(5000);
  digitalWrite(CFPin_DirStep, !dir);
  for (uint32_t i = 0 ; i < 32000 ; i ++) { //68.5
    digitalWrite(CFPin_PulseStep, 0);
    delayMicroseconds(100);
    digitalWrite(CFPin_PulseStep, 1);
    delayMicroseconds(100);
  }
  digitalWrite(CFPin_EnStepPump, 0);
}

void vongquay(uint8_t vong) {
  digitalWrite(CFPin_EnStepPump, 1);
  digitalWrite(CFPin_DirStep, 0);
  Serial.println(millis());
  //  for (uint16_t j = 9 ; j < 4*69; j++) {
  for (uint32_t i = 0 ; i < 800 ; i ++) { //68.5
    digitalWrite(CFPin_PulseStep, 0);
    delayMicroseconds(100);
    digitalWrite(CFPin_PulseStep, 1);
    delayMicroseconds(100);
  }
  //    delay(500);
  //  }
  Serial.println(millis());
  digitalWrite(CFPin_EnStepPump, 0);
  inputString = "";
}
void tuanhoan() {
  if (inputString == "t") {
    digitalWrite(CFPin_EnStepPump, 1);
    digitalWrite(CFPin_DirStep, 0);
    digitalWrite(CFPin_PulseStep, 0);
    delayMicroseconds(50);
    digitalWrite(CFPin_PulseStep, 1);
    delayMicroseconds(50);
  }
  else if (inputString == "n") {
    digitalWrite(CFPin_EnStepPump, 1);
    digitalWrite(CFPin_DirStep, 1);
    digitalWrite(CFPin_PulseStep, 0);
    delayMicroseconds(50);
    digitalWrite(CFPin_PulseStep, 1);
    delayMicroseconds(50);
  } else
    digitalWrite(CFPin_EnStepPump, 0);
}
