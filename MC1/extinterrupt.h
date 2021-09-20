/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <extinterrupt.h>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <13 - 12 - 2020>
 *
 * [DESCRIPTION]: <Header file for external interrupts>
 * ---------------------------------------------------------------------------------------------------
 */


#ifndef EXTINTERRUPT_H_
#define EXTINTERRUPT_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*
 * enum for possible interrupt IDs
 */
typedef enum{
	INTno0, INTno1, INTno2
}INT_ID;

/*
 * enum for possible edges
 */
typedef enum{
	INT_RISING, INT_FALLING, INT_TOGGLE
}INT_EdgeType;

/*	struct to config the ext interrupt */
typedef struct{
	INT_ID ID;
	INT_EdgeType edge;
}INT_ConfigType;

void INT_setCallBack(INT_ID ID, void(*a_ptr)(void));
void INT_init(const INT_ConfigType* ConfigPtr);

#endif /* EXTINTERRUPT_H_ */
