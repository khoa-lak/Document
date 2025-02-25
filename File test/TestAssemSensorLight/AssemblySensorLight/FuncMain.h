#ifndef FuncMain_h
#define FuncMain_h
#include <Wire.h>
#include <BH1750.h>
#include "TCS34725.h"
#include "simA7600.h"
#define num_led 6
#define led  PC13
class FuncMain {
  private:
    bool first_run = true;
    uint8_t pinLed[num_led] = {PC0, PC1, PC2, PC3, PA4, PA5};
    uint8_t type_sensor = 0;
    float lux;
    float r;
    float g;
    float b;
    uint32_t delay_get_value_sensor = millis() ;
    uint32_t delay_led = millis();
    void handleSensor();
    void valueBH1750();
    void valueTCS34725();
    void valueTEMT6000();
    void valueTSL2561();
    bool setupSim();
    void blinkLed();
  public:
    void mainSetup();
    void mainRunning();
};
#endif
