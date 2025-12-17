#include <SPI.h>
#include <SD.h>

#define SD_CS PA4 // Chân CS của mô-đun SD
File logFile;
unsigned long lastLogTime = 0;
unsigned long interval = millis(); // 5 phút (300,000 ms)

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // Khởi tạo SD
  if (!SD.begin(SD_CS)) {
    Serial.println("Khởi tạo SD thất bại!");
    while (1);
  }
  logFile = SD.open("logs.txt", FILE_READ);
  Serial.println(logFile.size());
  logFile.close();
  //SD.remove("logs.txt");
  Serial.println("SD khởi tạo thành công.");

  //  // Mở file log (tạo nếu chưa có)
  //  logFile = SD.open("log.txt", FILE_WRITE);
  //  if (!logFile) {
  //    Serial.println("Không thể mở file log.txt!");
  //    while (1);
  //  }
  //  logFile.close();
}

void loop() {
  //  unsigned long currentTime = millis();
  //
  //  // Ghi log mỗi 1 giây
  //  if (currentTime - lastLogTime >= 1000) {
  //    logFile = SD.open("log.txt", FILE_WRITE);
  //    if (logFile) {
  //      String logData = "Log at " + String(currentTime / 1000) + "s: Data sample";
  //      logFile.println(logData);
  //      Serial.println("Ghi: " + logData);
  //      logFile.close();
  //    } else
  //      Serial.println("Ghi lỗi");
  //    lastLogTime = currentTime;
  //  }
  //
  //  // Đọc và xóa nội dung log sau 5 phút
  //  if (millis() - interval > 20000) {
  //    Serial.println("Đọc logs từ thẻ SD:");
  //    logFile = SD.open("log.txt");
  //    if (logFile) {
  //      while (logFile.available()) {
  //        Serial.write((char)logFile.read());
  //      }
  //      logFile.close();
  //    } else {
  //      Serial.println("Không thể mở file log.txt để đọc!");
  //    }
  //    logFile = SD.open("log.txt", O_WRITE | O_CREAT | O_TRUNC);
  //    if (logFile) {
  //      logFile.close();
  //      Serial.println("Đã xóa nội dung file log.txt");
  //    } else {
  //      Serial.println("Không thể xóa nội dung file log.txt!");
  //    }
  //    interval = millis();
  //  }
}
