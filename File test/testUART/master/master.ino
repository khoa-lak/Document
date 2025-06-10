//HardwareSerial Serial2(PA3, PA2); // UART2: RX=PA3, TX=PA2
//#define Serial2 Serial1
bool hasMsg = false;
void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
}

void loop() {
  // Giai đoạn 1: Gửi lệnh RUN và chờ ACK
  //  while (Serial1.available() > 0) {
  //    if (hasMsg) {
  //      Serial.println("");
  //      hasMsg = false;
  //    }
  //    char inchar = (char)Serial1.read();
  //    Serial.print(inchar);
  //    if (inchar == '\n' || inchar == '\r' ) {
  //      hasMsg = true;
  //      Serial1.write("feedback");
  //      break;
  //    }
  //  }


  Serial1.write("RUN next \r\n");
  //Serial1.write("next");
  delay(1000);
  //
  //  unsigned long startTime = millis();
  //  bool ackReceived = false;
  //
  //  while (millis() - startTime < 1000) {
  //    if (Serial2.available() >= 3) {
  //      char buffer[3];
  //      Serial2.readBytes(buffer, 3);
  //      if (strncmp(buffer, "ACK", 3) == 0) {
  //        ackReceived = true;
  //        Serial.println("Master: ACK received from Slave");
  //        break;
  //      }
  //    }
  //  }
  //
  //  if (!ackReceived) {
  //    Serial.println("Error: No ACK from Slave");
  //    delay(2000);
  //    return;
  //  }
  //
  //  // Giai đoạn 2: Chờ 20s để nhận mảng uint16_t[6] + checksum
  //  Serial.println("Master: Waiting 20s for data...");
  //  startTime = millis();
  //  bool dataReceived = false;
  //  uint16_t receivedData[6];
  //  uint8_t receivedChecksum;
  //
  //  while (millis() - startTime < 20000) {
  //    if (Serial2.available() >= 13) { // 6 uint16_t (12 byte) + 1 byte checksum
  //      Serial2.readBytes((uint8_t*)receivedData, 12); // Nhận mảng
  //      receivedChecksum = Serial2.read(); // Nhận checksum
  //
  //      // Tính checksum từ dữ liệu nhận được
  //      uint8_t calculatedChecksum = 0;
  //      for (int i = 0; i < 12; i++) {
  //        calculatedChecksum += ((uint8_t*)receivedData)[i];
  //      }
  //
  //      if (calculatedChecksum == receivedChecksum) {
  //        dataReceived = true;
  //        Serial.print("Master: Received data: ");
  //        for (int i = 0; i < 6; i++) {
  //          Serial.print(receivedData[i]);
  //          Serial.print(" ");
  //        }
  //        Serial.print("Checksum: ");
  //        Serial.println(receivedChecksum, HEX);
  //        Serial2.write("ACK", 3); // Gửi ACK nếu checksum đúng
  //        Serial.println("Master: Sent ACK to Slave");
  //      } else {
  //        Serial.println("Error: Checksum mismatch!");
  //        Serial2.write("NACK", 4); // Gửi NACK nếu checksum sai
  //      }
  //      break;
  //    }
  //  }
  //
  //  if (!dataReceived) {
  //    Serial.println("Error: No valid data received from Slave after 20s");
  //  }
  //
  //  delay(2000);
}
