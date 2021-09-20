/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <lcd.c>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <30 - 10 - 2020>
 *
 * [DESCRIPTION]: source file for LCD
 * ---------------------------------------------------------------------------------------------------
*/

#include "lcd.h"

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]: LCD_init
 *
 * [DESCRIPTION]: Function to initialize LCD
 *
 * [Args]:	None
 *
 * [IN]:	None
 *
 * [OUT]:	None
 *
 * [IN / OUT]:	None
 *
 * [RETURNS]:	None
 * ---------------------------------------------------------------------------------------------------
 */


void LCD_init(void){
	/*
	* 	Make CTRL pins work as output pins
	*/
	LCD_CTRL_PORT_DIR |= (1 << RS) | (1 << E) | (1 << RW);
	
	#if BIT_MODE == 4
		#if (UPPER_PORT_PINS == 1)
			LCD_DATA_PORT_DIR |= 0xf0;
		#else
			LCD_DATA_PORT_DIR |= 0x0f;
		#endif
			LCD_sendCommand(GO_HOME_FOUR_BITS_DATA_MODE);
			LCD_sendCommand(TWO_LINE_LCD_4_BIT_MODE);
	#elif BIT_MODE == 8
		LCD_DATA_PORT_DIR = 0xff;
		LCD_sendCommand(TWO_LINE_LCD_8_BIT_MODE);
	#endif
		LCD_sendCommand(CURSOR_OFF);
		LCD_sendCommand(CLEAR_COMMAND);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	LCD_sendCommand
 *
 * [DESCRIPTION]: function to send command to the LCD
 *
 * [Args]:
 *
 * [IN]:	a_command includes the command
 *
 * [OUT]:	None
 *
 * [IN / OUT]:	None
 *
 * [RETURNS]:	None
 * ---------------------------------------------------------------------------------------------------
 */

void LCD_sendCommand(uint8 a_command){
	CLEAR_BIT(LCD_CTRL_PORT, RS);
	CLEAR_BIT(LCD_CTRL_PORT, RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT, E);
	_delay_ms(1);
	#if BIT_MODE == 4
		#if UPPER_PORT_PINS == 1
			LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f) | (a_command & 0xf0);
		#else
			LCD_DATA_PORT = (LCD_DATA_PORT & 0xf0) | ((a_command & 0xf0) >> 4);
		#endif
		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT, E);
		_delay_ms(1);
		SET_BIT(LCD_CTRL_PORT, E);
		_delay_ms(1);
		#if UPPER_PORT_PINS == 1
			LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f) | ((a_command & 0x0f) << 4);
		#else
			LCD_DATA_PORT = (LCD_DATA_PORT & 0xf0) | (a_command & 0x0f);
		#endif
		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT, E);
		_delay_ms(1);
	#elif (BIT_MODE == 8)
		LCD_DATA_PORT = a_command;
		_delay_us(1000);
		CLEAR_BIT(LCD_CTRL_PORT, E);
		_delay_ms(1);
	#endif
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	LCD_displayCharacter
 *
 * [DESCRIPTION]:	Function to Display character on the LCD
 *
 * [Args]:
 *
 * [IN]:	The ASCII of the Character you want to display it on the LCD
 *
 * [OUT]:	None
 *
 * [IN / OUT]:	None
 *
 * [RETURNS]:	None
 * ---------------------------------------------------------------------------------------------------
 */

void LCD_displayCharacter(uint8 a_data){
	SET_BIT(LCD_CTRL_PORT, RS);
	CLEAR_BIT(LCD_CTRL_PORT, RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT, E);
	_delay_ms(1);
	#if BIT_MODE == 4
		#if UPPER_PORT_PINS == 1
			LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f) | (a_data & 0xf0);
		#else
			LCD_DATA_PORT = (LCD_DATA_PORT & 0xf0) | ((a_data & 0xf0) >> 4);
		#endif
		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT, E);
		_delay_ms(1);
		SET_BIT(LCD_CTRL_PORT, E);
		_delay_ms(1);
		#if UPPER_PORT_PINS == 1
			LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f) | ((a_data & 0x0f) << 4);
		#else
			LCD_DATA_PORT = (LCD_DATA_PORT & 0xf0) | (a_data & 0x0f);
		#endif
		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT, E);
		_delay_ms(1);
	#elif BIT_MODE == 8
		LCD_DATA_PORT = a_data;
		_delay_us(1000);
		CLEAR_BIT(LCD_CTRL_PORT, E);
		_delay_ms(1);
	#endif
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]: LCD_displayString
 *
 * [DESCRIPTION]:	Function to Print string(more than) on the LCD
 *
 * [Args]:
 *
 * [IN]:	string(Array of characters)
 *
 * [OUT]:	None
 *
 * [IN / OUT]:	None
 *
 * [RETURNS]:	None
 * ---------------------------------------------------------------------------------------------------
 */


void LCD_displayString(const uint8* a_string){
	for(; *a_string != '\0'; a_string++){
		LCD_displayCharacter(*a_string);
	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	LCD_goToRowColumn
 *
 * [DESCRIPTION]:	Function to set the cursor in a specific location
 *
 * [Args]:
 *
 * [IN]: a_rw => 	the row of the cursor
 * 		a_column=> 	the column of the cursor
 *
 * [OUT]:		None
 *
 * [IN / OUT]:	None
 *
 * [RETURNS]:	None
 * ---------------------------------------------------------------------------------------------------
 */
void LCD_goToRowColumn(uint8 a_row, uint8 a_col){
	uint8 address;
	switch(a_row){
	case 0:
		address = a_col;
		break;
	case 1:
		address = a_col + 0x40;
		break;
	case 2:
		address = a_col + 0x10;
		break;
	case 3:
		address = a_col + 0x50;
		break;
	}

	LCD_sendCommand(address | SET_CURSOR_LOCATION);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	LCD_displayStringRowColumn
 *
 * [DESCRIPTION]:	Function to set the cursor in a specific location and start writing a string
 *
 * [Args]:
 *
 * [IN]: a_rw => 	the row of the cursor
 * 		a_column=> 	the column of the cursor
 * 		a_string => the string
 *
 * [OUT]:		None
 *
 * [IN / OUT]:	None
 *
 * [RETURNS]:	None
 * ---------------------------------------------------------------------------------------------------
 */
void LCD_displayStringRowColumn(uint8 a_row, uint8 a_col, const uint8* a_string){
	LCD_goToRowColumn(a_row, a_col);
	LCD_displayString(a_string);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:		LCD_clearScreen
 *
 * [DESCRIPTION]:		Function to clear the screen
 *
 * [Args]:
 *
 * [IN]:				None
 *
 * [OUT]:				None
 *
 * [IN / OUT]:			None
 *
 * [RETURNS]:			None
 * ---------------------------------------------------------------------------------------------------
 */

void LCD_clearScreen(void){
	LCD_sendCommand(CLEAR_COMMAND);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:		LCD_integarToString
 *
 * [DESCRIPTION]:		Function to display a decimal value on the LCD
 *
 * [Args]:
 *
 * [IN]:				The decimal value
 *
 * [OUT]:				None
 *
 * [IN / OUT]:			None
 *
 * [RETURNS]:			None
 * ---------------------------------------------------------------------------------------------------
 */

void LCD_integerToString(uint32 data){
	uint8 buffer[16];
	itoa(data, buffer, 10);
	LCD_displayString(buffer);
}





