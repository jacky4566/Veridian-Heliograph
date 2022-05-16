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

//Handles
extern SPI_HandleTypeDef hspi1; //Get the SPI handle from main

//Local Functions
uint8_t getVcom(void);

//Commands
#define MLCD_WR 0x80 	//MLCD write line command
#define MLCD_CM 0x20 	//MLCD clear memory command
#define MLCD_TR 0x00 	//MLCD trailer

//Buffers
static uint8_t LCD_BUFFER[LCD_RES_PX_X / 8][LCD_RES_PX_Y];
static bool Line_Tracker[LCD_RES_PX_Y]; //Used to keep track of lines to update
static uint8_t SPIBUFFER[(((LCD_RES_PX_X / 8) + 2) * LCD_RES_PX_Y) + 2]; //Currently use 1 BIG buffer. Per line callbacks might be better

void lcd_clear(void) {
	SPIBUFFER[0] = MLCD_CM;
	SPIBUFFER[1] = MLCD_TR;
	HAL_GPIO_WritePin(LCD_CS_PIN, LCD_CS_PORT, GPIO_PIN_SET);
	HAL_SPI_Transmit_DMA(&hspi1, SPIBUFFER, 2);
}

HAL_SPI_StateTypeDef lcd_busy(void) {
	return HAL_SPI_GetState(&hspi1);
}

void lcd_draw(void) {
	uint16_t bytesToWrite = 0;
	SPIBUFFER[bytesToWrite++] = MLCD_WR;
	for (uint16_t i = 0; i < sizeof(Line_Tracker); i++) { //Check for updates on line#
		if (Line_Tracker[i]) { //This line needs updating
			SPIBUFFER[bytesToWrite++] = i; //Add line number to buffer
			for (uint16_t j = 0; j < LCD_RES_PX_X / 8; j++) { //add row data
				SPIBUFFER[bytesToWrite++] = LCD_BUFFER[j][i];
			}
			SPIBUFFER[bytesToWrite++] = MLCD_TR; //Add trailer to buffer
			Line_Tracker[i] = false; //line was updated
		}
	}
	SPIBUFFER[bytesToWrite++] = MLCD_TR; //Add trailer to buffer
	if (bytesToWrite == 2) { //nothing to print
		return;
	}
	HAL_GPIO_WritePin(LCD_CS_PIN, LCD_CS_PORT, GPIO_PIN_SET);
	HAL_SPI_Transmit_DMA(&hspi1, SPIBUFFER, bytesToWrite);
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

void lcd_fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool bDraw) {
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
