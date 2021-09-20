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
#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*
 * ------------------------------------------------------------------------------------------------
 * 								Prototypes
 * ------------------------------------------------------------------------------------------------
 */

void setMotorDirectionPIN(void);
void motorCW(void);
void motorCCW(void);
void motorOff(void);


#endif /* MOTOR_H_ */
