#include <avr/sleep.h>

void setup() {
  DDRC |= (1 << PC3); //PC3 OUTPUT

}

void loop() {
  if (readVcc() > 3000) {
    PORTC |= (1 << PC3); //Set PC3
    delay(100);
  } else {
    PORTC &= ~(1 << PC3); //Clear PC3
    goToSleep();
  }
}

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(1); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}

void goToSleep() {
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  sleep_mode();
  // Upon waking up, sketch continues from this point.
  sleep_disable();
}
