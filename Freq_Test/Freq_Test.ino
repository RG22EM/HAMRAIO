#define HARDGEN

volatile bool overflowed;
volatile uint16_t captured[2];

ISR(TIMER1_OVF_vect) {
  TCCR1B &= ~(bit(CS12) | bit(CS11) | bit(CS10)); // Stop Timer1
  overflowed = true;
}

ISR(TIMER1_CAPT_vect) {
  uint16_t icr = ICR1;

  if (icr > captured[1]) { // Ignore same value
    captured[0] = captured[1];
    captured[1] = icr;
  }
}

#ifdef HARDGEN
EMPTY_INTERRUPT(TIMER2_COMPB_vect);
#endif

void setup() {
  Serial.begin(115200);
  Serial.println(F("Trying to detect frequency on pin 8..."));

  pinMode(3, OUTPUT); // OC2B
  pinMode(5, OUTPUT);
  pinMode(8, INPUT); // ICP1
  analogWrite(5, 127);

  noInterrupts();

  // Setup Timer1
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B = bit(ICNC1) | bit(ICES1); // Input Capture Noise Canceler enabled, detecting rising edge
  TIMSK1 = bit(ICIE1) | bit(TOIE1); // Input Capture and Overflow Interrupts Enable

#ifdef HARDGEN
  // Setup Timer2
  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 = bit(OCIE2B); // Output Compare Match B Interrupt Enable
  TIFR2 = bit(OCF2B); // Clear pending interrupts
  TCCR2A = bit(COM2B0) | bit(WGM21); // CTC, Toggle on Compare Match B
  OCR2A = 4; // 200 kHz
  OCR2B = OCR2A;
  TCNT2 = 0;
  TCCR2B = bit(CS21); // Start Timer2 with prescaler of 8
#endif

  interrupts();
}

void loop() {
  noInterrupts();

  overflowed = false;
  captured[0] = captured[1] = 0;
  TCNT1 = 0;
  TIFR1 = bit(ICF1) | bit(TOV1); // Clear pending interrupts
//  TCCR1B |= bit(CS10); // Start Timer1 with no prescaler
  TCCR1B |= bit(CS11); // Start Timer1 with prescaler of 8

  interrupts();

  while (! overflowed) { // Waiting for overflow
#ifndef HARDGEN
    PORTD ^= 0B00001000; // Toggle PD3
#endif
  }

  if (captured[0] && captured[1]) {
    Serial.print(F("Last detected pulse width "));
    Serial.print(captured[1] - captured[0]);
    Serial.print(F(" timer ticks, frequency "));
//    Serial.print(F_CPU / (captured[1] - captured[0]));
    Serial.print(F_CPU / 8 / (captured[1] - captured[0]));
    Serial.println(F(" Hz"));

    delay(1000);
  }
}
