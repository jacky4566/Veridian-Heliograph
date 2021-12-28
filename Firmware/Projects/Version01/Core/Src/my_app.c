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
void ADC_End( void );
void allow_stop( void );

void My_app_Init(void){
	APP_DBG_MSG("My_ADC_Init \n");
	//Register start and end tasks
	UTIL_SEQ_RegTask( 1<< CFG_TASK_StartBSP_EVT_ID, UTIL_SEQ_RFU, BSP_Start);
	UTIL_SEQ_RegTask( 1<< CFG_TASK_EndADC_EVT_ID, UTIL_SEQ_RFU, ADC_End);

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
	APP_DBG_MSG("Start BSP \n");
	//Start and Calibrate ADC
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	//Start ADC into DMA, Callback will handle data
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)rawADC, ADC_NUM_CHANNELS);
	//Set ADC running bit
	powerState |= (1UL << ADC_Running_Bit);

	//Set Next BSP timer
	if (VBatmV > 3000){
		HW_TS_Start(TimerMeasurement_Id, BSP_INTERVAL_FAST);
	}else{
		HW_TS_Start(TimerMeasurement_Id, BSP_INTERVAL_SLOW);
	}

	//Don't allow Stop mode
	UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
	return;
}

void ADC_End(){
	HAL_ADC_Stop_DMA(&hadc1);

	//Clear ADC running bit
	powerState &= ~(1UL << ADC_Running_Bit);
	allow_stop();

	//Convert data
	temperature = __HAL_ADC_CALC_TEMPERATURE(ADC_AVDD_MV, rawADC[0],ADC_RESOLUTION_12B);
	APP_DBG_MSG("ADC Result Temp %d \n", temperature);

	//Update Battery voltage for BLE BATLVL
	VBatmV = HAL_GetTick() & 0x3F;
	uint8_t BLEBatmeasurement = (uint8_t)VBatmV;
	Custom_STM_App_Update_Char(CUSTOM_STM_BATLVL, (uint8_t *)&BLEBatmeasurement);
	return;
}

void allow_stop(){
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