/*
 * disp.h
 *
 *  Created on: Dec 31, 2021
 *      Author: jacks
 */

#ifndef INC_DISP_H_
#define INC_DISP_H_

/*Public enum*/
typedef enum
{
  disp_rotate_0,
  disp_rotate_90,
  disp_rotate_180,
  disp_rotate_270
} disp_rotation_enum;

/*Public Functions*/
void my_disp_Init( void );
void draw( void );
void incDispCycler( void );
void setRotation( disp_rotation_enum );

#endif /* INC_DISP_H_ */
