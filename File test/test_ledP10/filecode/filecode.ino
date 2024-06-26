unsigned char Buffer_display[3][16][4];
#define BLACK     0x00
#define WHITE     0x07
#define RED       0x01
#define GREEN     0x02
#define BLUE      0x04
#define MAGENTA   0x05
#define YELLOW    0x03
#define CYAN      0x06

//khai báo các byte mặt nạ để lấy ra từng thành phần màu
#define R_MASK      0x01
#define G_MASK      0x02
#define B_MASK      0x04

#define R1 2
#define G1 3
#define B1 4
#define R2 5
#define G2 6
#define B2 7
#define CLK 8
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2






void setup() {

}

void loop() {
  //Matrix_setpx(0, 7, GREEN); //vẽ điểm ảnh tọa độ 0,7 màu xanh
  //ngat_quet_led();
  digitalWrite(LAT, LOW);
  hang(1);

  chuyen(1, 1, 1, 1, 1, 1);
  digitalWrite(LAT, 1);

}

void chuyen(uint8_t byte_r1, uint8_t  byte_g1, uint8_t  byte_b1, uint8_t  byte_r2, uint8_t  byte_g2, uint8_t  byte_b2)
{
  uint8_t i, mask;
  for (i = 0; i < 8; i++)
  {
    mask = 0x80 >> i;
    uint8_t data3 =  byte_r1 & mask;
    //    G1 = | byte_g1 & mask;
    //    B1 = | byte_b1 & mask;
    //    R2 = | byte_r2 & mask;
    //    G2 = | byte_g2 & mask;
    //    B2 = | byte_b2 & mask;
    shiftOut(R1, CLK, LSBFIRST, data3);
    digitalWrite(CLK, 0);
    digitalWrite(CLK, 1); // Tao xung dich du lieu
  }
}
void hang(unsigned char sohang)
{
  switch (sohang)
  {
    case 0: {
        digitalWrite(A, 0);
        digitalWrite(B, 0);
        digitalWrite(C, 0);
        break;
      }
    case 1: {
        digitalWrite(A, 0);
        digitalWrite(B, 0);
        digitalWrite(C, 1);
        break;
      }
    case 2: {
        digitalWrite(A, 0);
        digitalWrite(B, 1);
        digitalWrite(C, 0);
        break;
      }
    case 3: {
        digitalWrite(A, 0);
        digitalWrite(B, 1);
        digitalWrite(C, 1);
        break;
      }
    case 4: {
        digitalWrite(A, 1);
        digitalWrite(B, 0);
        digitalWrite(C, 0);
        break;
      }
    case 5: {
        digitalWrite(A, 1);
        digitalWrite(B, 0);
        digitalWrite(C, 1);
        break;
      }
    case 6: {
        digitalWrite(A, 1);
        digitalWrite(B, 1);
        digitalWrite(C, 0);
        break;
      }
    case 7: {
        digitalWrite(A, 1);
        digitalWrite(B, 1);
        digitalWrite(C, 1);
        break;
      }
  }
  digitalWrite(OE, 0); //cho phép bảng led sáng
}
//void ngat_quet_led()
//{
//  static unsigned char dem_hang;
//  unsigned char i;
//  digitalWrite(OE, 1) // tắt hết led trước khi truyền
//  for (i = 0; i < 4; i++)chuyen(Buffer_display[0][dem_hang][i], Buffer_display[1][dem_hang][i], Buffer_display[2][dem_hang][i], Buffer_display[0][dem_hang + 8][i], Buffer_display[1][dem_hang + 8][i], Buffer_display[2][dem_hang + 8][i]); //truyền data ra bảng led
//  dem_hang++; if (dem_hang == 8)dem_hang = 0;
//}
