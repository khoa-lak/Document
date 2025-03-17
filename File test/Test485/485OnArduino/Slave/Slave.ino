//#include <HardwareSerial.h>
//HardwareSerial Serial2(PC11, PC10);
//#define DE_RE_PIN PA15 // Chân điều khiển DE/RE của MAX485
#include <HardwareSerial.h>
// Khai báo USART2 trên PA2 (TX), PA3 (RX)
HardwareSerial Serial2(PA3, PA2); // RX, TX
#define DE_RE_PIN PA1  // Chân điều khiển DE/RE
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW);

  digitalWrite(DE_RE_PIN, HIGH);
  Serial2.println("Slave started");
  delay(50);
  digitalWrite(DE_RE_PIN, LOW);
  clearSerialBuffer();
}

uint8_t calculateChecksum(const char* data, int len) {
  uint8_t sum = 0;
  for (int i = 0; i < len; i++) {
    sum += data[i];
  }
  return sum;
}
void clearSerialBuffer() {
  while (Serial2.available() > 0) {
    Serial2.read();
  }
}
void loop() {
    char command[4];
    int index = 0;
    unsigned long timeout = millis() + 5000;
    if (Serial2.available() > 0) {
      if (Serial2.available() >= 4) {
        for (int i = 0; i < 3; i++) {
          command[i] = Serial2.read();
          Serial.print("Slave: Received byte: ");
          Serial.println(command[i]);
        }
        command[3] = '\0';
        uint8_t receivedChecksum = Serial2.read();
        uint8_t expectedChecksum = calculateChecksum(command, 3);
  
        Serial.print("Slave: Received checksum: ");
        Serial.println(receivedChecksum, DEC);
        Serial.print("Slave: Expected checksum: ");
        Serial.println(expectedChecksum, DEC);
  
        if (strcmp(command, "RUN") == 0 && receivedChecksum == expectedChecksum) {
          delay(200);
          Serial.println("Slave: Received valid RUN command.");
          char ack[] = "ACA";
          uint8_t ackChecksum = calculateChecksum(ack, 3);
          Serial.print("Slave: Sent ACK with checksum: ");
          Serial.println(ackChecksum, DEC);
          digitalWrite(DE_RE_PIN, HIGH);
          Serial2.write(ack, 3);
          Serial2.write(ackChecksum);
          delay(50);
          digitalWrite(DE_RE_PIN, LOW);
          delay(20000);
          char done[] = "DONE";
          uint8_t doneChecksum = calculateChecksum(done, 4);
          Serial.print("Slave: Sent DONE with checksum: ");
          Serial.println(doneChecksum, DEC);
          digitalWrite(DE_RE_PIN, HIGH);
          Serial2.write(done, 4);
          Serial2.write(doneChecksum);
          delay(50);
          digitalWrite(DE_RE_PIN, LOW);
          Serial.println("Slave: Task completed, sent DONE to Master.");
        } else {
          Serial.println("Slave: Invalid command or checksum error.");
        }
        clearSerialBuffer();
      }
    }


}
