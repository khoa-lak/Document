// #include <Arduino.h>
// #define LED_PIN PC13

// void setup() {
//   // QUAN TRỌNG NHẤT: Dời Vector Table về đúng địa chỉ bắt đầu của App
//   SCB->VTOR = 0x08008000;
//   __enable_irq();
//   pinMode(LED_PIN, OUTPUT);

//   // Reset USB để máy tính nhận diện lại Serial
//   // pinMode(PA12, OUTPUT);
//   // digitalWrite(PA12, LOW); delay(500);
//   // digitalWrite(PA12, HIGH);
//   // pinMode(PA12, INPUT);


//   Serial.begin(115200);
//   delay(2000);
//   Serial.println("App da chay thanh cong tai 0x08008000!");
// }

// void loop() {
//   digitalWrite(LED_PIN, LOW);
//   delay(1000);
//   digitalWrite(LED_PIN, HIGH);
//   delay(1000);
//   Serial.println("chương trình 1");
// }
#include <Arduino.h>
#define FLAG_ADDR 0x0803F800
// __attribute__((section(".data"), noinline))
void flash_write_ram_direct(uint32_t address, uint32_t value) {
  // 1. Chờ Flash hết bận
  while (FLASH->SR & FLASH_SR_BSY)
    ;

  // 2. Mở khóa Flash (Unlock) trực tiếp bằng thanh ghi
  if ((FLASH->CR & FLASH_CR_LOCK) != 0) {
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
  }

  // 3. Thực hiện xóa Page (Erase)
  FLASH->CR |= FLASH_CR_PER;   // Thiết lập chế độ xóa Page
  FLASH->AR = address;         // Nạp địa chỉ 0x0803F800
  FLASH->CR |= FLASH_CR_STRT;  // Bắt đầu xóa

  // Đợi xóa xong (CPU chạy lệnh while này từ RAM nên không bị đơ)
  while (FLASH->SR & FLASH_SR_BSY)
    ;
  FLASH->CR &= ~FLASH_CR_PER;

  // 4. Thực hiện ghi (Program) - F103 ghi theo từng 16-bit
  FLASH->CR |= FLASH_CR_PG;

  // Ghi 16-bit thấp
  *(__IO uint16_t*)address = (uint16_t)value;
  while (FLASH->SR & FLASH_SR_BSY)
    ;

  // Ghi 16-bit cao
  *(__IO uint16_t*)(address + 2) = (uint16_t)(value >> 16);
  while (FLASH->SR & FLASH_SR_BSY)
    ;

  FLASH->CR &= ~FLASH_CR_PG;
  FLASH->CR |= FLASH_CR_LOCK;  // Khóa lại
}
void switchProgram(uint32_t flagValue) {
  Serial.println("Resetting to switch 2...");
  delay(500);
  Serial.end();
  __disable_irq();
  flash_write_ram_direct(FLAG_ADDR, flagValue);
  NVIC_SystemReset();
}

void setup() {
  SCB->VTOR = 0x08008000;
  __enable_irq();
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);
  delay(2000);
  Serial.println("DA VAO CHUONG TRINH 1. Go '2' de switch.");
}
void loop() {
  digitalWrite(PC13, !digitalRead(PC13));
  delay(1000);
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '2') {
      Serial.println(c);
      switchProgram(0x22222222);  //22222222
    }
  }
}
// void serialEvent() {
//   String inputString;
//   while (Serial.available()) {
//     char inChar = (char)Serial.read();
//     inputString += inChar;
//     if (inChar == '\n') {
//       Serial.println(inputString);
//     }
//   }
// }