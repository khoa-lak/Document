#ifndef PIN_CONFIG_h
#define PIN_CONFIG_h
#include <Arduino.h>
//define input

#define X0   PC0
#define X1   PC2
#define X2   PC3
#define X3   PA0
#define X4   PA4
#define X5   PA5
#define X6   PA6
#define X7   PA7

#define AI        X0
#define lRot      X5
#define lLiRe     X3
#define lLiWa     X4
#define lWaTank   digitalRead(X2)
#define InNone    digitalRead(X1)
#define tempA     X6
#define tempW     X7

//define ouput
#define AO          PC1
#define pumpOUT     PC4
#define pumpIN      PC5
#define valvePond   PB0
#define valveDir    PB1
#define thermCooler PB10
#define pumpCooling PB11
#define pumpVial    PB12
#define pumpDis     PB13
#define motorMix    PB14
#define fan         PB15
#define stir       PC6
#define pinPos      PC7
#define lampGreen   PC8
#define horn        PC9
#define outnone     PA15
#define led         PC13

#define en485     PA1
#define tx485     PA2
#define rx485     PA3
#define enUART     PA8
//define step
#define enRot         PC10
#define dirRot        PC11
#define pulseRot      PC12
#define enPeris       PC9
#define dirPeris      PA15  //1:dis, 2: pump, 
#define pulsePerisRe  PC10  //1 ml: 18518 pulse => 1drop(0.035ml) = 650 pulse
#define pulsePerisWa  PC11  //1 ml: 12500 
#define enLinear      PB6
#define dirLinear     PB7  //1: up, 0: down
#define pulseLiRe     PB8
#define pulseLiWa     PB9



class CLASS_CONFIG {
  public:
    void FUNC_CONFIGPIN();
};
#endif
