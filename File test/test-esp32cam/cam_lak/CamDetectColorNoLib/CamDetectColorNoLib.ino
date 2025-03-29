#include "esp_camera.h"
#include <WiFi.h>

// Định nghĩa chân camera (giữ nguyên)
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
#define FLASH_PIN 4
void initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_RGB565;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

// Mảng giá trị web-safe
const uint8_t webSafeValues[6] = {0, 51, 102, 153, 204, 255}; //giá trị bộ màu cơ bản 6x6 = 216 màu
// Mảng tên màu (216 tên, đơn giản hóa)
const char* colorNames[216] = {
  "Black", "Dark Maroon", "Dark Red", "Red", "Bright Red", "Light Red",
  "Dark Olive", "Olive", "Brown", "Dark Orange", "Orange", "Light Orange",
  "Dark Green", "Green", "Lime", "Bright Green", "Light Green", "Pale Green",
  "Dark Teal", "Teal", "Sea Green", "Turquoise", "Aqua", "Light Aqua",
  "Dark Blue", "Blue", "Bright Blue", "Sky Blue", "Light Blue", "Pale Blue",
  "Dark Indigo", "Indigo", "Royal Blue", "Cornflower Blue", "Light Indigo", "Pale Indigo",
  "Dark Gray", "Gray", "Slate Gray", "Steel Gray", "Light Gray", "Silver",
  "Dark Brown", "Brown", "Sienna", "Chocolate", "Sandy Brown", "Light Brown",
  "Dark Forest", "Forest Green", "Emerald", "Jade", "Mint", "Pale Mint",
  "Dark cyan", "Cyan", "Bright Cyan", "Aquamarine", "Light Cyan", "Pale Cyan",
  "Navy", "Dark Navy", "Blue Gray", "Slate Blue", "Light Navy", "Pale Navy",
  "Dark Purple", "Purple", "Violet", "Magenta", "Hot Pink", "Light Pink",
  "Very Dark Gray", "Dark Slate", "Slate", "Medium Gray", "Light Slate", "Very Light Gray",
  "Dark Khaki", "Khaki", "Tan", "Beige", "Light Khaki", "Pale Khaki",
  "Dark Lime", "Lime Green", "Bright Lime", "Chartreuse", "Light Lime", "Pale Lime",
  "Dark Sea", "Sea Blue", "Bright Sea", "Light Sea", "Pale Sea", "Very Pale Sea",
  "Dark Sky", "Sky", "Bright Sky", "Light Sky", "Pale Sky", "Very Pale Sky",
  "Dark Violet", "Violet Red", "Bright Violet", "Orchid", "Light Violet", "Pale Violet",
  "Charcoal", "Dark Charcoal", "Grayish Blue", "Grayish Green", "Grayish Red", "Light Charcoal",
  "Dark Gold", "Gold", "Goldenrod", "Bright Gold", "Light Gold", "Pale Gold",
  "Dark Spring", "Spring Green", "Bright Spring", "Light Spring", "Pale Spring", "Very Pale Spring",
  "Dark Aqua", "Bright Aqua", "Light Aqua", "Pale Aqua", "Very Pale Aqua", "Extremely Pale Aqua",
  "Dark Cobalt", "Cobalt", "Bright Cobalt", "Light Cobalt", "Pale Cobalt", "Very Pale Cobalt",
  "Dark Magenta", "Bright Magenta", "Light Magenta", "Pale Magenta", "Very Pale Magenta", "Pink",
  "Dark Steel", "Steel", "Bright Steel", "Light Steel", "Pale Steel", "Very Light Steel",
  "Dark Yellow", "Yellow", "Bright Yellow", "Light Yellow", "Pale Yellow", "Very Pale Yellow",
  "Dark Chartreuse", "Bright Chartreuse", "Light Chartreuse", "Pale Chartreuse", "Very Pale Chartreuse", "Extremely Pale Chartreuse",
  "Dark Turquoise", "Bright Turquoise", "Light Turquoise", "Pale Turquoise", "Very Pale Turquoise", "Extremely Pale Turquoise",
  "Dark Cerulean", "Cerulean", "Bright Cerulean", "Light Cerulean", "Pale Cerulean", "Very Pale Cerulean",
  "Dark Fuchsia", "Fuchsia", "Bright Fuchsia", "Light Fuchsia", "Pale Fuchsia", "Very Pale Fuchsia",
  "Dark Silver", "Silver Gray", "Bright Silver", "Light Silver", "Pale Silver", "White"
};

// Hàm ánh xạ giá trị RGB sang web-safe: dựa trên mảng web-safe lựa ra giá trị web-safe gần đúng so với value
uint8_t mapToWebSafe(uint8_t value, bool isGreen = false) {
  uint16_t scaled = isGreen ? (value * 255 / 63) : (value * 255 / 31); //bởi vì green 6 bit nên chia 63, red&blue là 5 bit
  uint8_t closest = 0;
  int minDiff = 256;
  for (int i = 0; i < 6; i++) {
    int diff = abs(scaled - webSafeValues[i]);
    if (diff < minDiff) {
      minDiff = diff;
      closest = webSafeValues[i];
    }
  }
  return closest;
}

// Chuyển RGB thành chỉ số trong bảng 216 màu
int getColorIndex(uint8_t r, uint8_t g, uint8_t b) {
  //vì mỗi bậc của mảng web-safe là 51 => mỗi giá trị r,g,b sẽ tương ứng vs phần tử mảng nào đó
  int rIdx = r / 51;
  int gIdx = g / 51;
  int bIdx = b / 51;
  /*
   * chuyển đổi hệ số 3 chiều (r,b,g) sang 1 chiều(mảng mùa)
   * nếu tính theo tọa độ 3 chiều: thì mỗi phần tử R sẽ có ngẫu nhiên 6 phần từ G và 6 phần tử B -> r*36
   * chọn dc R, mỗi phần tử G sẽ có ngẫu nhiên 6 phần tử B -> g*6
   * chọn đc R,G thì phần tử B chỉ có 1 -> b
   */
  return rIdx * 36 + gIdx * 6 + bIdx;
}

void detectDominantColor() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  uint32_t colorCounts[216] = {0};

  for (uint32_t i = 0; i < fb->len; i += 2) {
    /*- mỗi pixel RGB565 chiếm 2 byte buf => ghép 2 byte lại
      - buf[i]:8 bit đầu (byte cao) , buf[i+1]: 8 bit sau (byte thấp)
      - vd: buf[i] = 0x10110000, buf[i+1] = 0x00010111
      - fb->buf[i] << 8 => 0x10110000 00000000, sau đó: | 0x00010111 =>  pixel = 0x10110000 00010111
    */
    uint16_t pixel = (fb->buf[i] << 8) | fb->buf[i + 1];
    /*
       - với 16 bit(0x10110000 00010111) ở trên thì 5 bit đầu là đỏ, 6 bit kế là xanh lá, 5 bit cuối là xanh dương
       - red   = (pixel >> 11) = 0x00000000 00010110 => &0x1F (0x00011111) =  0x00000000 00010110 => red = 0x10110
       - green = (pixel >> 5)  = 0x00000101 10000000 => &0x3F (0x00111111) =  0x00000000 10000000 => red = 0x10000000
       - blue  = (pixel)       = 0x10110000 00010111 => &0x1F (0x00011111) =  0x00000000 00010111 => red = 0x10111
    */
    uint8_t rRaw = (pixel >> 11) & 0x1F;
    uint8_t gRaw = (pixel >> 5) & 0x3F;
    uint8_t bRaw = pixel & 0x1F;

    uint8_t r = mapToWebSafe(rRaw, false);
    uint8_t g = mapToWebSafe(gRaw, true);
    uint8_t b = mapToWebSafe(bRaw, false);

    int colorIdx = getColorIndex(r, g, b); // tính ra được màu chủ đạo trong pixel này
    colorCounts[colorIdx]++;
  }

  uint32_t maxCount = 0;
  int dominantIdx = 0;
  for (int i = 0; i < 216; i++) {
    if (colorCounts[i] > maxCount) {
      maxCount = colorCounts[i];   //check xem màu nào có nhiều pixel nhất
      dominantIdx = i;
    }
  }
  //quy ngược mãng 1 chiều ra 3 cơ số (r,g,b)
  int rIdx = dominantIdx / 36;
  int gIdx = (dominantIdx % 36) / 6;
  int bIdx = dominantIdx % 6;
  uint8_t dominantR = webSafeValues[rIdx];
  uint8_t dominantG = webSafeValues[gIdx];
  uint8_t dominantB = webSafeValues[bIdx];

  Serial.printf("Dominant color: %s (RGB: %d, %d, %d) - Pixels: %d\n",
                colorNames[dominantIdx], dominantR, dominantG, dominantB, maxCount);

  esp_camera_fb_return(fb);
}

void setup() {
  Serial.begin(115200);
  pinMode(FLASH_PIN, OUTPUT);
  digitalWrite(FLASH_PIN, LOW);
  delay(1000);
  Serial.println("Starting ESP32-CAM...");
  initCamera();
}

void loop() {

  detectDominantColor();

  delay(5000);
}
