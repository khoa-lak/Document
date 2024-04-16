// RS485 communication pins
#define en485    PA8
#define RS485Serial Serial1
// Packet frame to be received
uint8_t frameLength = 5;
uint8_t frame[5];

// Packet to be sent as a response
uint8_t respond[] = {1, 5, 3, 8, 9};

void setup() {
  Serial.begin(9600);
  RS485Serial.begin(9600);
  pinMode(en485, OUTPUT);
  digitalWrite(en485, LOW);

}

void loop() {
  if (RS485Serial.available() >= frameLength) {
    // Read the packet frame from master
    RS485Serial.readBytes(frame, frameLength);
    // Print received packet frame
    Serial.print("Received Frame: ");
    for (int i = 0; i < frameLength; i++) {
      Serial.print(frame[i]);
      Serial.print(" ");
    }
    Serial.println();
    // Check if the first three elements match the frame
    if (frame[0] == respond[0] && frame[1] == respond[1] && frame[2] == respond[2]) {
      delay(10);
      digitalWrite(en485, HIGH);
      RS485Serial.write(respond, frameLength);
      // Print sent response packet
      Serial.print("Sent Response: ");
      for (int i = 0; i < sizeof(respond); i++) {
        Serial.print(respond[i]);
        Serial.print(" ");
      }
      Serial.println();
      delay(10); 
      digitalWrite(en485, LOW);
    }
  }
}
