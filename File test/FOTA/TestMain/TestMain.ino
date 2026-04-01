/* TEST BLINK - NO USB - MANUAL VTOR FIX */
#include <Arduino.h>

#define LED_PIN PC13

// --- BIẾN TOÀN CỤC ĐỂ DEBUG ---
// Dùng biến volatile để tránh trình biên dịch tối ưu hóa mất vòng lặp delay
volatile int i = 0;

// --- ĐOẠN MÃ QUAN TRỌNG NHẤT (PRE-INIT) ---
// Hàm này chạy trước cả main(), trước cả SystemInit của Arduino
// Nhiệm vụ: Sửa lại địa chỉ nhà (VTOR) ngay lập tức.
void pre_init(void) __attribute__((constructor));
void pre_init(void) {
  // 1. Khóa mọi ngắt để tránh xung đột với Bootloader cũ
  __disable_irq();

  // 2. Ép cứng thanh ghi VTOR về 0x08008000
  // 0xE000ED08 là địa chỉ của thanh ghi SCB->VTOR
  *((volatile uint32_t *)0xE000ED08) = 0x08008000;

  // 3. Không bật lại ngắt ở đây, để hàm setup() lo.
}
// ------------------------------------------

void setup() {
  // // Kéo chân D+ (PA12) xuống thấp để báo PC là "Tao rút dây rồi"
  // pinMode(PA12, OUTPUT);
  // digitalWrite(PA12, LOW);
  // // Reset thanh ghi USB trong chip (Dùng macro của HAL)
  // // Cái này giúp xóa sạch trạng thái lỗi của Bootloader để lại
  // __HAL_RCC_USB_FORCE_RESET();
  // delay(100);  // Giữ reset một chút
  // __HAL_RCC_USB_RELEASE_RESET();

  // // Đợi thêm chút nữa để PC kịp nhận ra sự ngắt kết nối
  // delay(200);

  // // Trả chân PA12 về cho USB Hardware
  // digitalWrite(PA12, HIGH);
  // pinMode(PA12, INPUT);

  // // --- 3. KHỞI TẠO SERIAL & LED ---
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // Bật lại ngắt (để USB hoạt động)
  __enable_irq();
}

void loop() {
  // Nháy đèn để biết chip đang sống
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));

  // Gửi dữ liệu qua USB
  Serial.println("Hello from APP via FOTA!");

  // // Xử lý lệnh từ máy tính (Ví dụ gõ 'R' để Reset)
  // if (Serial.available()) {
  //   char c = Serial.read();
  //   if (c == 'R' || c == 'r') {
  //     Serial.println("Resetting...");
  //     delay(100);
  //     NVIC_SystemReset();
  //   }
  // }

  delay(500);  // Chớp tắt 0.5s
}