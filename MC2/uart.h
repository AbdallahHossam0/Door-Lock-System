/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <uart.h>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <15 - 12 - 2020>
 *
 * [DESCRIPTION]: <Header file for UART driver>
 * ---------------------------------------------------------------------------------------------------
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/* Keep receive the string until you find the '#' */
#define END_CHARACTER '#'

typedef enum{
	NO_PARITY, EVEN_PARITY = 2, ODD_PARITY
}UART_ParityType;

typedef enum{
	BIT_1, BIT_2
}UART_StopBit;

typedef struct{
	uint16 UART_baudRate;
	uint8 INTRXEnable : 1;
	uint8 receiverEnable : 1;
	uint8 transmitterEnable : 1;
	UART_StopBit stopBitNo : 1;
	UART_ParityType parity : 2;
}UART_ConfigType;


void UART_init(UART_ConfigType *config_Ptr);
void UART_sendByte(const uint8 data);
uint8 UART_receiveByte(void);
void UART_sendString(const uint8 *str_Ptr);
void UART_receiveString(uint8 *str_Ptr);

#endif /* UART_H_ */
