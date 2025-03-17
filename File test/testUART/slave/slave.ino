//HardwareSerial Serial2(PA3, PA2); // UART2: RX=PA3, TX=PA2
#define Serial2 Serial1
uint16_t dataToSend[6] = {100, 200, 300, 400, 500, 600}; // Mảng cố định

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
}

uint8_t calculateChecksum(uint16_t* data, int length) {
  uint8_t checksum = 0;
  for (int i = 0; i < length * 2; i++) { // length * 2 vì mỗi uint16_t là 2 byte
    checksum += ((uint8_t*)data)[i];
  }
  return checksum;
}

void loop() {
  // Giai đoạn 1: Nhận lệnh RUN và gửi ACK
  if (Serial2.available() >= 3) {
    char buffer[3];
    Serial2.readBytes(buffer, 3);
    if (strncmp(buffer, "RUN", 3) == 0) {
      Serial.println("Slave: Received RUN command");
      Serial2.write("ACK", 3);
      Serial.println("Slave: Sent ACK to Master");

      // Giai đoạn 2: Xử lý data (giả lập 5s)
      Serial.println("Slave: Processing data...");
      delay(5000);

      // Tính checksum
      uint8_t checksum = calculateChecksum(dataToSend, 6);

      // Gửi mảng + checksum, thử lại nếu cần
      int retries = 3;
      bool ackReceived = false;

      while (retries > 0 && !ackReceived) {
        Serial.print("Slave: Sending data (attempt ");
        Serial.print(4 - retries);
        Serial.print(") with checksum ");
        Serial.println(checksum, HEX);
        Serial2.write((uint8_t*)dataToSend, 12); // Gửi 12 byte mảng
        Serial2.write(checksum); // Gửi 1 byte checksum

        // Chờ ACK hoặc NACK từ Master
        unsigned long startTime = millis();
        while (millis() - startTime < 1000) {
          if (Serial2.available() >= 3) {
            char response[4];
            int bytesRead = Serial2.readBytes(response, 4);
            if (bytesRead == 3 && strncmp(response, "ACK", 3) == 0) {
              ackReceived = true;
              Serial.println("Slave: Received ACK from Master");
              break;
            } else if (bytesRead == 4 && strncmp(response, "NACK", 4) == 0) {
              Serial.println("Slave: Received NACK, retrying...");
              break;
            }
          }
        }

        if (!ackReceived) {
          retries--;
          delay(500); // Chờ trước khi thử lại
        }
      }

      if (!ackReceived) {
        Serial.println("Slave: Failed to get ACK after retries");
      }
    }
  }
}
