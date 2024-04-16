#ifndef CONFIGPIN_h
#define CONFIGPIN_h
#include <Arduino.h>
/************************Input - Output****************************/
/****Input: I0-PB10, I1-PB11, I2-PC7, I3-PC12, I4-PD2, I5-PB3, I6-PC9, I7-PC8, I8-PB5, I9-PB4, I10-PC15, I11-PC14, I12-PA10, I13-PA8, I14-PA4***/
/****Output:Q0-PC0, Q1-PC1, Q2-PC2, Q3-PC3, Q4-PA0, Q5-PA1, Q6-PA2, Q7-PA3, Q8-PA5, Q9-PA6, Q10-PA7, Q11-PC4, Q12-PC5, Q13-PB0, Q14-PB1 ****/
//***********Full
#define I0  PB10  //limit rotate
#define I1  PB11  //limit d_1
#define I2  PC7   //limit center_1
#define I3  PC12  //limit u_1
#define I4  PD2   //limit u_2
#define I5  PB3   //limit d_2
#define I6  PC9   //limit u_3
#define I7  PC8   //limit d_3
#define I8  PB5   //S0
#define I9  PB4   //S1
#define I10 PC15  //S2
#define I11 PC14  //S3
#define I13 PA8   //Pulse_in
#define I14 PA4   //limit up_waterlevel
#define Q0  PC0  //Pulsestep
#define Q1  PC1  //Dirstep
#define Q2  PC2  //EnStep1
#define Q3  PC3  //EnStep2
#define Q4  PA0  //
#define Q5  PA1  //DirMotor
#define Q6  PA2  //LinearVertical
#define Q7  PA3  //LinearHorizontal
#define Q8  PA5  //PumpIn
#define Q9  PA6  //PumpOut
#define Q10 PA7  //PumpInTube
#define Q11 PC4  //PumpOutTube
#define Q12 PC5  //PumpSantity
#define Q13 PB0  //
#define Q14 PB1  //
#define Q15 PB7  //
#define Q16 PB6  //
#define CFPinLed   PC13
#define CFPinRxSim PA10
#define CFPinTxSim PA9
#define CFPinRx485 PC11
#define CFPinTx485 PC10
#define CFPinEn485 PA15
#define CFPinI2c_SCL PB6
#define CFPinI2c_SDA PB7
#define CFPin_Temp   PB8

#define CFPin_LimitR     digitalRead(I0)
#define CFPin_Limitu1    digitalRead(I1)
#define CFPin_Limitc1    digitalRead(I2)
#define CFPin_Limitd1    digitalRead(I3)
#define CFPin_Limitu2    digitalRead(I4)
#define CFPin_Limitd2    digitalRead(I5)
#define CFPin_Limitu3    digitalRead(I6)
#define CFPin_Limitd3    digitalRead(I7)
#define CFPin_Wateru     digitalRead(I14)
#define CFPin_PulseStep         Q0
#define CFPin_DirStep           Q1
#define CFPin_EnStep1           Q2
#define CFPin_EnStepPump        Q3
#define CFPin_LedSensor         Q4
#define CFPin_DirMotor          Q5
#define CFPin_MotorVertical     Q6
#define CFPin_MotorHorizontal   Q7
#define CFPin_PumpIn            Q8
#define CFPin_PumpOut           Q9
#define CFPin_PumpInTube        Q10
#define CFPin_PumpOutTube       Q11
#define CFPin_PumpSalinity      Q12
#define CFPin_Fan               Q13
#define CFPin_Stir              Q14
#define CFPin_StateTank         Q15
#define CFPin_Gate              Q16

#define CFPin_S0       I8
#define CFPin_S1       I9
#define CFPin_S2       I10
#define CFPin_S3       I11
#define CFPin_PulseIN           I13


class CLASS_CONFIGPIN {
  public:
    void Func_ConfigPin();
};
#endif
