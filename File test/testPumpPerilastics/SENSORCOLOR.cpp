#include "SENSORCOLOR.h"
void CLASS_SSC::Func_SaveCalib() {
  //Serial.println("RGB eeprom write: ");
  uint8_t dem = 0;
  for (int i = 0 ; i < 4 ; i++) {
    if (RBG_off[i] > 255) {
      EEPROM.write(dem, 255);
      EEPROM.write((dem + 1),  RBG_off[i] - 255);
    } else {
      EEPROM.write(dem, RBG_off[i]);
      EEPROM.write((dem + 1), 0);
    }

    if (RBG_on[i] > 255) {
      EEPROM.write((dem + 10), 255);
      EEPROM.write(((dem + 10) + 1), RBG_on[i] - 255);
    } else {
      EEPROM.write((dem + 10), RBG_on[i]);
      EEPROM.write(((dem + 10) + 1), 0);
    }
    dem += 2;
  }
}
void CLASS_SSC::Func_ReadCalib() {
  delay(5000);
  Serial.println("RGB eeprom read: ");
  uint8_t dem = 0;
  for (int i = 0 ; i < 4 ; i++) {
    RBG_off[i] = EEPROM.read(dem) + EEPROM.read((dem + 1));
    RBG_on[i] = EEPROM.read((dem + 10)) + EEPROM.read(((dem + 1) + 10));
    dem += 2;
  }
  Serial.println("RGB off ");
  for (int i = 0 ; i < 4 ; i++) {
    Serial.println(RBG_off[i]);
  }
  Serial.println("RGB on ");
  for (int i = 0 ; i < 4 ; i++) {
    Serial.println(RBG_on[i]);
  }
}
float CLASS_SSC::Func_Convert(float value, uint8_t order) {
  Serial.print("before RBG convert: ");
  Serial.println(value);
  if (value >= RBG_off[order])
    return 0;
  if (value <= RBG_on[order])
    return 255;
  float delta = (RBG_off[order] - RBG_on[order]) / 255.00;
  float convert = (RBG_off[order] - value) / delta;
  //  Serial.println("after RBG convert: ");
  //  Serial.println(convert);
  return value;
}
uint16_t CLASS_SSC::Func_RGBColor(uint8_t _S2, uint8_t _S3) {
  digitalWrite(CFPin_S2, _S2);
  digitalWrite(CFPin_S3, _S3);
  delay(200);
  uint16_t frequency_PulseIn = pulseIn(CFPin_PulseIN, LOW);
  //Serial.println(frequency_PulseIn);
  return frequency_PulseIn;
}
void CLASS_SSC::SCFunc_Setup() {
  // Setting frequency-scaling to 20%
  digitalWrite(CFPin_S0, 1);
  digitalWrite(CFPin_S1, 0);
  Func_ReadCalib();
  //Func_RunningCalib = &CLASS_SSC::Func_PrexCalib;
}
void CLASS_SSC::SCFunc_Running(float RGBme[], uint8_t solan) {
  //  for (uint8_t times = 0 ; times < 3; times++) {
  //    digitalWrite(CFPin_Stir, 0);
  //    delay(1000);
  //  }
  //  digitalWrite(CFPin_Stir, 1);
  float RGB[4] = {0};
  bool done = 0;
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[0]   += Func_RGBColor(0, 0);
  }
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[1]   += Func_RGBColor(1, 1);
  }
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[2]   += Func_RGBColor(0, 1);
  }
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[3]   += Func_RGBColor(1, 0);
  }
  for (int i = 0 ; i < 4 ; i++) {
    RGBme[i] = Func_Convert((RGB[i] / 5.0), i);
  }
}
bool CLASS_SSC::SCFunc_Calib() {
  digitalWrite(CFPin_LedSensor  , 1);
  Serial.println("Put white vials into sensor");
  delay(8000);
  uint32_t RGB[4] = {0};
  digitalWrite(CFPin_LedSensor  , 0);
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[0]   += Func_RGBColor(0, 0);
  }
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[1]   += Func_RGBColor(1, 1);
  }
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[2]   += Func_RGBColor(0, 1);
  }
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[3]   += Func_RGBColor(1, 0);
  }

  Serial.println("White:");
  for (int i = 0 ; i < 4 ; i++) {
    RBG_on[i] = RGB[i] / 5.0;
    RGB[i] = 0;
    if (i == 0) {
      Serial.print( "r: ");
    } else if (i == 1) {
      Serial.print( "g: ");
    } else if (i == 2) {
      Serial.print( "b: ");
    } else if (i == 3) {
      Serial.print( "c: ");
    }
    Serial.println( RBG_on[i]);

  }
  digitalWrite(CFPin_LedSensor  , 1);
  Serial.println("Put black vials into sensor");
  delay(8000);
  digitalWrite(CFPin_LedSensor  , 0);
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[0]   += Func_RGBColor(0, 0);
  }
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[1]   += Func_RGBColor(1, 1);
  }
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[2]   += Func_RGBColor(0, 1);
  }
  for (int i = 0 ; i < 5 ; i ++) {
    RGB[3]   += Func_RGBColor(1, 0);
  }
  Serial.println("Black:");
  for (int i = 0 ; i < 4 ; i++) {
    RBG_off[i] = RGB[i] / 5.0;
    RGB[i] = 0;
    if (i == 0) {
      Serial.print( "r: ");
    } else if (i == 1) {
      Serial.print( "g: ");
    } else if (i == 2) {
      Serial.print( "b: ");
    } else if (i == 3) {
      Serial.print( "c: ");
    }
    Serial.println( RBG_off[i]);
  }
  digitalWrite(CFPin_LedSensor  , 1);
  Func_SaveCalib();
  return 1;
}
