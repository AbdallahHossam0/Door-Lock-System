/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <buzzer.h>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <13 - 12 - 2020>
 *
 * [DESCRIPTION]: <Header file for BUZZER>
 * ---------------------------------------------------------------------------------------------------
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"
/*
 * ------------------------------------------------------------------------------------------------
 * 								ProtoTypes
 * ------------------------------------------------------------------------------------------------
 */
void setBuzzerDirectionPIN(void);
void buzzerOn(void);
void buzzerOff(void);


#endif /* BUZZER_H_ */
