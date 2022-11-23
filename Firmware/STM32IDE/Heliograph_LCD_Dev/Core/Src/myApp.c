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
static void printDateTime();
static void drawHeader();
static void drawGNSS();
static void startADC();

//defines
#define TEMP30_CAL_ADDR 	((uint16_t*)((uint32_t)0x1FF8007A))
#define TEMP30				(uint16_t)(30)
#define TEMP130_CAL_ADDR 	((uint16_t*)((uint32_t)0x1FF8007E))
#define TEMP130				(uint16_t)(130)
#define VREFINT_CAL_ADDR 	((uint16_t*)((uint32_t)0x1FF80078))
#define VREF_CAL_mV 		((uint32_t)3000)
#define adc_Channels 		2

//Variable
volatile bool ADCrunning;
volatile int16_t superCapmV;
volatile int32_t tempC;
volatile static uint16_t ADC_raw[adc_Channels];
volatile uint32_t guiTimer;
wakeUpSource lastWakeUpSource;

//functions
void myApp_init() {
	HAL_IWDG_Refresh(&hiwdg); //Kick watchdog
	HAL_ADCEx_EnableVREFINT();
	SET_BIT(SYSCFG->CFGR3, SYSCFG_CFGR3_ENBUF_SENSOR_ADC); //enable Temp sensor, 10uS wakeup

	startADC();

	//Power Stuff
	//SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_SLEEP);//Enable debug in sleep
	//SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP); //Enable debug in stop
	__HAL_RCC_PWR_CLK_ENABLE();
	RCC->CFGR |= RCC_CFGR_STOPWUCK; //Wakeup with HSI16
	//HAL_PWREx_EnableUltraLowPower();
	//HAL_PWREx_EnableFastWakeUp();

	//GNSS
	GNSS_Init();

	while (ADCrunning) {
		//wait for ADC
	}

	//LCD
	lcd_init();
	lcd_SetFont((GFXfont*) &FreeSans9pt7bMod);
}

void myApp_loop() {
	startADC();
	HAL_IWDG_Refresh(&hiwdg); //Kick watchdog
	GNSS_Power();
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
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
}

static void goToSleep() {
	if (superCapmV < mV_OV) {
		GNSS_Prep_Stop();
		HAL_SuspendTick();
		do {
			lastWakeUpSource = WKUP_CLEAR;
			HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		} while (lastWakeUpSource == WKUP_LPUART);
		LL_LPUART_DisableIT_WKUP(LPUART1);
		HAL_ResumeTick();
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
	HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
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

	HAL_ADC_Stop_DMA(hadc);
	//CLEAR_BIT(SYSCFG->CFGR3, SYSCFG_CFGR3_ENBUF_SENSOR_ADC); //disable Temp sensor

	ADCrunning = false;
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc) {
	guiTimer++;
	GNSSlastPacketAge++;
}

bool vBATOK() {
	return HAL_GPIO_ReadPin(VBAT_OK_PIN_GPIO_Port, VBAT_OK_PIN_Pin);
}
