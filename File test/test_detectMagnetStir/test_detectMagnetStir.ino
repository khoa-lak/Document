// // Pin definition
// #define PWM_INPUT_PIN PA0 // TIM2_CH1

// // Variables to store PWM measurements
// volatile uint32_t period = 0; // Period in timer ticks
// volatile uint32_t duty = 0;   // Duty cycle in timer ticks
// volatile float frequency = 0; // Frequency in Hz
// volatile float dutyCycle = 0; // Duty cycle in percentage

// void setup() {
//   Serial.begin(115200); // Initialize Serial for debugging

//   // Enable clock for TIM2
//   RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

//   // Configure PA0 as alternate function (TIM2_CH1)
//   pinMode(PWM_INPUT_PIN, INPUT);

//   // Configure GPIOA PA0 for alternate function (TIM2_CH1)
//   GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0); // Clear configuration
//   GPIOA->CRL |= GPIO_CRL_CNF0_1; // Alternate function push-pull
//   GPIOA->CRL |= GPIO_CRL_MODE0_1; // Output mode, max speed 2 MHz

//   // Configure TIM2 for PWM Input Mode
//   TIM2->CR1 = 0; // Disable timer initially
//   TIM2->PSC = 71; // Prescaler: 72 MHz / (71+1) = 1 MHz timer clock (1 µs tick)
//   TIM2->ARR = 0xFFFF; // Max counter value for maximum period measurement

//   // Configure Channel 1 (Rising edge) and Channel 2 (Falling edge)
//   TIM2->CCMR1 = (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_1); // CC1S=01 (IC1 mapped to TI1), CC2S=10 (IC2 mapped to TI1)
//   TIM2->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC2P; // Enable capture, CC2P=1 for falling edge on CH2
//   TIM2->SMCR = TIM_SMCR_TS_2 | TIM_SMCR_SMS_2; // Trigger on TI1FP1, reset mode
//   TIM2->DIER = TIM_DIER_CC1IE | TIM_DIER_CC2IE; // Enable interrupts for CC1 and CC2

//   // Enable the timer
//   TIM2->CR1 |= TIM_CR1_CEN;

//   // Enable interrupts
//   NVIC_EnableIRQ(TIM2_IRQn);
// }

// // Timer 2 interrupt handler
// extern "C" void TIM2_IRQHandler(void) {
//   if (TIM2->SR & TIM_SR_CC1IF) { // Rising edge (period)
//     period = TIM2->CCR1; // Capture period
//     TIM2->SR &= ~TIM_SR_CC1IF; // Clear interrupt flag
//   }
//   if (TIM2->SR & TIM_SR_CC2IF) { // Falling edge (duty)
//     duty = TIM2->CCR2; // Capture duty
//     TIM2->SR &= ~TIM_SR_CC2IF; // Clear interrupt flag

//     // Calculate frequency and duty cycle
//     if (period > 0) {
//       frequency = 1000000.0 / period; // Timer clock = 1 MHz (1 µs), so period in µs
//       dutyCycle = (duty * 100.0) / period; // Duty cycle in percentage
//     }
//   }
// }

// void loop() {
//   // Print PWM measurements
//   Serial.print("Frequency: ");
//   Serial.print(frequency);
//   Serial.print(" Hz, Duty Cycle: ");
//   Serial.print(dutyCycle);
//   Serial.println(" %");
//   delay(500); // Update every 500 ms
// }


#define ANALOG_HALL PA4
#define LED PC13
void setup() {
  Serial.begin(9600);  // Initialize Serial for debugging
  pinMode(LED, OUTPUT);
}
// Hàm tính giá trị trung bình
float mean(float data[], int n) {
  float sum = 0.0;
  for (int i = 0; i < n; i++) {
    sum += data[i];
  }

  return sum / n;
}

// Hàm tính phương sai
float variance(float data[], int n) {
  float avg = mean(data, n);
  float sumSq = 0.0;
  Serial.println("giá trị trung bình: " + String(avg));
  for (int i = 0; i < n; i++) {
    sumSq += (data[i] - avg) * (data[i] - avg);
  }
  return sumSq / n;  // dùng (n-1) nếu muốn ước lượng mẫu
}

// Hàm tính độ lệch chuẩn
float standard_deviation(float var) {
  return sqrt(var);
}
void loop() {
  // static uint32_t timer_read_hall = millis();
  // static bool check_hall = false;
  // if (millis() - timer_read_hall > 10000) {
  //   check_hall = handleHallSensor();
  //   timer_read_hall = millis();
  // }
  // digitalWrite(LED, check_hall ? HIGH : LOW);
  Serial.println(analogRead(ANALOG_HALL));
  delay(100);
}
bool handleHallSensor() {
  constexpr uint16_t times_read = 20;
  float data[times_read];
  for (uint8_t i = 0; i < times_read + 1; i++) {
    data[i] = analogRead(ANALOG_HALL);
    Serial.println(data[i]);
    delay(31);
  }
  float var = variance(data, times_read);
  float der = standard_deviation(var);
  Serial.println("phương sai: " + String(var) + " độ lệch chuẩn: " + String(der));
  if (der > 3.0)
    return true;
  else
    return false;
}
