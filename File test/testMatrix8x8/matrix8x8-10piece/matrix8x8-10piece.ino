#define DATA_PIN  PA10  // DS (Serial data input)
#define LATCH_PIN PA8   // ST_CP (Latch pin)
#define CLOCK_PIN PA9   // SH_CP (Clock pin)
const uint8_t font8x8[36][8] = {
  // Chữ A
  {0x00, 0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66},
  // Chữ B
  {0x00, 0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C},
  // Chữ C
  {0x00, 0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C},
  // ... (Thêm font cho D-Z, 0-9)
  // Chữ H
  {0x00, 0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66},
  // Chữ E
  {0x00, 0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x7E},
  // Chữ L
  {0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x66, 0x7E},
  // Chữ O
  {0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C},
  // ... (Thêm các ký tự khác nếu cần)
};
const uint8_t row[8] = {0b11111110, 0b11111101, 0b11111011, 0b11110111, 0b11101111, 0b11011111, 0b10111111, 0b01111111};
uint8_t displayBuffer[48];
// Chuỗi hiển thị
char message[] = "A";
int messageLength;
void setup() {
  // Initialize pins
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  // Initialize shift registers
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  messageLength = strlen(message);

}
void loop() {
  // if (messageLength <= 6) {
  //    // Hiển thị tĩnh cho chuỗi ngắn
  //    displayStatic();
  //  }
  //else {
  //    // Chạy chữ cho chuỗi dài
  //    for (int offset = 0; offset <= (messageLength * 8); offset++) {
  //      updateBuffer(offset);
  //      funcDisplayBuffer();
  //      delay(50); // Tốc độ scroll
  //    }
  //  }
  display();
}
void display() {
  for (int row = 0; row < 8; row++) {
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,  font8x8[0][row]); // Kích hoạt cột
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00000000); // Kích hoạt hàng

    digitalWrite(LATCH_PIN, HIGH);
    delay(5);
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00011000); // Kích hoạt cột
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b11111101); // Kích hoạt hàng
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00100100); // Kích hoạt cột
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b11111101); // Kích hoạt hàng
    digitalWrite(LATCH_PIN, HIGH);
    delay(5);
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00100100); // Kích hoạt cột
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b11111011); // Kích hoạt hàng
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00011000); // Kích hoạt cột
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b11111011); // Kích hoạt hàng
    digitalWrite(LATCH_PIN, HIGH);
    delay(5);
    //  }
  }
  void updateBuffer(int offset) {
    memset(displayBuffer, 0, 48); // Xóa buffer
    for (int charIdx = 0; charIdx < messageLength; charIdx++) {
      int fontIdx;
      if (message[charIdx] >= 'A' && message[charIdx] <= 'Z') {
        fontIdx = message[charIdx] - 'A';
      } else if (message[charIdx] >= '0' && message[charIdx] <= '9') {
        fontIdx = 26 + (message[charIdx] - '0');
      } else {
        continue; // Bỏ qua ký tự không hỗ trợ
      }

      // Copy font vào buffer
      for (int col = 0; col < 8; col++) {
        int bufferCol = charIdx * 8 + col - offset;
        if (bufferCol >= 0 && bufferCol < 48) {
          displayBuffer[bufferCol] = font8x8[fontIdx][col];
        }
      }
    }
  }

  // Hiển thị tĩnh
  void displayStatic() {
    for (int i = 0; i < 100; i++) { // Lặp để tạo hiệu ứng mượt
      funcDisplayBuffer();
      delay(10);
    }
  }

  // Hiển thị buffer trên ma trận
  void funcDisplayBuffer() {
    for (int row = 0; row < 8; row++) {
      digitalWrite(LATCH_PIN, LOW);
      // Gửi dữ liệu cột (48 bit = 6 byte)
      for (int col = 5; col >= 0; col--) {
        uint8_t colData = 0;
        for (int bit = 0; bit < 8; bit++) {
          if (displayBuffer[col * 8 + bit] & (1 << row)) {
            colData |= (1 << bit);
          }
        }
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, colData);
      }
      // Gửi dữ liệu hàng
      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 1 << row); // Kích hoạt hàng
      digitalWrite(LATCH_PIN, HIGH);
      delayMicroseconds(1000); // Thời gian quét mỗi hàng
    }
  }
