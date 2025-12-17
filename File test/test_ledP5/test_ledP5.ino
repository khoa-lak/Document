#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
//#include <Fonts/TomThumb.h>  // Font nhỏ nhất

#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 1

#define R1_PIN 25
#define G1_PIN 26
#define B1_PIN 27
#define R2_PIN 14
#define G2_PIN 12
#define B2_PIN 13
#define A_PIN 23
#define B_PIN 19
#define C_PIN 5
#define D_PIN 17
#define E_PIN -1
#define LAT_PIN 4
#define OE_PIN 15
#define CLK_PIN 16

HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);
MatrixPanel_I2S_DMA *dma_display = nullptr;

void setup() {
  // Cấu hình các chân
  mxconfig.gpio.r1 = R1_PIN;
  mxconfig.gpio.g1 = G1_PIN;
  mxconfig.gpio.b1 = B1_PIN;
  mxconfig.gpio.r2 = R2_PIN;
  mxconfig.gpio.g2 = G2_PIN;
  mxconfig.gpio.b2 = B2_PIN;
  mxconfig.gpio.a = A_PIN;
  mxconfig.gpio.b = B_PIN;
  mxconfig.gpio.c = C_PIN;
  mxconfig.gpio.d = D_PIN;
  mxconfig.gpio.e = E_PIN;
  mxconfig.gpio.lat = LAT_PIN;
  mxconfig.gpio.oe = OE_PIN;
  mxconfig.gpio.clk = CLK_PIN;
  mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_20M;  // add tần số i2s vào không sẽ bị nhòe chữ
  // mxconfig.double_buff = false;  // Tắt double buffering
  // mxconfig.clkphase = false;     // Thử phase

  // Khởi tạo panel
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(60);  // Giảm độ sáng để giảm nhiễu

  // Xóa màn hình và đặt nền đen
  dma_display->fillScreenRGB888(0, 0, 0);

  // Thiết lập font mặc định và hiển thị văn bản
  dma_display->setTextSize(1);  // Font mặc định
  //dma_display->setFont(&TomThumb);// Font nhỏ nhất
  dma_display->setTextWrap(false);  // Không tự động xuống dòng

  // Vẽ đường viền kiểm tra (màu xám nhạt)
  //dma_display->drawRect(0, 0, 64, 32, dma_display->color565(100, 100, 100));

  // Hàng 1: Chữ trắng trên nền cam
  //dma_display->fillRect(0, 0, 64, 11, dma_display->color565(255, 165, 0));  // Nền cam
  // dma_display->fillRect(0, 0, 64, 11, dma_display->color565(128, 83, 0));  //50% độ sáng
  dma_display->setCursor(3, 0);                                     // Vị trí ô 1 (hàng 1)
  dma_display->setTextColor(dma_display->color565(255, 255, 255));  // Chữ trắng
  dma_display->print("IoTOOM VN");
  // dma_display->setCursor(2, 2);     // Dịch chuyển nhẹ để in đậm
  // dma_display->print("IOTOOM VN");  // Vẽ lần 2


  // // Hàng 2: Chữ đỏ trên nền xanh lá
  // dma_display->setBrightness8(20);
  // dma_display->fillRect(0, 11, 64, 11, dma_display->color565(0, 255, 0));  // Nền xanh lá
  dma_display->setCursor(1, 8);                                 // Vị trí ô 1 (hàng 2)
  dma_display->setTextColor(dma_display->color565(255, 0, 0));  // Chữ đỏ
  dma_display->print("NO2:4.5");
  // dma_display->setCursor(2, 13);  // Dịch chuyển nhẹ để in đậm
  // dma_display->print("NO2:4.5");  // Vẽ lần 2


  // // Hàng 3: Chữ xanh lá trên nền đỏ
  // dma_display->setBrightness8(20);
  // dma_display->fillRect(0, 22, 64, 11, dma_display->color565(255, 0, 0));  // Nền đỏ
  dma_display->setCursor(1, 16);                                // Vị trí ô 1 (hàng 3)
  dma_display->setTextColor(dma_display->color565(0, 255, 0));  // Chữ xanh lá
  dma_display->print("PH:7.56");
  // dma_display->setCursor(2, 24);  // Dịch chuyển nhẹ để in đậm
  // dma_display->print("PH:7.56");  // Vẽ lần 2
  dma_display->setCursor(1, 24);                                // Vị trí ô 1 (hàng 3)
  dma_display->setTextColor(dma_display->color565(0, 255, 0));  // Chữ xanh lá
  dma_display->print("PH:7.56");
}

void loop() {
  // Không cần xử lý trong loop
}