#ifndef SENSORCOLOR_h
#define SENSORCOLOR_h
#include <EEPROM.h>    //ssc chiem 20byte eeprom (0 -> 20)
#include "ConfigPin.h"
class CLASS_SSC {
  private:
    float f_deltaRGB[4] = {0};
    uint8_t ui8_solando = 1;
    uint8_t ui8_solanrgb = 1;
    bool (CLASS_SSC::*Func_RunningCalib)();
    uint16_t RBG_off[4] = {0};
    uint16_t RBG_on[4] = {0};
    uint32_t ui32_deltaTru = 0;
    uint32_t ui32_delayCalib = 0;
    uint32_t ui32_delayReadRGB = 0;
    uint32_t ui32_delayLED = 0;
    uint16_t Func_RGBColor(uint8_t _S2, uint8_t _S3);
    float Func_Convert(float value, uint8_t order);  
    bool Func_ReadColor(uint8_t num, float *thongso);
    uint16_t Func_Color(uint8_t _S2, uint8_t _S3);
    void Func_SaveCalib();
    void Func_ReadCalib();
  public:
    void SCFunc_Setup();
    bool SCFunc_Calib();
    void SCFunc_Running(float RGBme[], uint8_t solan);
};
#endif
