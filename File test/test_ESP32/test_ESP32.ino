#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
// Thông tin WiFi
const char* ssid = "Huong Long Hai";
const char* password = "khongnhonua";
// Thông tin MQTT broker
const char* mqttServer = "mqtt.iotoom.com"; // Ví dụ: "broker.hivemq.com"
const int mqttPort = 1883;
const char* mqttTopic = "devices/DashBoard";
const char* clientId = "DashBoard";
const char* mqttUser = "lab";
const char* mqttPass = "IotoomVN";
#define BEARER_TOKEN "cHdxLTRlZmZjNjBkYjdiNjQ4NjE5OTgxYTU3ZTFlODQ2ZmMy"
#define API_DEVICE "https://apis.iotoom.com/devices/photo_water_quality/settings"
#define API_PONDS "https://apis.iotoom.com/ponds/pond-1863b410b7da4f8cb0c82d17e8ab9d21/get_water_quality?start=last_month&dev_mode=true&last_value=false"
const char* rootCACertificate = nullptr;
WiFiClient client;
WiFiClientSecure httpClient;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  // Cấu hình WiFiClientSecure cho HTTP
  if (rootCACertificate != nullptr) {
    httpClient.setCACert(rootCACertificate); // Đặt root CA cho kết nối an toàn
  } else {
    httpClient.setInsecure(); // Bỏ qua xác minh chứng chỉ (chỉ để test)
  }
  connectToMQTT();
}

void loop() {
  //sendMsgToMQTT("test dash board");
  getApiData();
  delay(4000);
}
void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect(mqttServer, mqttPort)) {
      Serial.println("Connected to MQTT broker");
      size_t userLen = strlen(mqttUser);
      size_t passLen = strlen(mqttPass);
      size_t clientIdLen = strlen(clientId);
      size_t remainingLen = 10 + 2 + clientIdLen + 2 + userLen + 2 + passLen;
      client.write((uint8_t)0x10); // Loại gói: CONNECT
      sendRemainingLength(remainingLen);
      client.write((uint8_t)0x00); // MSB độ dài tên giao thức
      client.write((uint8_t)0x04); // LSB độ dài tên giao thức (4 byte)
      client.write('M');
      client.write('Q');
      client.write('T');
      client.write('T');
      client.write((uint8_t)0x04); // MQTT 3.1.1
      client.write((uint8_t)0xC2); // Connect flags
      client.write((uint8_t)0x00); // MSB keep alive
      client.write((uint8_t)0x3C); // LSB keep alive
      client.write((uint8_t)(clientIdLen >> 8));   // MSB độ dài clientId
      client.write((uint8_t)(clientIdLen & 0xFF)); // LSB độ dài clientId
      client.print(clientId);
      client.write((uint8_t)(userLen >> 8));   // MSB độ dài username
      client.write((uint8_t)(userLen & 0xFF)); // LSB độ dài username
      client.print(mqttUser);
      client.write((uint8_t)(passLen >> 8));   // MSB độ dài password
      client.write((uint8_t)(passLen & 0xFF)); // LSB độ dài password
      client.print(mqttPass);
      Serial.println("Sent CONNECT packet with username and password");
    } else {
      Serial.println("Failed to connect, retrying...");
      delay(2000);
    }
  }
}
void sendMsgToMQTT(const char* payload) {
  size_t topicLen = strlen(mqttTopic);
  size_t remainingLen = topicLen + 2 + strlen(payload);
  client.write((uint8_t)0x30); // PUBLISH (QoS 0)
  sendRemainingLength(remainingLen);
  client.write((uint8_t)(topicLen >> 8));
  client.write((uint8_t)(topicLen & 0xFF));
  client.print(mqttTopic);
  client.write(payload, strlen(payload));
  Serial.println(payload);
}
void sendRemainingLength(size_t length) {
  do {
    uint8_t digit = length % 128;
    length /= 128;
    if (length > 0) digit |= 0x80;
    client.write((uint8_t)digit);
  } while (length > 0);
}
void getApiData() {
  HTTPClient http;

  // Bắt đầu kết nối HTTP
  http.begin(httpClient, API_DEVICE);
  
  // Thêm header
  http.addHeader("Authorization", String("Bearer ") + BEARER_TOKEN);

  // Gửi yêu cầu GET
  Serial.println("Đang gửi yêu cầu GET tới API...");
  int httpCode = http.GET();

  // Kiểm tra phản hồi
  if (httpCode > 0) {
    Serial.printf("Mã phản hồi HTTP: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println("Phản hồi API: " + response);
    } else {
      Serial.println("Yêu cầu API thất bại với mã: " + String(httpCode));
    }
  } else {
    Serial.println("Yêu cầu HTTP thất bại: " + String(http.errorToString(httpCode)));
  }

  // Dọn dẹp
  http.end();
}
