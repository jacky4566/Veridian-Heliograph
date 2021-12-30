/*
 * gnss.c
 *
 *  Created on: Dec 28, 2021
 *      Author: jacks
 *  Credit for Bolderflight for some of this code:
 *  https://github.com/bolderflight/ublox/
 */

#include <gnss.h>
#include "main.h"
#include "dbg_trace.h"
#include "stm32_seq.h"

//Private Defines and statics
#define GNSS_INTERVAL_SLOW   ( 60 * 1000000 / CFG_TS_TICK_VAL)  //timeout waiting for fix
#define GNSS_INTERVAL_SLEEP  ( 10* 60 * 1000000 / CFG_TS_TICK_VAL)	//time to hold Ublox in shutdown for next fix

static uint8_t UBX_HEADER_LEN = 2;
static uint8_t UBX_CLASS_ID_LEN = 4;
static uint8_t UBX_PVT_LEN = 92;
static uint8_t UBX_HEADER[2] = {0xB5, 0x62}; //UBX default header
static uint8_t UBX_CLASS[4] = {0x01, 0x07, 0x00, 0x92}; //Class, ID, and length information

//data types
typedef enum
{
  gnss_state_fast,
  gnss_state_slow,
  gnss_state_sleep,
  gnss_state_off
} gnss_status_t;

struct ubx_nav_pvt{
  uint16_t class_ID;
  uint16_t len;
  uint32_t itow;
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint8_t valid;
  uint32_t tacc;
  int32_t nano;
  uint8_t fix;
  uint8_t flags;
  uint8_t flags2;
  uint8_t numsv;
  int32_t lon;
  int32_t lat;
  int32_t height;
  int32_t hmsl;
  uint32_t hacc;
  uint32_t vacc;
  int32_t veln;
  int32_t vele;
  int32_t veld;
  int32_t gspeed;
  int32_t headmot;
  uint32_t sacc;
  uint32_t headacc;
  uint16_t pdop;
  uint8_t flags3;
  uint8_t reserved1[5];
  int32_t headveh;
  int16_t magdec;
  uint16_t magacc;
};

//Variables
gnss_status_t gnss_status;
uint8_t GNSSTimer_Id; //TimerID

struct ubx_nav_pvt ubx_nav_pvt_parsed;
volatile  uint8_t rx_buffer[96];
volatile uint16_t parser_pos;
volatile uint8_t checksum_buffer[2];
volatile gnss_wakeup_enum gnss_wakeup;

//Private Functions Declarations
void gnss_parse( uint8_t );
void gnss_timer_return( void );
uint16_t Checksum( uint8_t*, uint16_t );

//Functions
void gnss_Init( void ){
	//enable callback interrupt

	//Start Ublox chip and send settings

	//Create a timer for our Slow mode
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &GNSSTimer_Id, hw_ts_SingleShot, gnss_timer_return);

	//Start GPS in fast mode
	gnss_power_req(gnss_rate_fast);
	return;
}

void gnss_power_req( gnss_power_req_t gnss_new_state ){
	//Outside functions can kick gnss
	switch(gnss_new_state){
		case gnss_rate_fast:
			//Start GNSS chip
			gnss_status = gnss_state_fast;
			//Stop any running timers
			HW_TS_Stop(GNSSTimer_Id);
			break;
		case gnss_rate_slow:
			gnss_status = gnss_state_slow;
			//Start GNSS chip with timer
			HW_TS_Start(GNSSTimer_Id, GNSS_INTERVAL_SLOW);
			break;
		case gnss_rate_stop:
			//Stop GNSS chip
			//Stop running timer
			HW_TS_Stop(GNSSTimer_Id);
			break;
		default:
			break;
	}
	return;
}


void gnss_timer_return(void){
	switch (gnss_status){
		case gnss_state_fast:
			//stop timer, no action needed
			HW_TS_Stop(GNSSTimer_Id);
			break;
		case gnss_state_sleep:
			//turn on GPS
			//Set timer for timeout
			gnss_status = gnss_state_slow;
			HW_TS_Start(GNSSTimer_Id, GNSS_INTERVAL_SLOW);
			break;
		case gnss_state_slow:
			//turn off GPS
			//Set timer for next on time
			gnss_status = gnss_state_sleep;
			HW_TS_Start(GNSSTimer_Id, GNSS_INTERVAL_SLEEP);
			break;
		default:
			break;
	}
	return;
}

void gnss_parse(uint8_t byte_read){
	/* Identify the packet header */
	if (parser_pos < 2) {
	  if (byte_read == UBX_HEADER[parser_pos]) {
		parser_pos++;
	  } else {
		parser_pos = 0;
	  }
	/* Identify Class, ID, and Length */
	} else if(parser_pos < 6){
		if (byte_read == UBX_CLASS[parser_pos]) {
		rx_buffer[parser_pos - UBX_HEADER_LEN] = byte_read;
		parser_pos++;
	  } else {
		parser_pos = 0;
	  }
	/* Message payload */
	} else if (parser_pos < (UBX_PVT_LEN + UBX_HEADER_LEN)) {
	  rx_buffer[parser_pos - UBX_HEADER_LEN] = byte_read;
	  parser_pos++;
	/* Checksum */
	} else  if (parser_pos == (UBX_PVT_LEN + UBX_HEADER_LEN)) {
	  checksum_buffer[0] = byte_read;
	  parser_pos++;
	} else {
	  checksum_buffer[1] = byte_read;
	  uint16_t received_checksum =(uint16_t)(checksum_buffer[1]) << 8 | checksum_buffer[0];
	  uint16_t computed_checksum = Checksum((uint8_t*) &rx_buffer, UBX_PVT_LEN + UBX_CLASS_ID_LEN);
	  if (computed_checksum == received_checksum) {
		  //Data is good
		  memcpy(&ubx_nav_pvt_parsed,(uint8_t*) &rx_buffer, UBX_PVT_LEN);
	  }
	  parser_pos = 0;
	}
	return;
}

uint16_t Checksum(uint8_t *data, uint16_t len) {
  if (!data) {
    return 0;
  }
  uint8_t checksum_buffer[2] = {0, 0};
  for (unsigned int i = 0; i < len; i++) {
    checksum_buffer[0] += data[i];
    checksum_buffer[1] += checksum_buffer[0];
  }
  return (uint16_t)(checksum_buffer[1]) << 8 | checksum_buffer[0];
}

uint16_t gnss_year() {
  return ubx_nav_pvt_parsed.year;
}

uint8_t gnss_month() {
  return ubx_nav_pvt_parsed.month;
}

uint8_t gnss_day() {
  return ubx_nav_pvt_parsed.day;
}

uint8_t gnss_hour() {
  return ubx_nav_pvt_parsed.hour;
}

uint8_t gnss_min() {
  return ubx_nav_pvt_parsed.min;
}

uint8_t gnss_sec() {
  return ubx_nav_pvt_parsed.sec;
}

int32_t gnss_nano_sec() {
  return ubx_nav_pvt_parsed.nano;
}

FixType gnss_fix() {
  return ubx_nav_pvt_parsed.fix;
}

uint8_t gnss_flags1() {
  return ubx_nav_pvt_parsed.flags;
}

uint8_t gnss_num_satellites() {
  return ubx_nav_pvt_parsed.numsv;
}

int32_t gnss_height() {
  return ubx_nav_pvt_parsed.height;
}

uint32_t gnss_heightAcc() {
  return ubx_nav_pvt_parsed.hacc;
}

int32_t gnss_gSpeed() {
  return ubx_nav_pvt_parsed.gspeed;
}

int32_t gnss_heading() {
  return ubx_nav_pvt_parsed.headmot;
}

int16_t gnss_magDec() {
  return ubx_nav_pvt_parsed.magdec;
}


void setGNSSwakeup(gnss_wakeup_enum newState){
	gnss_wakeup = newState;
}

gnss_wakeup_enum getGNSSwakeup( void ){
	return gnss_wakeup;
}


void LPUART_CharReception_Callback(void)
{
	while (LL_LPUART_IsActiveFlag_RXNE(LPUART1)){ //empty FIFO
	  	/* Read Received character. RXNE flag is cleared by reading of RDR register */
		char newByte = LL_LPUART_ReceiveData8(LPUART1);

		APP_DBG_MSG("GNSS Char %c \n", newByte);
		//gnss_parse((uint8_t)LL_LPUART_ReceiveData8(LPUART1));
		if (getGNSSwakeup() == gnss_wakeUp_none){
			setGNSSwakeup(gnss_wakeUp_LPUART);
		}
	}
	LL_LPUART_ClearFlag_WKUP(LPUART1);
}
