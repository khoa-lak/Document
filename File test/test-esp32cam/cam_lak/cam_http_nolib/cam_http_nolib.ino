#include <WiFi.h>
#include "esp_camera.h"

// Thông tin WiFi
const char* ssid = "Huong Long Hai";
const char* password = "khongnhonua";

// Thông tin MQTT broker
const char* mqttServer = "mqtt.iotoom.com"; // Ví dụ: "broker.hivemq.com"
const int mqttPort = 1883;
const char* mqttTopic = "devices/esp32cam/video";
const char* clientId = "esp32cam";
const char* mqttUser = "lab";
const char* mqttPass = "IotoomVN";
// Chọn kiểu mã hóa (0: Binary, 1: Hex, 2: Base64)
#define ENCODING_TYPE 0 // Thay đổi giá trị này để chọn: 0, 1, hoặc 2
#define FRAME_RATE_MS 100 // 10 FPS (100ms/frame)
#define VIDEO_DURATION_MS 20000 // 20 giây gửi video
#define REST_DURATION_MS 10000 // 10 giây nghỉ
#define FLASH_PIN 4 // GPIO 4 cho flash trên ESP32-CAM
// Cấu hình camera
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

WiFiClient client;
const char base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char hexTable[] = "0123456789ABCDEF";
camera_config_t config;
void setup() {
  Serial.begin(115200);
  // Cấu hình chân flash
  pinMode(FLASH_PIN, OUTPUT);
  digitalWrite(FLASH_PIN, LOW); // Tắt flash ban đầu
  // Cấu hình chân power down camera
  pinMode(PWDN_GPIO_NUM, OUTPUT);
  digitalWrite(PWDN_GPIO_NUM, HIGH); // Tắt camera ban đầu
  // Khởi tạo camera
  //camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0; // chỉ định kênh tạo tín hiệu clock cho cam (dùng kênh 0, esp có 8 kênh từ 0 -7)
  config.ledc_timer = LEDC_TIMER_0;     // dùng loại kênh timer (dùng timer 0, esp có 4 timer 0-3)
  config.pin_d0 = Y2_GPIO_NUM;          // pin 0 - 7 là 8 chân dữ liệu //(data bus) từ cam về esp
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;      //chân nhận xung clock từ esp
  config.pin_pclk = PCLK_GPIO_NUM;      //chân thông báo mỗi pixel dc gửi
  config.pin_vsync = VSYNC_GPIO_NUM;    //chân phát tín hiệu bắt đầu hoặc kết thức 1 frame
  config.pin_href = HREF_GPIO_NUM;      //chân phát tín hiệu mỗi dòng dữ liệu của ảnh
  config.pin_sscb_sda = SIOD_GPIO_NUM;  //SCL để cấu hình cho cam
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;      //chân bật tắt nguồn camera (high == on, low = off)
  config.pin_reset = RESET_GPIO_NUM;    //chân rst phân cứng cam ( =-1 không dùng chân rst)
  config.xclk_freq_hz = 20000000;       // f = 20MHz
  config.pixel_format = PIXFORMAT_JPEG; //format định dạng ảnh
  config.frame_size = FRAMESIZE_QVGA;   //độ phân giải frame ảnh (FRAMESIZE_QVGA (320x240), FRAMESIZE_VGA (640x480), FRAMESIZE_SVGA (800x600), FRAMESIZE_XGA (1024x768), FRAMESIZE_UXGA (1600x1200))
  config.grab_mode = CAMERA_GRAB_LATEST;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;             //chất lượng độ nén (30 = ~20-30 kB)
  config.fb_count = 2;                  //số frame trong bộ nhớ(2= double buffer, có nghĩa là lấy frame mới trong khi frame cũ đang xử lý)
  /*
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      Serial.printf("Camera init failed with error 0x%x", err);
      delay(1000);
      ESP.restart(); // Khởi động lại ESP32 nếu lỗi
    }
    // Điều chỉnh thông số cảm biến để cải thiện chất lượng ảnh
    sensor_t *s = esp_camera_sensor_get();
    s->set_brightness(s, 1);      // Tăng độ sáng (giá trị: -2 đến 2)
    s->set_contrast(s, 1);        // Tăng độ tương phản (giá trị: -2 đến 2)
    s->set_saturation(s, 1);      // Tăng độ bão hòa màu (giá trị: -2 đến 2)
    s->set_whitebal(s, 1);        // Bật cân bằng trắng tự động (0: tắt, 1: bật)
    s->set_gain_ctrl(s, 1);       // Bật điều khiển gain tự động (0: tắt, 1: bật)
    s->set_exposure_ctrl(s, 1);   // Bật điều khiển phơi sáng tự động (0: tắt, 1: bật)
    s->set_framesize(s, FRAMESIZE_QVGA); // Đồng bộ độ phân giải với cấu hình

    Serial.println("Camera initialized successfully");
    Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());
    =>code cũ chưa bật tắt theo ý
  */
  // Kết nối WiFi
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  connectToMQTT();
}

void loop() {
  startCamera();
  //digitalWrite(FLASH_PIN, HIGH); // Bật flash
  unsigned long startTime = millis();
  while (millis() - startTime < VIDEO_DURATION_MS) {
    unsigned long frameStart = millis();
    if (!client.connected()) {
      connectToMQTT();
    }
    sendFrameToMQTT();
    unsigned long elapsed = millis() - frameStart;
    if (elapsed < FRAME_RATE_MS) {
      delay(FRAME_RATE_MS - elapsed); // Duy trì 10 FPS
    } else {
      Serial.printf("Frame processing took %lu ms, exceeding target %d ms\n", elapsed, FRAME_RATE_MS);
    }
  }
  digitalWrite(FLASH_PIN, LOW); // Tắt flash
  stopCamera();
  // Nghỉ 10 giây
  Serial.println("Camera off, resting for 10 seconds...");
  delay(REST_DURATION_MS);
  /*
    unsigned long startTime = millis();
    if (!client.connected()) {
    connectToMQTT();
    }
    sendFrameToMQTT();

    unsigned long elapsed = millis() - startTime;
    if (elapsed < FRAME_RATE_MS) {
    delay(FRAME_RATE_MS - elapsed); // 10 FPS
    }
    =>code cũ chưa bật tắt theo ý
  */
}
void startCamera() {
  digitalWrite(PWDN_GPIO_NUM, LOW); // Bật nguồn camera
  delay(100);
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  // Điều chỉnh thông số cảm biến
  sensor_t *s = esp_camera_sensor_get();
  s->set_brightness(s, 1);
  s->set_contrast(s, 1);
  s->set_saturation(s, 1);
  s->set_whitebal(s, 1);
  s->set_gain_ctrl(s, 1);
  s->set_exposure_ctrl(s, 1);
  s->set_framesize(s, FRAMESIZE_QVGA);

  Serial.println("Camera started");
}
void stopCamera() {
  esp_camera_deinit(); // Tắt camera
  digitalWrite(PWDN_GPIO_NUM, HIGH); // Tắt nguồn camera
  Serial.println("Camera stopped");
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

void sendFrameToMQTT() {
  //sendMsgToMQTT("Attempting to capture frame...");
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    sendMsgToMQTT("Camera capture failed");
    //delay(100);
    return;
  }
  Serial.printf("Frame captured, size: %u bytes\n", fb->len);
  if (client.connected()) {
    size_t payloadLen = 0;
    uint8_t* payload = NULL;

    if (ENCODING_TYPE == 0) {
      payloadLen = fb->len;
      payload = fb->buf;
      //      Serial.printf("Sending binary frame, size: %u bytes\n", payloadLen);
      //      Serial.println("data:...... ");
      //      for (size_t i = 0; i < payloadLen; i ++ ) {
      //        Serial.print(payload[i]);
      //        Serial.print(" ");
      //      }
      //      Serial.println("");
    }
    else if (ENCODING_TYPE == 1) {
      payloadLen = fb->len * 2;
      payload = (uint8_t*)malloc(payloadLen + 1);
      if (!payload) {
        Serial.println("Memory allocation failed");
        esp_camera_fb_return(fb);
        return;
      }
      for (size_t i = 0; i < fb->len; i++) {
        payload[i * 2] = hexTable[fb->buf[i] >> 4];
        payload[i * 2 + 1] = hexTable[fb->buf[i] & 0x0F];
      }
      payload[payloadLen] = '\0';
      Serial.printf("Sending hex frame, size: %u bytes\n", payloadLen);
    }
    else if (ENCODING_TYPE == 2) {
      payloadLen = ((fb->len + 2) / 3) * 4;
      payload = (uint8_t*)malloc(payloadLen + 1);
      if (!payload) {
        Serial.println("Memory allocation failed");
        esp_camera_fb_return(fb);
        return;
      }
      size_t j = 0;
      for (size_t i = 0; i < fb->len; i += 3) {
        uint32_t val = 0;
        val |= fb->buf[i] << 16;
        if (i + 1 < fb->len) val |= fb->buf[i + 1] << 8;
        if (i + 2 < fb->len) val |= fb->buf[i + 2];
        payload[j++] = base64Table[(val >> 18) & 0x3F];
        payload[j++] = base64Table[(val >> 12) & 0x3F];
        payload[j++] = (i + 1 < fb->len) ? base64Table[(val >> 6) & 0x3F] : '=';
        payload[j++] = (i + 2 < fb->len) ? base64Table[val & 0x3F] : '=';
      }
      payload[payloadLen] = '\0';
      Serial.printf("Sending base64 frame, size: %u bytes\n", payloadLen);
    }

    size_t topicLen = strlen(mqttTopic);
    size_t remainingLen = topicLen + 2 + payloadLen;

    client.write((uint8_t)0x30); // PUBLISH (QoS 0)
    sendRemainingLength(remainingLen);
    client.write((uint8_t)(topicLen >> 8));
    client.write((uint8_t)(topicLen & 0xFF));
    client.print(mqttTopic);
    client.write(payload, payloadLen);

    if (ENCODING_TYPE != 0) free(payload);
  }
  esp_camera_fb_return(fb);
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
}
void sendRemainingLength(size_t length) {
  do {
    uint8_t digit = length % 128;
    length /= 128;
    if (length > 0) digit |= 0x80;
    client.write((uint8_t)digit);
  } while (length > 0);
}
