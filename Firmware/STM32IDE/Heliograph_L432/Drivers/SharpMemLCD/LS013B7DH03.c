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
#include "myApp.h"
#include "ls013b7dh03.h"
#include "gfxfont.h"

//Handles
extern SPI_HandleTypeDef hspi1;

//Commands
static const uint8_t MLCD_WR = 0x01; //MLCD write line command
static const uint8_t MLCD_CM = 0x04; //MLCD clear memory command
static const uint8_t MLCD_TR = 0x00; //MLCD trailer

//Buffers
static uint8_t LCD_BUFFER[LCD_RES_PX_Y][LCD_RES_PX_X_b];
char strbuffer[strbufferSize];

//Variables
static uint8_t rotation;
static uint16_t cursor_y;
static uint16_t cursor_x;
static GFXfont *gfxFontPtr;

//State Machine
volatile lcd_State_enum lcd_state = LCD_OFF;

//Internal Function declarations
static void lcd_DoTX();
static void lcd_clear(void); 			//Clear Display
static bool lcd_hasData();
static uint8_t lcd_writeChar(uint8_t x, uint8_t y, uint8_t c);

//Functions
lcd_State_enum LCD_Power() {
	switch (lcd_state) {
	case LCD_OFF:
		if (superCapmV >= mV_LCD_SLOW) {
			//Turn on LCD
			lcd_state = LCD_READY;
			//HAL_LPTIM_PWM_Start(&hlptim1, 2047, 1023); //32768 DIV16 DIV2048 1HZ
			HAL_GPIO_WritePin(DISP_EN_GPIO_Port, DISP_EN_Pin, GPIO_PIN_SET);
			lcd_clear();
		}
		break;
	case LCD_TIMER:
		if (superCapmV < mV_LCD_OFF) {
			lcd_state = LCD_OFF;
			HAL_GPIO_WritePin(DISP_EN_GPIO_Port, DISP_EN_Pin, GPIO_PIN_RESET);
		} else if ((superCapmV >= mV_LCD_FAST) && (guiTimer >= LCD_RATE_FAST)) {
			lcd_state = LCD_READY;
		} else if (guiTimer >= LCD_RATE_SLOW) {
			lcd_state = LCD_READY;
		}
		break;
	case LCD_READY:
		if (superCapmV < mV_LCD_OFF) {
			lcd_state = LCD_OFF;
			HAL_GPIO_WritePin(DISP_EN_GPIO_Port, DISP_EN_Pin, GPIO_PIN_RESET);
		}
		break;
	case LCD_SENDING_DATA:
	case LCD_SENDING_CLR:
		break;
	}
	return lcd_state;
}

void lcd_SetCursor(uint8_t x, uint8_t y) {
	cursor_x = x;
	cursor_y = y;
}

void lcd_SetFont(GFXfont *f) {
	gfxFontPtr = (GFXfont*) f;
}

uint8_t lcd_writeChar(uint8_t x, uint8_t y, uint8_t c) {
	if (c >= (uint8_t) gfxFontPtr->first && c <= (uint8_t) gfxFontPtr->last) { //Char present in this font?
		c -= (uint8_t) (gfxFontPtr->first);
		GFXglyph glyph = gfxFontPtr->glyph[c];
		uint8_t *bitmap = gfxFontPtr->bitmap;

		uint16_t bo = glyph.bitmapOffset;
		uint8_t w = glyph.width, h = glyph.height;
		int8_t xo = glyph.xOffset, yo = glyph.yOffset;
		uint8_t xx, yy, bits = 0, bit = 0;

		for (yy = 0; yy < h; yy++) {
			for (xx = 0; xx < w; xx++) {
				if (!(bit++ & 7)) {
					bits = bitmap[bo++];
				}
				if (bits & 0x80) {
					lcd_drawPixel(x + xo + xx, y + yo + yy, LCD_BLACK);
				}
				bits <<= 1;
			}
		}
		return (uint8_t) glyph.xAdvance; //Advance cursor
	}
	return 0;
}

void lcd_print(int n) {
	uint8_t bufPointer = 0;
	while (bufPointer < n) {
		cursor_x += lcd_writeChar(cursor_x, cursor_y, strbuffer[bufPointer++]);
	}
}

void lcd_print_char(uint8_t theChar) {
	cursor_x += lcd_writeChar(cursor_x, cursor_y, theChar);
}

void lcd_drawPixel(int16_t x, int16_t y, uint8_t bDraw) {
	if (x < 0 || y < 0 || x >= LCD_RES_PX_X || y >= LCD_RES_PX_Y)
		return;

	uint8_t XbitInByte = (0x01 << (x % 8));
	uint8_t xx = (x / 8) + 1; //X byte in array
	if (bDraw == LCD_WHITE) { //Set bit
		LCD_BUFFER[y][xx] |= XbitInByte; 	//set bit
	} else {
		LCD_BUFFER[y][xx] &= ~XbitInByte; //clear bit
	}
}

void lcd_drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
	if (x0 == x1) {
		//Vertical Line
		for (uint8_t i = y0; i <= y1; i++) {
			lcd_drawPixel(x0, i, color);
		}
	} else if (y0 == y1) {
		//Horizontal line
		for (uint8_t i = x0; i <= x1; i++) {
			lcd_drawPixel(i, y0, color);
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
	for (uint8_t i = 0; i < h; i++) {
		lcd_drawLine(x, y + i, x + w, y + i, color);
	}
}

void lcd_clearLines(uint8_t y0, uint8_t y1) {
	//Clear lines for writing
	while (y0 <= y1) {
		for (uint8_t xByte = 1; xByte < (LCD_RES_PX_X_b - 1); xByte++) {
			LCD_BUFFER[y0][xByte] = 0xff; 	//Clear line
		}
		LCD_BUFFER[y0][LCD_RES_PX_X_b - 1] = 0x00; 		//transmit this line
		y0++;
	}
}

void lcd_setRotation(uint8_t newRot) {
	if (rotation <= 3)
		rotation = newRot;
}

void lcd_clear(void) {
	uint8_t clearBuffer[] = { MLCD_CM, MLCD_TR };
	lcd_state = LCD_SENDING_CLR;
	HAL_GPIO_WritePin(DISP_CS_GPIO_Port, DISP_CS_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*) &clearBuffer, sizeof(clearBuffer));

	//Clear buffer
	for (uint8_t y = 0; y < LCD_RES_PX_Y; y++) {
		for (uint8_t x = 0; x < LCD_RES_PX_X_b; x++) {
			if (x == 0) {
				LCD_BUFFER[y][x] = y + 1; //assign line number
			} else if (x == (LCD_RES_PX_X_b - 1)) {
				LCD_BUFFER[y][x] = 0x00; //trailer with transmit flag
			} else {
				LCD_BUFFER[y][x] = 0xff; //white data
			}
		}
	}
}

lcd_State_enum lcd_draw(void) {
	if (lcd_state == LCD_OFF) {
		return lcd_state;
	}
	if (lcd_state == LCD_READY && lcd_hasData()) {
		//start new transfer
		lcd_state = LCD_SENDING_DATA;
		HAL_GPIO_WritePin(DISP_CS_GPIO_Port, DISP_CS_Pin, GPIO_PIN_SET);
		HAL_SPI_Transmit(&hspi1, (uint8_t*) &MLCD_WR, sizeof(MLCD_WR), HAL_MAX_DELAY);
		lcd_DoTX();
	}
	return lcd_state;
}

static bool lcd_hasData() {
	for (uint8_t y = 0; y < LCD_RES_PX_Y; y++) {
		if (LCD_BUFFER[y][LCD_RES_PX_X_b - 1] == 0x00) {
			//we have some data to send
			return true;
		}
	}
	return false;
}

static void lcd_DoTX() {
	static uint8_t linePointer = 0;
	if (lcd_hasData()) {
		//start next transfer
		while (1) {
			if (LCD_BUFFER[linePointer][LCD_RES_PX_X_b - 1] == 0x00) {
				//Send the line
				HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*) &LCD_BUFFER[linePointer][0], LCD_RES_PX_X_b);
				LCD_BUFFER[linePointer][LCD_RES_PX_X_b - 1] = 0x01; //Line Sent
				return;
			}
			if (++linePointer >= LCD_RES_PX_Y) {
				linePointer = 0;
			}
		}
	} else {
		//Done
		HAL_SPI_Transmit(&hspi1, (uint8_t*) &MLCD_TR, sizeof(MLCD_TR), HAL_MAX_DELAY); //send Trailer command
		HAL_GPIO_WritePin(DISP_CS_GPIO_Port, DISP_CS_Pin, GPIO_PIN_RESET);
		guiTimer = 0; //drawing done
		lcd_state = LCD_TIMER; //Enter timer mode for power() to clear
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	if (lcd_state == LCD_SENDING_DATA) {
		lcd_DoTX();
	} else if (lcd_state == LCD_SENDING_CLR) {
		HAL_GPIO_WritePin(DISP_CS_GPIO_Port, DISP_CS_Pin, GPIO_PIN_RESET);
		lcd_state = LCD_READY;
	}
}
