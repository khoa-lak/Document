//#include <ModbusMaster.h>   //thư viện modbus
//#include <SoftwareSerial.h>
//HardwareSerial mod( PA10, PA9);
//
//#define DE PA8 // chân DE được nối với chân số 3 arduino
//// chân RE được nối với chân số 2 arduino
//ModbusMaster node;
//const byte O2[] = {0x01 , 0x03 , 0x00 , 0x00 , 0x00 , 0x06 , 0xc5 , 0xc8};
//byte values[20];
//
//void setup()
//{
//  pinMode(DE, OUTPUT);
//  // Chân cảm biến hồng ngoại được nối với chân số 4 arduino
//  digitalWrite(DE, 0);//Đưa chân DE xuống mức thấp
//  Serial.begin(9600);  //tốc độ baud 115200
//  mod.begin(4800);
////  node.begin(1,mod);  //Slave có ID là 1
////  node.preTransmission(preTransmission);
////  node.postTransmission(postTransmission);
//}
//void loop()
//{
//
//  float val1 ;
//  float val2 ;
//  Calculate();
////  val1 = ((values[4] * 256) + values[6]) * 0.1;
////  Serial.print("อุณหภูมิ: ");
////  Serial.print(values[4]);
////  Serial.print(" °C   /   ");
////  val2 = ((values[7] * 256) + values[4]) * 0.01;
////  Serial.print("ค่าออกซิเจนในน้ำ: ");
////  Serial.print(values[7]);
////  Serial.println(" mg/L ");
//  delay(2000);
//}
//
//byte Calculate() {
//  digitalWrite(DE, 1);
//  delay(100);
//  if (mod.write(O2, sizeof(O2)) == 8) { //
//    digitalWrite(DE, 0);
//    delay(5);
//    for (byte i = 0; i < 20; i++) {
//      //Serial.println(mod.read(),HEX);
//      values[i] = mod.read();
//      Serial.println(values[i],HEX);
//      //Serial.print(" ");
//    }
//    Serial.println();
//  }
//  return values[6];
//}
//
#include <SoftwareSerial.h>
SoftwareSerial RS485(PA3, PA2); // Khởi tạo kết nối với MAX3485 thông qua chân PC10 và PC11
#define DE PA1
String inputString;
//SoftwareSerial RS485(PA10, PA9); // Khởi tạo kết nối với MAX3485 thông qua chân PC10 và PC11
//#define DE PA8
void setup() {
  Serial.begin(9600); // Khởi tạo kết nối Serial với máy tính
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //RS485.begin(4800); // Khởi tạo kết nối RS485 với cảm biến
  RS485.begin(9600); // Khởi tạo kết nối RS485 với cảm biến
  pinMode(DE, OUTPUT);
}

void loop() {
  //byte request[] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x06, 0xc5, 0xFB}; // DORenke
  //byte request[] = {0x01, 0x06, 0x10, 0x22, 0x27, 0x97, 0x77, 0x5e};
  //char request[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x30, 0xDE, 0x45}; // Dữ liệu truyền đi
  // byte request[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x03, 0x05, 0xCB}; //PHsuprema
  // byte request[] = {0x04, 0x03, 0x00, 0x00, 0x00, 0x04, 0x44, 0x5c}; //SanDaxi
  // byte request[] = {0x03, 0x03, 0x20, 0x00, 0x00, 0x06, 0xCF, 0xEA}; //Disen
  //uint8_t request[] = {0x03, 0x03, 0x21, 0x00, 0x00, 0x02, 0xCF, 0xD5};  //01 03 00 00 00 04 44 09
  //uint8_t request[] = {0x01, 0x06, 0x00, 0x0B, 0x00, 0x03, 0xB8, 0x09};
  uint8_t request[] = {0x05, 0x03, 0x26, 0x00, 0x00, 0x06, 0xCF, 0x04};
  uint8_t response[17]; // Dữ liệu nhận về
  digitalWrite(DE, 1);
  RS485.write(request, 8); // Gửi dữ liệu đi
  delay(5); // Đợi cho cảm biến xử lý và trả về dữ liệu
  digitalWrite(DE, 0);
  //while (millis() -  tTimeOut < timeout) {
  RS485.readBytes(response, 17);
  //}
  for (int j = 0; j < 17; j++) {

    Serial.print(response[j], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");
  delay(2000); // Chờ 1 giây trước khi gửi yêu cầu tiếp theo
  //while (1);
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
