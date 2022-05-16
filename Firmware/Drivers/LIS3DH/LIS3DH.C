/*
 * LIS3DH.C
 *
 *  Created on: May 16, 2022
 *      Author: Jackson-E15Gen3
 */

#include "main.h"
#include "lis3dh.h"
#include <stdbool.h>

//Handles
extern SPI_HandleTypeDef hspi1; //Get the SPI handle from main

//buffers
byte txBuffer[6];
byte rxBuffer[6];

//Defines
#define LIS3DH_READ_BIT          0x80
#define LIS3DH_MS_BIT            0x40
#define LIS3DH_ID                0x33

#define LIS3DH_REG_STATUS_AUX    0x07
#define LIS3DH_REG_OUT_ADC1_L    0x08
#define LIS3DH_REG_OUT_ADC1_H    0x09
#define LIS3DH_REG_OUT_ADC2_L    0x0a
#define LIS3DH_REG_OUT_ADC2_H    0x0b
#define LIS3DH_REG_OUT_ADC3_L    0x0c
#define LIS3DH_REG_OUT_ADC3_H    0x0d
#define LIS3DH_REG_INT_COUNTER   0x0e
#define LIS3DH_REG_WHO_AM_I      0x0f
#define LIS3DH_REG_TEMP_CFG      0x1f
#define LIS3DH_REG_CTRL1         0x20
#define LIS3DH_REG_CTRL2         0x21
#define LIS3DH_REG_CTRL3         0x22
#define LIS3DH_REG_CTRL4         0x23
#define LIS3DH_REG_CTRL5         0x24
#define LIS3DH_REG_CTRL6         0x25
#define LIS3DH_REG_REFERENCE     0x26
#define LIS3DH_REG_STATUS        0x27
#define LIS3DH_REG_OUT_X_L       0x28
#define LIS3DH_REG_OUT_X_H       0x29
#define LIS3DH_REG_OUT_Y_L       0x2a
#define LIS3DH_REG_OUT_Y_H       0x2b
#define LIS3DH_REG_OUT_Z_L       0x2c
#define LIS3DH_REG_OUT_Z_H       0x2d
#define LIS3DH_REG_FIFO_CTRL     0x2e
#define LIS3DH_REG_FIFO_SRC      0x2f
#define LIS3DH_REG_INT1_CFG      0x30
#define LIS3DH_REG_INT1_SRC      0x31
#define LIS3DH_REG_INT1_THS      0x32
#define LIS3DH_REG_INT1_DUR      0x33
#define LIS3DH_REG_INT2_CFG      0x34
#define LIS3DH_REG_INT2_SRC      0x35
#define LIS3DH_REG_INT2_THS      0x36
#define LIS3DH_REG_INT2_DUR      0x37
#define LIS3DH_REG_CLICK_CFG     0x38
#define LIS3DH_REG_CLICK_SRC     0x39
#define LIS3DH_REG_CLICK_THS     0x3a
#define LIS3DH_REG_TIME_LIMIT    0x3b
#define LIS3DH_REG_TIME_LATENCY  0x3c
#define LIS3DH_REG_TIME_WINDOW   0x3d

//Internal functions
bool lis3dh_start() {
	txBuffer[0] = LIS3DH_REG_WHO_AM_I | LIS3DH_READ_BIT;
	HAL_SPI_Transmit(&hspi1, txBuffer, 1, TIMEOUTVALUE);
	HAL_SPI_Receive(&hspi1, rxBuffer, 1, TIMEOUTVALUE);
	if (rxBuffer[0] != LIS3DH_ID) {
		return false;
	}

	//Setup
	txBuffer[0] = LIS3DH_REG_CTRL1 | LIS3DH_MS_BIT; //start at REG1
	txBuffer[1] = 0x57; 			//REG1: 100Hz, Normal, ZXY
	txBuffer[2] = 0x00;				//REG2:
	txBuffer[3] = 0x83;				//REG3: CLICK, FIFO watermark, FIFO OVER
	txBuffer[4] = 0x88;				//REG4: BLOCK, 2G, HIGH RES
	HAL_SPI_Transmit(&hspi1, txBuffer, 5, TIMEOUTVALUE);
}

void lis3dh_stop() {
	txBuffer[0] = LIS3DH_REG_CTRL1 | LIS3DH_MS_BIT;
	txBuffer[1] = 0x00; //Stop ODR, disable axis
	HAL_SPI_Transmit(&hspi1, txBuffer, 2, TIMEOUTVALUE);
}

uint8_t lis3dh_get_oreo() {
	txBuffer[0] = LIS3DH_REG_OUT_X_L | LIS3DH_READ_BIT | LIS3DH_MS_BIT;
	HAL_SPI_Transmit(&hspi1, txBuffer, 1, TIMEOUTVALUE);
	HAL_SPI_Receive(&hspi1, rxBuffer, 6, TIMEOUTVALUE);
	lis3dh_axis_t temp;
	temp.LIS3DH_AXIS_X = (int16_t) rxBuffer[0] | ((uint16_t) rxBuffer[1] << 8);
	temp.LIS3DH_AXIS_Y = (int16_t) rxBuffer[0] | ((uint16_t) rxBuffer[1] << 8);
	temp.LIS3DH_AXIS_Z = (int16_t) rxBuffer[0] | ((uint16_t) rxBuffer[1] << 8);
	if (abs(LIS3DH_AXIS_X) > abs(LIS3DH_AXIS_Y)){
		if (LIS3DH_AXIS_X >= 0){
			return 1;
		}else{
			return 3;
		}
	}else{
		if (LIS3DH_AXIS_Y >= 0){
			return 2;
		}else{
			return 4;
		}
	}

}

uint8_t lis3dh_get_click() {

}

