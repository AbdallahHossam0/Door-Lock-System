/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <motor.h>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <13 - 12 - 2020>
 *
 * [DESCRIPTION]: <Header file for Motor driver>
 * ---------------------------------------------------------------------------------------------------
 */

#include "motor.h"

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	setMotorDirectionPIN
 *
 * [DESCRIPTION]:	Function to config the motor (2 output Pins)
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
void setMotorDirectionPIN(void){
	SET_BIT(DDRA, PA6);
	SET_BIT(DDRA, PA7);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	motorCW
 *
 * [DESCRIPTION]:	Function to turn the motor in in clock wise direction.
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
void motorCW(void){
	SET_BIT(PORTA, PA6);
	CLEAR_BIT(PORTA, PA7);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	motorCCW
 *
 * [DESCRIPTION]:	Function to turn the motor in in counter clock wise direction.
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
void motorCCW(void){
	SET_BIT(PORTA, PA7);
	CLEAR_BIT(PORTA, PA6);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	motorOff
 *
 * [DESCRIPTION]:	Function to turn the motor off.
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
void motorOff(void){
	CLEAR_BIT(PORTA, PA6);
	CLEAR_BIT(PORTA, PA7);
}
