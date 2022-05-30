/*
 * LS013B7DH03.h
 *
 *  Created on: May 16, 2022
 *      Author: Jackson-E15Gen3
 */

#ifndef SHARPMEMLCD_LS013B7DH03_H_
#define SHARPMEMLCD_LS013B7DH03_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gfxfont.h"

#define LCD_BLACK (0)
#define LCD_WHITE (1)
#define LCD_RES_PX_X (128)
#define LCD_RES_PX_X_b ((LCD_RES_PX_X / 8) + 2)
#define LCD_RES_PX_Y (128)

typedef enum  {
	LCD_OFF, 		//Disabled
	LCD_READY, 		//No more data to send
	LCD_SENDING 	//SPI, DMA actively working
}lcd_State_enum;

extern char strbuffer[];
extern uint16_t timer;

//Functions
void lcd_init(void);		//Start LCD
void lcd_clear(void); 		//Clear Display
void LCD_Power(); 			//Controls power state
lcd_State_enum lcd_draw(void); 	//Draws lines, Call until return done

void lcd_drawPixel(uint8_t x, uint8_t y, uint8_t bDraw);
void lcd_togglePixel (uint8_t x, uint8_t y);
void lcd_setRotation(uint8_t newRot);
void lcd_drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t colour);
void lcd_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t colour);
void lcd_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t colour);
void lcd_SetFont(GFXfont *f);
void lcd_SetCursor(uint8_t x, uint8_t y);
void lcd_print(int n);
void lcd_print_char(uint8_t);

#endif /* SHARPMEMLCD_LS013B7DH03_H_ */
