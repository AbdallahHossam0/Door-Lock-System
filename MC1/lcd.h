/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <lcd.h>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <30 - 10 - 2020>
 *
 * [DESCRIPTION]: source file for LCD
 * ---------------------------------------------------------------------------------------------------
*/

#ifndef LCD_H_
#define LCD_H_

#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"
#include <stdlib.h>

/*
 * choose the bit mode
 * 		make it equal 4 for 4 bit mode
 * 		make it equal 8 for 8 bit mode
 */

#define BIT_MODE 8

/*
 * it works only in the 4 bit mode
 * 		set it to 1 if you want to work with the upper (most) 4 PINs
 * 		Px 4, Px 5, Px 6, Px 7
 *
 * 		clear it to 0 if you want to work with the lower (least) 4 bits
 * 		Px 0, Px 1, Px 2, Px 3
 */
#if BIT_MODE == 4
#define UPPER_PORT_PINS 1
#endif


#define LCD_CTRL_PORT PORTD
#define LCD_CTRL_PORT_DIR DDRD

#define LCD_DATA_PORT PORTC
#define LCD_DATA_PORT_DIR DDRC

#define RS PD4
#define RW PD5
#define E  PD6


#define CLEAR_COMMAND 0x01
#define GO_HOME_FOUR_BITS_DATA_MODE 0x02
#define TWO_LINE_LCD_8_BIT_MODE 0x38
#define TWO_LINE_LCD_4_BIT_MODE 0x28
#define CURSOR_ON 0x0E
#define CURSOR_OFF 0x0C
#define SET_CURSOR_LOCATION 0x80

void LCD_init(void);
void LCD_sendCommand(uint8 a_command);
void LCD_displayCharacter(uint8 a_data);
void LCD_displayString(const uint8* a_string);
void LCD_goToRowColumn(uint8 a_row, uint8 a_col);
void LCD_displayStringRowColumn(uint8 a_row, uint8 a_col, const uint8* a_string);\
void LCD_clearScreen(void);
void LCD_integerToString(uint32 data);


#endif /* LCD_H_ */
