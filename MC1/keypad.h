/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <ksyPad.h>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <30 - 10 -2020>
 *
 * [DESCRIPTION]: <Header file for keypad driver>
 * ---------------------------------------------------------------------------------------------------
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#define COLUMNS 3
#define ROWS 4
#define calc 0

#define KEYPAD_PORT_DIR DDRB
#define KEYPAD_PORT_OUT PORTB
#define KEYPAD_PORT_IN PINB

uint8 KEYPAD_getPressedKey(void);


#endif /* KEYPAD_H_ */
