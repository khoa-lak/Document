void setup() {
  Serial.begin(9600);
}

void loop() {
  uint16_t num = 0x5555; // Số hex cần chuyển đổi
  char hexArray[4]; // Mảng ký tự lưu trữ kết quả
  sprintf(hexArray, "%02X", num); // Chuyển đổi giá trị hex thành mảng ký tự
  //char* hexData = "5555";
  float hexDecimal = 0;
  uint16_t hexInt = (uint16_t) strtol(hexArray, NULL, 16);
  hexDecimal = hexInt / 100.0;
  Serial.println(hexDecimal);
  delay(1000);
}
