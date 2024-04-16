
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
Adafruit_TSL2561_Unified ts1 = Adafruit_TSL2561_Unified(TSL2561_ADDR_HIGH, 12345);
String inputString = "";
void setup() {
  Serial.begin(9600);
  Serial.println("Light Sensor Test"); Serial.println("");
  if (!ts1.begin())
  {
    Serial.print("Ooops, no TSL2561-1 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  displaySensorDetails();
  configureSensor();
  Serial.println("");
}

void loop() {
  sensors_event_t event1;
  ts1.getEvent(&event1);
  /* Display the results (light is measured in lux) */
  if (event1.light)
  {
    Serial.print(event1.light); Serial.println(" lux");
  }
  else
  {
    Serial.println("Sensor 1 overload");
  }
}
void displaySensorDetails(void)
{
  sensor_t sensor1;
  ts1.getSensor(&sensor1);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor1:       "); Serial.println(sensor1.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor1.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor1.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor1.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor1.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor1.resolution); Serial.println(" lux");
  Serial.println("------------------------------------");
  delay(500);
}
void configureSensor(void)
{
  ts1.enableAutoRange(true);
  ts1.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
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
