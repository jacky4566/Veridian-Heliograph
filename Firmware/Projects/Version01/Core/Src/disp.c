/*
 * disp.c
 *
 *  Created on: Dec 31, 2021
 *      Author: jacks
 */


#include "disp.h"
#include "main.h"
#include "bitmaps.h"
#include "fonts.h"

#define LCDHEIGHTpx		(128)
#define LCD_ROWS		(LCDHEIGHTpx)
#define LCD_ROWS_BYTES	(LCDHEIGHTpx / 8)
#define LCDWIDTHpx		(128)
#define LCD_COLS_32		(LCDWIDTHpx / 32)
#define LCD_COLS_BYTES  (LCDWIDTHpx / 8)

#define USEHARDWAREEXT

#define NUMDisplays 1

/*Commands Defines*/
#define CMD_CLEAR 0x04
#define CMD_WRITE 0x01
#define CMD_TRAIL 0x00

#define CMD_WRITE_BYTES 1
#define CMD_TRAIL_BYTES 1
#define CMD_LINE_BYTES  1

#define SPIBUFFERSIZE (CMD_WRITE_BYTES + ((CMD_LINE_BYTES + LCD_COLS_BYTES + CMD_TRAIL_BYTES) * LCD_ROWS_BYTES) + CMD_TRAIL_BYTES)

/*Private Variable*/
uint32_t lcdShadow[LCD_ROWS][LCD_COLS_32]; 	//Previously on the screen
uint32_t lcdBuffer[LCD_ROWS][LCD_COLS_32]; 	//Currently on the screen
uint8_t spiBuffer[SPIBUFFERSIZE]; 			//Feeds SPI DMA, Maximum size
uint8_t displayCycler = 0;					//Cycles through various displays

/*Private Functions*/
void clearMem( void );
void fillBuffer( void );
void printBitmap( uint16_t, uint16_t, uint8_t* , uint16_t, uint16_t, uint8_t );
void putString( uint16_t, uint16_t, uint8_t *Text, sFONT*, uint8_t );
void plotHLine( uint16_t, uint16_t, uint16_t, uint8_t );
void plotVLine( uint16_t, uint16_t, uint16_t, uint8_t );
void plotCircle( int, int, int, uint8_t);
void setPixel( uint16_t, uint16_t, uint8_t);
void startSPI( uint16_t );

void my_disp_Init(){
	//Start hardware pin wiggler

	//initialize memory sets
	clearMem();
}

void clearMem( void ){
	memset(lcdShadow,0xffffffff,sizeof(lcdShadow));//Clear shadow
	memset(lcdShadow,0xffffffff,sizeof(lcdShadow));//Clear buffer
}

void draw( void ){
	//Draw stuff to lcdBuffer
	switch(displayCycler){
	case 0:

		break;
	default:
		break;
	}
}

void incDispCycler(){
	if (NUMDisplays <= ++displayCycler){
		displayCycler = 0;
	}
}

void printBitmap(uint16_t x, uint16_t y, uint8_t* imgBuff, uint16_t size_x, uint16_t size_y, uint8_t color){
	uint16_t bitCounter = 0;
	for(uint16_t yPtr = 0; (yPtr < size_y) && (yPtr < LCDHEIGHTpx); yPtr++){
		for(uint16_t xPtr = 0; (xPtr < size_x) && (xPtr < LCDWIDTHpx); ){
			setPixel(xPtr, yPtr, imgBuff[bitCounter / 8] & ( 1 << (bitCounter / 8)));
			xPtr++;

			if (!((xPtr < size_x) && (xPtr < LCDWIDTHpx))){//if we are done with the row
				while(!(bitCounter % 8)){ //round up bitCounter to 8 bits
					bitCounter++;
				}
			}
		}
	}
}

void putString(uint16_t x, uint16_t y, uint8_t *text, sFONT *fonts, uint8_t color){
	uint8_t *ptr = text;
	uint32_t i = 0;
	uint32_t size = 0;

	while (*ptr++) size ++ ;

	uint16_t charWidthBytes = ( fonts->Width + 1 ) / 8;   //round up

	while (size--){
		uint8_t asciiuint = (uint8_t)text - (uint8_t)32;
		uint16_t charCol = x + (i * fonts->Width);
		//Check the char fits on screen
		if (((charCol + fonts->Width) > LCDWIDTHpx) && ((y + fonts->Height) > LCDHEIGHTpx)){
			for(uint16_t yPtr = 0; yPtr < fonts->Height; yPtr++){ 						//Iterate Y
				for(uint16_t xBytePtr = 0; xBytePtr < charWidthBytes; xBytePtr++){		//Iterate bytes of X in the table
					uint8_t bitThisByte = MIN(fonts->Width - (xBytePtr * 8), 8);
					for(uint8_t xBitPtr = 0; xBitPtr < bitThisByte; xBitPtr++){  //Iterate bits of the byte
						setPixel(charCol + (xBytePtr * 8) + bitThisByte, y + yPtr, fonts->table[asciiuint] & (1 << xBitPtr));
						asciiuint++;
					}
				}
			}
		}
		text++;
		i++;
	}
}

void plotHLine(uint16_t x, uint16_t y, uint16_t l, uint8_t color){
	//Plot Left to right
	while(l--){
		if ((x + 1 < LCDWIDTHpx)){ //check for out of bounds
			setPixel(x++, y, color);
		}else{
			break;
		}
	}
}

void plotVLine(uint16_t x, uint16_t y, uint16_t l, uint8_t color){
	//Plot top to bottom
	while(l--){
		if ((y + 1 < LCDHEIGHTpx)){ //check for out of bounds
			setPixel(x, y++, color);
		}else{
			break;
		}
	}
}

void plotCircle(int x, int y, int r, uint8_t color){
	  int f = 1 - r;
	  int ddF_x = 1;
	  int ddF_y = -2 * r;
	  int i = 0;
	  int j = r;

	  setPixel(x, y + r,color);
	  setPixel(x, y - r,color);
	  setPixel(x + r, y,color);
	  setPixel(x - r, y,color);

	  while(i < j)
	  {
	    if(f >= 0)
	    {
	      j--;
	      ddF_y += 2;
	      f += ddF_y;
	    }
	    i++;
	    ddF_x += 2;
	    f += ddF_x;
	    setPixel(x + i, y + j,color);
	    setPixel(x - i, y + j,color);
	    setPixel(x + i, y - j,color);
	    setPixel(x - i, y - j,color);
	    setPixel(x + j, y + i,color);
	    setPixel(x - j, y + i,color);
	    setPixel(x + j, y - i,color);
	    setPixel(x - j, y - i,color);
	  }
}

void setPixel(uint16_t x, uint16_t y, uint8_t color)
{
	uint8_t row32 = x / 32;
	uint32_t mask = 0x01 << (x % 32);
	if((x < LCDWIDTHpx) && (y < LCDHEIGHTpx)){
		if(color){
			lcdBuffer[y][row32] = lcdShadow[y][row32] | mask;

		}
		else
		{
			mask = ~mask;
			lcdBuffer[y][row32] = lcdShadow[y][row32] & mask;
		}
	}
}

void fillBuffer(){
	//write command to buffer
	uint16_t spiByteToWrite = 0;
	spiBuffer[spiByteToWrite++] = CMD_WRITE;

	//Inspects lcdShadow and lcdBuffer, copies lcdBuffer to spiBuffer
	for(uint8_t i = 0; i < LCD_ROWS; i++){ //For Each row
		for(uint8_t j = 0; j < LCD_COLS_32; j++){ //for each column
			if (lcdShadow[i][j] != lcdBuffer[i][j]){ //if elements dont match
				//write row to buffer
				//write line number
				spiBuffer[spiByteToWrite++] = i;
				//write data
				for(uint8_t k = 0; k < LCD_COLS_32; k++){
					memcpy(&spiBuffer[spiByteToWrite++], &lcdBuffer[i][k], sizeof(lcdBuffer[i][k]));
				}
				//write trailer
				spiBuffer[spiByteToWrite++] = CMD_TRAIL;
				break;
			}
		}
	}

	//write trail to buffer
	spiByteToWrite = 0;
	spiBuffer[spiByteToWrite] = CMD_TRAIL;

	//Start transfer
	startSPI(spiByteToWrite);

	//Copy buffer to shadow
	memcpy(lcdShadow, &lcdBuffer, sizeof(lcdBuffer));
}

void startSPI(uint16_t spiByteToWrite){
	//If there is enough in the buffer
	if (spiByteToWrite <= (CMD_WRITE_BYTES + CMD_TRAIL_BYTES)){
		return;
	}
}


