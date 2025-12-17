#ifndef pin_config_h
#define pin_config_h
#include <Arduino.h>

//#define X0 PB7
//#define X1 PB6
//#define X2 PB5
//#define X3 PB4
//#define X4 PB3
//#define X5 PD2
//#define X6 PC12
//
//#define Y0 PC0
//#define Y1 PC1
//#define Y2 PC2
//#define Y3 PC3
//#define Y4 PA0
//#define Y5 PA4
//#define Y6 PA5
//#define Y7 PA6
//#define Y8 PA7
//#define Y9 PC4
//#define Y10 PC5
//#define Y11 PB0
//#define Y12 PB1
//#define Y13 PB10
//#define Y14 PC6
//#define Y15 PC7
//#define Y16 PC8
//#define Y17 PC9
//#define Y18 PA15
//#define Y19 PC10
//#define Y20 PC11
//
//#define ss_rot     X0
//#define ss_lire    X1
//#define ss_liwa    X2
//#define ss_wa_pond       digitalRead(X3)
//#define ss_wa_pond2      digitalRead(X4)
////#define ss_lire_down    X5
//#define tem_wa    X6
//
//#define thermal_cooling               Y0
//#define pump_out                      Y1
//#define pump_pond                     Y7
//#define pump_purity                   Y3
//#define pump_dis                      Y4
//#define pump_cooling                  Y5
//#define pump_vim                      Y6
//#define pump_add_water_cooling        Y9  //y20
//#define mix_vial                      Y8
//#define mix_cuvette                   Y20  //y9
//#define en_rot                        Y10
//#define dir_rot                       Y11
//#define pulse_rot                     Y12
//#define en_li                         Y13
//#define dir_li                        Y14
//#define pulse_li_re                   Y15
//#define pulse_li_wa                   Y16
//#define en_pump                       Y17
//#define dir_pump                      Y18
//#define pulse_pump_re                 Y19
////#define pulse_pump_wa                 Y20

#define di0_lora PC8
#define rst_lora PB11
#define nss_lora PB12
#define sck_lora PB13
#define mis_lora PB14
#define mos_lora PB15
//#define en_485   PA1
//#define tx_485   PA2
//#define rx_485   PA3
//#define en_UART  PA8
#define led      PC13

class PinConfig {
  public:
    void setupPin();
};
#endif
