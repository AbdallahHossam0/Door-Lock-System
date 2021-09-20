/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <i2c.h>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <15 - 12 - 2020>
 *
 * [DESCRIPTION]: <Header file for I2C driver>
 * ---------------------------------------------------------------------------------------------------
 */

#ifndef I2C_H_
#define I2C_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/* enum to keep values of prescaler */
typedef enum{
	TWI_PRESCLER_1, TWI_PRESCLER_4, TWI_PRESCLER_16, TWI_PRESCLER_64
}TWI_Prescaler;

/* struct to configure the the I2C */
typedef struct{
	TWI_Prescaler prescalrer;
	uint8 bitRate;
	uint8 myAddress;
}TWI_ConfigType;

/*
 * ------------------------------------------------------------------------------------------------
 * 								STATUS VALUES
 * ------------------------------------------------------------------------------------------------
 */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start 
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave

/*
 * ------------------------------------------------------------------------------------------------
 * 						Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */
void TWI_init(TWI_ConfigType* configPtr);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8 data);
uint8 TWI_readWithACK(void);
uint8 TWI_readWithNACK(void);
uint8 TWI_getStatus(void);


#endif /* I2C_H_ */
