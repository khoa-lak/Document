#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

// Cấu hình các ngưỡng dòng điện (mA) cho động cơ N20
// Các thông số này bạn cần đo thực tế để tinh chỉnh lại
const float THRESHOLD_NO_LOAD = 10.0;   // Dưới 10mA coi như không cấp điện hoặc đứt dây
const float THRESHOLD_NORMAL_MAX = 350.0; // Dòng chạy bình thường của N20 thường < 300mA
const float THRESHOLD_STALL = 500.0;    // Trên 500mA là có dấu hiệu kẹt (Stall)

void setup(void) {
  Serial.begin(115200);
  while (!Serial) { delay(1); }

  Serial.println("Đang khởi tạo INA219...");

  // Khởi tạo I2C trên STM32F103 (PB6, PB7)
  if (!ina219.begin()) {
    Serial.println("Không tìm thấy chip INA219. Vui lòng kiểm tra dây dẫn!");
    while (1) { delay(10); }
  }

  // Tùy chỉnh dải đo để đạt độ chính xác cao nhất cho dòng điện nhỏ
  // N20 dùng dòng nhỏ nên dùng dải 32V, 1A hoặc 16V, 400mA
  ina219.setCalibration_32V_1A();

  Serial.println("Bắt đầu giám sát động cơ N20...");
}

void loop(void) {
  float current_mA = 0;

  // Đọc giá trị dòng điện (lấy trung bình 3 lần để ổn định)
  for(int i=0; i<3; i++) {
    current_mA += ina219.getCurrent_mA();
    delay(10);
  }
  current_mA /= 3.0;

  Serial.print("Dòng điện hiện tại: "); Serial.print(current_mA); Serial.println(" mA");

  // PHÂN TÍCH TRẠNG THÁI
  if (current_mA < THRESHOLD_NO_LOAD) {
    // 1. Cấp điện mà động cơ không chạy (hoặc chưa cấp điện/đứt dây)
    Serial.println("=> TRẠNG THÁI: Không có dòng điện (Động cơ không chạy/Hở mạch)");
  } 
  else if (current_mA > THRESHOLD_STALL) {
    // 2. Chạy mà bị kẹt (Dòng vọt lên rất cao)
    Serial.println("=> CẢNH BÁO: ĐỘNG CƠ BỊ KẸT (STALL)!!!");
    // Bạn có thể thêm lệnh ngắt PWM tại đây để bảo vệ động cơ
  } 
  else if (current_mA > THRESHOLD_NO_LOAD && current_mA <= THRESHOLD_NORMAL_MAX) {
    // 3. Động cơ đang chạy bình thường
    Serial.println("=> TRẠNG THÁI: Động cơ đang quay bình thường.");
  }
  else {
    // Trường hợp dòng điện nằm giữa mức bình thường và mức kẹt (đang quá tải nhẹ)
    Serial.println("=> TRẠNG THÁI: Đang tải nặng.");
  }

  Serial.println("------------------------------------");
  delay(500); // Đọc mỗi 0.5 giây
}