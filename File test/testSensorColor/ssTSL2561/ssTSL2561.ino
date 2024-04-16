#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
int latchPin = 9;
int clockPin = 10;
int dataPin = 8;
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);
Adafruit_TSL2561_Unified ts2 = Adafruit_TSL2561_Unified(TSL2561_ADDR_HIGH, 12345);
Adafruit_TSL2561_Unified ts3 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
//#define LED1 8
//#define LED2 9
//#define LED3 10
//#define LED4 11
String inputString = "";
/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor1;
  tsl.getSensor(&sensor1);
  sensor_t sensor2;
  ts2.getSensor(&sensor2);
  sensor_t sensor3;
  ts2.getSensor(&sensor3);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor1:       "); Serial.println(sensor1.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor1.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor1.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor1.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor1.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor1.resolution); Serial.println(" lux");
  Serial.println("------------------------------------");
  Serial.println("");
  Serial.println("------------------------------------");
  Serial.print  ("Sensor2:       "); Serial.println(sensor2.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor2.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor2.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor2.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor2.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor2.resolution); Serial.println(" lux");
  Serial.println("------------------------------------");
  Serial.println("");
  Serial.println("------------------------------------");
  Serial.print  ("Sensor3:       "); Serial.println(sensor3.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor3.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor3.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor3.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor3.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor3.resolution); Serial.println(" lux");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
  ts2.enableAutoRange(true);
  ts2.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
  ts3.enableAutoRange(true);
  ts3.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
  /* Update these values depending on what you've set above! */
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(9600);
  Serial.println("Light Sensor Test"); Serial.println("");
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //  pinMode(LED1, OUTPUT);
  //  pinMode(LED2, OUTPUT);
  //  pinMode(LED3, OUTPUT);
  //  pinMode(LED4, OUTPUT);
  //  digitalWrite(LED1, 0);
  //  digitalWrite(LED2, 0);
  //  digitalWrite(LED3, 0);
  //  digitalWrite(LED4, 0);

  /* Initialise the sensor */
  //use tsl.begin() to default to Wire,
  //tsl.begin(&Wire2) directs api to use Wire2, etc.
  if (!tsl.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561-1 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  if (!ts2.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561-2 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  if (!ts2.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561-3 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  //  /* Display some basic information on this sensor */
  displaySensorDetails();
  //
  //  /* Setup the sensor gain and integration time */
  configureSensor();
  //
  //  /* We're ready to go! */
  Serial.println("");
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  //test4LED();
  if (inputString == "d") {
    //    tetsCB();
    test6LED();
    inputString = "";
  }


}
void tetsCB() {
  Serial.print("ss1: ");
  sensors_event_t event1;
  tsl.getEvent(&event1);
  /* Display the results (light is measured in lux) */
  if (event1.light)
  {
    Serial.print(event1.light); Serial.println(" lux");
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor 1 overload");
  }
  delay(250);

  Serial.print("ss2: ");
  sensors_event_t event2;
  ts2.getEvent(&event2);
  /* Display the results (light is measured in lux) */
  if (event2.light)
  {
    Serial.print(event2.light); Serial.println(" lux");
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor 2 overload");
  }
  delay(250);
  Serial.print("ss3: ");

  sensors_event_t event3;
  ts3.getEvent(&event3);
  /* Display the results (light is measured in lux) */
  if (event3.light)
  {
    Serial.print(event3.light); Serial.println(" lux");
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor 3 overload");
  }
  delay(250);
}
void test6LED() {
  uint8_t data1 = 0;
  uint8_t data2 = 0;
  uint8_t data3 = 0;
  for (int i = 0 ; i < 3; i++) {
    Serial.println("cụm led số: " + String(i + 1));
    for (int j = 1; j < 7; j++) {

      if (i == 0) {
        data1 = (1 << j);
        data2 = 0;
        data3 = 0;
      } else if (i == 1 ) {
        data2 = (1 << j);
        data1 = 0;
        data3 = 0;
      } else if (i == 2 ) {
        data3 = (1 << j);
        data1 = 0;
        data2 = 0;
      }
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, data3);
      shiftOut(dataPin, clockPin, MSBFIRST, data2);
      shiftOut(dataPin, clockPin, MSBFIRST, data1);
      digitalWrite(latchPin, HIGH);
      Serial.print("led so " + String(j ));
      delay(1000);
      if (i == 0) {
        sensors_event_t event;
        tsl.getEvent(&event);
        Serial.println(" gia tri led: " + String(event.light));
      }
      if (i == 1) {
        sensors_event_t event;
        ts2.getEvent(&event);
        Serial.println(" gia tri led: " + String(event.light));
      }
      if (i == 2) {
        sensors_event_t event;
        ts3.getEvent(&event);
        Serial.println(" gia tri led: " + String(event.light));
      }
      digitalWrite(latchPin, LOW);


      shiftOut(dataPin, clockPin, LSBFIRST, 0);
      shiftOut(dataPin, clockPin, LSBFIRST, 0);
      shiftOut(dataPin, clockPin, LSBFIRST, 0);
      digitalWrite(latchPin, HIGH);
    }
    delay(8000);
  }
  //  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  //  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  //  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  //  digitalWrite(latchPin, LOW);
  //  digitalWrite(latchPin, HIGH);
  Serial.println("da xong");
  inputString = "";
}
void serialEvent() {
  inputString = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n' && inChar != ' ' && inChar != '\r') {
      inputString += inChar;
    }
  }
  Serial.println(inputString);
}
//void test4LED() {
//  Serial.println("bat dau do:");
//  uint32_t sum = 0;
//  for (int i = 0; i < 5 ; i++) {
//    digitalWrite(LED1, 1);
//    delay(500);
//    sensors_event_t event;
//    tsl.getEvent(&event);
//    if (event.light)
//    {
//      sum += event.light;
//    }
//    else
//    {
//      sum += 0;
//    }
//    digitalWrite(LED1, 0);
//    delay(500);
//  }
//  Serial.println("gia tri led 1: " + String(sum / 5.0));
//  sum = 0;
//  for (int i = 0; i < 5 ; i++) {
//    digitalWrite(LED2, 1);
//    delay(500);
//    sensors_event_t event;
//    tsl.getEvent(&event);
//    if (event.light)
//    {
//      sum += event.light;
//    }
//    else
//    {
//      sum += 0;
//    }
//    digitalWrite(LED2, 0);
//    delay(500);
//  }
//  Serial.println("gia tri led 2: " + String(sum / 5.0));
//  sum = 0;
//  for (int i = 0; i < 5 ; i++) {
//    digitalWrite(LED3, 1);
//    delay(500);
//    sensors_event_t event;
//    tsl.getEvent(&event);
//    if (event.light)
//    {
//      sum += event.light;
//    }
//    else
//    {
//      sum += 0;
//    }
//    digitalWrite(LED3, 0);
//    delay(500);
//  }
//  Serial.println("gia tri led 3: " + String(sum / 5.0));
//  sum = 0;
//  for (int i = 0; i < 5 ; i++) {
//    digitalWrite(LED4, 1);
//    delay(500);
//    sensors_event_t event;
//    tsl.getEvent(&event);
//    if (event.light)
//    {
//      sum += event.light;
//    }
//    else
//    {
//      sum += 0;
//    }
//    digitalWrite(LED4, 0);
//    delay(500);
//  }
//  Serial.println("gia tri led 4: " + String(sum / 5.0));
//  sum = 0;
//  //Serial.println("gia tri la: " + String(sum / 5.0));
//  while (1);
//}
