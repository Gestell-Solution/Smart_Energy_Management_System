/**
 * @file EEPROM_Interface.h
 * @brief This file contains the interface for the EEPROM module.
 * @version 1.0
 * @author Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Basma Khaled  (basmak55@gmail.com)
 * @date 2025-10-5
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#ifndef EEPROM_INTERFACE_H
#define EEPROM_INTERFACE_H

/**
 * @addtogroup Mcal
 * @{
 * @addtogroup EEPROM_Internal
 * @{
 */

#include "EEPROM_Private.h"
#include "../Atmega32RegistersAddress.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/**
 * @fn mEEPROM_WriteByte
 * @brief Writes a byte of data to the specified EEPROM address.
 * @param Address The EEPROM address to write to (0x0000 to 0x03FF).
 * @param Data The byte of data to write.
 * @return uint8_t Status of the write operation (EEPROM_Write_Success or EEPROM_Write_Fail).
 */
uint8_t mEEPROM_WriteByte(uint16_t Address, uint8_t Data);
/**
 * @fn mEEPROM_ReadByte
 * @brief Reads a byte of data from the specified EEPROM address.
 * @param Address The EEPROM address to read from (0x0000 to 0x03FF).
 */
uint8_t mEEPROM_ReadByte(uint16_t Address);
/**
 * @fn mEEPROM_WriteBlock
 * @brief Writes a block of data to the specified EEPROM address.
 * @param Address The starting EEPROM address to write on (0x0000 to 0x03FF).
 * @param data Pointer to the block of data to write.
 * @param length The number of bytes to write.
 */
void mEEPROM_WriteBlock(uint16_t Address, uint8_t *data, uint16_t length);
/**
 * @fn mEEPROM_ReadBlock
 * @brief Reads a block of data from the specified EEPROM address.
 * @param Address The starting EEPROM address to read from (0x0000 to 0x03FF).
 * @param data Pointer to the block of data to store the read data.
 * @param length The number of bytes to read.
 */
void mEEPROM_ReadBlock(uint16_t Address, uint8_t *data, uint16_t length);

/**
 * @}
 * @}
 */

#endif