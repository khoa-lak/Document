#ifndef PIN_CONFIG_h
#define PIN_CONFIG_h
#include <Arduino.h>

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


#define Y0    PC4
#define Y1    PC5
#define Y2    PB0
#define Y3    PB1
#define Y4    PB10
#define Y5    PB11
#define Y6    PB13
#define Y7    PB12
#define Y8    PB14
#define Y9    PB15

#define stir1    Y0
#define stir2    Y1
#define stir3    Y2
#define stir4    Y4
#define stir5    Y5
#define led    PC13

class CLASS_CONFIG {
  public:
    void FUNC_CONFIGPIN();
};
#endif
