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

enum GNSS_FixType{
	NO_FIX = 0, DEAD_RECKONING, FIX_2D, FIX_3D, GNSS_AND_DEAD_RECKONING, TIME_ONLY
};

enum GNSS_rate{
	GNSS_UINT, GNSS_STOP, GNSS_SLOW, GNSS_FAST
};

extern volatile uint32_t GNSSlastPacketAge;
extern volatile bool GNSSAlive;
extern volatile bool GNSSNewData;
extern volatile enum GNSS_rate GNSSlastRate;
extern volatile struct locationFix LastFix;

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
float getHAcc();
int getGroundSpeed_kph();
int getMotionHeading_deg();

//Status
uint8_t getNumSatellites();
enum GNSS_FixType getFixType();
bool isGnssFixOk();
bool isTimeFullyResolved();

void LPUART_CharReception_Callback();

#endif
