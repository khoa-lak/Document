#ifndef CALCULATE_VALUE_H
#define CALCULATE_VALUE_H

#include <stdint.h>

#define LED1  0
#define LED2  1
#define LED3  2
#define LED4  3
#define LED5  4
#define LED6  5

class CalculateValue {
private:
    uint8_t dropCA = 1;
    const float mlSample = 5.0;
    float preValueX1 = 0.0;
    float preValueX2 = 0.0;

    float valueNH3(uint16_t* ledBefore, uint16_t* ledAfter, float* coef, float inter, uint8_t* leds, uint8_t amount);
    float valueNO2(uint16_t* ledBefore, uint16_t* ledAfter, float* coef, float inter, uint8_t* leds, uint8_t amount);
    float valueCL(uint16_t* ledBefore, uint16_t* ledAfter);
    float valueKH(uint16_t* ledBefore, uint16_t* ledAfter, uint8_t drop, float inter, uint8_t* leds);
    float valueMG(uint16_t* ledBefore, uint16_t* ledAfter, uint8_t drop, float inter, uint8_t* leds);
    float valueCA(uint16_t* ledBefore, uint16_t* ledAfter, uint8_t drop, float inter, uint8_t* leds);
    float calMethod0(float* X, float* coefs, float inter, uint8_t amount);
    float calMethod1(uint16_t ledBefore, uint16_t ledAfter);

public:
    float findValue(uint16_t* ledBefore, uint16_t* ledAfter, uint8_t reagent, uint8_t drop, float* coef, float inter, uint8_t* leds, uint8_t amount);
    bool checkErr(uint16_t* ledBefore, uint16_t* ledAfter, uint8_t type); // type: 1 - water, 0 - reagent
};

#endif