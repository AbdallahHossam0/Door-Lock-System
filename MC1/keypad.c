/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <keyPad.c>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <30 - 10 -2020>
 *
 * [DESCRIPTION]: <Source file for keypad driver>
 * ---------------------------------------------------------------------------------------------------
 */
#include "keypad.h"
//include the header file of keypad

#if (COLUMNS == 3)
static uint8 keypad_4x3_adjustSwitchNumber(uint8 a_buttonNumber);
/*
 * function responsible for mapping the switch number in the keypad
 * it works if the keypad dimensions is 4x3
 */
#elif COLUMNS == 4
#if calc == 0
/*
 * function responsible for mapping switch number in the keypad
 * it works if the keypad is 4x4
 * it is for normal 4x4 keypads
 * 		1	2	3	A
 * 		4	5	6	B
 * 		7	8	9	C
 * 		*	0	#	D
 */
static uint8 keypad_4x4_adjustSwitchNumber(uint8 a_buttonNumber);
#else
static uint8 keypad_4x4_adjustSwitchNumber_calc(uint8 a_buttonNumber);
/*
 * function responsible for mapping switch number in the keypad
 * it works if the keypad is 4x4
 * it is for calculator  keypads
 * 		7	8	9	/
 * 		4	5	6 	*
 * 		1	2	3	-
 * 		ON	0	=	+
 */
#endif
#endif

uint8 KEYPAD_getPressedKey(void){
	uint8 col, row;
	for(;;){

		for(col = 0; col < COLUMNS; col++){

			KEYPAD_PORT_DIR = 0x10 << col;

			/*
			 * each time we make only one pit as output
			 * and the rest of pins is input
			 */

			KEYPAD_PORT_OUT = ~(0x10 << col);
			/*
			 * clear the output of certain pin in the port to make it works as GND
			 * and set the rest of pins to enable the pull up resistors
			 */

			for (row = 0; row < ROWS; row++){
				if(BIT_IS_CLEAR(KEYPAD_PORT_IN, row)){
				#if COLUMNS == 3
					return keypad_4x3_adjustSwitchNumber(row * COLUMNS + col + 1);
				#elif COLUMNS == 4
					#if calc == 0
						return keypad_4x4_adjustSwitchNumber(row * COLUMNS + col + 1);
					#else
						return keypad_4x4_adjustSwitchNumber_calc(row * COLUMNS + col + 1);
					#endif
				#endif
				}
			}
		}
	}
	return 0;
}
#if COLUMNS == 3
	static uint8 keypad_4x3_adjustSwitchNumber(uint8 a_buttonNumber){
		switch(a_buttonNumber){
		case 10:
			return '*';		 /* ASCII of * */
		case 11:
			return 0;
		case 12:
			return '#';		/* ASCII of # */
		default:
			return a_buttonNumber;

		}
	}
#elif COLUMNS == 4
	#if calc == 0
		static uint8 keypad_4x4_adjustSwitchNumber(uint8 a_buttonNumber){
			switch(a_buttonNumber){
			case 4: return 'A';		/* ASCII of A */
			case 5: return 4;
			case 6: return 5;
			case 7: return 6;
			case 8: return 'B';		/* ASCII of B */
			case 9: return 7;
			case 10: return 8;
			case 11: return 9;
			case 12: return 'C';		/* ASCII of C */
			case 13: return '*';		/* ASCII of * */
			case 14: return 0;
			case 15: return '#';		/* ASCII of # */
			case 16: return 'D';		/* ASCII of D */
			default : return a_buttonNumber;
			}
		}
	#else
		static uint8 keypad_4x4_adjustSwitchNumber_calc(uint8 a_buttonNumber){
			switch(a_buttonNumber){
			case 1: return 7;
			case 2: return 8;
			case 3: return 9;
			case 4: return '/';		/* ASCII of / */
			case 5: return 4;
			case 6: return 5;
			case 7: return 6;
			case 8: return 'X';		/* ASCII of X */
			case 9: return 1;
			case 10: return 2;
			case 11: return 3;
			case 12: return '-';		/* ASCII of - */
			case 13: return 13;			/* ASCII of Enter */
			case 14: return 0;
			case 15: return '=';		/* ASCII of = */
			case 16: return '+';		/* ASCII of + */
			}
		}
	#endif
#endif
