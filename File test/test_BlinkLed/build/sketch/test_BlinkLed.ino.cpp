#include <Arduino.h>
#line 1 "C:\\Users\\lkhoa\\OneDrive\\Desktop\\Project\\File test\\test_BlinkLed\\test_BlinkLed.ino"
 /*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

// the setup function runs once when you press reset or power the board
#line 26 "C:\\Users\\lkhoa\\OneDrive\\Desktop\\Project\\File test\\test_BlinkLed\\test_BlinkLed.ino"
void setup();
#line 41 "C:\\Users\\lkhoa\\OneDrive\\Desktop\\Project\\File test\\test_BlinkLed\\test_BlinkLed.ino"
void loop();
#line 47 "C:\\Users\\lkhoa\\OneDrive\\Desktop\\Project\\File test\\test_BlinkLed\\test_BlinkLed.ino"
void Pump();
#line 50 "C:\\Users\\lkhoa\\OneDrive\\Desktop\\Project\\File test\\test_BlinkLed\\test_BlinkLed.ino"
void ButtonLed();
#line 66 "C:\\Users\\lkhoa\\OneDrive\\Desktop\\Project\\File test\\test_BlinkLed\\test_BlinkLed.ino"
void BlinkLed();
#line 26 "C:\\Users\\lkhoa\\OneDrive\\Desktop\\Project\\File test\\test_BlinkLed\\test_BlinkLed.ino"
void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PC13, OUTPUT);
  pinMode(PA0, OUTPUT);
  pinMode(PB7, OUTPUT);
  pinMode(PB8, OUTPUT);
  pinMode(PB9, OUTPUT);
  pinMode(PB6, OUTPUT);
  digitalWrite(PB7, 0);
  digitalWrite(PB8, 0);
  digitalWrite(PB9, 0);
}

// the loop function runs over and over again forever
void loop() {
  BlinkLed();
// Pump();
  Serial.println("232322");
}

void Pump() {
  digitalWrite(PB6, LOW);
}
void ButtonLed() {
  if (digitalRead(PB8) == 1) {
    digitalWrite(PC13, HIGH);
    digitalWrite(PB7, LOW);
  } else {
    digitalWrite(PB7, HIGH);
    digitalWrite(PC13, LOW);
  }
  if (digitalRead(PB9) == 1) {
    digitalWrite(PB6, HIGH);
    digitalWrite(PA0, HIGH);
  } else {
    digitalWrite(PB6, LOW);
    digitalWrite(PA0, LOW);
  }
}
void BlinkLed() {
  digitalWrite(PC13, HIGH);
 
  delay(100);
 
  digitalWrite(PC13, LOW);
 
  delay(100);                       // wait for a second
}

