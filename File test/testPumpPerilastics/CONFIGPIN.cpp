#include "CONFIGPIN.h"
void CLASS_CONFIG::FUNC_CONFIGPIN() {
  pinMode(X1 ,    INPUT_PULLUP);
  pinMode(X2 ,    INPUT_PULLUP);
  pinMode(X3 ,    INPUT_PULLUP);
  pinMode(X4,    INPUT_PULLUP);
  pinMode(X5,    INPUT_PULLUP);
  //pinMode(X6,  INPUT_PULLUP);
  // pinMode(X7,   INPUT_PULLUP);

  pinMode(pumpIN, OUTPUT);
  pinMode(pumpOUT, OUTPUT);
  pinMode(valveDir, OUTPUT);
  pinMode(valvePond, OUTPUT);
  pinMode(thermCooler, OUTPUT);
  pinMode(pumpCooling, OUTPUT);
  pinMode(pumpVial, OUTPUT);
  pinMode(pumpDis, OUTPUT);
  pinMode(motorMix, OUTPUT);
  pinMode(fan, OUTPUT);
  //pinMode(lampRed, OUTPUT);
  pinMode(stir , OUTPUT);
  pinMode(pinPos, OUTPUT);
  pinMode(horn, OUTPUT);
  pinMode(outnone, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(en485, OUTPUT);
  pinMode(enUART, OUTPUT);

  pinMode(enRot, OUTPUT);
  pinMode(dirRot, OUTPUT);
  pinMode(pulseRot, OUTPUT);
  pinMode(enLinear, OUTPUT);
  pinMode(dirLinear, OUTPUT);
  pinMode(pulseLiRe, OUTPUT);
  pinMode(pulseLiWa, OUTPUT);
  pinMode(enPeris, OUTPUT);
  pinMode(dirPeris, OUTPUT);
  pinMode(pulsePerisRe, OUTPUT);
  pinMode(pulsePerisWa, OUTPUT);

  digitalWrite(pumpIN, 0);
  digitalWrite(pumpOUT, 0);
  digitalWrite(valveDir, 0); //in
  digitalWrite(valvePond,  LOW); //
  digitalWrite(thermCooler,  LOW);
  digitalWrite(pumpCooling, 0);
  digitalWrite(pumpVial, 0);
  digitalWrite(pumpDis, 0);
  digitalWrite(motorMix, LOW);
  digitalWrite(fan, LOW);
  //digitalWrite(lampRed, LOW);
  digitalWrite(stir , LOW);
  digitalWrite(pinPos, LOW);
  digitalWrite(horn, LOW);
  digitalWrite(outnone, LOW);
  digitalWrite(en485, LOW);
  digitalWrite(enUART, 1);

  digitalWrite(enRot, LOW);
  digitalWrite(dirRot, LOW);
  digitalWrite(pulseRot, LOW);
  digitalWrite(enLinear, LOW);
  digitalWrite(dirLinear, LOW);
  digitalWrite(pulseLiRe, LOW);
  digitalWrite(pulseLiWa, LOW);
  digitalWrite(enPeris, LOW);
  digitalWrite(dirPeris, LOW);
  digitalWrite(pulsePerisRe, LOW);
  digitalWrite(pulsePerisWa, LOW);

}
