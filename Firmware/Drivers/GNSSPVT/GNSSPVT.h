#ifndef GNSSPVT_H_
#define GNSSPVT_H_

/*
 *  GNSSPVT
 *
 *  Created on: May 20, 2022
 *  Author: Jackson-E15Gen3
 */

#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum  {
	NO_FIX = 0,
	DEAD_RECKONING,
	FIX_2D,
	FIX_3D,
	GNSS_AND_DEAD_RECKONING,
	TIME_ONLY
} GNSS_FixType;

typedef enum {
	GNSS_STOP, GNSS_SLOW, GNSS_FAST, GNSS_FULL
} GNSS_rate;

#define GNSS_EX_PIN 5		//Replace Later
#define GNSS_EX_PORT 5		//Replace Later

//Public Functions
void GNSS_Setup(GNSS_rate);

//Time
uint16_t GNSS_getYear();
uint8_t GNSS_getMonth();
uint8_t GNSS_getDay();
uint8_t GNSS_getHour();
uint8_t GNSS_getMin();
uint8_t GNSS_getSec();

//Location
double getLongitude_deg();
double getLatitude_deg();
double getMSLHeight_m();
double getHorizontalAccuracy_m();
double getGroundSpeed_kph();
double getMotionHeading_deg();

//Status
uint8_t getNumSatellites();
double getHorizontalAccuracy_m();
GNSS_FixType getFixType();
bool isGnssFixOk();
bool isTimeFullyResolved();

#endif
