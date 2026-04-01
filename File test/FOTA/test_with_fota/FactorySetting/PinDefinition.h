#ifndef pin_definition_h
#define pin_definition_h
#include <Arduino.h>

#define LED PC13

#define X0 PC1  // ||PC2
#define X1 PB7
#define X2 PB8
#define X3 PB9
#define X4 PC3
#define SENSOR_TEMP X0
#define SWITCH_SENSOR_TEMP X1
#define SWITCH_SENSOR_DO X2
#define SWITCH_DETECT_PUMP X3

// #define Y0 PB5
// #define Y1 PB8
// #define Y2 PB9
// #define Y3 PE0
// #define Y4 PE1
// #define Y5 PE2
// #define LED_300 Y5
// #define LED_400 Y4
// #define LED_500 Y3
// #define LED_600 Y2
// #define LED_700 Y1
// #define LED_800 Y0

// #define ADC1 PC0
// #define ADC2 PC1
// #define ADC3 PC2
// #define ADC4 PC3
// #define ADC5 PA0
// #define ADC6 PA1
// #define DROPS_COUNTER_1 ADC1
// #define DROPS_COUNTER_2 ADC2
// #define DROPS_COUNTER_3 ADC3
// #define DROPS_COUNTER_4 ADC4
// #define CURRENT_SENSOR_1 ADC5
// #define CURRENT_SENSOR_2 ADC6

// #define Y6 PE3
// #define Y7 PE4
// #define Y8 PE5
// #define Y9 PE6
// #define Y10 PA2
// #define Y11 PA3
// #define Y12 PC4
// #define Y13 PC5
// #define Y14 PB0
// #define Y15 PB1
// #define Y16 PE7
// #define Y17 PE8
// #define Y18 PE9
// #define Y19 PE10
// #define Y20 PE11
// #define Y21 PE12
// #define Y22 PE13
// #define Y23 PE14

// #define EMPTY_OUTPUT_1 Y12
// #define PELTIER Y7
// #define PUMP_NEAR_MACHINE Y8
// #define PUMP_MORE_COOLING Y9
// #define PUMP_OUT_CUVETTE Y10
// #define PUMP_OUT_TANK Y11
// #define PUMP_CLEAN_SENSOR Y6
// #define empty1 Y13
// #define PUMP_CYCLE_COOLING Y14
// #define PUMP_VIM Y15
// #define PUMP_PURITY Y16
// #define PUMP_POND Y17
// #define empty2 Y18
// #define STIR_CUVETTE Y19
// #define FAN Y20
// #define LED_CAM Y21

// #define STIR_VIM Y23
// #define STIR_VIAL Y22

// #define Y25 PD8
// #define Y26 PD9
// #define Y27 PD10
// #define Y28 PD11
// #define Y29 PD12
// #define Y30 PD13
// #define Y31 PD14
// #define Y32 PD15
// #define Y33 PC6
// #define Y34 PC7
// #define Y35 PC8
// #define Y36 PC9
// #define Y37 PA8
// #define ENABLE_STEPPER_ROT Y25
// #define DIR_STEPPER_ROT Y26
// #define STEP_STEPPER_ROT Y27
// #define ENABLE_STEPPER_PUMP Y28
// #define DIR_STEPPER_PUMP Y29
// #define STEP_STEPPER_PUMP Y30
// #define ENABLE_STEPPER_LINEAR Y31
// #define DIR_STEPPER_LINEAR_VIM Y32
// #define STEP_STEPPER_LINEAR_VIM Y33
// #define DIR_STEPPER_LINEAR_POND Y34
// #define STEP_STEPPER_LINEAR_POND Y35
// #define DIR_STEPPER_LINEAR_REAGENT Y36
// #define STEP_STEPPER_LINEAR_REAGENT Y37

// #define LORA_IRQ PB10  //D0
// #define LORA_RST PB11
// #define LORA_NSS PB12
// #define LORA_SCK PB13
// #define LORA_MISO PB14
// #define LORA_MOSI PB15

// #define RS485_PWR PD3
// #define RS485_EN PD4
// #define RS485_TX PD5
// #define RS485_RX PD6

#define UART_DTR PC11
#define UART_NET PC10
#define UART_PEN PA15
#define UART_TX PA9
#define UART_RX PA10
// class PinDefinition {
// public:
//   void setupPin() {
//     pinMode(X0, INPUT_PULLUP);
//     pinMode(X1, INPUT_PULLUP);
//     pinMode(X2, INPUT_PULLUP);
//     pinMode(X3, INPUT_PULLUP);
//     pinMode(X4, INPUT_PULLUP);
//     pinMode(X5, INPUT_PULLUP);
//     pinMode(X6, INPUT_PULLUP);

//     pinMode(Y0, OUTPUT);
//     pinMode(Y1, OUTPUT);
//     pinMode(Y2, OUTPUT);
//     pinMode(Y3, OUTPUT);
//     pinMode(Y4, OUTPUT);
//     pinMode(Y5, OUTPUT);
//     pinMode(Y6, OUTPUT);
//     pinMode(Y7, OUTPUT);
//     pinMode(Y8, OUTPUT);
//     pinMode(Y9, OUTPUT);
//     pinMode(Y10, OUTPUT);
//     pinMode(Y11, OUTPUT);
//     pinMode(Y12, OUTPUT);
//     pinMode(Y13, OUTPUT);
//     pinMode(Y14, OUTPUT);
//     pinMode(Y15, OUTPUT);
//     digitalWrite(Y0, 1);
//     digitalWrite(Y1, 1);
//     digitalWrite(Y2, 1);
//     digitalWrite(Y3, 1);
//     digitalWrite(Y4, 1);
//     digitalWrite(Y5, 1);
//     digitalWrite(Y6, 1);
//     digitalWrite(Y7, 1);
//     digitalWrite(Y8, 1);
//     digitalWrite(Y9, 1);
//     digitalWrite(Y10, 1);
//     digitalWrite(Y11, 1);
//   }
// };
#endif
