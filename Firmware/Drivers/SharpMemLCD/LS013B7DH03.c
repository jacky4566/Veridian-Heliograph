/*
 * LS013B7DH03.c
 *
 *  Created on: May 16, 2022
 *      Author: Jackson-E15Gen3
 */

#include "main.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "ls013b7dh03.h"
#include "Fonts.h"

//State Machine
enum LCD_State interal_state = LCD_DONE;

//Handles
extern SPI_HandleTypeDef hspi1; //Get the SPI handle from main

//Commands
#define MLCD_WR 0x80 	//MLCD write line command
#define MLCD_CM 0x20 	//MLCD clear memory command
#define MLCD_TR 0x00 	//MLCD trailer

//Buffers
static uint8_t LCD_BUFFER[LCD_RES_PX_X_b][LCD_RES_PX_Y];
static uint8_t Line_Tracker[LCD_RES_PX_Y_b]; //Used to keep track of lines to update
static uint8_t SPIBuffer[LCD_RES_PX_X_b + 2];

bool lcd_hasData() {
	for (uint8_t i = 0; i < sizeof(Line_Tracker); i++) {
		if (Line_Tracker[i] != 0x00) {
			//we have some data to send
			return true;
		}
	}
	return false;
}

uint16_t getNextLine() {
	uint16_t nextLine = 0;
	for (uint8_t i = 0; i < sizeof(Line_Tracker); i++) {
		for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
			if (Line_Tracker[i] & mask) {
				return nextLine;
			}
			nextLine++;
		}
	}
	return 0xffff;
}

enum LCD_State lcd_draw(void) {
	switch (interal_state) {
	case LCD_SENDING:
		//We are waiting for DMA/SPI to finish, call again later
		break;
	case LCD_READY:
		if (!lcd_hasData()) {
			//No data to send, DONE
			interal_state = LCD_DONE;
			//de-initialize SPI
			HAL_GPIO_WritePin(LCD_CS_PIN, LCD_CS_PORT, GPIO_PIN_RESET);
			break;
		}
		//start a new transfer
		interal_state = LCD_SENDING;
		SPIBuffer[0] = MLCD_WR;
		memcpy();
		HAL_SPI_Transmit_DMA(&hspi1, SPIBuffer, LCD_RES_PX_X_b + 2);
		break;
	case LCD_DONE:
		if (lcd_hasData()) {
			//We have data to send
			interal_state = LCD_READY;
			//initialize SPI
			HAL_GPIO_WritePin(LCD_CS_PIN, LCD_CS_PORT, GPIO_PIN_SET);
		}
		break;
	default:
		break;
	}
	return interal_state;
}

void lcd_clear(void) {
	SPIBuffer[0] = MLCD_CM;
	SPIBuffer[1] = MLCD_TR;
	HAL_GPIO_WritePin(LCD_CS_PIN, LCD_CS_PORT, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, SPIBuffer, 2, 10); //No DMA here
	HAL_GPIO_WritePin(LCD_CS_PIN, LCD_CS_PORT, GPIO_PIN_RESET);
}

void lcd_drawpoint(uint16_t x, uint16_t y, bool bDraw) {
	if (x > LCD_RES_PX_X - 1 || y > LCD_RES_PX_Y - 1)
		return;

	uint8_t bitInByte = x % 8;
	x = x / 8;
	if (bDraw)
		LCD_BUFFER[x][y] |= (1 << bitInByte); //set bit
	else
		LCD_BUFFER[x][y] &= ~(1 << bitInByte); //clear bit

}

void lcd_fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
bool bDraw) {
	uint16_t wi, hi;
	for (hi = 0; hi < h; hi++) {
		for (wi = 0; wi < w; wi++)
			lcd_drawpoint(x + wi, y + hi, bDraw);
	}
}

void lcd_fillRectByXY(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
bool bDraw) {
	uint16_t xStart = 0, yStart = 0;
	uint16_t w, h;

	if (x0 < x1) {
		xStart = x0;
		w = x1 - x0 + 1;
	} else {
		xStart = x1;
		w = x0 - x1 + 1;
	}

	if (y0 < y1) {
		yStart = y0;
		h = y1 - y0 + 1;
	} else {
		yStart = y1;
		h = y0 - y1 + 1;
	}
	lcd_fillRect(xStart, yStart, w, h, bDraw);
}

void lcd_print_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size,
		uint8_t mode) {
	uint8_t temp, t, t1;
	uint8_t y0 = y;
	uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
	chr = chr - ' ';
	for (t = 0; t < csize; t++) {
		if (size == 12)
			temp = asc2_1206[chr][t];
		else if (size == 16)
			temp = asc2_1608[chr][t];
		else if (size == 24)
			temp = asc2_2412[chr][t];
		else
			return;
		for (t1 = 0; t1 < 8; t1++) {
			if (temp & 0x80)
				lcd_drawpoint(x, y, mode);
			else
				lcd_drawpoint(x, y, !mode);
			temp <<= 1;
			y++;
			if ((y - y0) == size) {
				y = y0;
				x++;
				break;
			}
		}
	}
}

void lcd_print_string(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size) {
	while ((*p <= '~') && (*p >= ' ')) {
		if (x > (128 - (size / 2))) {
			x = 0;
			y += size;
		}
		if (y > (128 - size)) {
			y = x = 0;
			lcd_clear();
		}
		lcd_print_char(x, y, *p, size, 1);
		x += size / 2;
		p++;
	}

}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	interal_state = LCD_READY; //Transfer done, ready for next call
}
