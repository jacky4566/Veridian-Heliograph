/*
 * LS013B7DH03.c
 *
 *  Created on: May 16, 2022
 *      Author: Jackson-E15Gen3
 */

#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "ls013b7dh03.h"
#include "gfxfont.h"

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

//Variables
static uint8_t rotation;
static uint8_t cursor_y;
static uint8_t cursor_x;
static GFXfont *gfxFontPtr;

//Functions
void lcd_SetCursor(uint8_t x, uint8_t y) {
	cursor_x = x;
	cursor_y = y;
}

void lcd_SetFont(GFXfont *f) {
	gfxFontPtr = (GFXfont*) f;
}

void lcd_writeChar(uint8_t c) {
	if (c >= (uint8_t) gfxFontPtr->first && c <= (uint8_t) gfxFontPtr->last) { //Char present in this font?
		c -= (uint8_t) gfxFontPtr->first; //remove first offset
		GFXglyph *glyph = &(gfxFontPtr->glyph)[c]; //get our glyph
		if ((glyph->width > 0) && (glyph->height > 0)) { // Is there an associated bitmap?
			uint8_t xx, yy, bits = 0, bit = 0;
			int8_t xo = glyph->xOffset;
			int8_t yo = glyph->yOffset;
			uint8_t *bitmap = gfxFontPtr->bitmap;
			uint16_t bo = glyph->bitmapOffset;

			for (yy = 0; yy < glyph->height; yy++) {
				for (xx = 0; xx < glyph->width; xx++) {
					if (!(bit++ & 7)) {
						bits = bitmap[bo++];
					}
					if (bits & 0x80) {
						if (glyph->width == 1 && glyph->height == 1) {
							lcd_drawPixel(cursor_x + xo + xx, cursor_y + yo + yy, 0x01);
						}
					}
					bits <<= 1;
				}
			}
		}
		cursor_x += (uint8_t) glyph->xAdvance; //Advance cursor
	}
}

void lcd_drawPixel(uint8_t x, uint8_t y, bool bDraw) {
	if (x >= LCD_RES_PX_X || y >= LCD_RES_PX_Y)
		return;

	uint8_t bitInByte = (0x80 >> (x % 8));
	x = x / 8;
	if (bDraw)
		LCD_BUFFER[x][y] |= bitInByte; //set bit
	else
		LCD_BUFFER[x][y] &= ~bitInByte; //clear bit
}

void lcd_drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
	if (x0 == x1) {
		//Vertical Line
		uint8_t length = y1 - y0;
		for (uint8_t i = 0; i < length; i++) {
			lcd_drawPixel(x0 + i, y0, color);
		}
	} else if (y0 == y1) {
		//Horizontal line
		uint8_t length = x1 - x0;
		for (uint8_t i = 0; i < length; i++) {
			lcd_drawPixel(x0, y0 + 1, color);
		}
	} else {
		//Angled line
	}
}

void lcd_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
	//Draw 4 line!
	lcd_drawLine(x, y, x + w, y, color);			//Bottom
	lcd_drawLine(x, y, x, y + h, color); 			//Left
	lcd_drawLine(x + w, y, x + w, y + h, color); 	//Right
	lcd_drawLine(x, y + h, x + w, y + h, color); 	//Top
}

void lcd_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
	for (uint8_t i = y; i < y + h; i++) {
		lcd_drawLine(x, y + i, x + w, y + i, color);
	}
}

void lcd_setRotation(uint8_t newRot) {
	if (rotation <= 3)
		rotation = newRot;
}

void lcd_SPI1_Init(void) {
	if (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_RESET) {
		Error_Handler();
	}
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; //16MHz HSI / 16DIV = 1MHz SPI
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
}

static void lcd_SPI1_DeInit(void) {
	HAL_SPI_DeInit(&hspi1);
}

void lcd_clear(void) {
	SPIBuffer[0] = MLCD_CM;
	SPIBuffer[1] = MLCD_TR;
	lcd_state = LCD_SENDING;
	lcd_SPI1_Init();
	HAL_GPIO_WritePin(LCD_CS_PIN, LCD_CS_PORT, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, SPIBuffer, 2, 10); //No DMA here
	HAL_GPIO_WritePin(LCD_CS_PIN, LCD_CS_PORT, GPIO_PIN_RESET);
	lcd_SPI1_DeInit();
	lcd_state = LCD_READY;
}

bool lcd_hasData(void) {
	for (uint8_t i = 0; i < sizeof(Line_Tracker); i++) {
		if (Line_Tracker[i] != 0x00) {
			//we have some data to send
			return true;
		}
	}
	return false;
}

static uint8_t getNextLine(void) {
	uint8_t nextLine = 0;
	for (uint8_t i = 0; i < sizeof(Line_Tracker); i++) {
		for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
			if (Line_Tracker[i] & mask) {
				Line_Tracker[i] &= ~mask; //clear bit
				return nextLine;
			}
			nextLine++;
		}
	}
	return 0xff;
}

void lcd_DoTX() {
	if (lcd_hasData()) {
		//start next transfer
		uint8_t nextLine = getNextLine();
		SPIBuffer[0] = nextLine;
		memcpy(&SPIBuffer[1], &LCD_BUFFER[0][nextLine], LCD_RES_PX_X_b); //could maybe swap these commands for speed
		HAL_SPI_Transmit_DMA(&hspi1, SPIBuffer, LCD_RES_PX_X_b + 2);
	} else {
		//Done
		SPIBuffer[0] = MLCD_TR; //Load Trailer command
		HAL_SPI_Transmit(&hspi1, SPIBuffer, 1, 10); //Send Trailer Command, Blocking
		HAL_GPIO_WritePin(LCD_CS_PIN, LCD_CS_PORT, GPIO_PIN_RESET);
		lcd_SPI1_DeInit();
		lcd_state = LCD_READY;
	}
}

enum lcd_State_enum lcd_draw(void) {
	if (lcd_state == LCD_SENDING) {
		//We are waiting for DMA/SPI to finish, call again later
		return LCD_SENDING;
	}
	if (!lcd_hasData()) {
		//No data. still ready to send
		return LCD_READY;
	} else {
		if (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_RESET) {
			//SPI is not ready for us
			return SPI_BUSY;
		}
		//start a new transfer
		lcd_state = LCD_SENDING;
		lcd_SPI1_Init();
		SPIBuffer[0] = MLCD_WR;
		HAL_GPIO_WritePin(LCD_CS_PIN, LCD_CS_PORT, GPIO_PIN_SET);
		HAL_SPI_Transmit(&hspi1, SPIBuffer, 1, 10); //Send Write Command, Blocking
		lcd_DoTX();

	}
	return lcd_state;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	lcd_DoTX();
}
