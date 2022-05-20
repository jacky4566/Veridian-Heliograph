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

#define LCD_RES_PX_X (128)
#define LCD_RES_PX_X_b (LCD_RES_PX_X / 8)
#define LCD_RES_PX_Y (128)
#define LCD_RES_PX_Y_b (LCD_RES_PX_Y / 8)

#define LCD_CS_PIN 5		//Replace Later
#define LCD_CS_PORT 5		//Replace Later

enum lcd_State {
	LCD_READY, 		//No more data to send
	LCD_SENDING 	//SPI, DMA actively working
};

void lcd_Init(void); 		//Init Display
void lcd_clear(void); 		//Clear Display
enum lcd_State lcd_draw(void); 		//Draws lines, Call until return done

void lcd_drawpoint(uint16_t x,uint16_t y,bool bDraw);
void lcd_fillRectByXY(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,bool bDraw);
void lcd_fillRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,bool bDraw);

void lcd_print_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void lcd_print_num(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void lcd_print_string(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);

#endif /* SHARPMEMLCD_LS013B7DH03_H_ */
