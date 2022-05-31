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
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define gnssFixOKmask 0x01
#define timeFullyResolved 0x04
#define longABSMask (0x7FFFFFFF)
#define longSignMask (0x80000000)

//Power State
GNSS_rate GNSSlastRate;

//Parsing stuff
const uint8_t UBX_HEADER_[2] = { 0xB5, 0x62 };
const uint8_t UBX_HEADER_LEN_ = 6;
const uint8_t UBX_PAYLOAD_OFFSET_ = 4;
const uint8_t UBX_NAV_CLASS_ = 0x01;
const uint8_t UBX_NAV_PVT = 0x07;
const uint8_t UBX_PVT_LEN_ = 92;
uint32_t packets;
uint16_t parser_pos_ = 0;
uint8_t msg_len_buffer_[2];
uint16_t msg_len_;
uint8_t checksum_buffer_[2];
uint8_t pvt_buffer_[96];
uint8_t uart_buffer_[12];
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
struct locationFix LastFix;

//Private functions
void parse(uint8_t byte_read);
uint16_t Checksum(uint8_t *data, uint16_t len);
static void GNSS_Set_Power(GNSS_rate);
static void LPUART_Transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout);
static void updateLocation();

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
void updateLocation() {
	LastFix.Lat_Deg = (uint8_t) ((ubx_nav_pvt.lat_deg & longABSMask) / 10000000UL);
	LastFix.Long_Deg = (uint8_t) ((ubx_nav_pvt.lon_deg & longABSMask) / 10000000UL);
	LastFix.Lat_Dec = (uint16_t) (ubx_nav_pvt.lat_deg & longABSMask) - LastFix.Lat_Deg;
	LastFix.Long_Dec = (uint16_t) (ubx_nav_pvt.lon_deg & longABSMask) - LastFix.Long_Deg;
	LastFix.NS = ubx_nav_pvt.lat_deg & longSignMask;
	LastFix.EW = ubx_nav_pvt.lon_deg & longSignMask;
	LastFix.Elevation_M = (uint16_t) (abs(ubx_nav_pvt.hmsl) / 1000UL);
	if (ubx_nav_pvt.hacc > 255) {
		LastFix.HzAccM = 0xff;
	} else {
		LastFix.HzAccM = (uint8_t) (abs(ubx_nav_pvt.hacc) / 1000UL);
	}
	if (isTimeFullyResolved()) {
		setTime();
	}
}

uint8_t getGroundSpeed_kph() {
	return (uint8_t) (ubx_nav_pvt.gspeed * ((1000UL * 1000UL) / 3600UL));
}
uint8_t getMotionHeading_deg() {
	return (uint8_t) (ubx_nav_pvt.headmot / 10000UL);
}

//Status
uint8_t getNumSatellites() {
	return ubx_nav_pvt.numsv;
}
GNSS_FixType getFixType() {
	return (GNSS_FixType) ubx_nav_pvt.fix;
}
bool isGnssFixOk() {
	return ubx_nav_pvt.flags & gnssFixOKmask;
}
bool isTimeFullyResolved() {
	return ubx_nav_pvt.valid & timeFullyResolved;
}

void GNSS_Init() {
	//Enable Interrupts and STOP0
	LL_LPUART_Enable(LPUART1);
	HAL_GPIO_WritePin(GNSS_EXT_GPIO_Port, GNSS_EXT_Pin, GPIO_PIN_RESET); //DISABLE GNSS
	LPUART_Transmit((uint8_t*) 0xff, 1, HAL_MAX_DELAY); //wakeup
//Turn off a bunch of stuff
	LPUART_Transmit((uint8_t*) &UBX_NAV_GGA_OFF, sizeof(UBX_NAV_GGA_OFF), HAL_MAX_DELAY);
	LPUART_Transmit((uint8_t*) &UBX_NAV_GLL_OFF, sizeof(UBX_NAV_GLL_OFF), HAL_MAX_DELAY);
	LPUART_Transmit((uint8_t*) &UBX_NAV_GSA_OFF, sizeof(UBX_NAV_GSA_OFF), HAL_MAX_DELAY);
	LPUART_Transmit((uint8_t*) &UBX_NAV_GSV_OFF, sizeof(UBX_NAV_GSV_OFF), HAL_MAX_DELAY);
	LPUART_Transmit((uint8_t*) &UBX_NAV_GSV_OFF, sizeof(UBX_NAV_GSV_OFF), HAL_MAX_DELAY);
	LPUART_Transmit((uint8_t*) &UBX_NAV_RMC_OFF, sizeof(UBX_NAV_RMC_OFF), HAL_MAX_DELAY);
	LPUART_Transmit((uint8_t*) &UBX_NAV_VTG_OFF, sizeof(UBX_NAV_VTG_OFF), HAL_MAX_DELAY);
//Enable PVT message
	LPUART_Transmit((uint8_t*) &UBX_NAV_PVT_ON, sizeof(UBX_NAV_PVT_ON), HAL_MAX_DELAY);
//Enable PSM
	LPUART_Transmit((uint8_t*) &UBX_CFG_RXM_1, sizeof(UBX_CFG_RXM_1), HAL_MAX_DELAY);
//Save
	LPUART_Transmit((uint8_t*) &UBX_CFG_CFG, sizeof(UBX_CFG_CFG), HAL_MAX_DELAY);
//Move from uninitlized
	GNSS_Power();
}

void GNSS_Prep_Stop() {
	while (LL_LPUART_IsActiveFlag_RXNE(LPUART1)) { //Empty RX buffer
		parse(LL_LPUART_ReceiveData8(LPUART1));
	}
	LL_LPUART_ClearFlag_ORE(LPUART1);
	/* Make sure that no LPUART transfer is on-going */
	while (LL_LPUART_IsActiveFlag_BUSY(LPUART1) == 1) {
	}
	/* Make sure that LPUART is ready to receive */
	while (LL_LPUART_IsActiveFlag_REACK(LPUART1) == 0) {
	}
	LL_LPUART_ClearFlag_WKUP(LPUART1);
	LL_LPUART_EnableIT_WKUP(LPUART1);
	LL_LPUART_EnableInStopMode(LPUART1);
}

void GNSS_Power() {
	switch (GNSSlastRate) {
	case GNSS_DEFAULT:
		GNSS_Set_Power(GNSS_SLOW);
		break;
	case GNSS_STOP:
		if (superCapmV > 3000) {
			GNSS_Set_Power(GNSS_SLOW);
		}
		break;
	case GNSS_Hot_Hold:
		if (superCapmV < 2500) {
			GNSS_Set_Power(GNSS_STOP);
		} else if (superCapmV > 3000) {
			GNSS_Set_Power(GNSS_SLOW);
		}
		break;
	case GNSS_SLOW:
		if (superCapmV < 2800) {
			GNSS_Set_Power(GNSS_Hot_Hold);
		} else if (superCapmV > 3400) {
			GNSS_Set_Power(GNSS_FAST);
		}
		break;
	case GNSS_FAST:
		if (superCapmV < 3300) {
			GNSS_Set_Power(GNSS_SLOW);
		}
		break;
	}
}

static void GNSS_Set_Power(GNSS_rate newRate) {
	LPUART_CharReception_Callback();
	if (newRate == GNSSlastRate) {
		return;
	}
	switch (newRate) {
	case GNSS_STOP:
		LPUART_Transmit((uint8_t*) 0xff, 1, HAL_MAX_DELAY); //wakeup
		LPUART_Transmit((uint8_t*) &UBX_CFG_PWR_STNBY, sizeof(UBX_CFG_PWR_STNBY), HAL_MAX_DELAY); //Stop GPS
		HAL_Delay(1);
		HAL_GPIO_WritePin(GNSS_EXT_GPIO_Port, GNSS_EXT_Pin, GPIO_PIN_RESET); //Disable GNSS
		GNSSlastRate = GNSS_STOP;
		break;
	case GNSS_Hot_Hold:
		HAL_GPIO_WritePin(GNSS_EXT_GPIO_Port, GNSS_EXT_Pin, GPIO_PIN_RESET); //Disable GNSS
		GNSSlastRate = GNSS_Hot_Hold;
		break;
	case GNSS_DEFAULT:
	case GNSS_SLOW:
		HAL_GPIO_WritePin(GNSS_EXT_GPIO_Port, GNSS_EXT_Pin, GPIO_PIN_SET); //Enable GNSS
		if (GNSSlastRate == GNSS_DEFAULT || GNSSlastRate == GNSS_STOP) {
			LPUART_Transmit((uint8_t*) 0xff, 1, HAL_MAX_DELAY); //wakeup
			HAL_Delay(400);
		}
		LPUART_Transmit((uint8_t*) &UBX_CFG_PMS_1HZ, sizeof(UBX_CFG_PMS_1HZ), HAL_MAX_DELAY);
		LPUART_Transmit((uint8_t*) &UBX_CFG_CFG, sizeof(UBX_CFG_CFG), HAL_MAX_DELAY);
		GNSSlastRate = GNSS_SLOW;
		break;
	case GNSS_FAST:
		HAL_GPIO_WritePin(GNSS_EXT_GPIO_Port, GNSS_EXT_Pin, GPIO_PIN_SET); //Enable GNSS
		LPUART_Transmit((uint8_t*) &UBX_CFG_PMS_1HZ, sizeof(UBX_CFG_PMS_1HZ), HAL_MAX_DELAY);
		LPUART_Transmit((uint8_t*) &UBX_CFG_CFG, sizeof(UBX_CFG_CFG), HAL_MAX_DELAY);
		GNSSlastRate = GNSS_FAST;
		break;
	}
}

//Parsing
uint16_t Checksum(uint8_t *data, uint16_t len) {
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
			memcpy(&ubx_nav_pvt, pvt_buffer_ + UBX_PAYLOAD_OFFSET_, UBX_PVT_LEN_);
			updateLocation();
			packets++;
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

