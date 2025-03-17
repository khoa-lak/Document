#include <Wire.h>

#define AS7341_ADDR 0x39 // Địa chỉ I2C của AS7341

// Định nghĩa các thanh ghi quan trọng
#define REG_ENABLE 0x80
#define REG_ATIME 0x81
#define REG_ASTEP_L 0xCA
#define REG_ASTEP_H 0xCB
#define REG_CFG1 0xA9 // GAIN
#define REG_STATUS2 0x93
#define REG_CH0_L 0x95 // Dữ liệu kênh bắt đầu từ đây (F1 low byte)
#define REG_CH0_H 0x96 // F1 high byte
void configureSMUX() {
  writeRegister(0xAF, 0x10); // Bật SMUX
  // Cấu hình ánh xạ kênh (theo datasheet)
  writeRegister(0x00, 0x01); // F1 -> ADC1
  writeRegister(0x01, 0x02); // F2 -> ADC2
  writeRegister(0x02, 0x03); // F3 -> ADC3
  writeRegister(0x03, 0x04); // F4 -> ADC4
  writeRegister(0x04, 0x05); // F5 -> ADC5
  writeRegister(0x05, 0x06); // F6 -> ADC6
  writeRegister(0x06, 0x07); // F7 -> ADC1
  writeRegister(0x07, 0x08); // F8 -> ADC2
  writeRegister(0x08, 0x09); // Clear -> ADC3
  writeRegister(0x09, 0x0A); // NIR -> ADC4
  writeRegister(0xAF, 0x00); // Kết thúc SMUX
}
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Wire.begin(); // Khởi tạo I2C (PB6: SCL, PB7: SDA trên STM32F103RCT6)

  // Kiểm tra kết nối với AS7341
  Wire.beginTransmission(AS7341_ADDR);
  if (Wire.endTransmission() != 0) {
    Serial.println("Không tìm thấy AS7341! Kiểm tra kết nối.");
    while (1) delay(10);
  }

  // Cấu hình cảm biến
  writeRegister(REG_ENABLE, 0x01); // Bật nguồn (PON = 1)
  delay(10);
  configureSMUX();
  writeRegister(REG_ATIME, 100);   // Thời gian tích hợp = 100
  writeRegister(REG_ASTEP_L, 0xFF); // ASTEP = 999 (0x03E7)
  writeRegister(REG_ASTEP_H, 0x03);
  writeRegister(REG_CFG1, 0x07);   // GAIN = 256x (0x07)
  writeRegister(REG_ENABLE, 0x03); // Bật đo quang phổ (PON + SP_EN)

}

void loop() {
  uint16_t channels[11];

  // Chờ dữ liệu sẵn sàng với timeout
  unsigned long startTime = millis();
  while (!(readRegister(REG_STATUS2) & 0x08)) {
    if (millis() - startTime > 1000) { // Timeout sau 1 giây
      Serial.println("Timeout chờ dữ liệu!");
      return;
    }
    delay(10);
  }

  // Đọc dữ liệu
  Wire.beginTransmission(AS7341_ADDR);
  Wire.write(REG_CH0_L);
  Wire.endTransmission(false);
  if (Wire.requestFrom(AS7341_ADDR, 22) != 22) {
    Serial.println("Lỗi đọc dữ liệu từ AS7341!");
    return;
  }
  Serial.println("bytes: ");
  for (int i = 0; i < 11; i++) {
    uint8_t low = Wire.read();
    uint8_t high = Wire.read();
    channels[i] = (high << 8) | low;
    Serial.print(String(low) + " " + String(high) + " - ");
  }
  Serial.println("");
  // In giá trị
  Serial.println("Gia tri 11 kenh cua AS7341:");
  Serial.print("F1 (415nm): "); Serial.println(channels[0]);
  Serial.print("F2 (445nm): "); Serial.println(channels[1]);
  Serial.print("F3 (480nm): "); Serial.println(channels[2]);
  Serial.print("F4 (515nm): "); Serial.println(channels[3]);
  Serial.print("F5 (555nm): "); Serial.println(channels[4]);
  Serial.print("F6 (590nm): "); Serial.println(channels[5]);
  Serial.print("F7 (630nm): "); Serial.println(channels[6]);
  Serial.print("F8 (680nm): "); Serial.println(channels[7]);
  Serial.print("Clear: "); Serial.println(channels[8]);
  Serial.print("NIR (~910nm): "); Serial.println(channels[9]);
  Serial.print("Flicker: "); Serial.println(channels[10]);

  delay(1000);
}

// Hàm ghi dữ liệu vào thanh ghi
void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(AS7341_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// Hàm đọc 1 byte từ thanh ghi
uint8_t readRegister(uint8_t reg) {
  Wire.beginTransmission(AS7341_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(AS7341_ADDR, 1);
  return Wire.read();
}
