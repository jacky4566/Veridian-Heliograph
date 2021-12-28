/*
 * gnss.h
 *
 *  Created on: Dec 28, 2021
 *      Author: jacks
 */

#ifndef INC_GNSS_H_
#define INC_GNSS_H_

//External Enum
typedef enum
{
  gnss_rate_fast,
  gnss_rate_slow,
  gnss_rate_stop
} gnss_power_req_t;

//External functions
void gnss_Init( void );
void gnss_power_req ( gnss_power_req_t );

#endif /* INC_GNSS_H_ */
