//void setup() {
//  Serial.begin(9600);
//}
//bool isLeapYear(int year) {
//  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
//    return true;
//  }
//  return false;
//}
//
//void loop() {
//  delay(5000);
//  // Unix timestamp (example: current time)
//  unsigned long unixTime = 1723537863; // Example Unix timestamp
//
//  // Số giây tương ứng với múi giờ +7
//  unsigned long timezoneOffset = 7 * 3600; // 7 hours * 60 minutes * 60 seconds
//
//  // Chuyển đổi timestamp theo múi giờ +7
//  unixTime += timezoneOffset;
//
//  // Tính toán các thành phần ngày/tháng/năm/giờ
//  unsigned long secondsInDay = 86400; // 24 hours * 60 minutes * 60 seconds
//  unsigned long secondsInYear = 31536000; // 365 days * 24 hours * 60 minutes * 60 seconds
//
//  // Tính toán năm
//  unsigned long year = 1970 + (unixTime / secondsInYear);
//  Serial.println(year);
//  // Tính toán số giây còn lại sau khi loại bỏ các năm
//
//  unsigned long long secondsSinceEpoch = 0;
//  for (int i = 1970; i < year; i++) {
//    if (isLeapYear(i)) {
//      secondsSinceEpoch += 366 * 24 * 60 * 60; // Số giây trong năm nhuận
//    } else {
//      secondsSinceEpoch += 365 * 24 * 60 * 60; // Số giây trong năm không nhuận
//    }
//  }
//  unsigned long remainingSeconds = unixTime - secondsSinceEpoch;
//  // Calculate month
//  unsigned long month = 1;
//  unsigned long secondsInMonth = 0;
//  while (secondsInMonth < remainingSeconds) {
//    switch (month) {
//      case 1: case 3: case 5: case 7: case 8: case 10: case 12:
//        secondsInMonth = 31 * 24 * 60 * 60;
//        break;
//      case 4: case 6: case 9: case 11:
//        secondsInMonth = 30 * 24 * 60 * 60;
//        break;
//      case 2:
//        // Leap year check
//        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
//          secondsInMonth = 29 * 24 * 60 * 60;
//        } else {
//          secondsInMonth = 28 * 24 * 60 * 60;
//        }
//        break;
//    }
//    if (secondsInMonth < remainingSeconds) {
//      remainingSeconds -= secondsInMonth;
//      month++;
//    }
//  }
//  Serial.println(remainingSeconds);
//  // Calculate day
//  unsigned long day = 1 + (remainingSeconds / (24 * 60 * 60));
//
//  // Calculate hours, minutes, seconds
//  unsigned long hours = (remainingSeconds % (24 * 60 * 60)) / (60 * 60);
//  unsigned long minutes = (remainingSeconds % (60 * 60)) / 60;
//  unsigned long seconds = remainingSeconds % 60;
//
//  // Print date/time components
//  Serial.print("Ngày: ");
//  Serial.print(day);
//  Serial.print("/");
//  Serial.print(month);
//  Serial.print("/");
//  Serial.print(year);
//  Serial.print("  Giờ: ");
//  Serial.print(hours);
//  Serial.print(":");
//  Serial.print(minutes);
//  Serial.print(":");
//  Serial.println(seconds);
//  unsigned long q = day;
//  unsigned long m = month;
//  unsigned long y = (m < 3) ? year - 1 : year;
//  m = (m < 3) ? m + 12 : m;
//  unsigned long K = y % 100;
//  unsigned long J = y / 100;
//  unsigned long h = (q + 13 * (m + 1) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
//
//  // Mã hóa thứ
//  const char* days[] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
//  const char* dayOfWeek = days[h];
//
//  // In ra thứ
//  Serial.print("Thứ: ");
//  Serial.println(dayOfWeek);
//}
#include "TIME_UNIX.h"
#define led PC13
CLASS_TIME_UNIXSTAMP unixTime;
String inputString = "";
uint32_t timeLed = millis();
void setup() {
  Serial.begin(9600);
}


void loop() {
  uint32_t tim = 1723458614;
  char* txt = "24/08/12,17:30:14+07";
  if (inputString == "unix") {
    Serial.println(unixTime.convertToUnix(txt));
    inputString = "";
  }
  else if (inputString == "txt") {
    char* txtt = unixTime.convertToDay(tim);
    Serial.println(txtt);
    delete[] txtt;
    inputString = "";
  }
  else if (inputString == "date") {
    Serial.println(unixTime.convertToDate(txt));
    inputString = "";
  }
  blinkLed();
}
void serialEvent() {
  inputString = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n' && inChar != ' ' && inChar != '\r') {
      inputString += inChar;
    }
  }
  Serial.println(inputString);
}
void blinkLed() {
  uint8_t en = digitalRead(led);
  if (millis() - timeLed > 200) {
    // IWatchdog.reload();
    digitalWrite(led, !en);
    timeLed = millis();
  }
}
//7:20:38.603 -> +CCLK: "70/01/01,00:05:23+00"
//17:20:38.603 ->
//17:20:38.603 -> OK
//17:21:09.517 -> AT+CCLK="14/01/01,02:14:36+08"
//17:21:09.843 -> OK
//17:21:12.712 -> AT+CCLK?
//17:21:12.712 -> +CCLK: "14/01/01,02:14:39+08"
//17:21:09.517 -> AT+CCLK="14/01/01,02:14:36+08"
//17:21:09.843 -> OK
//17:21:12.712 -> AT+CCLK?
//17:21:12.712 -> +CCLK: "14/01/01,02:14:39+08"
