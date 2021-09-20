/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <timer.h>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <14 - 12 - 2020>
 *
 * [DESCRIPTION]: <Header file for timer driver>
 * ---------------------------------------------------------------------------------------------------
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/* Enum to set Timer IDs */
typedef enum{
	TIMER0, TIMER1A, TIMER1B, TIMER2
}TIMER_ID;

/*	Enum for set the clock */
typedef enum{
	NO_CLOCK, F_CPU_CLOCK, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
}TIMER_Clock;

typedef enum{
	TIMER_DISCONNECTED, TIMER_TOGGLE, TIMER_CLEAR, TIMER_SET
}TIMER_Compare_Output_Mode;

/* Struct to config the timer */
typedef struct{
	TIMER_ID ID;
	TIMER_Clock clock;
	TIMER_Compare_Output_Mode mode;
	uint16 initialValue;
	uint16 compareValue;
	uint16 compareValueB;
}TIMER_ConfigType;

/*
 * ------------------------------------------------------------------------------------------------
 * 					Prototypes
 * ------------------------------------------------------------------------------------------------
 */
void TIMER_init(const TIMER_ConfigType* configPtr);
void TIMER_setCallback(TIMER_ID ID, void(*a_Ptr)(void));
void TIMER_stop(TIMER_ID ID);
void TIMER_resume(TIMER_ID ID, TIMER_Clock clock);

#endif /* TIMER_H_ */
