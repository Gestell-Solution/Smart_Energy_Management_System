/** 
 * @file      LCD_Interface.h
 * @brief     Public API for the LCD hardware abstraction layer.
 * @details   Declares the functions, types and configuration structures that
 *            application code uses to initialize the LCD, write text, control
 *            cursor and backlight, and perform higher-level display operations.
 *            Keep only stable, public-facing declarations in this header.
 * @version   1.0.0
 * @author    Mohammed Diaa <mohammeddiaato@gmail.com>
 * @date      2025-10-21
 * @copyright Copyright (c) 2025 , Gestell Company 
 */
#ifndef _LCD_INTERFACE_H_
#define _LCD_INTERFACE_H_

/**
 * @addtogroup Hal
 * @{
 * @addtogroup LCD_Display
 * @{
 */

#include <stdint.h>
#include "LCD_Config.h"
#include "LCD_Private.h"
#include "../../Common/Macros.h"
#include "../../Mcal/DIO/DIO_Interface.h"
/**
 * @fn     hLCD_Init
 * @brief  Initialize the LCD in the configured interface mode (e.g. 4-bit).
 * @details Performs the required power-up and initialization sequence for the
 *          LCD controller (function set, display on/off, clear display, entry
 *          mode set). Should be called once before any other LCD API.
 * @return void
 * @note   Timing delays required by the LCD controller are handled inside this
 *         function. Do not call other LCD functions until initialization
 *         completes.
 */
void hLCD_Init(void);

/**
 * @fn     hLCD_Clear
 * @brief  Clear particular line or the whole display.
 * @details Clears the requested display line or the entire display. The exact
 *          parameter values (line indices or a special CLEAR_ALL macro) are
 *          defined in LCD_Config.h. Calling this will also position the cursor
 *          according to the controller's clear behavior (usually home).
 * @param  ClearLine  Identifier for which line to clear (e.g. 0..3 for line
 *                    indices) or a predefined macro (e.g. LCD_CLEAR_ALL) to
 *                    clear the whole display.
 * @return void
 * @note   Use the macros in LCD_Config.h for readable code instead of numeric
 *         literals.
 */
void hLCD_Clear(uint8_t ClearLine);

/**
 * @fn     hLCD_SetCursor
 * @brief  Position the cursor at given row and column.
 * @details Sets the LCD cursor to the requested row and column. Valid row and
 *          column ranges depend on the physical display (common: rows 0..3,
 *          columns 0..15). If out-of-range values are provided, behavior is
 *          controller-dependent (may wrap or be clamped).
 * @param  Line    Row index (0-based).
 * @param  Digits  Column index (0-based).
 * @return void
 * @warning Ensure Line and Digits are within the supported ranges for your
 *          display. Call hLCD_Init() before using this function.
 */
void hLCD_SetCursor(uint8_t Line, uint8_t Digits);

/**
 * @fn     hLCD_WriteChar
 * @brief  Write a single character at the current cursor position.
 * @details Sends the given character code to the LCD as data. After writing,
 *          the cursor advances according to the current entry mode.
 * @param  Character  ASCII character to display.
 * @return void
 * @note   Character encoding is ASCII; custom characters must be loaded via
 *         the driver if supported.
 */
void hLCD_WriteChar(char Character);

/**
 * @fn     hLCD_WriteString
 * @brief  Write a null-terminated string starting at the current cursor.
 * @details Iterates over the provided null-terminated C string and writes each
 *          character sequentially. The function will stop at the terminating
 *          '\\0' or when the display limits are reached.
 * @param  string  Pointer to a null-terminated string to display.
 * @return void
 * @warning The string must be null-terminated. Long strings may overflow the
 *          visible display; handle wrapping or truncation in application code
 *          or extend the driver.
 */
void hLCD_WriteString(const char *str);

/**
 * @fn     hLCD_WriteNumber
 * @brief  Convert an integer to ASCII and write it to the display.
 * @details Converts the signed 32-bit integer 'Number' to its decimal ASCII
 *          representation and writes the characters starting at the current
 *          cursor position. Negative numbers include a leading '-' sign.
 * @param  Number  32-bit signed integer to display.
 * @return void
 * @note   Function uses decimal base. For other bases, convert to string in
 *         application code and call hLCD_WriteString().
 */
void hLCD_WriteNumber(int32_t Number);


/**
 * @fn     hLCD_SendCommand
 * @brief  Send a raw instruction/command byte to the LCD controller.
 * @details Writes the provided command byte to the LCD as a controller instruction.
 *          In 4-bit mode the function transmits the high nibble followed by the
 *          low nibble with the required enable strobes and timing. RS is cleared
 *          for command mode.
 * @param  Command  Command byte to send to the LCD (see HD44780 instruction set).
 * @return void
 * @note   Call hLCD_Init() before using this function.
 */
void hLCD_SendCommand(uint8_t Command);


/**
 * @}
 * @}
 */

#endif