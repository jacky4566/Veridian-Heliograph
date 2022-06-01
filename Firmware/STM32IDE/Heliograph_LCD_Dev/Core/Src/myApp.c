/*
 * myApp.c
 *
 *  Created on: May 29, 2022
 *      Author: jacks
 */

#include "main.h"
#include "math.h"
#include "myApp.h"
#include <time.h>
#include <stdbool.h>
#include "UBXDATA.h"
#include "LS013B7DH03.h"
#include <Fonts/FreeSans9pt7bMod.h>
#include "GNSSPVT.h"

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0
#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

//Get the handle from main
extern RTC_HandleTypeDef hrtc;
extern ADC_HandleTypeDef hadc;
extern IWDG_HandleTypeDef hiwdg;

//internal Functions
static void goToIdle();
static void goToSleep();
static void printTime();
static void drawHeader();
static void drawGNSS();

//defines
#define TEMP30_CAL_ADDR 	((uint16_t*)((uint32_t)0x1FF8007A))
#define TEMP30				(uint16_t)(30)
#define TEMP130_CAL_ADDR 	((uint16_t*)((uint32_t)0x1FF8007E))
#define TEMP130				(uint16_t)(130)
#define VREFINT_CAL_ADDR 	((uint16_t*)((uint32_t)0x1FF80078))
#define VREF_CAL_mV 		((uint32_t)3000)
#define adc_Channels 		2

//Variable
volatile int16_t superCapmV;
volatile int32_t tempC;
volatile static uint16_t ADC_raw[adc_Channels];
volatile uint32_t guiTimer;
wakeUpSource lastWakeUpSource;

//functions
void myApp_init() {
	HAL_IWDG_Refresh(&hiwdg); //Kick watchdog
	startADC();

	//Power Stuff
	//SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_SLEEP);//Enable debug in sleep
	//SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP); //Enable debug in stop
	__HAL_RCC_PWR_CLK_ENABLE();
	RCC->CFGR |= RCC_CFGR_STOPWUCK; //Wakeup with HSI16
	HAL_PWREx_EnableUltraLowPower();
	HAL_PWREx_EnableFastWakeUp();

	//GNSS
	GNSS_Init();

	//LCD
	lcd_init();
	lcd_SetFont((GFXfont*) &FreeSans9pt7bMod);

}

void myApp_loop() {
	HAL_IWDG_Refresh(&hiwdg); //Kick watchdog
	GNSS_Power();
	if (LCD_Power() == LCD_READY) {
		startADC();
		drawHeader();
		drawGNSS();
		goToIdle(); //wait for DMA services to finish
		LCDTimer();
	}
	goToSleep();
}

static void goToIdle() {
	while ((lcd_draw() == LCD_SENDING_DATA)) {
		HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
	}
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
	lcd_print(npf_snprintf(strbuffer, strbufferSize, " %3.2fV", (float) superCapmV / 1000.0f));
}

static void drawGNSS() {
	lcd_clearArea(0, 18, LCD_RES_PX_X - 1, 96);
	//GPS STATE
	if (GNSSlastRate == GNSS_ON) {
		lcd_fillRect(124, 18, 3, 3, LCD_BLACK);
	}
	if (GNSSAlive){
		lcd_fillRect(124, 22, 3, 3, LCD_BLACK);
	}

	//Lat Long
	lcd_SetCursor(1, 32);
	lcd_print(npf_snprintf(strbuffer, strbufferSize, "%9.4f ", fabsf(getLat())));
	if (getLat() < 0) {
		lcd_print_char('S');
	} else {
		lcd_print_char('N');
	}
	lcd_SetCursor(1, 48);
	lcd_print(npf_snprintf(strbuffer, strbufferSize, "%9.4f ", fabsf(getLong())));
	if (getLong() < 0) {
		lcd_print_char('W');
	} else {
		lcd_print_char('E');
	}

	lcd_SetCursor(1, 64);
	lcd_print(npf_snprintf(strbuffer, strbufferSize, "Sat:%d Fix:%d", getNumSatellites(), getFixType()));

	lcd_SetCursor(1, 80);
	lcd_print(npf_snprintf(strbuffer, strbufferSize, "%dkph HD:%d", getGroundSpeed_kph(), getMotionHeading_deg()));

	lcd_SetCursor(1, 96);
	lcd_print(npf_snprintf(strbuffer, strbufferSize, "%dC age:%lu", (int) tempC, GNSSlastPacket));
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
	lcd_print(npf_snprintf(strbuffer, strbufferSize, "%02d:%02d:%02d ", sTime.Hours, sTime.Minutes, sTime.Seconds));
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

	//Temperature, Magic code from Datasheet
	tempC = ((ADC_raw[1] * superCapmV / VREF_CAL_mV) - (int32_t) *TEMP30_CAL_ADDR);
	tempC = tempC * (int32_t) (TEMP130 - TEMP30 );
	tempC = tempC / (int32_t) (*TEMP130_CAL_ADDR - *TEMP30_CAL_ADDR);
	tempC = tempC + 30;

	HAL_ADCEx_DisableVREFINT();
	HAL_ADCEx_DisableVREFINTTempSensor();
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc) {
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	guiTimer++;
	GNSSlastPacket++;
}

bool vBATOK() {
	return HAL_GPIO_ReadPin(VBAT_OK_PIN_GPIO_Port, VBAT_OK_PIN_Pin);
}
