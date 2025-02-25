#include "FuncMain.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
Adafruit_TSL2561_Unified ts1 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
BH1750 lightMeter;
TCS34725 tcs;
SimA7600 simMain;
JsonPWQ jsonMain;
TwoWire WIRE1(PB9,PB8);
void FuncMain::mainSetup() {
  for (uint8_t i = 0; i < num_led ; i++) {
    pinMode(pinLed[i] , OUTPUT);
    digitalWrite(pinLed[i] , 1);
  }
  simMain.setupSim();
  //Wire.begin();
  //  lightMeter.begin();
  //  if (!tcs.attach(Wire))
  //    Serial.println("ERROR: TCS34725 NOT FOUND !!!");
  //  tcs.integrationTime(33); // ms
  //  tcs.gain(TCS34725::Gain::X01);
  if (!ts1.begin(&WIRE1))
  {
    Serial.println("Ooops, no TSL2561-1 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  delay(100);
  ts1.enableAutoRange(true);
  ts1.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
}
void FuncMain::mainRunning() {
  blinkLed();
  handleSensor();
  return;
}
void FuncMain::blinkLed() {
  uint8_t en = digitalRead(led);
  if (millis() - delay_led > 1000) {
    digitalWrite(led, !en);
    delay_led = millis();
  }
}
bool FuncMain::setupSim() {
  if (simMain.initSim()) {
    simMain.followTopic();
    if (simMain.cmdRun)
      return true;
  }
  return false;
}
void FuncMain::handleSensor() {
  if (millis() - delay_get_value_sensor > 60000 || first_run) {
    float arr_value_led[num_led];
    uint16_t arr_value_r[num_led];
    uint16_t arr_value_g[num_led];
    uint16_t arr_value_b[num_led];
    for (uint8_t i = 0 ; i < num_led ; i++) {
      digitalWrite(pinLed[i] , 0);
      delay(10);
      valueTSL2561();
      //      valueTCS34725();
      //      valueTEMT6000();
      //      if (type_sensor == 0) {
      //        valueBH1750();
      //        sprintf(name_sensor, "BH1750");
      //      }
      //      else if (type_sensor == 1) {
      //        valueTCS34725();
      //        sprintf(name_sensor, "TCS34725");
      //      }
      //      else if (type_sensor == 2) {
      //        valueTEMT6000();
      //        sprintf(name_sensor, "TEMT6000");
      //      }
      digitalWrite(pinLed[i] , 1);
      if (lux > 0) {
        arr_value_led[i] = lux ;
        arr_value_r[i] = r;
        arr_value_g[i] = g;
        arr_value_b[i] = b;
        lux = 0;
        r = 0;
        g = 0;
        b = 0;
      }
      delay(1000);
    }
    char* js = jsonMain.serMesData(arr_value_led, arr_value_r, arr_value_g, arr_value_b, num_led);
    //simMain.pubData(js, type_sensor);
    Serial.println(js);
    delete[] js;
    first_run = false;
    delay_get_value_sensor = millis();
    //    if (type_sensor >= 2)
    //      type_sensor = 0;
    //    else
    //      type_sensor++;
  }
  return;
}
void FuncMain::valueBH1750() {
  lux = lightMeter.readLightLevel();
  r = 0;
  g = 0;
  b = 0;
  return;
}
void FuncMain::valueTCS34725() {
  if (tcs.available()) {
    lux = tcs.lux();
    TCS34725::Color color = tcs.color();
    r = color.r;
    g = color.g;
    b = color.b;
  }
  return;
}
void FuncMain::valueTEMT6000() {
  uint16_t val = analogRead(PA0);
  lux = float(val);
  r = 0;
  g = 0;
  b = 0;
  return;
}
void FuncMain::valueTSL2561() {
  sensors_event_t event1;
  ts1.getEvent(&event1);
  uint16_t value = event1.light;
  lux = value * 1.00;
  r = 0;
  g = 0;
  b = 0;
  //Serial.println(value);
  return;
}
