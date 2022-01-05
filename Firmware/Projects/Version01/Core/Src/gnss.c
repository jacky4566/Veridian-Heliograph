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
#include "UBXCFG.h"

//Private Defines and statics
#define GNSS_INTERVAL_SLOW   ( 60 * 1000000 / CFG_TS_TICK_VAL)  //timeout waiting for fix
#define GNSS_INTERVAL_SLEEP  ( 10* 60 * 1000000 / CFG_TS_TICK_VAL)	//time to hold Ublox in shutdown for next fix

static uint8_t UBX_HEADER_LEN = 2;
static uint8_t UBX_CLASS_ID_LEN = 4;
static uint8_t UBX_PVT_LEN = 92;
static uint8_t UBX_HEADER[2] = {0xB5, 0x62}; //UBX default header
static uint8_t UBX_CLASS[4] = {0x01, 0x07, 0x00, 0x92}; //Class, ID, and length information

//data types
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
gnss_power_t gnss_status = gnss_rate_stop;
uint8_t GNSSTimer_Id; //TimerID

struct ubx_nav_pvt ubx_nav_pvt_parsed;
volatile  uint8_t rx_buffer[96];
volatile uint16_t parser_pos;
volatile uint8_t checksum_buffer[2];

//Private Functions Declarations
void gnss_parse( uint8_t );
void gnss_timer_return( void );
void GPSStart( void );
void GPSStop( void );
void UBX_CFG( void );
void LPUART_Transmit_Blocking( const void* uint8_t, int);
uint16_t Checksum( uint8_t*, uint16_t );

//Temporary
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;

//Functions
void gnss_Init( void ){
	//Create a timer for our Slow mode
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &GNSSTimer_Id, hw_ts_SingleShot, gnss_timer_return);

	//Start GPS in fast mode
	gnss_power_req(gnss_rate_fast);

	//Send config
	UBX_CFG();
	return;
}

void gnss_power_req( gnss_power_t gnss_new_state ){
	//Outside functions can kick gnss
	if (gnss_new_state != gnss_status){
		gnss_status = gnss_new_state;
		switch(gnss_status){
			case gnss_rate_fast:
				GPSStart();
				HW_TS_Stop(GNSSTimer_Id);
				break;
			case gnss_rate_slow:
				GPSStart();
				HW_TS_Start(GNSSTimer_Id, GNSS_INTERVAL_SLOW);
				break;
			case gnss_rate_sleep:
				GPSStop();
				HW_TS_Start(GNSSTimer_Id, GNSS_INTERVAL_SLEEP);
				break;
			case gnss_rate_stop:
				GPSStop();
				HW_TS_Stop(GNSSTimer_Id);
				break;
			default:
				break;
		}
	}
	return;
}


void gnss_timer_return(void){
	switch (gnss_status){
		case gnss_rate_fast:
			gnss_power_req(gnss_rate_fast);
			break;
		case gnss_rate_slow:
			gnss_power_req(gnss_rate_sleep);
			break;
		case gnss_rate_sleep:
			gnss_power_req(gnss_rate_slow);
			break;
		case gnss_rate_stop:
			gnss_power_req(gnss_rate_stop);
			break;
		default:
			break;
	}
	return;
}


void GPSStart(){
	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); //Set Pin HIGH
	if (!LL_LPUART_IsEnabled(LPUART1)){
		LL_LPUART_Enable(LPUART1);
		while((!(LL_LPUART_IsActiveFlag_TEACK(LPUART1))) || (!(LL_LPUART_IsActiveFlag_REACK(LPUART1)))){}
	}
}

void GPSStop(){
	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); //Set Pin LOW
	LL_LPUART_Disable(LPUART1);
}

void UBX_CFG(){
	if (!(HAL_RTCEx_BKUPRead(&hrtc, RTCbckupGNSS) & 0x01)){ //check if we are already set
		//Sends the commands to inialize Ublox chips
		LPUART_Transmit_Blocking(UBX_CFG_GNSS, sizeof(UBX_CFG_GNSS));
		LPUART_Transmit_Blocking(UBX_NAV_GGA_OFF, sizeof(UBX_NAV_GGA_OFF));
		LPUART_Transmit_Blocking(UBX_NAV_GLL_OFF, sizeof(UBX_NAV_GLL_OFF));
		LPUART_Transmit_Blocking(UBX_NAV_GSA_OFF, sizeof(UBX_NAV_GSA_OFF));
		LPUART_Transmit_Blocking(UBX_NAV_GSV_OFF, sizeof(UBX_NAV_GSV_OFF));
		LPUART_Transmit_Blocking(UBX_NAV_RMC_OFF, sizeof(UBX_NAV_RMC_OFF));
		LPUART_Transmit_Blocking(UBX_NAV_VTG_OFF, sizeof(UBX_NAV_VTG_OFF));
		LPUART_Transmit_Blocking(UBX_NAV_PVT_ON, sizeof(UBX_NAV_PVT_ON));
		LPUART_Transmit_Blocking(UBX_CFG_RXM, sizeof(UBX_CFG_RXM));
		LPUART_Transmit_Blocking(UBX_CFG_PM2, sizeof(UBX_CFG_PM2));
		LPUART_Transmit_Blocking(UBX_CFG_SAVE, sizeof(UBX_CFG_SAVE));
		//Save config done to RTC register
		HAL_RTCEx_BKUPWrite(&hrtc, RTCbckupGNSS, HAL_RTCEx_BKUPRead(&hrtc, RTCbckupGNSS) | 0x01);
	}
}

void LPUART_Transmit_Blocking(const void* data, int len ){
	const uint8_t* b = data;
	//Low power stuff
	LL_LPUART_EnableIT_TXE(LPUART1);
	HAL_SuspendTick();
	LL_LPM_EnableSleep( );
	while (len--) {
		LL_LPUART_TransmitData8(LPUART1, *b++);
		while(LL_LPUART_IsActiveFlag_TXE(LPUART1)){
			__WFI( );
			//Transmit flag should wake here
		}
	}
	LL_LPUART_DisableIT_TXE(LPUART1);
	HAL_ResumeTick();
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

void LPUART_CharReception_Callback(void)
{
	while (LL_LPUART_IsActiveFlag_RXNE(LPUART1)){ //empty FIFO
	  	/* Read Received character. RXNE flag is cleared by reading of RDR register */
		char newByte = LL_LPUART_ReceiveData8(LPUART1);
		//HAL_UART_Transmit(&huart1, &newByte, 1, 1000);
		//APP_DBG_MSG("GNSS Char %c \n", newByte);
		//gnss_parse((uint8_t)LL_LPUART_ReceiveData8(LPUART1));
	}
}
