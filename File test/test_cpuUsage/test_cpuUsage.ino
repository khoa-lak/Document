#include <Arduino.h>

unsigned long totalExecutionTime = 0;
unsigned long totalIdleTime = 0;
unsigned long lastExecutionTime = 0;
unsigned long lastIdleTime = 0;
uint32_t lastTick = micros();
uint32_t lastLoop = 0;
uint32_t loopCounter = 0;
void setup() {
  Serial.begin(9600);
}

//void loop() {
//  unsigned long startTime = micros();
//
//  // Đo lường thời gian thực thi của một phần code ở đây
//  //delay(1); // Ví dụ: Delay 10ms
//
//  unsigned long executionTime = micros() - startTime;
//  totalExecutionTime += executionTime;
//
//  // Đo lường thời gian "idle"
//  unsigned long idleTime = 10000 - executionTime; // 10ms - thời gian thực thi
//  totalIdleTime += idleTime;
//
//  lastExecutionTime = executionTime;
//  lastIdleTime = idleTime;
//
//  // Tính toán CPU usage
//  float cpuUsage = (totalExecutionTime / (float)(totalExecutionTime + totalIdleTime)) * 100.0;
//
//  Serial.print("CPU Usage (%): ");
//  Serial.println(cpuUsage);
//
//  delay(1000); // Đợi 1 giây trước khi tính toán lại
//}

void loop() {
  calCpu();
 //uint8_t del = random(0, 10);
  //Serial.println("delay là: " + String(del));
  delay(1);
}
void calCpu() {
  loopCounter++; // Tăng giá trị của loopCounter mỗi lần lặp
  uint32_t currentTick = micros();
  uint32_t currentLoop = loopCounter;
  float cpuUsage = 0.0;
  if (currentTick - lastTick >= 1000000) { // Kiểm tra mỗi giây
    uint32_t ticksElapsed = currentTick - lastTick;
    uint32_t loopsElapsed = currentLoop - lastLoop;
    cpuUsage = (float)loopsElapsed / ticksElapsed * 100; // CPU Usage trong 1 giây
    lastTick = currentTick;
    lastLoop = currentLoop;
    Serial.println(cpuUsage);
  }
  //delay(1);
}
