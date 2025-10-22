/** 
 * @file      LCD_Private.h
 * @brief     Private definitions and internals for the LCD HAL.
 * @details   Contains private macros, static helper declarations, register-level
 *            definitions, internal types and constants that must not be exposed
 *            to application code. Use this header only inside the LCD driver
 *            implementation (LCD_Program.c).
 * @version   1.0.0
 * @author    Mohammed Diaa <mohammeddiaato@gmail.com>
 * @date      2025-10-21
 * @copyright Copyright (c) 2025 , Gestell Company 
 */
#ifndef _LCD_PRIVATE_H_
#define _LCD_PRIVATE_H_

/** 
 * @def Row_1_Start 
 * @brief Start address of Row 1 in DDRAM.
 */
#define Row_1_Start  0x00

/** 
 * @def Row_1_End 
 * @brief End address of Row 1 in DDRAM.
 */
#define Row_1_End    0x0F

/** 
 * @def Row_2_Start 
 * @brief Start address of Row 2 in DDRAM.
 */
#define Row_2_Start  0x40

/** 
 * @def Row_2_End 
 * @brief End address of Row 2 in DDRAM.
 */
#define Row_2_End    0x4F

/** 
 * @def Row_3_Start 
 * @brief Start address of Row 3 in DDRAM.
 */
#define Row_3_Start  0x10

/** 
 * @def Row_3_End 
 * @brief End address of Row 3 in DDRAM.
 */
#define Row_3_End    0x1F

/** 
 * @def Row_4_Start 
 * @brief Start address of Row 4 in DDRAM.
 */
#define Row_4_Start  0x50

/** 
 * @def Row_4_End 
 * @brief End address of Row 4 in DDRAM.
 */
#define Row_4_End    0x5F





/** 
 * @def MILLI_Second 
 * @brief Base unit for millisecond delays.
 */
#define MILLI_Second    1

/** 
 * @def Enable_Pulse_Wait 
 * @brief Delay for enable pulse in microseconds.
 */
#define Enable_Pulse_Wait 2*MILLI_Second

/** 
 * @def POWER_ON_WAIT 
 * @brief Delay after power-on in milliseconds.
 */
#define POWER_ON_WAIT 45*MILLI_Second

/** 
 * @def FUNCTION_SET_WAIT 
 * @brief Delay after function set command in microseconds.
 */
#define FUNCTION_SET_WAIT    100*MILLI_Second
/** 
 * @def DISPLAY_CONTROL_WAIT 
 * @brief Delay after display control command in microseconds.
 */
#define DISPLAY_CONTROL_WAIT 100*MILLI_Second

/** 
 * @def DISPLAY_CLEAR_WAIT 
 * @brief Delay after display clear command in milliseconds.
 */
#define DISPLAY_CLEAR_WAIT   10*MILLI_Second



/** 
 * @def Function_Set_Command_1 
 * @brief First function set command.
 */
#define Function_Set_Command_1 0x02

/** 
 * @def Function_Set_Command_2 
 * @brief Second function set command.
 */
#define Function_Set_Command_2 0x02

/** 
 * @def Function_Set_Command_3 
 * @brief Third function set command (2 lines, 5x8 dots).
 */
#define Function_Set_Command_3 0x08

/** 
 * @def Display_Control_1 
 * @brief First display control command.
 */
#define Display_Control_1 0x00

/** 
 * @def Display_Control_2 
 * @brief Second display control command (Display ON, Cursor ON, Blink ON).
 */
#define Display_Control_2 0x0F

/** 
 * @def Display_Clear_1 
 * @brief First display clear command.
 */
#define Display_Clear_1     0x00

/** 
 * @def Display_Clear_2 
 * @brief Second display clear command.
 */
#define Display_Clear_2     0x01

/** 
 * @def Entry_Mode_Set_1 
 * @brief First entry mode set command.
 */
#define Entry_Mode_Set_1    0x00

/** 
 * @def Entry_Mode_Set_2 
 * @brief Second entry mode set command (increment mode).
 */
#define Entry_Mode_Set_2    0x06
/** 
 * @def SET_DDRAM 
 * @brief Base command to set DDRAM address.
 */
#define SET_DDRAM 0x80



/** 
 * @def LCD_Port_Output 
 * @brief Port used for LCD data and control.
 */
#define LCD_Port_Output PORTC_Reg

/** 
 * @def Data_Pin4 
 * @brief LCD data pin D4.
 */
#define Data_Pin4 PIN0

/** 
 * @def Data_Pin5 
 * @brief LCD data pin D5.
 */
#define Data_Pin5 PIN1

/** 
 * @def Data_Pin6 
 * @brief LCD data pin D6.
 */
#define Data_Pin6 PIN2

/** 
 * @def Data_Pin7 
 * @brief LCD data pin D7.
 */
#define Data_Pin7 PIN3

/** 
 * @def RS_Pin 
 * @brief LCD Register Select pin.
 */
#define RS_Pin PIN4

/** 
 * @def EN_Pin 
 * @brief LCD Enable pin.
 */
#define EN_Pin PIN5

#endif