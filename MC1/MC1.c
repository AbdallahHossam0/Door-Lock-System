/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <MC1.c>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <15 - 12 - 2020>
 *
 * [DESCRIPTION]: <Code for first MicroController>
 * ---------------------------------------------------------------------------------------------------
 */


#include "lcd.h"
#include "keypad.h"
#include "buzzer.h"
#include "motor.h"
#include "extinterrupt.h"
#include "timer.h"
#include "uart_macros.h"
#include "uart.h"



uint8 setPassword(void);
void LCD_enterPassword(uint8 newPassword);
void LCD_homeScreen(void);
void changePassword(void);
uint8 checkPassword(void);
void openDoor(void);
void alert(void);
void set1Min(void);
void systemPause(void);
void controlMotor(void);



uint8 g_wrongPasswordCounter = 0;	/* variable to count the no of wrong password enter */
uint8 g_pauseFlag = 0;				/* variable to pause the system as it is not equal to 0 */

int main(void){
	GIE_ON();						/* Enable Global Interrupt Enable Bit (I - Bit) */
	setBuzzerDirectionPIN();		/* initialize buzzer */
	setMotorDirectionPIN();			/* initialize the motor */
	LCD_init();						/* initialize LCD */
	/* variable to configure the UART */
	UART_ConfigType configData = {9600, 0, 1, 1, BIT_1, NO_PARITY};
	UART_init(&configData);			/* initialize UART */
	_delay_ms(500);					/* delay to make sure that MC2 is ready */

	UART_sendByte(EEPROM_IS_CLEAR);	/* check if the EEPROM has password or no */

	if (UART_receiveByte() == YES){
		while(!setPassword());		/* if it has no password we will set 1*/
	}
	else
	{

	}

	/* super loop */
	for(;;){
		LCD_homeScreen();		/* display the home screen */
		switch(KEYPAD_getPressedKey()){
		case '*':
			_delay_ms(500);
			openDoor();			/* open door as user choice */
			break;
		case '#':
			_delay_ms(500);
			changePassword();	/*change password as user choice */
			break;
		default:
			/* for any other input we will display that is unknown input */
			LCD_clearScreen();
			LCD_displayString("Unknown Input");
			_delay_ms(1500);
			break;
		}


	}
}


/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	setPassword
 *
 * [DESCRIPTION]:	Function to set password for a first time or to change password
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		It returns 1 if password set correctly. or
 * 					or 0 if any error happened like if 2 password entered by user are not the same.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 setPassword(void){
	UART_sendByte(CAHNGE_PASSWORD);	/* tell MC2 to change the password */
	uint8 password[6] = {0};		/* set array initially = 0 */
	LCD_clearScreen();				/* clear the screen */
	LCD_enterPassword(1);			/* call function to set pattern to change password */
	/* for loop to get the value from the key pad */
	for(uint8 counter = 0; counter < 5; counter++){
		password[counter] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	password[5] ='\0';				/* to convert the password to string */
	UART_sendString(password);		/* send the password to MC2 */

	/* for loop to remove the first entered password */
	for(uint8 counter = 0; counter < 5; counter++){
		password[counter] = 0;
	}

	LCD_clearScreen();
	/* to display pattern to confirm password in the LCD */
	LCD_displayString("again,");
	LCD_enterPassword(0);
	/* get the password from user by keypad */
	for(uint8 counter = 0; counter < 5; counter++){
		password[counter] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	/* covert the password to string to send it by UART to MC2 */
	password[5] ='\0';
	UART_sendString(password);

	/* wait for MC2 to tell me if they are matched or no */
	switch(UART_receiveByte()){
	case YES:
		LCD_clearScreen();
		LCD_displayString("Password changed");
		_delay_ms(1500);
		return 1;
		break;
	case NO:
		LCD_clearScreen();
		LCD_displayString("Not matched..");
		LCD_goToRowColumn(1, 0);
		LCD_displayString("Try again.");
		_delay_ms(1500);
		return 0;
	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	LCD_enterPassword
 *
 * [DESCRIPTION]:	Function to display (Enter password Pattern in the LCD).
 *
 * [Args]:			newPassword
 * 					to tell the function is it new password or it`s current password
 *
 * [IN]:			newPassword.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void LCD_enterPassword(uint8 newPassword){
	LCD_displayString("Enter ");
	if(newPassword)
		LCD_displayString("New ");
	LCD_displayString("PIN:");
	LCD_displayStringRowColumn(1, 6, "-----");
	LCD_goToRowColumn(1, 6);		/* to start writing stars instead of dashes(-) */
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	LCD_homeScreen
 *
 * [DESCRIPTION]:	Function to display the home menu.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
void LCD_homeScreen(void){
	LCD_clearScreen();
	LCD_goToRowColumn(0, 1);
	LCD_displayString("* OPEN DOOR");
	LCD_goToRowColumn(1, 1);
	LCD_displayString("# CHANGE PIN");
}


/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	changePassword
 *
 * [DESCRIPTION]:	Function to change the password.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
void changePassword(void){
	/* first we ask for the current password if it`s correct then you would be able to change the password
	 * if no we increment the counter until it reached 3 timer we turn buzzer on to warn if there is a thief
	 */
	if(checkPassword()){
		while(!setPassword());		/* if yes we change the password */
		g_wrongPasswordCounter = 0;/* and clear counter for wrong password */
	}
	else{
		g_wrongPasswordCounter++;	/* we increment the counter for wrong times */
		/* when it reached 3 times we would be able to set Buzzer and pause the system for 1 minute */
		if(g_wrongPasswordCounter == 3){
			LCD_clearScreen();
			LCD_displayString("WARNINIG..");
			LCD_displayStringRowColumn(1, 0, "WARNINIG...");
			g_wrongPasswordCounter = 0;
			alert();				/* start alerting */
		}
	}

}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	checkPassword
 *
 * [DESCRIPTION]:	Function to check if the entered password is correct or no.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		it returns 1 if the password is correct, or
 * 							   0 if the password is not correct.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 checkPassword(void){
	UART_sendByte(CHECK_PASSWORD);	/* send to MC2 i want to check the following password */
	uint8 password[6] = {0};
	LCD_clearScreen();
	LCD_enterPassword(0);			/* to display the pattern of entering a password */
	/* for loop to get the password from the user */
	for(uint8 counter = 0; counter < 5; counter++){
		password[counter] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	password[5] ='\0';				/* to convert the password to string */
	UART_sendString(password);		/* send the password to MC2 */
	if(UART_receiveByte() == YES){	/* wait for MC2 to check if the password is correct or NO */
		return 1;					/* if correct return 1 */
	}
	else{
		LCD_clearScreen();
		LCD_displayString("NOT CORRECT");
		_delay_ms(2000);
		return 0;					/* if no return 0 */
	}

}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	openDoor
 *
 * [DESCRIPTION]:	Function to open door.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
void openDoor(void){
	if(checkPassword()){			/* it first check the password if yes it open the door by turn the motor ON */
		motorON();					/* Motor ON	*/
	}
	else{							/* if No we increase the counter for wrong password and if it reached 3 times, alert will start. */
		g_wrongPasswordCounter++;
		if(g_wrongPasswordCounter == 3){
			LCD_clearScreen();
			LCD_displayString("WARNINIG..");
			LCD_displayStringRowColumn(1, 0, "WARNINIG...");
			g_wrongPasswordCounter = 0;
			alert();
		}
	}

}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	alert
 *
 * [DESCRIPTION]:	Function to start Buzzer for 1 minute and pause the system.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
void alert(void){
	/* Start the timer (TIMER1A) to count 6 seconds */
	TIMER_ConfigType configPtr = {TIMER1A, F_CPU_1024, TIMER_DISCONNECTED, 0, 46875, 0};
	/* set the call back to a function to count 1 minute*/
	TIMER_setCallback(TIMER1A, set1Min);
	TIMER_init(&configPtr);			/* initialize the timer */
	buzzerOn();						/* buzzer ON */
	g_pauseFlag = 1;				/* pause the system */
	systemPause();
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	set1Min
 *
 * [DESCRIPTION]:	Function to count 1 minute if the it calls every 6 seconds.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
void set1Min(void){
	static uint8 counter = 0;		/* static counter to keep its value */
	counter++;						/* increment every time we entered the function */
	if(counter == 10){				/* if we reached 10 times then we completed 1 MIN */
		buzzerOff();				/* Turn the buzzer off */
		g_pauseFlag = 0;			/* stop pausing the system */
		counter = 0;				/* Clear the timer */
		TIMER_stop(TIMER1A);		/* stop the timer */
	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	systemPause
 *
 * [DESCRIPTION]:	Function to pause the system while g_pauseFlaag is Set
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
void systemPause(void){
	while(g_pauseFlag);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	motorON
 *
 * [DESCRIPTION]:	Function to open door.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
void motorON(void){
	/* to let timer1A count 3 seconds */
	TIMER_ConfigType configPtr1A = {TIMER1A, F_CPU_1024, TIMER_DISCONNECTED, 0, 23437, 0};
	TIMER_setCallback(TIMER1A, controlMotor);
	TIMER_init(&configPtr1A);
	motorCW();						/* motor ON clock wise direction */
	LCD_clearScreen();
	LCD_displayString("DOOR IS");
	LCD_displayStringRowColumn(1, 0, "OPENING...");
	g_pauseFlag = 1;				/* pause the system while door is opening and closing */
	systemPause();
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	controlMotor
 *
 * [DESCRIPTION]:	Function to control the door
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
void controlMotor(void){
	static counter = 0;
	counter++;
	switch(counter){
	case 5:							/* if you reached 15 second stop the motor */
		motorOff();
		LCD_clearScreen();
		LCD_displayString("WARNINIG..");
		LCD_goToRowColumn(1, 0);
		LCD_displayString("DOOR WILL CLOSE");
		break;
	case 6:							/*	hold for 3 seconds */
		LCD_clearScreen();
		LCD_displayString("DOOR IS");
		LCD_displayStringRowColumn(1, 0, "CLOSING...");
		motorCCW();					/* motor on CCW direction */
		break;
	case 11:						/* wait for another 15 sec */
		g_pauseFlag = 0;
		motorOff();					/* motor off */
		TIMER_stop(TIMER1A);
		counter = 0;				/* clear the timer */
		break;
	}
}







