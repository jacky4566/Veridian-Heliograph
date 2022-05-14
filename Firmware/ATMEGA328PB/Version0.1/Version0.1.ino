#include <avr/sleep.h>
#include <avr/wdt.h>
#include <Time.h>
#include <SPI.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
//#include "Ublox.h"

//Time
volatile time_t unix = 1652459254;

//Display
const int pin_LCD_BUSY = 6; //PD6
const int pin_LCD_RES = 7; //PD7
const int pin_LCD_DC = 8; //PB0
const int pin_LCD_CS = 9; //PB1
#include "GxEPD2_display_selection_new_style.h"
const uint8_t dispDrawFast = 30;
const uint8_t dispDrawSlow = 300;
uint32_t dispNextDraw = 0;
uint8_t dispDrawSpeed = dispDrawFast; //State machine

//GPS
//Ublox myGPS(&Serial1);
const int pin_GPS_EN = 18; //PC4
const int pin_GPS_EXINT = 19; //PC5

//IO
const int pin_LED = 17; //PC3
const int pin_TEST = 2; //PD2

//Power
uint32_t VCAPmV;
uint16_t rawADC;
const long powerSTBY = 2400;
const long powerLOW = 2600;
const long powerHIGH = 3200;
const long powerWARN = 3450;

void LCDBusyCallback(const void* p) { //Needs to be established before use
  cpuSleep(SLEEP_MODE_IDLE);
}

void setup() {
  PRR0 = 0b10011000;//Shutdown TWI0, UART1, TIM1
  PRR1 = 0b00111101;//Shutdown TWI1, PTC, TIM4, SPI1, TIM3
  pinMode(pin_LED, OUTPUT);
  tim2Setup();
  //display.epd2.setBusyCallback(LCDBusyCallback);
  display.setTextWrap(false);
  display.init();
}

void loop() {
  powerMachine();
  drawPaper();
  //goToSleep();
}

void powerMachine() {
  //Read VCC, Change states and timers
  readVcc();

  //DISP
  if ((VCAPmV < powerSTBY) && (dispDrawSpeed != 0)) { //stop Disp
    dispDrawSpeed = 0;
  } else if ((VCAPmV < powerHIGH) && (dispDrawSpeed != dispDrawSlow)) { //Draw slow
    dispDrawSpeed = dispDrawSlow;
  } else if (dispDrawSpeed != dispDrawFast) { //Draw fast
    dispDrawSpeed = dispDrawFast;
  }

  //GPS

  //LED
  if (VCAPmV > powerWARN) {
    //Drain extra power with LED
    digitalWrite(pin_LED, HIGH);
  } else {
    digitalWrite(pin_LED, LOW);
  }
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
  ADCSRA = (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //8MHZ DIV128 = 62.5kHZ
  ADMUX = (1 << REFS0) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
  ADCSRA |= (1 << ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  rawADC = ADCW; //toss this
  ADCSRA |= (1 << ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  rawADC = ADCW;
  VCAPmV = rawADC;
  VCAPmV = 1126400 / VCAPmV; // Back-calculate AVcc in mV
  ADCSRA = 0; //disable ADC
}

void drawPaper() {
  if (dispDrawSpeed != 0 && dispNextDraw < unix) {
    display.setRotation(0); //Get from ACCEL
    display.setFullWindow();
    display.firstPage();
    do
    {
      display.fillScreen(GxEPD_WHITE);
      display.setFont(&FreeSansBold12pt7b);
      //Time
      display.setCursor(0, 20);
      display.print(getTimeString());

      //Battery
      display.setCursor(0, 41);
      display.print(VCAPmV);
      display.print(F("mV"));

      //Fix Sats
      display.setCursor(0, 62);
      display.print(F("SATS "));
      //display.print(myGPS.num_satellites());

      //Age
      display.setCursor(0, 84);
      display.print(F("AGE "));
      //display.print(unix - myGPS.UTC());
      display.print(F("S"));

      //Lat
      display.setFont(&FreeSans24pt7b);
      display.setCursor(0, 127);
      //display.print(abs(myGPS.lat_deg()), 3);
      display.print(F("N"));
      //Long
      display.setCursor(0, 163);
      //display.print(abs(myGPS.long_deg()), 3);
      display.print(F("W"));
      //ALT
      display.setCursor(0, 199);
      //display.print(myGPS.alt_m(), 1);
      display.print(F("m"));
    }
    while (display.nextPage());
    display.hibernate();
    dispNextDraw = unix + dispDrawSpeed;
  }
}

char* getTimeString() {
  struct tm *tmp = gmtime(&unix);
  static char buf[9];
  sprintf(buf, "%02d:%02d:%02d %02d/%02d", tmp->tm_hour, tmp->tm_min, tmp->tm_sec, tmp->tm_mday + 1, tmp->tm_mon + 1);
  return buf;
}

void goToSleep() {
  if (VCAPmV < powerWARN) {
    cpuSleep(SLEEP_MODE_PWR_SAVE);
  }
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
  unix++;
}
