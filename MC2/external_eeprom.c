/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <external_eeprom.c>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <15 - 12 - 2020>
 *
 * [DESCRIPTION]: <Source file for External EEPROM>
 * ---------------------------------------------------------------------------------------------------
 */


#include "i2c.h"
#include "external_eeprom.h"

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	EEPROM_init.
 *
 * [DESCRIPTION]:	Function to initialize the EEPROM by initializing the I2C
 *
 * [Args]:			None
 *
 * [IN]:			None
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void EEPROM_init(void)
{
	TWI_ConfigType configPtr = {TWI_PRESCLER_1, 2, 0x10};
	TWI_init(& configPtr);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	EEPROM_writeByte.
 *
 * [DESCRIPTION]:	Function to send byte to EEPROM by:
 * 					1.	Send start bit.
 * 					2.	Send the address of the slave containing
 * 						the most 3 bits of location address.
 * 					3.	Send the remaining 8 bits of address location.
 * 					4.	Send the byte you want to send.
 * 					5.	Send stop bit.
 *
 *
 * [Args]:			1.	Address of the location.
 * 					2. 	Data you want to store in this address.
 *
 * [IN]:			1. address of type uint16.
 * 					2. data of type uint8.
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		Return 1 if we write the data correctly, or
 * 					Return 0 if any error happened.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 EEPROM_writeByte(uint16 address, uint8 data)
{
	/* Send Start Bit */
	TWI_start();
	if (TWI_getStatus() != TW_START)
		return EEPROM_ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_write((uint8)(0xA0 | ((address & 0x0700)>>7)));
	if (TWI_getStatus() != TW_MT_SLA_W_ACK)
		return EEPROM_ERROR;

	/* Send the required memory location address */
	TWI_write((uint8)(address));
	if (TWI_getStatus() != TW_MT_DATA_ACK)
		return EEPROM_ERROR;

	/* write byte to selected location*/
	TWI_write(data);
	if (TWI_getStatus() != TW_MT_DATA_ACK)
		return EEPROM_ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return EEPROM_SUCCESS;
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	EEPROM_readByte
 *
 * [DESCRIPTION]:	Function to read byte from EEPROM by:
 * 					1.	Send start bit.
 * 					2.	Send the address containing the mose 3 bits of location.
 * 					3.	Send the remaining 8 bits of address location.
 * 					4.	Send repeated Start to be able to change
 * 						from write operation to read operation.
 * 					5.	Send slave address containing the most 3 bits and bit for read operation.
 * 					6.	Read the data.
 * 					7.	Send stop bit.
 *
 *
 * [Args]:			address has the address of the locatio (11 bits).
 * 					pointer to data to return the byte we read
 *
 * [IN]:			address of type uint16
 *
 * [OUT]:			data of type uint8*
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		Return 1 if we read the data correctly, or
 * 					Return 0 if any error happened.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 EEPROM_readByte(uint16 address, uint8* data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TW_START)
		return EEPROM_ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_write((uint8)((0xA0) | ((address & 0x0700)>>7)));
	if (TWI_getStatus() != TW_MT_SLA_W_ACK)
		return EEPROM_ERROR;

	/* Send the required memory location address */
	TWI_write((uint8)(address));
	if (TWI_getStatus() != TW_MT_DATA_ACK)
		return EEPROM_ERROR;

	/* Send the Repeated Start Bit */
	TWI_start();
	if (TWI_getStatus() != TW_REP_START)
		return EEPROM_ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_write((uint8)((0xA0) | ((address & 0x0700)>>7) | 1));
	if (TWI_getStatus() != TW_MT_SLA_R_ACK)
		return EEPROM_ERROR;

	/* Read Byte from Memory without send ACK */
	*data = TWI_readWithNACK();
	if (TWI_getStatus() != TW_MR_DATA_NACK)
		return EEPROM_ERROR;

	/* Send the Stop Bit */
	TWI_stop();
	return EEPROM_SUCCESS;
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	EEPROM_writeData.
 *
 * [DESCRIPTION]:	Function to send some of bytes.
 *
 * [Args]:			1.	address you want to keep data in.
 * 					2.	pointer point to first location of data.
 * 					3.	size of data.
 *
 * [IN]:			1. 	address.
 * 					2.	data.
 * 					3.	dataSize.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		Return 1 if we write all data correctly, or
 * 					Return 0 if any error happened.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 EEPROM_writeData(uint16 address, const uint8* data, uint8 dataSize){
	/* send start bit */
	TWI_start();
	if (TWI_getStatus() != TW_START)
		return EEPROM_ERROR;

	/* send the slave address containing the most 3 bits of location address
	 * and write operation.
	 */
	TWI_write((uint8)(0xA0 | ((address & 0x0700)>>7)));
	if (TWI_getStatus() != TW_MT_SLA_W_ACK)
		return EEPROM_ERROR;

	/* send the remaining 8 bits of the address */
	TWI_write((uint8)(address));
	if (TWI_getStatus() != TW_MT_DATA_ACK)
		return EEPROM_ERROR;

	/*	for loop to write all bytes */
	for(uint8 counter = 0; counter < dataSize; counter++){
		TWI_write(data[counter]);
		if (TWI_getStatus() != TW_MT_DATA_ACK)
			return EEPROM_ERROR;
	}
	/* send stop bit */
	TWI_stop();

	return EEPROM_SUCCESS;
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	EEPROM_readData.
 *
 * [DESCRIPTION]:	Function to read some of bytes.
 * 					Master sends ACK to keep the slave sending bytes.
 * 					when Master don't want to read any more data, it send NACK.
 *
 * [Args]:			1.	address of first location.
 * 					2.	pointer to location you want to read the data in.
 * 					3.	size of data you want to read.
 *
 * [IN]:			1.	address.
 * 					2.	dataSize.
 *
 * [OUT]:			1. data. (pointer).
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		Return 1 if we read the data correctly, or
 * 					Return 0 if any error happened.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 EEPROM_readData(uint16 address, uint8* data, uint8 dataSize){
	/* send start bit */
	TWI_start();
	if (TWI_getStatus() != TW_START)
		return EEPROM_ERROR;

	/* send the slave address containing the most 3 bits of location address
	 * and write operation.
	 */
	TWI_write((uint8)((0xA0) | ((address & 0x0700)>>7)));
	if (TWI_getStatus() != TW_MT_SLA_W_ACK)
		return EEPROM_ERROR;

	/* send the remaining 8 bits of the address */
	TWI_write((uint8)(address));
	if (TWI_getStatus() != TW_MT_DATA_ACK)
		return EEPROM_ERROR;

	/* send repeated start to change write operation to read operation */
	TWI_start();
	if (TWI_getStatus() != TW_REP_START)
		return EEPROM_ERROR;

	/* send slave address and write operation */
	TWI_write((uint8)((0xA0) | ((address & 0x0700)>>7) | 1));
	if (TWI_getStatus() != TW_MT_SLA_R_ACK)
		return EEPROM_ERROR;

	/* read all data */
	for(uint8 counter = 0; counter < dataSize; counter++){
		/* check for last byte you want to read and send NACK to make slave stop sending */
		if(counter == dataSize -1){
			data[counter] = TWI_readWithNACK();
			if (TWI_getStatus() != TW_MR_DATA_NACK)
				return EEPROM_ERROR;
		}
		else{
			data[counter] = TWI_readWithACK();
			if(TWI_getStatus() != TW_MR_DATA_ACK){
				return EEPROM_ERROR;
			}
		}
	}
	/* send stop bit */
	TWI_stop();
	return EEPROM_SUCCESS;
}
