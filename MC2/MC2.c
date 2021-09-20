/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <MC2.c>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <15 - 12 - 2020>
 *
 * [DESCRIPTION]: <Source file for the second MicroController>
 * ---------------------------------------------------------------------------------------------------
 */


#include "external_eeprom.h"
#include "uart_macros.h"
#include "uart.h"

#define CHECK_PASSWORD_BIT 0x10
#define PASSWORD_ADDRESS 0x20


uint8 checkEquality(const uint8* arr1, const uint8* arr2);

int main(void){
	uint8 readEEPROM;				/* variable to read from EEPROM */
	uint8 password1[6];				/* array to keep the password */
	uint8 password2[6];				/* array to keep the password */
	EEPROM_init();					/* initialize the EEPROM */
	UART_ConfigType configData = {9600, 0, 1, 1, BIT_1, NO_PARITY};
	UART_init(&configData);			/* initialize UART */
	/* clear this bit because it starts with random variable */
	EEPROM_writeByte(CHECK_PASSWORD_BIT, '0');
	for(;;){
		/* receive byte from UART to decide which MC1 want to do */
		switch(UART_receiveByte()){
		case EEPROM_IS_CLEAR:			/* check if there is a password in the EEPROM or no */
			EEPROM_readByte(CHECK_PASSWORD_BIT, &readEEPROM);
			if(readEEPROM == '0')		/* if there is a password send yes */
				UART_sendByte(YES);
			else
				UART_sendByte(NO); 		/* if no send no */
			break;

			/* to change the password */
		case CAHNGE_PASSWORD:
			UART_receiveString(password1);	/* receive the fist password */
			UART_receiveString(password2);	/* receive the second password*/
			if(checkEquality(password1, password2)){
				UART_sendByte(YES);			/* if they are equal return YES */
				EEPROM_writeByte(CHECK_PASSWORD_BIT, '1');
				/* save the password in the EEPROM */
				for(uint8 counter = 0; counter < 5; counter++){
					EEPROM_writeByte((counter + PASSWORD_ADDRESS), password1[counter]);
				}
			}
			else
				UART_sendByte(NO);		/* if no return no */
			break;

		/* check the password by comparing it with password in the EEPROM	*/
		case CHECK_PASSWORD:
			/* load the password from the EEPROM */
			for(uint8 counter = 0; counter < 5; counter++){
				EEPROM_readByte((PASSWORD_ADDRESS + counter), (password1 + counter));
			}
			UART_receiveString(password2);
			/* check equality of password sent and password in the EEPROM */
			if(checkEquality(password1, password2)){
				UART_sendByte(YES);
			}
			else
				UART_sendByte(NO);
			break;
		default:
			break;
		}

	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	check Equality
 *
 * [DESCRIPTION]:	Function to compare 2 password.
 *
 * [Args]:			2 arrays
 *
 * [IN]:			arr1
 * 					arr2
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		it returns 1 if they are equal, or
 * 					0 if they are not equal.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 checkEquality(const uint8* arr1, const uint8* arr2){
	for(uint8 counter = 0; counter < 5; counter++){
		if(arr1[counter] != arr2[counter]){
			return 0;
		}
	}
	return 1;
}
