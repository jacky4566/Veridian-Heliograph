/*
 *  GNSSPVT
 *
 *  Created on: May 20, 2022
 *  Author: Jackson-E15Gen3
 */

#include "main.h"
#include "GNSSPVT.h"
#include "UBXDATA.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define gnssFixOKmask 0x01
#define timeFullyResolved 0x04

//State Machine
GNSS_rate lastRate = GNSS_STOP;

//Parsing stuff
const uint8_t UBX_HEADER_[2] = { 0xB5, 0x62 };
const uint8_t UBX_HEADER_LEN_ = 6;
const uint8_t UBX_PAYLOAD_OFFSET_ = 4;
const uint8_t UBX_NAV_CLASS_ = 0x01;
const uint8_t UBX_NAV_PVT = 0x07;
const uint8_t UBX_PVT_LEN_ = 92;
uint16_t parser_pos_ = 0;
uint8_t msg_len_buffer_[2];
uint16_t msg_len_;
uint8_t checksum_buffer_[2];
uint8_t rx_buffer_[96];
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

//Private Decs
void parse(uint8_t byte_read);
uint16_t Checksum(uint8_t *data, uint16_t len);

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
double getLongitude_deg() {
	return (double) ubx_nav_pvt.lon_deg / 10000000.0;
}
double getLatitude_deg() {
	return (double) ubx_nav_pvt.lat_deg / 10000000.0;
}
double getMSLHeight_m() {
	return (double) ubx_nav_pvt.hmsl / 1000.0;
}
double getHorizontalAccuracy_m() {
	return (double) ubx_nav_pvt.hacc / 1000.0;
}
double getGroundSpeed_kph() {
	return (double) ubx_nav_pvt.gspeed * ((1000.0 * 1000.0) / 3600.0);
}
double getMotionHeading_deg() {
	return (double) ubx_nav_pvt.headmot / 10000.0;
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

//Admin Functions
void startLPUART() {
	HAL_GPIO_WritePin(GNSS_EX_PIN, GNSS_EX_PORT, GPIO_PIN_SET);
	if (!LL_LPUART_IsEnabled(LPUART1)) {
		LL_LPUART_InitTypeDef LPUART_InitStruct = { 0 };

		LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

		/* Peripheral clock enable */
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPUART1);

		LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
		/**LPUART1 GPIO Configuration
		 PA2   ------> LPUART1_TX
		 PA3   ------> LPUART1_RX
		 */
		GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
		LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
		LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		NVIC_SetPriority(LPUART1_IRQn, 0);
		NVIC_EnableIRQ(LPUART1_IRQn);

		LPUART_InitStruct.BaudRate = 9600;
		LPUART_InitStruct.DataWidth = LL_LPUART_DATAWIDTH_8B;
		LPUART_InitStruct.StopBits = LL_LPUART_STOPBITS_1;
		LPUART_InitStruct.Parity = LL_LPUART_PARITY_NONE;
		LPUART_InitStruct.TransferDirection = LL_LPUART_DIRECTION_TX_RX;
		LPUART_InitStruct.HardwareFlowControl = LL_LPUART_HWCONTROL_NONE;
		LL_LPUART_Init(LPUART1, &LPUART_InitStruct);

		/* Configure LPUART1 transfer interrupts : */
		/* Clear WUF flag and enable the UART Wake Up from stop mode Interrupt */
		LL_LPUART_ClearFlag_WKUP(LPUART1);
		LL_LPUART_EnableIT_WKUP(LPUART1);

		/* Enable Wake Up From Stop */
		LL_LPUART_EnableInStopMode(LPUART1);
	}
}

void stopLPUART() {
	LL_LPUART_DeInit(LPUART1);
	HAL_GPIO_WritePin(GNSS_EX_PIN, GNSS_EX_PORT, GPIO_PIN_RESET);
}

void LPUARTPrinter(const uint8_t *buf, uint32_t len) {
	/* Send characters one per one, until last char to be sent */
	for (; len >= 0; len--) {
		while (!LL_LPUART_IsActiveFlag_TXE(LPUART1)) { // Wait for TX reg to be empty
		}
		LL_LPUART_TransmitData8(LPUART1, *buf++); //put char into transmit FIFO
	}
	while (!LL_LPUART_IsActiveFlag_TC(LPUART1)) { //wait for TX Complete Flush
	}
}

void GNSS_Setup(GNSS_rate newRate) {
	if (newRate == lastRate) {
		//No Change needed
		return;
	}
	switch (newRate) {
	case GNSS_STOP:
		if (lastRate == GNSS_FULL) { //Full speed has no power control
			GNSS_Setup(GNSS_SLOW);
		}
		stopLPUART();
		break;
	case GNSS_SLOW:
		startLPUART();
		//Send slow commands
		LPUARTPrinter(UBX_NAV_GGA_OFF, sizeof(UBX_NAV_GGA_OFF));
		LPUARTPrinter(UBX_NAV_GLL_OFF, sizeof(UBX_NAV_GLL_OFF));
		LPUARTPrinter(UBX_NAV_GSA_OFF, sizeof(UBX_NAV_GSA_OFF));
		LPUARTPrinter(UBX_NAV_GSV_OFF, sizeof(UBX_NAV_GSV_OFF));
		LPUARTPrinter(UBX_NAV_RMC_OFF, sizeof(UBX_NAV_RMC_OFF));
		LPUARTPrinter(UBX_NAV_VTG_OFF, sizeof(UBX_NAV_VTG_OFF));
		LPUARTPrinter(UBX_NAV_PVT_ON, sizeof(UBX_NAV_PVT_ON));
		LPUARTPrinter(UBX_CFG_PM2_4S, sizeof(UBX_CFG_PM2_4S));
		LPUARTPrinter(UBX_CFG_RXM_0, sizeof(UBX_CFG_RXM_0));
		LPUARTPrinter(UBX_CFG_CFG, sizeof(UBX_CFG_CFG));
		break;
	case GNSS_FAST:
		startLPUART();
		//Send fast commands
		LPUARTPrinter(UBX_NAV_GGA_OFF, sizeof(UBX_NAV_GGA_OFF));
		LPUARTPrinter(UBX_NAV_GLL_OFF, sizeof(UBX_NAV_GLL_OFF));
		LPUARTPrinter(UBX_NAV_GSA_OFF, sizeof(UBX_NAV_GSA_OFF));
		LPUARTPrinter(UBX_NAV_GSV_OFF, sizeof(UBX_NAV_GSV_OFF));
		LPUARTPrinter(UBX_NAV_RMC_OFF, sizeof(UBX_NAV_RMC_OFF));
		LPUARTPrinter(UBX_NAV_VTG_OFF, sizeof(UBX_NAV_VTG_OFF));
		LPUARTPrinter(UBX_NAV_PVT_ON, sizeof(UBX_NAV_PVT_ON));
		LPUARTPrinter(UBX_CFG_PM2_600S, sizeof(UBX_CFG_PM2_600S));
		LPUARTPrinter(UBX_CFG_RXM_0, sizeof(UBX_CFG_RXM_0));
		LPUARTPrinter(UBX_CFG_CFG, sizeof(UBX_CFG_CFG));
		break;
	case GNSS_FULL:
		startLPUART();
		//Send full power commands
		LPUARTPrinter(UBX_NAV_GGA_OFF, sizeof(UBX_NAV_GGA_OFF));
		LPUARTPrinter(UBX_NAV_GLL_OFF, sizeof(UBX_NAV_GLL_OFF));
		LPUARTPrinter(UBX_NAV_GSA_OFF, sizeof(UBX_NAV_GSA_OFF));
		LPUARTPrinter(UBX_NAV_GSV_OFF, sizeof(UBX_NAV_GSV_OFF));
		LPUARTPrinter(UBX_NAV_RMC_OFF, sizeof(UBX_NAV_RMC_OFF));
		LPUARTPrinter(UBX_NAV_VTG_OFF, sizeof(UBX_NAV_VTG_OFF));
		LPUARTPrinter(UBX_NAV_PVT_ON, sizeof(UBX_NAV_PVT_ON));
		LPUARTPrinter(UBX_CFG_RXM_0, sizeof(UBX_CFG_RXM_0));
		LPUARTPrinter(UBX_CFG_PMS_FULL, sizeof(UBX_CFG_PMS_FULL));
		LPUARTPrinter(UBX_CFG_CFG, sizeof(UBX_CFG_CFG));
		break;
	default:
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
			rx_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
			parser_pos_++;
		} else {
			parser_pos_ = 0;
		}
		/* Message ID */
	} else if (parser_pos_ == 3) {
		if (byte_read == UBX_NAV_PVT) {
			rx_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
			parser_pos_++;
		} else {
			parser_pos_ = 0;
		}
		/* Messgae length */
	} else if (parser_pos_ == 4) {
		msg_len_buffer_[0] = byte_read;
		rx_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
		parser_pos_++;
		/* Message length */
	} else if (parser_pos_ == 5) {
		msg_len_buffer_[1] = byte_read;
		msg_len_ = ((uint16_t) msg_len_buffer_[1]) << 8 | msg_len_buffer_[0];
		rx_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
		if (msg_len_ == UBX_PVT_LEN_) {
			parser_pos_++;
		} else {
			parser_pos_ = 0;
		}
		return;
		/* Message payload */
	} else if (parser_pos_ < (msg_len_ + UBX_HEADER_LEN_)) {
		rx_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
		parser_pos_++;
		/* Checksum */
	} else if (parser_pos_ == (msg_len_ + UBX_HEADER_LEN_)) {
		checksum_buffer_[0] = byte_read;
		parser_pos_++;
	} else {
		checksum_buffer_[1] = byte_read;
		uint16_t received_checksum = ((uint16_t) checksum_buffer_[1]) << 8
				| checksum_buffer_[0];
		uint16_t computed_checksum = Checksum(rx_buffer_,
				msg_len_ + UBX_HEADER_LEN_);
		if (computed_checksum == received_checksum) {
			if (rx_buffer_[UBX_PAYLOAD_OFFSET_ + 21] & 0x01) { //Fix OK
				memcpy(&ubx_nav_pvt, rx_buffer_ + UBX_PAYLOAD_OFFSET_,
						UBX_PVT_LEN_);
			}
			parser_pos_ = 0;
		} else {
			parser_pos_ = 0;
		}
	}
}

void LPUART_CharReception_Callback(void) {
	uint8_t newData = (uint8_t) LL_LPUART_ReceiveData8(LPUART1);
	parse(newData);
}
