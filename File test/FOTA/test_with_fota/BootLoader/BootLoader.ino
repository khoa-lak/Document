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
    Serial.println(APP1_STORAGE, HEX);
    copy_firmware(APP1_STORAGE, MAIN_ADDR, APP_SIZE);
  } else if (flag == 0x22222222) {
    Serial.println(APP2_STORAGE, HEX);
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