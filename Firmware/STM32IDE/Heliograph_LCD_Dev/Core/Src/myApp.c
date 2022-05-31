/*
 * myApp.c
 *
 *  Created on: May 29, 2022
 *      Author: jacks
 */

#include "main.h"
#include "myApp.h"
#include <time.h>
#include <stdbool.h>
#include "UBXDATA.h"
#include "LS013B7DH03.h"
#include <Fonts/FreeSans9pt7b.h>
#include "GNSSPVT.h"

//Get the handle from main
extern RTC_HandleTypeDef hrtc;
extern ADC_HandleTypeDef hadc;

//internal Functions
static void goToIdle();
static void goToSleep();
static void printTime();
static void drawHeader();
static void drawGNSS();

//defines
#define TS_CAL1_CAL_ADDR 	((uint16_t*)((uint32_t)0x1FF8007A))
#define TS_CAL1_TEMP		(uint16_t)(30)
#define TS_CAL2_CAL_ADDR 	((uint16_t*)((uint32_t)0x1FF8007E))
#define TS_CAL2_TEMP		(uint16_t)(130)
#define VREFINT_CAL_ADDR 	((uint16_t*)((uint32_t)0x1FF80078))
#define VREF_CAL_mV 		((uint32_t)3000)
#define adc_Channels 		2

//Variable
volatile int16_t superCapmV;
volatile int16_t tempC;
volatile static uint16_t ADC_raw[adc_Channels];
volatile uint32_t guiTimer;
wakeUpSource lastWakeUpSource;

//functions
void myApp_init() {
	startADC();

	//Power Stuff
	SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_SLEEP);//Enable debug in sleep
	SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP); //Enable debug in stop
	__HAL_RCC_PWR_CLK_ENABLE();
	RCC->CFGR |= RCC_CFGR_STOPWUCK; //Wakeup with HSI16
	HAL_PWREx_EnableUltraLowPower();
	HAL_PWREx_EnableFastWakeUp();

	//GNSS
	GNSS_Init();

	//LCD
	lcd_init();
	lcd_SetFont(&FreeSans9pt7b);

}

void myApp_loop() {
	GNSS_Power();
	if (LCD_Power() == LCD_READY) {
		startADC();
		drawHeader();
		drawGNSS();
		while (lcd_draw() == LCD_SENDING_DATA) {
			goToIdle();
		}
	}
	goToSleep();
}

static void goToIdle() {
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
}

static void goToSleep() {
	GNSS_Prep_Stop();
	HAL_SuspendTick();
	do {
		lastWakeUpSource = WKUP_CLEAR;
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	} while (lastWakeUpSource == WKUP_LPUART);
	LL_LPUART_DisableIT_WKUP(LPUART1);
	HAL_ResumeTick();
}

static void drawHeader() {
	lcd_clearArea(0, 0, 127, 16);
//Base Line
	lcd_drawLine(0, 17, 127, 17, LCD_BLACK);
//Time
	lcd_SetCursor(1, 14);
	printTime();
//Battery
	lcd_print(sprintf(strbuffer, "%.2d %.2d", superCapmV / 100, tempC));
}

static void drawGNSS() {
	lcd_clearArea(0, 18, LCD_RES_PX_X - 1, 77);
	//GPS STATE
	if (GNSSlastRate == GNSS_ON) {
		lcd_fillRect(124, 18, 3, 3, LCD_BLACK);
	}
	lcd_SetCursor(1, 32);
	lcd_print(sprintf(strbuffer, "%.3d %.4d", LastFix.Lat_Deg, LastFix.Lat_Dec / 10));
	if (LastFix.SN) {
		lcd_print_char('S');
	} else {
		lcd_print_char('N');
	}
	lcd_SetCursor(1, 47);
	lcd_print(sprintf(strbuffer, "%.3d %.4d", LastFix.Long_Deg, LastFix.Long_Dec / 10));
	if (LastFix.WE) {
		lcd_print_char('W');
	} else {
		lcd_print_char('E');
	}
	lcd_SetCursor(1, 62);
	lcd_print(sprintf(strbuffer, "%d %d %d %d", getNumSatellites(), getFixType(), isGnssFixOk(), isTimeFullyResolved()));
	lcd_SetCursor(1, 77);
	lcd_print(sprintf(strbuffer, "%lu", packets));
}

void setTimeGNSS() {
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	sTime.Hours = GNSS_getHour() - 8;
	sTime.Minutes = GNSS_getMin();
	sTime.Seconds = GNSS_getSec();
	sDate.Year = GNSS_getYear() - 2000;
	sDate.Month = GNSS_getMonth();
	sDate.Date = GNSS_getDay();
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

void printTime() {
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	lcd_print(sprintf(strbuffer, "%02d:%02d:%02d ", sTime.Hours, sTime.Minutes, sTime.Seconds));
}

void startADC() {
	HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
	HAL_ADCEx_EnableVREFINT();
	HAL_ADCEx_EnableVREFINTTempSensor();
	HAL_ADC_Start_DMA(&hadc, (uint32_t*) &ADC_raw, adc_Channels);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	//SCAN Forward from CHSEL0 to CHSEL18
	//solarmV = ADC_raw[0];

	superCapmV = VREF_CAL_mV * (*VREFINT_CAL_ADDR) / ADC_raw[0];

	tempC = (((TS_CAL2_TEMP - TS_CAL1_TEMP ) * (ADC_raw[1] - *TS_CAL1_CAL_ADDR)) / (*TS_CAL2_CAL_ADDR - *TS_CAL1_CAL_ADDR)) + TS_CAL1_TEMP;

	HAL_ADCEx_DisableVREFINT();
	HAL_ADCEx_DisableVREFINTTempSensor();
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc) {
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	guiTimer++;
}

bool vBATOK() {
	return HAL_GPIO_ReadPin(VBAT_OK_PIN_GPIO_Port, VBAT_OK_PIN_Pin);
}