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
extern ADC_HandleTypeDef hadc1;

//internal Functions
static void goToIdle();
static void goToSleep();
static void printDateTime();
static void drawHeader();
static void drawGNSS();
static void startADC();

//defines
#define adc_Channels 		3

//Variable
volatile bool ADCrunning;
volatile uint32_t superCapmV;
volatile int32_t tempC;
volatile static uint16_t ADC_raw[adc_Channels];
volatile uint32_t guiTimer;
wakeUpSource lastWakeUpSource;

//functions
void myApp_init() {
	startADC();

	//Power Stuff
	//SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_SLEEP);//Enable debug in sleep
	//SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP); //Enable debug in stop
	//HAL_PWREx_EnableUltraLowPower();
	//HAL_PWREx_EnableFastWakeUp();

	/* Enable Power Clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	/* Ensure that MSI is wake-up system clock */
	__HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);

	while (ADCrunning) {
		//wait for ADC
	}

	//LCD
	lcd_SetFont((GFXfont*) &FreeSans9pt7bMod);
}

void myApp_loop() {
	startADC();
	//GNSS_Power();
	if (LCD_Power() == LCD_READY) {
		drawHeader();
		drawGNSS();
		while ((lcd_draw() == LCD_SENDING_DATA)) {
			goToIdle(); //wait for DMA services to finish
		}
	}
	while (ADCrunning) {
		goToIdle(); //wait for ADC services to finish
	}
	goToSleep();
}

static void goToIdle() {
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

static void goToSleep() {
	if (superCapmV < mV_OV) {
		//HAL_GPIO_WritePin(SolarEN_GPIO_Port, SolarEN_Pin, GPIO_PIN_SET); //Allow charging
		GNSS_Prep_Stop();
		HAL_SuspendTick();
		do {
			lastWakeUpSource = WKUP_CLEAR;
			HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		} while (lastWakeUpSource == WKUP_LPUART);
		LL_LPUART_DisableIT_WKUP(LPUART1);
		HAL_ResumeTick();
	} else {
		//HAL_GPIO_WritePin(SolarEN_GPIO_Port, SolarEN_Pin, GPIO_PIN_RESET); //disable charging
	}
}

static void drawHeader() {
	lcd_clearLines(0, LCD_row_Height * 2);
//Date Time
	printDateTime();
//Battery
	lcd_SetCursor(80, LCD_row_Height - 2);
	lcd_print(npf_snprintf(strbuffer, strbufferSize, "%3.2fV", (float) superCapmV / 1000.0f));
//Temp
	lcd_SetCursor(94, (LCD_row_Height * 2) - 2);
	lcd_print(npf_snprintf(strbuffer, strbufferSize, "%.2dC", (int) tempC));
	//GNSS Status
	if (GNSSlastRate == GNSS_ON) {
		lcd_drawLine(0, 0, 0, 15, LCD_BLACK);
	}
	if (GNSSAlive) {
		lcd_drawLine(0, 16, 0, 32, LCD_BLACK);
	}
//Div line
	lcd_drawLine(0, LCD_row_Height * 2, 127, LCD_row_Height * 2, LCD_BLACK);
}

static void printDateTime() {
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	lcd_SetCursor(1, LCD_row_Height - 2);
	lcd_print(npf_snprintf(strbuffer, strbufferSize, "%02d:%02d:%02d ", sTime.Hours, sTime.Minutes, sTime.Seconds));
	lcd_SetCursor(1, (LCD_row_Height * 2) - 2);
	lcd_print(npf_snprintf(strbuffer, strbufferSize, "%02d/%02d/%02d", sDate.Date, sDate.Month, sDate.Year));
}

static void drawGNSS() {
	if (GNSSNewData) {
		lcd_clearLines((LCD_row_Height * 2) + 2, LCD_row_Height * 6);
		lcd_SetCursor(1, LCD_row_Height * 3);
		lcd_print(npf_snprintf(strbuffer, strbufferSize, "%8.4f ", fabsf(getLat())));
		if (getLat() < 0) {
			lcd_print_char('S');
		} else {
			lcd_print_char('N');
		}
		lcd_SetCursor(1, LCD_row_Height * 4);
		lcd_print(npf_snprintf(strbuffer, strbufferSize, "%8.4f ", fabsf(getLong())));
		if (getLong() < 0) {
			lcd_print_char('W');
		} else {
			lcd_print_char('E');
		}

		lcd_SetCursor(1, LCD_row_Height * 5);
		lcd_print(npf_snprintf(strbuffer, strbufferSize, "Sat:%d Acc:%2.0fm", getNumSatellites(), getHAcc() > 99.9 ? 99.9 : getHAcc()));

		lcd_SetCursor(1, LCD_row_Height * 6);
		lcd_print(npf_snprintf(strbuffer, strbufferSize, "%dkph HD:%d", getGroundSpeed_kph(), getMotionHeading_deg()));
		GNSSNewData = false;
	}

	//GNSS Age
	lcd_clearLines((LCD_row_Height * 6) + 2, LCD_row_Height * 7);
	lcd_SetCursor(2, LCD_row_Height * 7);
	if (GNSSlastPacketAge < 90) {
		lcd_print(npf_snprintf(strbuffer, strbufferSize, "Age:%lus", GNSSlastPacketAge));
	} else if (GNSSlastPacketAge < 3600) {
		lcd_print(npf_snprintf(strbuffer, strbufferSize, "Age:%.1fm", (float) GNSSlastPacketAge / 60.0f));
	} else {
		lcd_print(npf_snprintf(strbuffer, strbufferSize, "Age:%.1fh", (float) GNSSlastPacketAge / 3600.0f));
	}
}

void setTimeGNSS() {
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	sTime.Hours = GNSS_getHour();
	sTime.Minutes = GNSS_getMin();
	sTime.Seconds = GNSS_getSec();
	sDate.Year = (uint8_t) (GNSS_getYear() - (uint16_t) 2000);
	sDate.Month = GNSS_getMonth();
	sDate.Date = GNSS_getDay();
	sDate.WeekDay = 0;

	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

static void startADC() {
	if (ADCrunning) {
		//ADC already running
		return;
	}

	ADCrunning = true;
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADC_raw, adc_Channels);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	HAL_ADC_Stop_DMA(&hadc1);

	uint32_t vref = __HAL_ADC_CALC_VREFANALOG_VOLTAGE(ADC_raw[2], ADC_RESOLUTION_12B);

	//solarmV = ADC_raw[0];
	superCapmV = ((uint32_t) ADC_raw[0] * vref * 2) / 4095UL;

	//Temperature
	tempC = __HAL_ADC_CALC_TEMPERATURE(vref, ADC_raw[1], ADC_RESOLUTION_12B);

	ADCrunning = false;
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc) {
	guiTimer++;
	GNSSlastPacketAge++;
}
