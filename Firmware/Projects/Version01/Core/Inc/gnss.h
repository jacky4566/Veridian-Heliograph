/*
 * gnss.h
 *
 *  Created on: Dec 28, 2021
 *      Author: jacks
 */

#ifndef INC_GNSS_H_
#define INC_GNSS_H_

//includes
#include "main.h"

//External Enum
typedef enum
{
  gnss_rate_fast,
  gnss_rate_slow,
  gnss_rate_stop
} gnss_power_req_t;

typedef enum
{
  FIX_NONE = 0,
  FIX_DEAD_RECKONING_ONLY = 1,
  FIX_2D = 2,
  FIX_3D = 3,
  FIX_GNSS_DEAD_RECKONING = 4,
  FIX_TIME_ONLY = 5
} FixType;

typedef enum
{
  gnss_wakeUp_none,
  gnss_wakeUp_LPUART,
  gnss_wakeUp_OTHER,
} gnss_wakeup_enum;

//External functions
void gnss_Init( void );
void gnss_power_req ( gnss_power_req_t );
gnss_wakeup_enum getGNSSwakeup(void);
void setGNSSwakeup( gnss_wakeup_enum );

uint16_t gnss_year();
uint8_t gnss_month();
uint8_t gnss_day();
uint8_t gnss_hour();
uint8_t gnss_min();
uint8_t gnss_sec();
int32_t gnss_nano_sec();
FixType gnss_fix();
uint8_t gnss_flags1();
uint8_t gnss_num_satellites();
int32_t gnss_height();
uint32_t gnss_heightAcc();
int32_t gnss_gSpeed();
int32_t gnss_heading();
int16_t gnss_magDec();

#endif /* INC_GNSS_H_ */
