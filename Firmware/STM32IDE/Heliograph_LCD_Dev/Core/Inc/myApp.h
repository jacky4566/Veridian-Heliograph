/*
 * myApp.h
 *
 *  Created on: May 29, 2022
 *      Author: jacks
 */

#ifndef INC_MYAPP_H_
#define INC_MYAPP_H_

#include <stdbool.h>

//External Variables
extern volatile int16_t superCapmV;
extern volatile int16_t solarmV;
extern volatile int32_t tempC;
extern volatile uint32_t guiTimer;

#define LCD_RATE_FAST (1)
#define LCD_RATE_SLOW (15)
#define VBAT_GNSS_ON   ((int16_t) 3200 )
#define VBAT_GNSS_OFF  ((int16_t) 2800 )
#define VBAT_LCD_FAST  ((int16_t) 2700 )

typedef enum {
	WKUP_CLEAR, WKUP_LPUART, WKUP_RTC
} wakeUpSource;

extern wakeUpSource lastWakeUpSource;

//functions
void myApp_init();
void myApp_loop();
struct tm getTime();
void setTimeGNSS();
void startADC();
enum ADC_State_enum getADCState();
bool vBATOK();

#endif /* INC_MYAPP_H_ */
