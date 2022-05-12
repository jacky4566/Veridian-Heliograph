#include <avr/sleep.h>
#include <avr/wdt.h>
#include <SPI.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

volatile long vccmV = 0;
volatile uint32_t sys1Hz = 0;

//Display
const int pin_LCD_BUSY = 6; //PD6
const int pin_LCD_RES = 7; //PD7
const int pin_LCD_DC = 8; //PB0
const int pin_LCD_CS = 9; //PB1
#include "GxEPD2_display_selection_new_style.h"
const char HelloWorld[] = "Hello World!";

const int pin_LED = 17; //PC3

void setup() {
  PRR0 = 0b10011000;//Shutdown TWI0, UART1, TIM1
  PRR1 = 0b00111101;//Shutdown TWI1, PTC, TIM4, SPI1, TIM3
  pinMode(pin_LED, OUTPUT);
  tim2Setup();
  display.epd2.setBusyCallback(LCDBusyCallback);
  display.init();

}

void loop() {
  readVcc();
  drawPaper();
  LED();
  goToSleep();
}

void tim2Setup() {
  //RESET
  ASSR = 0;
  TCNT2 = 0;
  TCCR2A = 0;
  TCCR2B = 0;
  //SETUP
  ASSR = (1 << AS2);//Enable asynchronous mode
  TCCR2A = (1 << WGM21); //Clear on compare
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); //set prescaler 32768/1024
  OCR2A = 32; //output compare value
  while ((ASSR & ((1 << OCR2AUB) | (1 << OCR2BUB) | (1 << TCR2AUB) | (1 << TCR2BUB) | (1 << TCN2UB)))) { //wait for registers update
    cpuSleep(SLEEP_MODE_IDLE);
  }
  TIFR2  = (1 << TOV2) | (1 << OCF2A) | (1 << OCF2B);//clear interrupt flags
  TIMSK2  = (1 << OCIE2A); //enable Output Compare A interrupt
}

void readVcc() {
  // Read 1.1V reference against AVcc
  ADCSRA = (1 << ADEN) | (1 << ADIF) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Enable ADC, Interrupt Flag, 8MHZ DIV128 = 62.5kHZ
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  //Result 1, toss
  do {
    cpuSleep(SLEEP_MODE_IDLE);
  } while (bit_is_set(ADCSRA, ADSC));
  //Result 2, use
  do {
    cpuSleep(SLEEP_MODE_IDLE);
  } while (bit_is_set(ADCSRA, ADSC));
  vccmV = ADCL;
  vccmV |= ADCH << 8;
  vccmV = 1126400L / vccmV; // Back-calculate AVcc in mV
  ADCSRA = 0; //disable ADC
}

void LED() {
    if (vccmV > 3300L) {
      digitalWrite(pin_LED, HIGH);
    } else {
      digitalWrite(pin_LED, LOW);
    }
}

void drawPaper() {
  static uint32_t lastDisplay = 30;
  if ((sys1Hz - lastDisplay >= 30) && (vccmV > 3000)) {
    digitalWrite(pin_LED, HIGH);
    display.setRotation(0);
    display.setFont(&FreeMonoBold12pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setFullWindow();
    display.firstPage();
    do
    {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(0, 20);
      display.print(HelloWorld);
      display.setCursor(0, 40);
      display.print(vccmV);
      display.setCursor(0, 60);
      display.print(sys1Hz);
    }
    while (display.nextPage());
    display.hibernate();
    digitalWrite(pin_LED, LOW);
    lastDisplay = sys1Hz;
  }
}

void LCDBusyCallback(){
  digitalWrite(pin_LED, LOW);
  cpuSleep(SLEEP_MODE_IDLE);
  digitalWrite(pin_LED, HIGH);
}

void goToSleep() {
  cpuSleep(SLEEP_MODE_PWR_SAVE);
}


void cpuSleep(byte sleepMode) {
  set_sleep_mode(sleepMode);
  sleep_enable();
  sleep_mode();
  // Upon waking up, sketch continues from this point.
  sleep_disable();
}

void WDThandler() {
  /*Note on "Interrupt and System Reset Mode"
    The first timeout in the Watchdog Timer will set WDIF.
    Executing the corresponding interrupt vector will clear WDIE and WDIF automatically by hardware (the Watchdog goes to System Reset Mode).
    To stay in Interrupt and System Reset Mode, WDIE must be set after each interrupt.
  */
  cli();
  wdt_reset(); //reset so we dont trigger ISR by accident
  MCUSR = 0;
  WDTCSR |=  (1 << WDCE) | (1 << WDE); //Enable WDT changes
  WDTCSR  =  (1 << WDP0) | (1 << WDP3) | (1 << WDIE) | (1 << WDE); // 8.0 seconds with Interrupt and System Reset
  sei();
}

ISR(ADC_vect) {
  //when new ADC value ready
}

ISR(WDT_vect) {
  //Should go to top of loop and recheck
}

ISR(TIMER2_COMPA_vect) {
  sys1Hz++;
}
