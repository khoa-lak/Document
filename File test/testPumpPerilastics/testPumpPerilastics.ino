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
CLASS_CONFIG mainCONFIGPIN;
uint8_t pinPulse;
uint32_t pulseWillRun = 0;
uint32_t pulseLinear = 0;
uint32_t delayTimeStepper = micros();
uint32_t ui32_delayLed = millis();
String inputString = "";
void setup() {
  Serial.begin(9600);
  mainCONFIGPIN.FUNC_CONFIGPIN();

}
void loop() {

  if (inputString == "clear") {
    inputString = "";
    mainCONFIGPIN.FUNC_CONFIGPIN();
  }
  else if (inputString.indexOf("pump1-") >= 0) {
    float ml = (inputString.charAt(6) - '0') * 10 + (inputString.charAt(7) - '0') + (inputString.charAt(8) - '0') / 10.00;
    uint32_t pulse1ml = (inputString.charAt(10) - '0') * 100000 + (inputString.charAt(11) - '0') * 10000 + (inputString.charAt(12) - '0') * 1000 + (inputString.charAt(13) - '0') * 100 + (inputString.charAt(14) - '0') * 10 + (inputString.charAt(15) - '0');
    uint8_t dir = (inputString.charAt(17) - '0');
    pinPulse = pulsePerisRe;
    uint32_t sumpulse = pulse1ml * ml;
    if (PUMP_STEPPER( sumpulse, dir)) {
      Serial.println("bom xong, ml: " + String(ml) + " " + String(pulse1ml));
      inputString = "";
    }
  }
  else if (inputString.indexOf("pump2-") >= 0) {
    float ml = (inputString.charAt(6) - '0') * 10 + (inputString.charAt(7) - '0') + (inputString.charAt(8) - '0') / 10.00;
    uint32_t pulse1ml = (inputString.charAt(10) - '0') * 100000 + (inputString.charAt(11) - '0') * 10000 + (inputString.charAt(12) - '0') * 1000 + (inputString.charAt(13) - '0') * 100 + (inputString.charAt(14) - '0') * 10 + (inputString.charAt(15) - '0');
    uint8_t dir = (inputString.charAt(17) - '0');
    pinPulse = pulsePerisWa;
    uint32_t sumpulse = pulse1ml * ml;
    if (PUMP_STEPPER( sumpulse, dir)) {
      Serial.println("bom xong, ml: " + String(ml) + " " + String(pulse1ml));
      inputString = "";
    }
  }
  else if (inputString.indexOf("line1-") >= 0) {
    float ml = (inputString.charAt(6) - '0') * 10 + (inputString.charAt(7) - '0') + (inputString.charAt(8) - '0') / 10.00;
    uint32_t pulse1ml = (inputString.charAt(10) - '0') * 100000 + (inputString.charAt(11) - '0') * 10000 + (inputString.charAt(12) - '0') * 1000 + (inputString.charAt(13) - '0') * 100 + (inputString.charAt(14) - '0') * 10 + (inputString.charAt(15) - '0');
    uint8_t dir = (inputString.charAt(17) - '0');
    pinPulse = pulseLiRe;
    uint32_t sumpulse = pulse1ml * ml;
    if (LINEAR_STEPPER( sumpulse, dir)) {
      Serial.println("chay xong, ml: " + String(ml) + " " + String(pulse1ml));
      inputString = "";
    }
  }
  else if (inputString.indexOf("line2-") >= 0) {
    float ml = (inputString.charAt(6) - '0') * 10 + (inputString.charAt(7) - '0') + (inputString.charAt(8) - '0') / 10.00;
    uint32_t pulse1ml = (inputString.charAt(10) - '0') * 100000 + (inputString.charAt(11) - '0') * 10000 + (inputString.charAt(12) - '0') * 1000 + (inputString.charAt(13) - '0') * 100 + (inputString.charAt(14) - '0') * 10 + (inputString.charAt(15) - '0');
    uint8_t dir = (inputString.charAt(17) - '0');
    pinPulse = pulseLiWa;
    uint32_t sumpulse = pulse1ml * ml;
    if (LINEAR_STEPPER( sumpulse, dir)) {
      Serial.println("chay xong, ml: " + String(ml) + " " + String(pulse1ml));
      inputString = "";
    }
  }
  BlinkLed();
}
bool STEPPER(uint16_t spe) {
  //uint16_t spe = 200;
  uint8_t state = digitalRead(pinPulse);  
  if (micros() - delayTimeStepper > spe) {
    digitalWrite(pinPulse, !state);
    pulseWillRun --;
    delayTimeStepper = micros();

  }
  if (pulseWillRun == 0) {
    pulseWillRun = 0;
    return 1;
  }
  return 0;
}

bool PUMP_STEPPER(uint32_t pulse, uint8_t dir) {
  uint16_t spe = 250;
  if (pulseWillRun <= 0) {
    pulseWillRun = pulse;
    Serial.println("pulse la: " + String(pulseWillRun) + " bom theo chieu: " + String(dir));
    digitalWrite(enPeris , 1);
  }
  digitalWrite(dirPeris , !dir);
  bool done = STEPPER(spe);
  if (done) {
    digitalWrite(enPeris , 0);
    pulseWillRun = 0;
  }
  return done;
}
bool LI_STEPPER(uint16_t spe) {
  //uint16_t spe = 200;
  uint8_t state = digitalRead(pinPulse);
  if (micros() - delayTimeStepper > spe) {
    digitalWrite(pinPulse, !state);
    pulseLinear --;
    delayTimeStepper = micros();

  }
  if (pulseLinear == 0) {
    pulseLinear = 0;
    return 1;
  }
  return 0;
}

bool LINEAR_STEPPER(uint32_t pulse, uint8_t dir) {
  uint16_t spe = 100;
  if (pulseLinear <= 0) {
    pulseLinear = pulse;
    Serial.println("pulse la: " + String(pulseLinear) + " linear theo chieu: " + String(dir));
    digitalWrite(enLinear , 1);
  }
  digitalWrite(dirLinear , !dir);
  bool done = LI_STEPPER(spe);
  if (done) {
    digitalWrite(enLinear  , 0);
    pulseLinear = 0;
  }
  return done;
}

void BlinkLed() {
  uint8_t state13 = digitalRead(PC13);
  if (millis() - ui32_delayLed > 1000) {
    digitalWrite(PC13, !state13);
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
