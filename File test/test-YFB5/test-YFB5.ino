/*
  Dây đỏ kết nối nguồn 5V arduino
  Dây đen nối GND arduino
  Dây vàng (tín hiệu) kết nối chân số 2 arduino
  Mở Serial Monitor chọn No line ending, baud 9600
  Thổi cho cánh quạt trong cảm biến xoay để kiểm tra
 */

byte statusLed = 13;

byte sensorInterrupt = 0; // 0 = digital pin 2
byte sensorPin = 2;       //--------------------------------------------------chân tín hiệu

float calibrationFactor = 4.5;

volatile byte pulseCount;

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;

void setup()
{

    // Initialize a serial connection for reporting values to the host
    Serial.begin(9600);

    // Set up the status LED line as an output
    pinMode(statusLed, OUTPUT);
    digitalWrite(statusLed, HIGH); // We have an active-low LED attached

    pinMode(sensorPin, INPUT);
    digitalWrite(sensorPin, HIGH);

    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    oldTime = 0;

    // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
    // Configured to trigger on a FALLING state change (transition from HIGH
    // state to LOW state)
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

void loop()
{

    if ((millis() - oldTime) > 1000) // Only process counters once per second
    {
        detachInterrupt(sensorInterrupt);
        flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
        oldTime = millis();
        flowMilliLitres = (flowRate / 60) * 1000;
        totalMilliLitres += flowMilliLitres;

        unsigned int frac;

        // Print the flow rate for this second in litres / minute
        Serial.print("Flow rate: ");
        Serial.print(int(flowRate)); // Print the integer part of the variable
        Serial.print("L/min");
        Serial.print("\t"); // Print tab space

        // Print the cumulative total of litres flowed since starting
        Serial.print("Output Liquid Quantity: ");
        Serial.print(totalMilliLitres);
        Serial.println("mL");
        Serial.print("\t"); // Print tab space
        Serial.print(totalMilliLitres / 1000);
        Serial.print("L");

        // Reset the pulse counter so we can start incrementing again
        pulseCount = 0;

        // Enable the interrupt again now that we've finished sending output
        attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    }
}

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
    // Increment the pulse counter
    pulseCount++;
}
