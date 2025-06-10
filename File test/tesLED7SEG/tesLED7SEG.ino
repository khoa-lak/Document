// Pin definitions for STM32F103C8T6
#define DATA_PIN  PA10  // DS (Serial data input)
#define LATCH_PIN PA8   // ST_CP (Latch pin)
#define CLOCK_PIN PA9   // SH_CP (Clock pin)

// 7-segment patterns for digits 0-9 (common anode, active low)
const byte digitPatterns[10] = {
  0b10111111, // 0: G
  0b10000110, // 1: ADEFG
  0b11011011, // 2: BCF
  0b11001111, // 3: EFG
  0b11100110, // 4: ADE
  0b11101101, // 5: BCE
  0b11111101, // 6: B
  0b10000111, // 7: DEFG
  0b11111111, // 8: (none)
  0b11101111  // 9: E
};

// Digit selection patterns (active low for common anode)
const byte digitSelect[3] = {
  0b11111110, // Digit 1
  0b11111101, // Digit 2
  0b11111011
};

// Number to display (0-999)
int displayNumber = 000;

void setup() {
  // Initialize pins
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  // Initialize shift registers
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
}

void loop() {

  //
  //  // Extract digits
  //  int hundreds = displayNumber / 100;
  //  int tens = (displayNumber / 10) % 10;
  //  int ones = displayNumber % 10;
  //
  //  // Display each digit with multiplexing
  //  displayDigit(0, hundreds);
  //  delay(5); // Multiplexing delay
  //  displayDigit(1, tens);
  //  delay(5);
  //  displayDigit(2, ones);
  //  delay(5);
  uint32_t delayLed = millis() + 500;
  while (millis() < delayLed)
    displayDigit(0, displayNumber);
  if (displayNumber == 999)
    displayNumber = 0;
  displayNumber = (displayNumber + 111) ;
}

void displayDigit(int digit, int value) {
  int hundreds = displayNumber / 100;
  int tens = (displayNumber / 10) % 10;
  int ones = displayNumber % 10;
  // Shift out segment pattern (first 74HC595)
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitSelect[0]);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitPatterns[hundreds]);
  digitalWrite(LATCH_PIN, HIGH);
  delay(5);
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitSelect[1]);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitPatterns[tens]);
  digitalWrite(LATCH_PIN, HIGH);
  delay(5);
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitSelect[2]);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitPatterns[ones]);
  digitalWrite(LATCH_PIN, HIGH);
  delay(5);
  // Shift out digit selection (second 74HC595)
  // Latch the data


}
