/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <extinterrupt.c>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <13 - 12 - 2020>
 *
 * [DESCRIPTION]: <Source file for external interrupts>
 * ---------------------------------------------------------------------------------------------------
 */


#include "extinterrupt.h"
/*
 * 	Pointers for callBack to each interrupt
 */
static volatile void (*g_callBackINT0Ptr)(void) = NULL_PTR;
static volatile void (*g_callBackINT1Ptr)(void) = NULL_PTR;
static volatile void (*g_callBackINT2Ptr)(void) = NULL_PTR;

/*
 * ISR to INT0
 */
ISR(INT0_vect){
	if(g_callBackINT0Ptr != NULL_PTR){
		g_callBackINT0Ptr();
	}
}

/*
 * ISR to INT1
 */
ISR(INT1_vect){
	if(g_callBackINT1Ptr != NULL_PTR){
		g_callBackINT1Ptr();
	}
}

/*
 * ISR to INT2
 */
ISR(INT2_vect){
	if(g_callBackINT2Ptr != NULL_PTR){
		g_callBackINT2Ptr();
	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	INT_setCallBack
 *
 * [DESCRIPTION]:	Function to set the call back
 *
 * [Args]:
 *
 * [IN]:			ID of the interrupt
 * 					pointer to function to set the callback
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void INT_setCallBack(INT_ID ID, void(*a_ptr)(void)){
	switch(ID){
	case 0:
		g_callBackINT0Ptr = a_ptr;
		break;
	case 1:
		g_callBackINT1Ptr = a_ptr;
		break;
	case 2:
		g_callBackINT2Ptr = a_ptr;
	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	INT_init
 *
 * [DESCRIPTION]:	Function to config external interrupt
 * 					set the edge
 * 					and the interrupt ID
 *
 * [Args]:
 *
 * [IN]:			Pointer to structure
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void INT_init(const INT_ConfigType* ConfigPtr){
	switch(ConfigPtr->ID){
	case 0:
		CLEAR_BIT(DDRD, PD2);
		SET_BIT(GICR, INT0);
		switch(ConfigPtr -> edge){
		case INT_TOGGLE:
			SET_BIT(MCUCR, ISC00);
			CLEAR_BIT(MCUCR, ISC01);
			break;
		case INT_FALLING:
			SET_BIT(MCUCR, ISC01);
			CLEAR_BIT(MCUCR, ISC00);
			break;
		case INT_RISING:
			SET_BIT(MCUCR, ISC00);
			SET_BIT(MCUCR, ISC01);
			break;
		}
		break;
		case 1:
			CLEAR_BIT(DDRD, PD3);
			SET_BIT(GICR, INT1);
			switch(ConfigPtr -> edge){
			case INT_TOGGLE:
				SET_BIT(MCUCR, ISC10);
				CLEAR_BIT(MCUCR, ISC11);
				break;
			case INT_FALLING:
				SET_BIT(MCUCR, ISC11);
				CLEAR_BIT(MCUCR, ISC10);
				break;
			case INT_RISING:
				SET_BIT(MCUCR, ISC10);
				SET_BIT(MCUCR, ISC11);
				break;
			}
			break;
		case 2:
			CLEAR_BIT(DDRB, PB2);
			SET_BIT(GICR, INT2);
			switch(ConfigPtr -> edge){
			case INT_FALLING:
				CLEAR_BIT(MCUCSR, ISC2);
				break;
			case INT_RISING:
				SET_BIT(MCUCSR, ISC2);
			}
			break;
	}

}

