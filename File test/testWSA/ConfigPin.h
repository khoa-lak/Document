#ifndef ConfigPin_h
#define ConfigPin_h
#include <Arduino.h>

#define X0 PB9  //HT Rotate 
#define X1 PB8  //CTHT UP Step2
#define X2 PA8  //CTHT DOWN Step2
#define X3 PC9  //CTHT UP Step3
#define X4 PC8  
#define X5 PC7  //CTHT DOWN Step3
#define X6 PC6  
#define X7 PB15 
#define X8 PB14 //PulseInSensor
#define X9 PB13
#define X10 PB12

#define Y0 PA0 //Pulse step1
#define Y1 PA4 //Pulse step2
#define Y2 PA2 //Pulse step3
#define Y3 PA3 //Direct step1
#define Y4 PA6 //Direct step23
#define Y5 PA7 //Enable step23
#define Y6 PC4 //Enable step1
#define Y7 PC5 //Magnetic stirrer
#define Y8 PB0 //Pump1
#define Y9 PB1 //Pump2
#define Y10 PB10 //DirPump

#define Y11 PC2 //S0
#define Y12 PC3 //S1
#define Y13 PA5 //S2
#define Y14 PB6 //S3

#define X14_I2CSDA PB7
#define X15_MOSI PB5

#define X17_RX1 PD2   //UART SIM76xx
#define Y17_TX1 PC12

#define ADC1 PC0

#define limit_Home1   digitalRead(X0)
#define limit_Up2     digitalRead(X1)
#define limit_Down2   digitalRead(X2)
#define limit_Up3     digitalRead(X3)
#define limit_Down3   digitalRead(X5)
#define btn_Run       digitalRead(X10)
#define btn_1         X9
#define frequency_PulseIn  pulseIn(X8,LOW)

#define pulse_Step1 Y0
#define pulse_Step2 Y1
#define pulse_Step3 Y2
#define dir_Step1   Y3
#define dir_Step23  Y4
#define en_Step23   Y5
#define en_Step1    Y6
#define en_Magnetic Y7
#define pump1       Y8
#define pump2       Y9
#define dir_pump    Y10

#define S0    Y11
#define S1    Y12
#define S2    Y13
#define S3    Y14

class ConfigPin {
  public:
    void Config_InOut();

};
#endif
