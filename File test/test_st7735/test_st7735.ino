// ST7735 library example
// 160x128 pixel JPEG Slideshow
// Requires Arduino_ST7735_STM and JpgDecoder_STM libraries and stm32duino
// (C)2019-20 Pawel A. Hernik
// YouTube video: https://youtu.be/kK9MdP6MZYc

/*
  ST7735 128x160 1.8" LCD pinout (header at the top, from left):
  #1 LED   -> 3.3V
  #2 SCK   -> SCL/D13/PA5
  #3 SDA   -> MOSI/D11/PA7
  #4 A0/DC -> D8/PA1  or any digital
  #5 RESET -> D9/PA0  or any digital
  #6 CS    -> D10/PA2 or any digital
  #7 GND   -> GND
  #8 VCC   -> 3.3V
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#define TFT_CS  PA2
#define TFT_DC  PA1
#define TFT_RST PA0
#include <Arduino_ST7735_STM.h>
Arduino_ST7735 lcd = Arduino_ST7735(TFT_DC, TFT_RST, TFT_CS);
//#include "0.h"
//#include "1.h"
//#include "2.h"
//#include "3.h"
//#include "4.h"
//#include "5.h"
//#include "6.h"
#include "frame.h"
//#include "meo1.h"
//#include "meo2.h"
//#include "meo3.h"
//#include "meo4.h"
//#include "meo5.h"
//#include "meo6.h"
//#include "meo7.h"
//#include "meo8.h"
//#include "meo9.h"
//#include "meo10.h"
//#include "meo11.h"
//#include "meo12.h"
//#include "meo13.h"
//#include "meo14.h"
//#include "meo15.h"
// -------------------
#include <SdFat.h>
// undefine USE_SDFAT_LIBRARY in JpgDecoder_STM.h if SdFat is not used

#include <JpgDecoder_STM.h>

// the callback function below renders decoded JPEG on the LCD
bool renderLCD(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  //lcdSPI();
  if (y >= lcd.height()) return 0; // 0 - to stop decoding
  lcd.drawImage(x, y, w, h, bitmap);
  return 1; // 1 - to decode next block
}

// align to a 32 bit boundary for the best performance
#define DECODE_BUFFER_LEN 3100
uint8_t decodeBuffer[DECODE_BUFFER_LEN] __attribute__((aligned(4)));
JpgDecoder jpeg(renderLCD, decodeBuffer, DECODE_BUFFER_LEN);
// -------------------

void setup()
{
  Serial.begin(115200);
  lcd.begin();
  lcd.fillScreen(BLACK);
  lcd.setRotation(3);
}

char txt[50];
int del = 1000;
int scale = 0;
int thumb = 0;
int cnt = 0;

void showJPG(const unsigned char *jpg, int jpglen)
{
  uint16_t w, h;
  jpeg.getDim(&w, &h, jpg, jpglen);
  uint32_t t = millis();
  if (thumb) {
    switch (cnt) {
      case 0: jpeg.show( 0,  0, jpg, jpglen, scale); break;
      case 1: jpeg.show(80,  0, jpg, jpglen, scale); break;
      case 2: jpeg.show( 0, 64, jpg, jpglen, scale); break;
      case 3: jpeg.show(80, 64, jpg, jpglen, scale); break;
    }
  } else jpeg.show(0, 0, jpg, jpglen, scale);
  t = millis() - t;
  Serial.print(w); Serial.print(" x "); Serial.print(h); Serial.print(", len = "); Serial.print(jpglen);
  Serial.print(", time = "); Serial.print(t); Serial.println(" ms");
  if (del > 0 && !thumb) delay(del);
  if (del > 0 && thumb && cnt == 3) delay(del);
  if (++cnt > 3) cnt = 0;
}

void slideshow()
{

  //  showJPG(meo1, sizeof(meo1));
  //  showJPG(meo2, sizeof(meo2));
  //  showJPG(meo3, sizeof(meo3));
  //  showJPG(meo4, sizeof(meo4));
  //  showJPG(meo5, sizeof(meo5));
  //  showJPG(meo6, sizeof(meo6));
  //  showJPG(meo7, sizeof(meo7));
  //  showJPG(meo8, sizeof(meo8));
  //  showJPG(meo9, sizeof(meo9));
  //  showJPG(meo10, sizeof(meo10));
  //  showJPG(meo11, sizeof(meo11));
  //  showJPG(meo12, sizeof(meo12));
  //  showJPG(meo13, sizeof(meo13));
  //  showJPG(meo14, sizeof(meo14));
  //  showJPG(meo15, sizeof(meo15));

  //  showJPG(cu0, sizeof(cu0));
  //  showJPG(cu1, sizeof(cu1));
  //  showJPG(cu2, sizeof(cu2));
  //  showJPG(cu3, sizeof(cu3));
  //  showJPG(cu4, sizeof(cu4));
  //  showJPG(cu5, sizeof(cu5));
  //  showJPG(cu6, sizeof(cu6));

  showJPG(frame0, sizeof(frame0));
  showJPG(frame1, sizeof(frame1));
  showJPG(frame2, sizeof(frame2));
  showJPG(frame3, sizeof(frame3));
  showJPG(frame4, sizeof(frame4));
//  showJPG(frame5, sizeof(frame5));
//  showJPG(frame6, sizeof(frame6));
//  showJPG(frame7, sizeof(frame7));
//  showJPG(frame8, sizeof(frame8));
//  showJPG(frame9, sizeof(frame9));
//  showJPG(frame10, sizeof(frame10));
//  showJPG(frame11, sizeof(frame11));
//  showJPG(frame12, sizeof(frame12));
//  showJPG(frame13, sizeof(frame13));
//  showJPG(frame14, sizeof(frame14));
//  showJPG(frame15, sizeof(frame15));
//  showJPG(frame16, sizeof(frame16));
//  showJPG(frame17, sizeof(frame17));
//  showJPG(frame18, sizeof(frame18));
//  showJPG(frame19, sizeof(frame19));
//  showJPG(frame20, sizeof(frame20));
//  
//  showJPG(frame21, sizeof(frame21));
//  showJPG(frame22, sizeof(frame22));
//  showJPG(frame23, sizeof(frame23));
//  showJPG(frame24, sizeof(frame24));
//  showJPG(frame25, sizeof(frame25));
//  showJPG(frame26, sizeof(frame26));
//  showJPG(frame27, sizeof(frame27));
//  showJPG(frame28, sizeof(frame28));
//  showJPG(frame29, sizeof(frame29));
//  showJPG(frame30, sizeof(frame30));
//  showJPG(frame31, sizeof(frame31));
//  showJPG(frame32, sizeof(frame32));
//  showJPG(frame33, sizeof(frame33));
//  showJPG(frame34, sizeof(frame34));
//  showJPG(frame35, sizeof(frame35));
//  showJPG(frame36, sizeof(frame36));
//  showJPG(frame37, sizeof(frame37));
//  showJPG(frame38, sizeof(frame38));
//  showJPG(frame39, sizeof(frame39));
//  showJPG(frame40, sizeof(frame40));
//  showJPG(frame41, sizeof(frame41));
//  showJPG(frame42, sizeof(frame42));
//  showJPG(frame43, sizeof(frame43));
//  showJPG(frame44, sizeof(frame44));
//  showJPG(frame45, sizeof(frame45));
//  showJPG(frame46, sizeof(frame46));
//  showJPG(frame47, sizeof(frame47));
//  showJPG(frame48, sizeof(frame48));
//  showJPG(frame49, sizeof(frame49));
//  showJPG(frame50, sizeof(frame50));
//  showJPG(frame51, sizeof(frame51));
//  showJPG(frame52, sizeof(frame52));
//  showJPG(frame53, sizeof(frame53));
//  showJPG(frame54, sizeof(frame54));
//  showJPG(frame55, sizeof(frame55));
//  showJPG(frame56, sizeof(frame56));
//  showJPG(frame57, sizeof(frame57));
//  showJPG(frame58, sizeof(frame58));
//  showJPG(frame59, sizeof(frame59));
//  showJPG(frame60, sizeof(frame60));
//  showJPG(frame61, sizeof(frame61));
//  showJPG(frame62, sizeof(frame62));
//  showJPG(frame63, sizeof(frame63));
//  showJPG(frame64, sizeof(frame64));
//  showJPG(frame65, sizeof(frame65));
//  showJPG(frame66, sizeof(frame66));
//  showJPG(frame67, sizeof(frame67));
//  showJPG(frame68, sizeof(frame68));
//  showJPG(frame69, sizeof(frame69));
//  showJPG(frame70, sizeof(frame70));
//  showJPG(frame71, sizeof(frame71));
//  showJPG(frame72, sizeof(frame72));
//  showJPG(frame73, sizeof(frame73));
//  showJPG(frame74, sizeof(frame74));
//  showJPG(frame75, sizeof(frame75));
//  showJPG(frame76, sizeof(frame76));
//  showJPG(frame77, sizeof(frame77));
//  showJPG(frame78, sizeof(frame78));
//  showJPG(frame79, sizeof(frame79));
//  showJPG(frame80, sizeof(frame80));
//  showJPG(frame81, sizeof(frame81));
//  showJPG(frame82, sizeof(frame82));
//  showJPG(frame83, sizeof(frame83));
//  showJPG(frame84, sizeof(frame84));
//  showJPG(frame85, sizeof(frame85));
//  showJPG(frame86, sizeof(frame86));
//  showJPG(frame87, sizeof(frame87));
//  showJPG(frame88, sizeof(frame88));
//  showJPG(frame89, sizeof(frame89));
//  showJPG(frame90, sizeof(frame90));
//  showJPG(frame91, sizeof(frame91));
//  showJPG(frame92, sizeof(frame92));
//  showJPG(frame93, sizeof(frame93));

}

void loop()
{
  del = 0; scale = 0; thumb = 0;
  slideshow();

}
