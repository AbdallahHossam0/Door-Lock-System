/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <i2c.c>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <15 - 12 - 2020>
 *
 * [DESCRIPTION]: <Source file for I2C driver>
 * ---------------------------------------------------------------------------------------------------
 */

#include "i2c.h"

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	TWI_init.
 *
 * [DESCRIPTION]:	Function to initialize the TWI.
 *
 * [Args]:			configPtr of type TWI_ConfigType.
 * 					it contains	1.	the address master.
 * 								2.	bit rate.
 * 								3.	prescaler.
 *
 *
 * [IN]:			configPtr.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void TWI_init(TWI_ConfigType* configPtr)
{
	TWBR = configPtr -> bitRate;

	TWSR |= (configPtr -> prescalrer) & 0x03;
	/*  pass the address of this MC if any master slave want to call this MC  */
	TWAR = configPtr -> myAddress;

	/* Enable I2C */
	TWCR = (1<<TWEN);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	TWI_start
 *
 * [DESCRIPTION]:	Send the Start BIT
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
void TWI_start(void)
{
	/*
	 * 	Clear the flag
	 * 	Send the Start BIT
	 * 	Enable I2C
	 */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	/* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}


/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	TWI_stop
 *
 * [DESCRIPTION]:	Send the Stop BIT
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
void TWI_stop(void)
{
	/*
	 * 	Clear the flag
	 * 	Send the stop BIT
	 * 	Enable I2C
	 */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}


/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	TWI_write
 *
 * [DESCRIPTION]:	Function to send data with I2C.
 *
 * [Args]:			data you want to send
 *
 * [IN]:			data
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
void TWI_write(uint8 data)
{
	/* Put the data which will be transmitted in the register */
	TWDR = data;
	/*
	 * 	Enable I2C
	 * 	Clear the flag
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait until I2C finished and make sure that the data is sent successfully */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	TWI_readWithACK
 *
 * [DESCRIPTION]:	Function to read data and send ACK.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 TWI_readWithACK(void)
{
	/*
	 * 	Enable the I2C
	 * 	Enable the ACKNOWLEGEMENT
	 * 	Clear the flag
	 */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	/* Wait until I2C finish receiving and set the flag */
	while(BIT_IS_CLEAR(TWCR,TWINT));

	/* Read Data */
	return TWDR;
}


/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	TWI_readWithNACK
 *
 * [DESCRIPTION]:	Function to read data and send NACK.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 TWI_readWithNACK(void)
{
	/*
	 * 	Clear the INT flag
	 * 	Enable the I2C
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/*	Wait until I2C finish receiving and set the flag */
	while(BIT_IS_CLEAR(TWCR,TWINT));
	/* Read Data */
	return TWDR;
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	TWI_getStatus
 *
 * [DESCRIPTION]:	Function to get I2C status.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 TWI_getStatus(void)
{
	/*
	 *  NO need to enable the TWI here (IMPORTANT)---------------
	 *
	 */

	/*
	 * 	anding with 0xf8 to delete the first 3 bits
	 * 	and get the most 5 bits (status bits)
	 */
	return (TWSR & 0xf8);
}
