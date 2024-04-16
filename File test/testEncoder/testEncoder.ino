#include<SoftwareSerial.h>

SoftwareSerial Seria (PB10, PA9);
const byte encoderPinA = PA0;//outputA digital pin2
const byte encoderPinB = PA1;//outoutB digital pin3
volatile int count = 0;
int protectedCount = 0;
int previousCount = 0;
//
//#define readA bitRead(PortA ,(PortA << 4) + 0x00)//faster than digitalRead()
//#define readB bitRead(PortA ,(PortA << 4) + 0x01)//faster than digitalRead()
#define readA digitalRead((PortA << 4) + 0x00)//faster than digitalRead()
#define readB digitalRead((PortA << 4) + 0x01)//faster than digitalRead()

void setup() {
  Seria.begin (9600);

  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), isrA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), isrB, CHANGE);
}

void loop() {
  noInterrupts();
  protectedCount = count;
  interrupts();

  if (protectedCount != previousCount) {
    Seria.println(protectedCount);
  }
  previousCount = protectedCount;
}

void isrA() {
  if (readB != readA) {
    count ++;
  } else {
    count --;
  }
}
void isrB() {
  if (readA == readB) {
    count ++;
  } else {
    count --;
  }
}
