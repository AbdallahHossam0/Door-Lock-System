/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <buzzer.c>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <13 - 12 - 2020>
 *
 * [DESCRIPTION]: <Source file for BUZZER>
 * ---------------------------------------------------------------------------------------------------
 */
#include "buzzer.h"
/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	setBuzzerDirectionPIN
 *
 * [DESCRIPTION]:	Function to config the PINS for the buzzer (one output PIN)
 *
 * [Args]:
 *
 * [IN]:			None
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void setBuzzerDirectionPIN(void){
	SET_BIT(DDRD, PD7);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:		buzzerOn
 *
 * [DESCRIPTION]:		Function to turn on the buzzer
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
void buzzerOn(void){
	SET_BIT(PORTD, PD7);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:		buzzerOff
 *
 * [DESCRIPTION]:		Function to turn off the buzzer
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
void buzzerOff(void){
	CLEAR_BIT(PORTD, PD7);
}
