/*
 * gnss.c
 *
 *  Created on: Dec 28, 2021
 *      Author: jacks
 */

#include <gnss.h>
#include "main.h"

//Private Defines
#define GNSS_INTERVAL_SLOW   ( 60 * 1000000 / CFG_TS_TICK_VAL)  //timeout waiting for fix
#define GNSS_INTERVAL_SLEEP  ( 10* 60 * 1000000 / CFG_TS_TICK_VAL)	//time to hold Ublox in shutdown for next fix

//enums
typedef enum
{
  gnss_state_fast,
  gnss_state_slow,
  gnss_state_sleep,
  gnss_state_off
} gnss_status_t;

//Private variables
gnss_status_t gnss_status;
uint8_t GNSSTimer_Id; //TimerID

//Private Functions Declarations
void gnss_process( void );
void gnss_timer_return( void );

//Functions
void gnss_Init( void ){
	//Start Ublox chip and send settings

	//Create a timer for our Slow mode
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &GNSSTimer_Id, hw_ts_SingleShot, gnss_timer_return);
	return;
}

void gnss_power_req( gnss_power_req_t gnss_new_state ){
	//Outside functions can kick gnss
	switch(gnss_new_state){
		case gnss_rate_fast:
			//Start GNSS chip
			break;
		case gnss_rate_slow:
			//Stop GNSS chip
			break;
		case gnss_rate_stop:
			//Stop GNSS chip
			break;
		default:
			break;
	}
	return;
}

void gnss_timer_return(void){

}

void gnss_process(void){

	return;
}
