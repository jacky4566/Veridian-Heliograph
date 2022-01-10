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
#include "app_ble.h"

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
extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart1;

//Private Variables
uint8_t TimerMeasurement_Id; //TimerID
volatile uint16_t rawADC[ADC_NUM_CHANNELS];		//stores values from the ADC DMA

int16_t temperature; 	//stores the last temperature measurement
uint16_t VBatmV; 		//stores the last Battery measurement mV
uint16_t VBatMidmV; 	//stores the last mid Battery measurement mV
uint16_t VSolarmV; 		//stores the last solar measurement mV

//Private Functions
void queueBSP( void );
void BSP_Start( void );
void ADC_Start( void );
void ADC_End( void );
void SPI_Start( void );
void myAppReadyStop( void );

void my_app_Init(void){
	//Setup ACCEL

	//Start GNSS
	gnss_Init();

	//Register start and end tasks
	UTIL_SEQ_RegTask( 1<< CFG_TASK_StartBSP_EVT_ID, UTIL_SEQ_RFU, BSP_Start);
	UTIL_SEQ_RegTask( 1<< CFG_TASK_EndADC_EVT_ID, UTIL_SEQ_RFU, ADC_End);

	//Run it on a timer
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &TimerMeasurement_Id, hw_ts_SingleShot, queueBSP);
	HW_TS_Start(TimerMeasurement_Id, BSP_INTERVAL_FAST);
}

void myAppReadyStop(){
	//Disable STOP if we are running perphierals
	if (((ADC1->CR & ADC_CR_ADEN)
			& (SPI1->CR1 & SPI_CR1_SPE)
			& (HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX ))
			== 0UL){
		UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_ENABLE);
	} else{
		UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
	}
}

void queueBSP( void ){
	//Queue BSP task
	UTIL_SEQ_SetTask( 1<<CFG_TASK_StartBSP_EVT_ID, CFG_SCH_PRIO_0);
	return;
}

void BSP_Start(void){

	ADC_Start();

	SPI_Start();

	//Don't allow Stop mode
	UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);

	//Set Next BSP Power states
	if (VBatmV > 3000){
		HW_TS_Start(TimerMeasurement_Id, BSP_INTERVAL_FAST);
		//Start BLE
	}else{
		HW_TS_Start(TimerMeasurement_Id, BSP_INTERVAL_SLOW);
		//Stop BLE
	}

	//Set new GNSS power states
	/*if (VBatmV > 4000){
		gnss_power_req(gnss_rate_fast);
	}else if (VBatmV > 2000){
		gnss_power_req(gnss_rate_slow);
	}else{
		gnss_power_req(gnss_rate_stop);
	}*/

	//BLE on/off
	if (VBatmV > 3000){
		//startBle();
	}else if (VBatmV < 2000){
		//stopBle();
	}

	//Testing BLE connect and kill
	/*static uint8_t counter = 0;
	if (8 == counter){
		stopBle();
	} else if (30 == counter){
		startBle();
	}
	Custom_STM_App_Update_Char(CUSTOM_STM_BATLVL, (uint8_t *)&counter);
	if (counter < 250){
		counter++;
	}*/
	return;
}

void ADC_Start(){
	//Start and Calibrate ADC
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	//Start ADC into DMA, Callback will handle data
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)rawADC, ADC_NUM_CHANNELS);
}

void ADC_End(){
	HAL_ADC_Stop_DMA(&hadc1);

	//Convert data
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	temperature = __HAL_ADC_CALC_TEMPERATURE(ADC_AVDD_MV, rawADC[0],ADC_RESOLUTION_12B);
	APP_DBG_MSG("%02d.%02d.%02d ADC Result Temp %d \n", sTime.Hours,sTime.Minutes,sTime.Seconds,temperature);

	//Update Battery voltage for BLE BATLVL
	uint8_t BLEBatmeasurement = (uint8_t)temperature;
	//Custom_STM_App_Update_Char(CUSTOM_STM_BATLVL, (uint8_t *)&BLEBatmeasurement);
	return;
}

void SPI_Start(){
	//Start SPI

	//Read Accel

	//DMA Write display
}


void HAL_ADC_ConvCpltCallback  (ADC_HandleTypeDef *hadc) {
	//Queue ADC end task
	UTIL_SEQ_SetTask( 1<<CFG_TASK_EndADC_EVT_ID, CFG_SCH_PRIO_0);
}

void UTIL_SEQ_PreIdle(void){
	myAppReadyStop();
}
