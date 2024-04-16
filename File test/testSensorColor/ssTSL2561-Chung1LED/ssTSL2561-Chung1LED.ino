#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_HIGH, 12345);

#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6
#define LED6 7
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

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{

  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */

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

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  digitalWrite(LED1, 1);
  digitalWrite(LED2, 1);
  digitalWrite(LED3, 1);
  digitalWrite(LED4, 1);
  digitalWrite(LED5, 1);
  digitalWrite(LED6, 1);

  /* Initialise the sensor */
  //use tsl.begin() to default to Wire,
  //tsl.begin(&Wire2) directs api to use Wire2, etc.
  if (!tsl.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561-1 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

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
  if (inputString == "m") {
    //    tetsCB();
    measure();
    inputString = "";
  }


}

void measure() {
  Serial.println("=====================");
  digitalWrite(LED1, 0);
  delay(500);
  Serial.println("gia tri led 1: " + String(data()));
  digitalWrite(LED1, 1);
  digitalWrite(LED2, 0);
  delay(500);
  Serial.println("gia tri led 2: " + String(data()));
  digitalWrite(LED2, 1);
  digitalWrite(LED3, 0);
  delay(500);
  Serial.println("gia tri led 3: " + String(data()));
  digitalWrite(LED3, 1);
  digitalWrite(LED4, 0);
  delay(500);
  Serial.println("gia tri led 4: " + String(data()));
  digitalWrite(LED4, 1);
  digitalWrite(LED5, 0);
  delay(500);
  Serial.println("gia tri led 5: " + String(data()));
  digitalWrite(LED5, 1);
  digitalWrite(LED6, 0);
  delay(500);
  Serial.println("gia tri led 6: " + String(data()));
  digitalWrite(LED6, 1);
  Serial.println("đã đo xong");

}
uint32_t data() {
  delay(500);
  sensors_event_t event;
  tsl.getEvent(&event);
  return event.light;
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
