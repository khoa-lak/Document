#include <SoftwareSerial.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        PA9  //Serial Receive pin
#define SSerialTX        PA10  //Serial Transmit pin

#define SSerialTxControl PB7   //RS485 Direction control

#define RS485Transmit   LOW
#define RS485Receive     HIGH

#define Pin13LED         PC13

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

/*-----( Declare Variables )-----*/
int byteReceived;
int byteSend;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);


  pinMode(Pin13LED, OUTPUT);
  pinMode(SSerialTxControl, OUTPUT);

  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver

  // Start the software serial port, to another device
  Serial1.begin(4800);   // set the data rate

}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  Serial.println("YourDuino.com SoftwareSerial remote loop example");
  Serial.println("Use Serial Monitor, type in upper window, ENTER");
  //digitalWrite(Pin13LED, HIGH);  // Show activity
  //  if (Serial.available())
  //  {
  //    byteReceived = Serial.read();
  //
  //    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit
  //    RS485Serial.write(byteReceived);          // Send byte to Remote Arduino
  //
  //    digitalWrite(Pin13LED, LOW);  // Show activity
  //    delay(10);
  //    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
  //  }

  if (Serial1.available())  //Look for data from other Arduino
  {

    byteSend = Serial1.read();   // Read the byte

    digitalWrite(Pin13LED, HIGH);  // Show activity
    delay(10);
    digitalWrite(Pin13LED, LOW);
    Serial.write(byteSend);
  }
  delay(1000);
}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//NONE
//*********( THE END )***********
