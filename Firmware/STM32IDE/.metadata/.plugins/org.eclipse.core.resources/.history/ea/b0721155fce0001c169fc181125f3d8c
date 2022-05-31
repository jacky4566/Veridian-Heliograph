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
extern volatile int16_t tempC;

//functions
void myApp_init();
void myApp_loop();
uint32_t getUnix();
void startADC();
enum ADC_State_enum getADCState();
bool vBATOK();

#endif /* INC_MYAPP_H_ */
