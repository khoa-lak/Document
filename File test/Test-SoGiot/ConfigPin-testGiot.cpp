#include "ConfigPin-testGiot.h"

void ConfigPin::Config_InOut() {
  analogReadResolution(12);

  pinMode(X0, INPUT_PULLUP);
  pinMode(X1, INPUT_PULLUP);
  pinMode(X2, INPUT_PULLUP);
  pinMode(X3, INPUT_PULLUP);
  pinMode(X4, INPUT_PULLUP);
  pinMode(X5, INPUT_PULLUP);
  pinMode(X6, INPUT_PULLUP);
  pinMode(X7, INPUT_PULLUP);
  pinMode(X8, INPUT_PULLUP);
  pinMode(X9, INPUT_PULLUP);
  pinMode(X10, INPUT_PULLUP);

  //pinMode(X14_I2CSDA, INPUT_PULLUP);
  pinMode(X15_MOSI,   INPUT_PULLUP);

  pinMode(Y0, OUTPUT);
  pinMode(Y1, OUTPUT);
  pinMode(Y2, OUTPUT);
  pinMode(Y3, OUTPUT);
  pinMode(Y4, OUTPUT);
  pinMode(Y5, OUTPUT);
  pinMode(Y6, OUTPUT);
  pinMode(Y7, OUTPUT);
  pinMode(Y8, OUTPUT);
  pinMode(Y9, OUTPUT);
  pinMode(Y10, OUTPUT);
  pinMode(Y11, OUTPUT);
  pinMode(Y12, OUTPUT);
  pinMode(Y13, OUTPUT);
  pinMode(Y14, OUTPUT);
  pinMode(PC13, OUTPUT);
  pinMode(PB11, OUTPUT);

  digitalWrite(Y0, HIGH);
  digitalWrite(Y1, HIGH);
  digitalWrite(Y2, HIGH);
  digitalWrite(Y3, HIGH);
  digitalWrite(Y4, HIGH);
  digitalWrite(Y5, LOW);
  digitalWrite(Y6, HIGH);
  digitalWrite(Y7, HIGH);
  digitalWrite(Y8, HIGH);
  digitalWrite(Y9, HIGH);
  digitalWrite(Y10, LOW);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

}
