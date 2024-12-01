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
void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PC13, OUTPUT);
//  pinMode(PA0, OUTPUT);
//  pinMode(PB7, OUTPUT);
//  pinMode(PB8, OUTPUT);
//  pinMode(PB9, OUTPUT);
//  pinMode(PB6, OUTPUT);
//  digitalWrite(PB7, 0);
//  digitalWrite(PB8, 0);
//  digitalWrite(PB9, 0);
}
int freeRam(void)
{
  extern int __bss_end;
  extern int *__brkval;
  int free_memory;
  if ((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
  }
  return free_memory;
}
// the loop function runs over and over again forever
void loop() {
  //Serial.println("in trước: " + String(freeRam));
  BlinkLed();
  // Pump();
  Serial.println("232322");
  //Serial.println("in sau: " + String(freeRam));
}

//void Pump() {
//  digitalWrite(PB6, LOW);
//}
//void ButtonLed() {
//  if (digitalRead(PB8) == 1) {
//    digitalWrite(PC13, HIGH);
//    digitalWrite(PB7, LOW);
//  } else {
//    digitalWrite(PB7, HIGH);
//    digitalWrite(PC13, LOW);
//  }
//  if (digitalRead(PB9) == 1) {
//    digitalWrite(PB6, HIGH);
//    digitalWrite(PA0, HIGH);
//  } else {
//    digitalWrite(PB6, LOW);
//    digitalWrite(PA0, LOW);
//  }
//}
void BlinkLed() {
  digitalWrite(PC13, HIGH);

  delay(100);

  digitalWrite(PC13, LOW);

  delay(100);                       // wait for a second
}
