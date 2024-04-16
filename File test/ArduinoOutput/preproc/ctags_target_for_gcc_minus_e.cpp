# 1 "C:\\Users\\lkhoa\\OneDrive\\Desktop\\Project\\File test\\test_BlinkLed\\test_BlinkLed.ino"
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
# 25 "C:\\Users\\lkhoa\\OneDrive\\Desktop\\Project\\File test\\test_BlinkLed\\test_BlinkLed.ino"
// the setup function runs once when you press reset or power the board
void setup() {
  SerialUSB.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(17 /* LED Bluepill*/, 0x1);
  pinMode(0xC0, 0x1);
  pinMode(2, 0x1);
  pinMode(1, 0x1);
  pinMode(0, 0x1);
  pinMode(3, 0x1);
  digitalWrite(2, 0);
  digitalWrite(1, 0);
  digitalWrite(0, 0);
}

// the loop function runs over and over again forever
void loop() {
  BlinkLed();
// Pump();
  SerialUSB.println("222222222222222");
}

void Pump() {
  digitalWrite(3, 0x0);
}
void ButtonLed() {
  if (digitalRead(1) == 1) {
    digitalWrite(17 /* LED Bluepill*/, 0x1);
    digitalWrite(2, 0x0);
  } else {
    digitalWrite(2, 0x1);
    digitalWrite(17 /* LED Bluepill*/, 0x0);
  }
  if (digitalRead(0) == 1) {
    digitalWrite(3, 0x1);
    digitalWrite(0xC0, 0x1);
  } else {
    digitalWrite(3, 0x0);
    digitalWrite(0xC0, 0x0);
  }
}
void BlinkLed() {
  digitalWrite(17 /* LED Bluepill*/, 0x1);

  delay(200);

  digitalWrite(17 /* LED Bluepill*/, 0x0);

  delay(200); // wait for a second
}
