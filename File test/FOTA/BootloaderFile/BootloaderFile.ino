// basic bootloader
// #include <Arduino.h>
// #define APP_ADDR 0x08008000
// #define LED_PIN PC13
// uint32_t readWord(uint32_t addr) {
//   return *(volatile uint32_t*)addr;
// }
// void setup() {
//   pinMode(LED_PIN, OUTPUT);
//   for (int i = 0; i < 3; i++) {
//     digitalWrite(LED_PIN, HIGH);
//     delay(100);
//     digitalWrite(LED_PIN, LOW);
//     delay(100);
//   }
//   Serial.begin(115200);
//   delay(2000);
//   Serial.println("\n=== DIAGNOSTIC BOOTLOADER ===");
//   uint32_t sp = readWord(APP_ADDR);
//   uint32_t rv = readWord(APP_ADDR + 4);
//   Serial.print("Stack Pointer: 0x");
//   Serial.println(sp, HEX);
//   Serial.print("Reset Vector: 0x");
//   Serial.println(rv, HEX);
//   if ((sp & 0x2FFE0000) != 0x20000000) {
//     Serial.println("Loi: Khong tim thay code hop le tai 0x08008000");
//     return;
//   }
//   Serial.println("Jumping...");
//   delay(100);
//   Serial.end();
///* 26/2/2026 test thử cmt lại vẫn chuyển được chương trình */
// HAL_RCC_DeInit();
// HAL_DeInit();
// SysTick->CTRL = 0;
// SysTick->LOAD = 0;
// SysTick->VAL = 0;
// for (int i = 0; i < 8; i++) {
//   NVIC->ICER[i] = 0xFFFFFFFF;  // Disable all interrupts
//   NVIC->ICPR[i] = 0xFFFFFFFF;  // Clear all pending interrupts
// }
// SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);

//   uint32_t JumpAddress = *(volatile uint32_t*)(APP_ADDR + 4);
//   void (*reset_handler)(void) = (void (*)(void))JumpAddress;
//   __set_MSP(*(volatile uint32_t*)APP_ADDR);
//   reset_handler();
// }
// void loop() {}

// switch chương trình
// #include <Arduino.h>

// #define APP1_ADDR 0x08008000
// #define APP2_ADDR 0x08010000
// #define FLAG_ADDR 0x0803F800
// #define LED_PIN PC13
// uint32_t readWord(uint32_t addr) {
//   return *(volatile uint32_t*)addr;
// }
// static inline void execute_jump(uint32_t addr) {
//   Serial.println("\n=== DIAGNOSTIC BOOTLOADER ===");
//   Serial.println(addr, HEX);
//   uint32_t sp = readWord(addr);
//   uint32_t rv = readWord(addr + 4);

//   Serial.print("Stack Pointer: 0x");
//   Serial.println(sp, HEX);
//   Serial.print("Reset Vector: 0x");
//   Serial.println(rv, HEX);

//   if ((sp & 0x2FFE0000) != 0x20000000) {
//     Serial.println("Loi: Khong tim thay code hop le tai 0x08008000");
//     return;
//   }
//   Serial.println("Jumping...");
//   delay(100);
//   Serial.end();
//   // if (Serial) {
//   //   Serial.flush();  // Đợi truyền hết dữ liệu
//   //   Serial.end();    // Giải phóng ngoại vi UART
//   // }
//   // __HAL_RCC_USART1_FORCE_RESET();
//   // __HAL_RCC_USART1_RELEASE_RESET();
//   // __HAL_RCC_USART1_CLK_DISABLE();
//   HAL_RCC_DeInit();
//   HAL_DeInit();
//   SysTick->CTRL = 0;
//   SysTick->LOAD = 0;
//   SysTick->VAL = 0;
//   for (int i = 0; i < 8; i++) {
//     NVIC->ICER[i] = 0xFFFFFFFF;  // Disable all interrupts
//     NVIC->ICPR[i] = 0xFFFFFFFF;  // Clear all pending interrupts
//   }
//   SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);

//   uint32_t JumpAddress = *(volatile uint32_t*)(addr + 4);
//   void (*reset_handler)(void) = (void (*)(void))JumpAddress;
//   __set_MSP(*(volatile uint32_t*)addr);
//   reset_handler();
// }
// void setup() {
//   pinMode(LED_PIN, OUTPUT);
//   Serial.begin(115200);
//   delay(2000);
//   uint32_t addr = APP1_ADDR;
//   uint32_t flagValue = *(volatile uint32_t*)FLAG_ADDR;
//   if (flagValue == 0x22222222)
//     addr = APP2_ADDR;
//   Serial.println(flagValue, HEX);
//   execute_jump(addr);
// }

// void loop() {}

// bootloader copy vùng nhớ
#define MAIN_ADDR 0x08008000
#define APP1_STORAGE 0x08018000
#define APP2_STORAGE 0x08028000
#define FLAG_ADDR 0x0803F800
#define APP_SIZE (64 * 1024)  // 64KB
#define LED_PIN PC13
// Hàm tính CRC32 sử dụng Hardware CRC của STM32
uint32_t calculate_crc32(uint32_t start_addr, uint32_t length) {
  __HAL_RCC_CRC_CLK_ENABLE();  // Bật Clock cho bộ CRC
  CRC->CR |= CRC_CR_RESET;     // Reset bộ tính toán

  uint32_t *pData = (uint32_t *)start_addr;
  for (uint32_t i = 0; i < (length / 4); i++) {
    CRC->DR = pData[i];  // Đưa dữ liệu 32-bit vào bộ tính toán
  }
  return CRC->DR;
}
void copy_firmware(uint32_t src, uint32_t dest, uint32_t size) {
  while ((FLASH->SR & FLASH_SR_BSY))
    ;
  if ((FLASH->CR & FLASH_CR_LOCK) != 0) {
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
  }
  // Xóa vùng Main (2KB mỗi Page)
  for (uint32_t addr = dest; addr < dest + size; addr += 0x800) {
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = addr;
    FLASH->CR |= FLASH_CR_STRT;
    while (FLASH->SR & FLASH_SR_BSY)
      ;
  }
  FLASH->CR &= ~FLASH_CR_PER;
  // Chép dữ liệu [cite: 15]
  FLASH->CR |= FLASH_CR_PG;
  for (uint32_t i = 0; i < size; i += 2) {
    *(__IO uint16_t *)(dest + i) = *(__IO uint16_t *)(src + i);
    while (FLASH->SR & FLASH_SR_BSY)
      ;
  }
  FLASH->CR &= ~FLASH_CR_PG;
  FLASH->CR |= FLASH_CR_LOCK;
}
void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(2000);
  uint32_t flag = *(volatile uint32_t *)FLAG_ADDR;
  uint32_t sp_main = *(volatile uint32_t *)MAIN_ADDR;
  // Logic: Nếu Main trống hoặc có Flag -> Chép code
  if (flag == 0x11111111 || (sp_main & 0x2FFE0000) != 0x20000000) {
    copy_firmware(APP1_STORAGE, MAIN_ADDR, APP_SIZE);
  } else if (flag == 0x22222222) {
    copy_firmware(APP2_STORAGE, MAIN_ADDR, APP_SIZE);
  }
  if (flag != 0xFFFFFFFF) {
    copy_firmware(0, FLAG_ADDR, 0x800);  // Xóa page flag
  }
  Serial.println("\n=== DIAGNOSTIC BOOTLOADER 1111111 ===");
  Serial.println(MAIN_ADDR, HEX);
  uint32_t sp = *(volatile uint32_t *)(MAIN_ADDR);
  uint32_t rv = *(volatile uint32_t *)(MAIN_ADDR + 4);
  Serial.print("Stack Pointer: 0x");
  Serial.println(sp, HEX);
  Serial.print("Reset Vector: 0x");
  Serial.println(rv, HEX);
  Serial.println("Jumping...");
  delay(100);
  Serial.end();
  HAL_RCC_DeInit();
  HAL_DeInit();
  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;
  for (int i = 0; i < 8; i++) {
    NVIC->ICER[i] = 0xFFFFFFFF;  // Disable all interrupts
    NVIC->ICPR[i] = 0xFFFFFFFF;  // Clear all pending interrupts
  }
  SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
  uint32_t JumpAddress = *(volatile uint32_t *)(MAIN_ADDR + 4);
  void (*reset_handler)(void) = (void (*)(void))JumpAddress;
  __set_MSP(*(volatile uint32_t *)MAIN_ADDR);
  reset_handler();
}

void loop() {}