/*
 * LIS3DH.h
 *
 *  Created on: May 16, 2022
 *      Author: Jackson-E15Gen3
 */

#ifndef LIS3DH_LIS3DH_H_
#define LIS3DH_LIS3DH_H_

#include <stdbool.h>

struct lis3dh_axis_t {
	int16_t LIS3DH_AXIS_X;
	int16_t LIS3DH_AXIS_Y;
	int16_t LIS3DH_AXIS_Z;
};

bool lis3dh_start();
void lis3dh_stop();
uint8_t lis3dh_get_oreo();
uint8_t lis3dh_get_click();

#endif /* LIS3DH_LIS3DH_H_ */
