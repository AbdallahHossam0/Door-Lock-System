/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <external_eeprom.h>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <15 - 12 - 2020>
 *
 * [DESCRIPTION]: <Header file for External EEPROM>
 * ---------------------------------------------------------------------------------------------------
 */

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define EEPROM_ERROR 0
#define EEPROM_SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void EEPROM_init(void);
uint8 EEPROM_writeByte(uint16 address, uint8 data);
uint8 EEPROM_readByte(uint16 address, uint8* data);
uint8 EEPROM_writeData(uint16 adress, const uint8* data, uint8 dataSize);
uint8 EEPROM_readData(uint16 adress, uint8* data, uint8 dataSize);

#endif /* EXTERNAL_EEPROM_H_ */
