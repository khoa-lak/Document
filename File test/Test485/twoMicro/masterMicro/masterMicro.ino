#include <HardwareSerial.h>
#define en485     PA1
// SoftwareSerial object for RS485 communication
HardwareSerial Serial2(PA3, PA2);
#define RS485Serial Serial2

// Packet frame to be sent
uint8_t frame[] = {1, 5, 3, 6, 7};
uint8_t frameLength = 5;
void setup() {
  Serial.begin(9600);
  RS485Serial.begin(9600);

  // Enable RS485 transmitter mode
  pinMode(en485, OUTPUT);
  digitalWrite(en485 , HIGH);
}

void loop() {
  // Send packet frame to slave
  RS485Serial.write(frame, sizeof(frame));
  delay(10); 
  digitalWrite(en485, LOW);
  delay(5);  
  uint8_t recieve[frameLength] = {0};
  if (RS485Serial.available()) {
    // Read the packet frame from master
    RS485Serial.readBytes(recieve, frameLength);

    // Print received packet frame
    Serial.print("Received Frame: ");
    for (int i = 0; i < frameLength; i++) {
      Serial.print(recieve[i]);
      Serial.print(" ");
    }
     Serial.println(" ");
  }
  delay(1000);
  digitalWrite(en485 , HIGH);
}
