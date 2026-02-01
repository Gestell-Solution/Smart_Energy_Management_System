/**
 * @file EEPROM_Private.h
 * @brief This file contains the private definitions for the EEPROM module.
 * @version 1.0
 * @author Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Basma Khaled  (basmak55@gmail.com)
 * @date 2025-10-5
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#ifndef EEPROM_PRIVATE_H
#define EEPROM_PRIVATE_H
#include "../../Common/Config.h"
#include "../../Common/Macros.h"
#include <stdint.h>

/**
 * @defgroup EEPROM Control Register (EECR) Bit Definitions
 * @brief    This section defines the bit positions for the EEPROM Control Register (EECR).
 * @details The EECR register is used to control the operations of the EEPROM. , Bits Values in this register
 *          can be accessed using the defined bit positions. bit values can be only 0 or 1.
 *          bit from 4 to 7 are reserved.
 * @{
 */

#define EERE_Bit 0  /**< Setting this bit to 1 Enables Reading */
#define EEWE_Bit 1  /**< Setting this bit to 1 Enables EEPROM Writing (EEMWE Must = 1) */
#define EEMWE_Bit 2 /**< Setting this bit to 1 Enables the EEPROM Master Write  */
#define EERIE_Bit 3 /**< Setting this bit to 1 Enables the EEPROM Ready Interrupt */
/** @} */           // End of EECR_Reg Bit names
/**
 * @defgroup Addresses for memory map of EEPROM
 * @brief    This section defines the memory map addresses for the EEPROM.
 * @details This Addresses are used to store data in the EEPROM
 *          a proper value should be written on the address register (EEARH, EEARL) before any read or write operation.
 * @{
 */
#define Device_ID_Add 0x00         /**<   1  Byte*/
#define Voltage_Calib_Add 0x01     /**<   2  Byte*/
#define Current_Calib_Add 0x03     /**<   2  Byte*/
#define Energy_Counter_Add 0x05    /**<   4  Byte*/
#define Overvoltage_Limit_Add 0x09 /**<   2  Byte*/
#define Overcurrent_Limit_Add 0x0B /**<   2  Byte*/
#define Voltage_RMS_Add 0x0D       /**<   2  Byte*/
#define Current_RMS_Add 0x0F       /**<   2  Byte*/
#define Power_Add 0x11             /**<   2  Byte*/
/** @} */                          // End of Addresses of Memory map

/**
 * @def AVR_EEPROM_MAXAddress
 * @brief Maximum addressable EEPROM location for Atmega32.
 * @details The Atmega32 microcontroller has 1024 bytes of EEPROM memory,
 *          which means the valid address range is from 0x0000 to 0x03FF.
 */
#define AVR_EEPROM_MAXAddress 1023

#endif
