#include "CONFIGPIN.h"
void CLASS_CONFIGPIN::Func_ConfigPin() {
  pinMode(I0, INPUT_PULLUP);
  pinMode(I1, INPUT_PULLUP);
  pinMode(I2, INPUT_PULLUP);
  pinMode(I3, INPUT_PULLUP);
  pinMode(I4, INPUT_PULLUP);
  pinMode(I5, INPUT_PULLUP);
  pinMode(I6, INPUT_PULLUP);
  pinMode(I7, INPUT_PULLUP);
  //  pinMode(I8, INPUT_PULLUP);
  //  pinMode(I9, INPUT_PULLUP);
  //  pinMode(I10, INPUT_PULLUP);
  //  pinMode(I11, INPUT_PULLUP);
  //  pinMode(I12, INPUT_PULLUP);
  pinMode(I13, INPUT_PULLUP);
  pinMode(I14, INPUT_PULLUP);
  //  pinMode(I15, INPUT_PULLUP);
  //  pinMode(I16, INPUT_PULLUP);
  pinMode(Q0, OUTPUT);
  pinMode(Q1, OUTPUT);
  pinMode(Q2, OUTPUT);
  pinMode(Q3, OUTPUT);
  pinMode(Q4, OUTPUT);
  pinMode(Q5, OUTPUT);
  pinMode(Q6, OUTPUT);
  pinMode(Q7, OUTPUT);
  pinMode(Q8, OUTPUT);
  pinMode(Q9, OUTPUT);
  pinMode(Q10, OUTPUT);
  pinMode(Q11, OUTPUT);
  pinMode(Q12, OUTPUT);
  pinMode(Q13, OUTPUT);
  pinMode(Q14, OUTPUT);
  pinMode(PC13, OUTPUT);
  pinMode(CFPin_S0, OUTPUT);
  pinMode(CFPin_S1, OUTPUT);
  pinMode(CFPin_S2, OUTPUT);
  pinMode(CFPin_S3, OUTPUT);
  //  pinMode(Q15, OUTPUT);
  //  pinMode(Q16, OUTPUT);

  pinMode(CFPinEn485, OUTPUT);
  pinMode(CFPinLed,   OUTPUT);
  //

  digitalWrite(Q0,  1);
  digitalWrite(Q1,  1);
  digitalWrite(Q2,  1);
  digitalWrite(Q3,  0);
  digitalWrite(Q4,  1);
  digitalWrite(Q5,  1);
  digitalWrite(Q6,  1);
  digitalWrite(Q7,  1);
  digitalWrite(Q8,  1);
  digitalWrite(Q9,  1);
  digitalWrite(Q10, 1);
  digitalWrite(Q11, 1);
  digitalWrite(Q12, 1);
  digitalWrite(Q13, 1);
  digitalWrite(Q14, 1);

  digitalWrite(CFPinEn485,  HIGH);
  digitalWrite(CFPinLed ,  HIGH);
}
