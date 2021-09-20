/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <timer.c>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <14 - 12 - 2020>
 *
 * [DESCRIPTION]: <Source file for timer driver>
 * ---------------------------------------------------------------------------------------------------
 */

#include "timer.h"
/* Global variable for callback */
static volatile void (*g_callBackTimer0Ptr)(void) = NULL_PTR;
static volatile void (*g_callBackTimer1APtr)(void) = NULL_PTR;
static volatile void (*g_callBackTimer1BPtr)(void) = NULL_PTR;
static volatile void (*g_callBackTimer2Ptr)(void) = NULL_PTR;

/* Interrupt service routine for interrupts */
ISR(TIMER0_COMP_vect){
	if(g_callBackTimer0Ptr != NULL_PTR){
		g_callBackTimer0Ptr();
	}
}

ISR(TIMER1_COMPA_vect){
	if(g_callBackTimer1APtr != NULL_PTR){
		g_callBackTimer1APtr();
	}
}

ISR(TIMER1_COMPB_vect){
	if(g_callBackTimer1BPtr != NULL_PTR){
		g_callBackTimer1BPtr();
	}
}

ISR(TIMER2_COMP_vect){
	if(g_callBackTimer2Ptr != NULL_PTR){
		g_callBackTimer2Ptr();
	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	TIMER_init
 *
 * [DESCRIPTION]:	Function to initialize the timer
 *
 * [Args]:			Pointer of type TIMER_ConfigType
 *
 * [IN]:			Pointer of type TIMER_ConfigType
 * 					it has the configuration information about timer you want to init
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void TIMER_init(const TIMER_ConfigType* configPtr){
	switch(configPtr -> ID){
	case TIMER0:
		SET_BIT(TCCR0, FOC0);
		SET_BIT(TCCR0, WGM01);
		TCCR0 = (TCCR0 & 0xcf) | (((configPtr -> mode) & 0x03) << 4);
		TCCR0 = (TCCR0 & 0xf8) | ((configPtr -> clock) & 0x07);
		TCNT0 = (configPtr -> initialValue & 0x0f);
		OCR0 = (configPtr -> compareValue & 0x0f);
		if((configPtr -> mode == TIMER_DISCONNECTED) && ((configPtr -> compareValue) != 0)){
			SET_BIT(TIMSK, OCIE0);
		}
		break;
	case TIMER1A:
	case TIMER1B:
		SET_BIT(TCCR1A, FOC1A);
		SET_BIT(TCCR1A, FOC1B);
		SET_BIT(TCCR1B, WGM12);
		TCCR1A = (TCCR1A & 0x3f) | (((configPtr -> mode) & 0x03) << 6);
		TCCR1A = (TCCR1A & 0xcf) | (((configPtr -> mode) & 0x03) << 4);
		TCCR1B = (TCCR1B & 0xf8) | (((configPtr -> clock) & 0x07));
		TCNT1 = configPtr -> initialValue;
		OCR1A = configPtr -> compareValue;
		OCR1B = configPtr -> compareValueB;
		if((configPtr -> mode == TIMER_DISCONNECTED) && (configPtr -> compareValue != 0)){
			SET_BIT(TIMSK, OCIE1A);
		}
		if((configPtr -> mode == TIMER_DISCONNECTED) && ((configPtr -> compareValueB) != 0)){
			SET_BIT(TIMSK, OCIE1B);
		}
		break;
	case TIMER2:
		SET_BIT(TCCR2, FOC2);
		SET_BIT(TCCR2, WGM21);
		TCCR2 = (TCCR2 & 0xcf) | (((configPtr -> mode) & 0x03) << 4);
		TCCR2 = (TCCR2 & 0xf8) | (((configPtr -> clock) & 0x07));
		TCNT2 = configPtr -> initialValue;
		OCR2 = configPtr -> compareValue;
		if((configPtr -> mode == TIMER_DISCONNECTED) && ((configPtr -> compareValue) != 0)){
			SET_BIT(TIMSK, OCIE2);
		}
		break;
	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	TIMER_setCallback
 *
 * [DESCRIPTION]:	Function to set the callback
 *
 * [Args]:			ID of the timer
 * 					Pointer to a function which will be called in the ISR
 *
 * [IN]:			TIMER_ID
 * 					Pointer to a function
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void TIMER_setCallback(TIMER_ID ID, void(*a_Ptr)(void)){
	switch(ID){
	case TIMER0:
		g_callBackTimer0Ptr = a_Ptr;
		break;
	case TIMER1A:
		g_callBackTimer1APtr = a_Ptr;
		break;
	case TIMER1B:
		g_callBackTimer1BPtr = a_Ptr;
		break;
	case TIMER2:
		g_callBackTimer2Ptr = a_Ptr;
		break;
	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]: TIMER_stop
 *
 * [DESCRIPTION]:	Function to stop the timer
 *
 * [Args]:			ID of the timer you want to stop
 *
 * [IN]:			TIMER ID
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void TIMER_stop(TIMER_ID ID){
	switch(ID){
	case TIMER0:
		CLEAR_BIT(TCCR0, CS00);
		CLEAR_BIT(TCCR0, CS01);
		CLEAR_BIT(TCCR0, CS02);
		break;
	case TIMER1A:
	case TIMER1B:
		CLEAR_BIT(TCCR1B, CS10);
		CLEAR_BIT(TCCR1B, CS11);
		CLEAR_BIT(TCCR1B, CS12);
		break;
	case TIMER2:
		CLEAR_BIT(TCCR2, CS20);
		CLEAR_BIT(TCCR2, CS21);
		CLEAR_BIT(TCCR2, CS22);
		break;
	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]: TIMER_resume
 *
 * [DESCRIPTION]:	Function to resume the timer
 *
 * [Args]:			TIMER_ID ID, TIMER_Clock clock
 *
 * [IN]:			1. 	ID argument of type TIMER_ID
 *						to know which timer will be resumed
 *					2. 	clock argument of type TIMER_Clock
 *						to know with which frequency will resume
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void TIMER_resume(TIMER_ID ID, TIMER_Clock clock){
	switch(ID){
	case TIMER0:
		TCCR0 = (TCCR0 & 0xf8) | ((clock) & 0x07);
		break;
	case TIMER1A:
	case TIMER1B:
		TCCR1B = (TCCR1B & 0xf8) | ((clock) & 0x07);
		break;
	case TIMER2:
		TCCR2 = (TCCR2 & 0xf8) | ((clock) & 0x07);
		break;
	}
}
