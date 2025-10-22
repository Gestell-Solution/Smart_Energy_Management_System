/** 
 * @file      LCD_Config.h
 * @brief     Configuration definitions for the LCD HAL.
 * @details   Provides compile-time configuration macros for the LCD hardware
 *            abstraction layer: pin mappings, interface mode (4/8-bit), timing
 *            constants and default options used by the LCD driver implementation.
 * @version   1.0.0
 * @author    Mohammed Diaa <mohammeddiaato@gmail.com>
 * @date      2025-10-21
 * @copyright Copyright (c) 2025 , Gestell Company 
 */
#ifndef _LCD_CONFIG_H_
#define _LCD_CONFIG_H_
#include "LCD_Private.h"
#include "../../Mcal\DIO\DIO_Interface.h"
/** 
 * @def Clear_Line_String 
 * @brief String used to clear a line on the LCD.
 */
#define  Clear_Line_String "               "

/** 
 * @def LCD_Group 
 * @brief Defines the group used for LCD control.
 */
#define LCD_Group GroupC

/** 
 * @def Upper_Nibble_Masking 
 * @brief Extracts the upper nibble (4 bits) from a register.
 */
#define Upper_Nibble_Masking(Reg) ((Reg>>4)&0x0F)

/** 
 * @def Lower_Nibble_Masking 
 * @brief Extracts the lower nibble (4 bits) from a register.
 */
#define Lower_Nibble_Masking(Reg) ((Reg)&0x0F)


/** 
 * @def NumOfRows 
 * @brief Total number of display rows.
 */
#define NumOfRows 4

/** 
 * @def Rows_Addresses_Array 
 * @brief Array of starting addresses for each LCD row.
 */
#define Rows_Addresses_Array {Row_1_Start,Row_2_Start,Row_3_Start,Row_4_Start}


/** 
 * @def Array_Num_Of_Digits 
 * @brief Maximum number of digits for a 32-bit unsigned integer.
 */
#define Array_Num_Of_Digits 10 

#endif
