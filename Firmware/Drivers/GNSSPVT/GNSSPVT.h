#ifndef GNSSPVT_H_
#define GNSSPVT_H_

/*
 *  GNSSPVT
 *
 *  Created on: May 20, 2022
 *  Author: Jackson-E15Gen3
 */

#include "main.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	NO_FIX = 0, DEAD_RECKONING, FIX_2D, FIX_3D, GNSS_AND_DEAD_RECKONING, TIME_ONLY
} GNSS_FixType;

typedef enum {
	GNSS_UINT, GNSS_STOP, GNSS_ON
} GNSS_rate;

extern volatile uint32_t GNSSlastPacketAge;
extern volatile bool GNSSAlive;
extern volatile bool GNSSNewData;
extern GNSS_rate GNSSlastRate;
extern struct locationFix LastFix;

//Public Functions
void GNSS_Init();
void GNSS_Power();
void GNSS_Prep_Stop();

//Time
uint16_t GNSS_getYear();
uint8_t GNSS_getMonth();
uint8_t GNSS_getDay();
uint8_t GNSS_getHour();
uint8_t GNSS_getMin();
uint8_t GNSS_getSec();

//Location
float getLat();
float getLong();
int getElevation_M();
uint8_t getHAcc();
uint8_t getGroundSpeed_kph();
uint8_t getMotionHeading_deg();

//Status
uint8_t getNumSatellites();
GNSS_FixType getFixType();
bool isGnssFixOk();
bool isTimeFullyResolved();

void LPUART_CharReception_Callback();

#endif
