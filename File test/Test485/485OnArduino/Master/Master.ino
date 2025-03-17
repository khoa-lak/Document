//#include <HardwareSerial.h>
//// Khai báo USART2 trên PA2 (TX), PA3 (RX)
//HardwareSerial Serial2(PA3, PA2); // RX, TX
//#define DE_RE_PIN PA1  // Chân điều khiển DE/RE
#include <HardwareSerial.h>
HardwareSerial Serial2(PC11, PC10);
#define DE_RE_PIN PA15 // Chân điều khiển DE/RE của MAX485
#define MAX_RETRIES 5

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW);

  digitalWrite(DE_RE_PIN, HIGH);
  Serial2.println("Master started");
  delay(50);
  digitalWrite(DE_RE_PIN, LOW);
  clearSerialBuffer();
}
void clearSerialBuffer() {
  while (Serial2.available() > 0) {
    Serial2.read();
  }
}
uint8_t calculateChecksum(const char* data, int len) {
  uint8_t sum = 0;
  for (int i = 0; i < len; i++) {
    sum += data[i];
  }
  return sum;
}

bool sendCommandAndWaitForAck() {
  char command[] = "RUN";
  uint8_t checksum = calculateChecksum(command, 3);
  digitalWrite(DE_RE_PIN, HIGH);
  Serial2.write(command, 3);
  Serial2.write(checksum);
  delay(50);
  digitalWrite(DE_RE_PIN, LOW);
  Serial.println("Master: Sent RUN command, waiting for ACK...");
  char response[4]; // "ACK" + null terminator
  int index = 0;
  unsigned long timeout = millis() + 500;

  // Xóa bộ đệm UART trước khi nhận
  while (Serial2.available()) Serial2.read();
  uint8_t receivedChecksum = 0;
  uint8_t expectedChecksum = 0;
  while (millis() < timeout ) {
    if (Serial2.available() >= 4) {
      for (int i = 0; i < 3; i++) {
        response[i] = Serial2.read();
        Serial.print("Master: Received byte: ");
        Serial.println(response[i]);
      }
      response[3] = '\0';
      receivedChecksum = Serial2.read();
      expectedChecksum = calculateChecksum(response, 3);
      break;
    }
  }


  if (strcmp(response, "ACA") == 0) {
    // Đảm bảo đủ dữ liệu checksum
    Serial.print("Master: Received checksum: ");
    Serial.println(receivedChecksum, DEC);
    Serial.print("Master: Expected checksum: ");
    Serial.println(expectedChecksum, DEC);
    if (receivedChecksum == expectedChecksum) {
      Serial.println("Master: Received valid ACK from Slave.");
      return true;
    } else {
      Serial.println("Master: ACK checksum mismatch!");
    }
  }
  else {
    Serial.println("Master: Received invalid response (not ACK).");
  }
  return false;
}

void loop() {
    bool ackReceived = false;
    int retries = 0;
  
    while (retries < MAX_RETRIES && !ackReceived) {
      ackReceived = sendCommandAndWaitForAck();
      if (!ackReceived) {
        retries++;
        Serial.print("Master: Retry ");
        Serial.print(retries);
        Serial.print(" of ");
        Serial.println(MAX_RETRIES);
        delay(1000);
      }
    }
  
    if (ackReceived) {
      Serial.println("Master: ACK confirmed. Waiting for DONE...");
      char response[5]; // "DONE" + null terminator
      int index = 0;
      unsigned long timeout = millis() + 30000;
      while (Serial2.available()) Serial2.read(); // Xóa bộ đệm
      uint8_t receivedChecksum = 0;
      uint8_t expectedChecksum = 0;
      while (millis() < timeout ) {
        if (Serial2.available() > 4) {
          for (int i = 0; i < 4; i++) {
            response[i] = Serial2.read();
            Serial.print("Master: Received byte: ");
            Serial.println(response[i]);
          }
          response[4] = '\0';
          receivedChecksum = Serial2.read();
          expectedChecksum = calculateChecksum(response, 4);
          break;
        }
      }
  
      if (strcmp(response, "DONE") == 0) {
        Serial.print("Master: Received checksum: ");
        Serial.println(receivedChecksum, DEC);
        Serial.print("Master: Expected checksum: ");
        Serial.println(expectedChecksum, DEC);
        if (receivedChecksum == expectedChecksum) {
          Serial.println("Master: Received valid DONE from Slave. Task completed!");
        } else {
          Serial.println("Master: DONE checksum mismatch!");
        }
      }
    } else {
      Serial.println("Master: Failed to receive ACK after all retries. Giving up.");
    }
  
    delay(5000);


}
