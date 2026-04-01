#include <Arduino.h>
#include "FuncMain.h"
FuncMain funcMain;
uint32_t interval_blink = 0;
// #define FLAG_ADDR 0x0803F800
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
  *(__IO uint16_t *)address = (uint16_t)value;
  while (FLASH->SR & FLASH_SR_BSY)
    ;
  // Ghi 16-bit cao
  *(__IO uint16_t *)(address + 2) = (uint16_t)(value >> 16);
  while (FLASH->SR & FLASH_SR_BSY)
    ;
  FLASH->CR &= ~FLASH_CR_PG;
  FLASH->CR |= FLASH_CR_LOCK;  // Khóa lại
}
void switchProgram(uint32_t flagValue) {
  Serial.println("upload new program...");
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
  Serial.println("Chương trình chạy");
  funcMain.setupMain();
  IWatchdog.begin(20000000);
}
void loop() {
  blinkLed();
  if (funcMain.runMain() == 1) {
    switchProgram(0x22222222);  //0x22222222
  } else if (funcMain.runMain() == 2) {
    switchProgram(0x11111111);  //0x22222222
  }
  IWatchdog.reload();

}
void blinkLed() {
  if (millis() - interval_blink > 200) {
    digitalWrite(PC13, !digitalRead(PC13));
    interval_blink = millis();
  }
}