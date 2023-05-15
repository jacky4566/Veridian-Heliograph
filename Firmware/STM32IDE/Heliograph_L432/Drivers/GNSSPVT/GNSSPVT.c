/*
 *  GNSSPVT
 *
 *  Created on: May 20, 2022
 *  Author: Jackson-E15Gen3
 */

#include "main.h"
#include "GNSSPVT.h"
#include "UBXDATA.h"
#include "myApp.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define gnssFixOKmask 0x01
#define timeFullyResolved 0x04

//Power State
volatile enum GNSS_rate GNSSlastRate = GNSS_UINT;

//Parsing stuff
const uint8_t UBX_HEADER_[2] = { 0xB5, 0x62 };
const uint8_t UBX_HEADER_LEN_ = 6;
const uint8_t UBX_PAYLOAD_OFFSET_ = 4;
const uint8_t UBX_NAV_CLASS_ = 0x01;
const uint8_t UBX_NAV_PVT = 0x07;
const uint8_t UBX_PVT_LEN_ = 92;
volatile uint32_t GNSSlastPacketAge;
volatile bool GNSSAlive;
volatile bool GNSSNewData = true;
volatile uint16_t parser_pos_ = 0;
volatile uint8_t msg_len_buffer_[2];
volatile uint16_t msg_len_;
volatile uint8_t checksum_buffer_[2];
volatile uint8_t pvt_buffer_[96];
struct {
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
	int32_t lon_deg;
	int32_t lat_deg;
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
} ubx_nav_pvt;

//Private functions
void parse(uint8_t byte_read);
void GNSS_Config_Slow();
void GNSS_Config_Fast();
uint16_t Checksum(volatile uint8_t *data, uint16_t len);
static void GNSS_Set_Power(enum GNSS_rate);
static void LPUART_Transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout);

//Functions
//Time
uint16_t GNSS_getYear() {
	return ubx_nav_pvt.year;
}
uint8_t GNSS_getMonth() {
	return ubx_nav_pvt.month;
}
uint8_t GNSS_getDay() {
	return ubx_nav_pvt.day;
}
uint8_t GNSS_getHour() {
	return ubx_nav_pvt.hour;
}
uint8_t GNSS_getMin() {
	return ubx_nav_pvt.min;
}
uint8_t GNSS_getSec() {
	return ubx_nav_pvt.sec;
}

//Location
float getLat() {
	return ((float) ubx_nav_pvt.lat_deg) * 1e-7;
}

float getLong() {
	return ((float) ubx_nav_pvt.lon_deg) * 1e-7;
}

float getHAcc() {
	return (float) ubx_nav_pvt.hacc * 1e-3;
}

int getGroundSpeed_kph() {
	return (int) ((ubx_nav_pvt.gspeed / 1000) * 3600) / 1000; // km/h
}
int getMotionHeading_deg() {
	return (int) (ubx_nav_pvt.headmot * 1e-5);
}

//Status
uint8_t getNumSatellites() {
	return ubx_nav_pvt.numsv;
}

enum GNSS_FixType getFixType() {
	return (enum GNSS_FixType) ubx_nav_pvt.fix;
}

bool isGnssFixOk() {
	return ubx_nav_pvt.flags & gnssFixOKmask;
}

bool isTimeFullyResolved() {
	return ubx_nav_pvt.valid & timeFullyResolved;
}

void GNSS_Prep_Stop() {
	if (!LL_LPUART_IsEnabled(LPUART1)) {
		return;
	}
	while (LL_LPUART_IsActiveFlag_RXNE(LPUART1)) { //Empty RX buffer
		parse(LL_LPUART_ReceiveData8(LPUART1));
	}
	LL_LPUART_ClearFlag_ORE(LPUART1);
	/* Make sure that LPUART is ready to receive */
	while (LL_LPUART_IsActiveFlag_REACK(LPUART1) == 0) {
	}
	LL_LPUART_ClearFlag_WKUP(LPUART1);
	LL_LPUART_EnableIT_WKUP(LPUART1);
	LL_LPUART_EnableInStopMode(LPUART1);
}

void GNSS_Power() {
	switch (GNSSlastRate) {
	case GNSS_UINT:
		GNSS_Set_Power(GNSS_STOP);
		break;
	case GNSS_STOP:
		if (superCapmV >= mV_GNSS_SLOW) {
			GNSS_Set_Power(GNSS_SLOW);
		}
		break;
	case GNSS_SLOW:
		if (superCapmV >= mV_GNSS_FAST) {
			GNSS_Set_Power(GNSS_FAST);
		} else if (superCapmV < mV_GNSS_OFF) {
			GNSS_Set_Power(GNSS_STOP);
		}
		break;
	case GNSS_FAST:
		if (superCapmV < mV_GNSS_OFF) {
			GNSS_Set_Power(GNSS_STOP);
		} else if (superCapmV < mV_GNSS_SLOW) {
			GNSS_Set_Power(GNSS_SLOW);
		}
		break;
	}
}

static void GNSS_Set_Power(enum GNSS_rate newRate) {
	if (newRate == GNSSlastRate) {
		return;
	}
	switch (GNSSlastRate) {
	case GNSS_UINT:
	case GNSS_STOP:
		GNSSAlive = false;
		HAL_GPIO_WritePin(GNSS_EXT_GPIO_Port, GNSS_EXT_Pin, GPIO_PIN_RESET);
		USER_LPUART1_UART_DeInit();
		GNSSlastRate = GNSS_STOP;
		break;
	case GNSS_SLOW:
		USER_LPUART1_UART_Init();
		GNSS_Config_Slow();
		GNSSlastRate = GNSS_SLOW;
		break;
	case GNSS_FAST:
		USER_LPUART1_UART_Init();
		GNSS_Config_Fast();
		GNSSlastRate = GNSS_SLOW;
		break;
	}
}

void GNSS_Config_Slow() {
	if (!HAL_GPIO_ReadPin(GNSS_EXT_GPIO_Port, GNSS_EXT_Pin)) {
		//GNSS was asleep
		//Wakeup
		HAL_GPIO_WritePin(GNSS_EXT_GPIO_Port, GNSS_EXT_Pin, GPIO_PIN_SET);
		LPUART_Transmit((uint8_t*) 0xff, 1, HAL_MAX_DELAY);
		HAL_Delay(500);
		//Enable PVT message
		LPUART_Transmit((uint8_t*) &UBX_CFG_MSGOUT_UART1, sizeof(UBX_CFG_MSGOUT_UART1), HAL_MAX_DELAY);
	}
	//Enable PMOO
	LPUART_Transmit((uint8_t*) &UBX_CFG_PM_OPERATEMODE_SLOW, sizeof(UBX_CFG_PM_OPERATEMODE_SLOW), HAL_MAX_DELAY);
}

void GNSS_Config_Fast() {
	//Run at full speed
	LPUART_Transmit((uint8_t*) &UBX_CFG_PM_OPERATEMODE_FULL, sizeof(UBX_CFG_PM_OPERATEMODE_FULL), HAL_MAX_DELAY);
}

//Parsing
uint16_t Checksum(volatile uint8_t *data, uint16_t len) {
	if (!data) {
		return 0;
	}
	uint8_t checksum_buffer[2] = { 0, 0 };
	for (unsigned int i = 0; i < len; i++) {
		checksum_buffer[0] += data[i];
		checksum_buffer[1] += checksum_buffer[0];
	}
	return ((uint16_t) checksum_buffer_[1]) << 8 | checksum_buffer_[0];
}

void parse(uint8_t byte_read) {
	/* Identify the packet header */
	if (parser_pos_ < 2) {
		if (byte_read == UBX_HEADER_[parser_pos_]) {
			parser_pos_++;
		} else {
			parser_pos_ = 0;
		}
		/* Message class */
	} else if (parser_pos_ == 2) {
		if (byte_read == UBX_NAV_CLASS_) {
			pvt_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
			parser_pos_++;
		} else {
			parser_pos_ = 0;
		}
		/* Message ID */
	} else if (parser_pos_ == 3) {
		if (byte_read == UBX_NAV_PVT) {
			pvt_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
			parser_pos_++;
		} else {
			parser_pos_ = 0;
		}
		/* Messgae length */
	} else if (parser_pos_ == 4) {
		msg_len_buffer_[0] = byte_read;
		pvt_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
		parser_pos_++;
		/* Message length */
	} else if (parser_pos_ == 5) {
		msg_len_buffer_[1] = byte_read;
		msg_len_ = ((uint16_t) msg_len_buffer_[1]) << 8 | msg_len_buffer_[0];
		pvt_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
		if (msg_len_ == UBX_PVT_LEN_) {
			parser_pos_++;
		} else {
			parser_pos_ = 0;
		}
		return;
		/* Message payload */
	} else if (parser_pos_ < (msg_len_ + UBX_HEADER_LEN_)) {
		pvt_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
		parser_pos_++;
		/* Checksum */
	} else if (parser_pos_ == (msg_len_ + UBX_HEADER_LEN_)) {
		checksum_buffer_[0] = byte_read;
		parser_pos_++;
	} else {
		checksum_buffer_[1] = byte_read;
		uint16_t received_checksum = ((uint16_t) checksum_buffer_[1]) << 8 | checksum_buffer_[0];
		uint16_t computed_checksum = Checksum(pvt_buffer_, msg_len_ + UBX_HEADER_LEN_);
		if (computed_checksum == received_checksum) {
			if (pvt_buffer_[20 + UBX_PAYLOAD_OFFSET_] >= FIX_2D) {
				memcpy(&ubx_nav_pvt, (void*) pvt_buffer_ + UBX_PAYLOAD_OFFSET_, UBX_PVT_LEN_);
				GNSSlastPacketAge = 0;
				if (isTimeFullyResolved()) {
					setTimeGNSS();
				}
				GNSSNewData = true;
			}
			GNSSAlive = !GNSSAlive;
			parser_pos_ = 0;
		} else {
			parser_pos_ = 0;
		}
	}
}

void LPUART_CharReception_Callback(void) {
	while (LL_LPUART_IsActiveFlag_RXNE(LPUART1)) {
		parse(LL_LPUART_ReceiveData8(LPUART1));
	}
}

static void LPUART_Transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout) {
	if ((pData == NULL) || (Size == 0U)) {
		return;
	}

	while (Size--) {
		/* Wait for TXE flag to be raised */
		while (!LL_LPUART_IsActiveFlag_TXE(LPUART1)) {
		}
		uint8_t sendThis = (uint8_t) (*pData);
		LL_LPUART_TransmitData8(LPUART1, sendThis);
		pData++;
	}

	/* Wait for TC flag to be raised for last char */
	while (!LL_LPUART_IsActiveFlag_TC(LPUART1)) {
	}
}

