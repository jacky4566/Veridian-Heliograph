/*
 * LS013B7DH03.h
 *
 *  Created on: May 16, 2022
 *      Author: Jackson-E15Gen3
 */

#ifndef SHARPMEMLCD_LS013B7DH03_H_
#define SHARPMEMLCD_LS013B7DH03_H_

#include <stdint.h>
#include <stdbool.h>
#include "gfxfont.h"

#define LCD_RES_PX_X (128)
#define LCD_RES_PX_X_b (LCD_RES_PX_X / 8)
#define LCD_RES_PX_Y (128)
#define LCD_RES_PX_Y_b (LCD_RES_PX_Y / 8)

#define LCD_CS_PIN 5		//Replace Later
#define LCD_CS_PORT 5		//Replace Later

enum lcd_State_enum {
	LCD_READY, 		//No more data to send
	LCD_SENDING, 	//SPI, DMA actively working
	SPI_BUSY
};

//State Machine
enum lcd_State_enum lcd_state = LCD_READY;

//Functions
void lcd_clear(void); 		//Clear Display
enum lcd_State_enum lcd_draw(void); 	//Draws lines, Call until return done

void lcd_drawPixel(uint8_t x, uint8_t y, bool bDraw);
void lcd_setRotation(uint8_t newRot);
void lcd_drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void lcd_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void lcd_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void lcd_SetFont(GFXfont *f);
void lcd_SetCursor(uint8_t x, uint8_t y);
void lcd_writeChar(uint8_t c);

#endif /* SHARPMEMLCD_LS013B7DH03_H_ */
