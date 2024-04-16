const int LED = PC0; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 150;        // value output to the PWM (analog out)
char char1 = '0';

uint32_t delayLED = millis();


void setup() {
  pinMode(PC0, OUTPUT);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

}

void loop() {
  LEDIn(1);
}
void serialEvent() {
  char1 = '\n';
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n') {
      char1 = inChar;
    }
    Serial.println(char1);
  }
}
void LEDIn(uint8_t intensity) {
  uint8_t state = digitalRead(LED);
  uint16_t timedelay = 0;
  switch (intensity) {
    case 1:
      if (state) timedelay = 990;
      else if (!state) timedelay = 10;
      break;
    case 2:
      if (state) timedelay = 200;
      else if (!state) timedelay = 800;
      break;
    default:
      break;
  }
  if (micros() - delayLED > timedelay) {
    digitalWrite(LED, !state);
    delayLED = micros();
  }
}
