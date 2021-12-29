/*
 * My_App.c
 *
 *  Created on: Dec 26, 2021
 *      Author: jacks
 */

#include <my_app.h>
#include "main.h"
#include "dbg_trace.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"
#include "stm32_lpm.h"
#include "gnss.h"

//Private Defines
#define BSP_INTERVAL_FAST   ( 1000000 / CFG_TS_TICK_VAL)  /** 1s */
#define BSP_INTERVAL_SLOW   ( 5 * 1000000 / CFG_TS_TICK_VAL)
#define ADC_NUM_CHANNELS   2
#define ADC_AVDD_MV   3300
#define ADC_Running_Bit 0
#define SPI_Running_Bit 1

//HAL Handles
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern RTC_HandleTypeDef hrtc;
extern IWDG_HandleTypeDef hiwdg;

//Private Variables
uint8_t TimerMeasurement_Id; //TimerID
volatile uint16_t rawADC[ADC_NUM_CHANNELS];		//stores values from the ADC DMA

int16_t temperature; 	//stores the last temperature measurement
uint16_t VBatmV; 		//stores the last Battery measurement mV
uint16_t VBatMidmV; 	//stores the last mid Battery measurement mV
uint16_t VSolarmV; 		//stores the last solar measurement mV

uint32_t powerState; 	//bitwise power machine

//Private Functions
void queueBSP( void );
void BSP_Start( void );
void ADC_Start( void );
void ADC_End( void );
void SPI_Start( void );
void SPI_End( void );

void my_app_Init(void){
	APP_DBG_MSG("Magic Number 2 \n");
	//Setup ACCEL

	//Start GNSS
	gnss_Init();

	//Register start and end tasks
	UTIL_SEQ_RegTask( 1<< CFG_TASK_StartBSP_EVT_ID, UTIL_SEQ_RFU, BSP_Start);
	UTIL_SEQ_RegTask( 1<< CFG_TASK_EndADC_EVT_ID, UTIL_SEQ_RFU, ADC_End);
	UTIL_SEQ_RegTask( 1<< CFG_TASK_EndSPI_EVT_ID, UTIL_SEQ_RFU, ADC_End);

	//Run it on a timer
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &TimerMeasurement_Id, hw_ts_SingleShot, queueBSP);
	HW_TS_Start(TimerMeasurement_Id, BSP_INTERVAL_FAST);
}

void queueBSP( void ){
	//Queue BSP task
	UTIL_SEQ_SetTask( 1<<CFG_TASK_StartBSP_EVT_ID, CFG_SCH_PRIO_0);
	return;
}

void BSP_Start(void){
	//Kick the dog
	HAL_IWDG_Refresh(&hiwdg);

	ADC_Start();

	SPI_Start();

	//Set Next BSP timer
	if (VBatmV > 3000){
		HW_TS_Start(TimerMeasurement_Id, BSP_INTERVAL_FAST);
	}else{
		HW_TS_Start(TimerMeasurement_Id, BSP_INTERVAL_SLOW);
	}

	//Set GNSS timer
	if (VBatmV > 4000){
		gnss_power_req(gnss_rate_fast);
	}else if (VBatmV > 2000){
		gnss_power_req(gnss_rate_slow);
	}else{
		gnss_power_req(gnss_rate_stop);
	}

	//Don't allow Stop mode
	UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
	return;
}

void ADC_Start(){
	//Start and Calibrate ADC
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	//Start ADC into DMA, Callback will handle data
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)rawADC, ADC_NUM_CHANNELS);
	//Set ADC running bit
	powerState |= (1UL << ADC_Running_Bit);
}

void ADC_End(){
	HAL_ADC_Stop_DMA(&hadc1);

	//Clear ADC running bit
	powerState &= ~(1UL << ADC_Running_Bit);
	my_app_allow_stop();

	//Convert data
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	temperature = __HAL_ADC_CALC_TEMPERATURE(ADC_AVDD_MV, rawADC[0],ADC_RESOLUTION_12B);
	APP_DBG_MSG("%02d.%02d.%02d ADC Result Temp %d \n", sTime.Hours,sTime.Minutes,sTime.Seconds,temperature);

	//Update Battery voltage for BLE BATLVL
	VBatmV = HAL_GetTick() & 0x3F;
	uint8_t BLEBatmeasurement = (uint8_t)VBatmV;
	Custom_STM_App_Update_Char(CUSTOM_STM_BATLVL, (uint8_t *)&BLEBatmeasurement);
	return;
}

void SPI_Start(){
	//Start SPI
	//powerState |= (1UL << SPI_Running_Bit);

	//Read Accel

	//Write display
}

void SPI_End(){
	//Clear SPI running bit
	powerState &= ~(1UL << SPI_Running_Bit);
	my_app_allow_stop();
}

void my_app_allow_stop(){
	//Only allow stop mode if powerState == 0
	if(powerState == 0){
		//Allow Stop mode
		UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_ENABLE);
	}
}


void HAL_ADC_ConvCpltCallback  (ADC_HandleTypeDef *hadc) {
	//Queue ADC_end task
	UTIL_SEQ_SetTask( 1<<CFG_TASK_EndADC_EVT_ID, CFG_SCH_PRIO_0);
}

void HAL_SPI_MasterTxCpltCallback(){
	UTIL_SEQ_SetTask( 1<<CFG_TASK_EndSPI_EVT_ID, CFG_SCH_PRIO_0);
}
