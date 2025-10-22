/** 
 * @file      LCD_Program.c
 * @brief     LCD HAL implementation.
 * @details   Implements the LCD driver: initialization sequence, low-level
 *            command/data transfer routines, high-level helpers (print, clear,
 *            set cursor), timing delays and any required synchronization.
 *            This source uses configuration from LCD_Config.h and private
 *            definitions from LCD_Private.h.
 * @version   1.0.0
 * @author    Mohammed Diaa <mohammeddiaato@gmail.com>
 * @date      2025-10-21
 * @copyright Copyright (c) 2025 , Gestell Company 
 */
#include "LCD_Config.h"
#include "LCD_Interface.h"
#include "LCD_Private.h"
#include <cstdint>
#include <stdint.h>
#include <util/delay.h>


/**
 * @var LinesOfDisplay
 * @brief Holds the starting DDRAM addresses for each LCD row.
 *
 * This static array is initialized using the predefined macro Rows_Addresses_Array in the configuration file,
 * which contains the start addresses for Row 1 to Row 4. It is used to calculate
 * the cursor position on the LCD when setting the DDRAM address.
 */
static uint8_t LinesOfDisplay[NumOfRows] = Rows_Addresses_Array;



void hLCD_Init(void)
{

        mDIO_SetDirectionForPin(LCD_Group, Data_Pin4, Output);
        mDIO_SetDirectionForPin(LCD_Group, Data_Pin5, Output);
        mDIO_SetDirectionForPin(LCD_Group, Data_Pin6, Output);
        mDIO_SetDirectionForPin(LCD_Group, Data_Pin7, Output);
        mDIO_SetDirectionForPin(LCD_Group, RS_Pin, Output);
        mDIO_SetDirectionForPin(LCD_Group, EN_Pin, Output);

        _delay_ms( POWER_ON_WAIT);
        hLCD_SendCommand(Function_Set_Command_1);
        hLCD_SendCommand(Function_Set_Command_2);
        hLCD_SendCommand(Function_Set_Command_3);
        _delay_us( FUNCTION_SET_WAIT);
        hLCD_SendCommand(Display_Control_1);
        hLCD_SendCommand(Display_Control_2);
        _delay_us( DISPLAY_CONTROL_WAIT);
        hLCD_SendCommand(Display_Clear_1);
        hLCD_SendCommand(Display_Clear_2);
        _delay_ms( DISPLAY_CLEAR_WAIT);
        hLCD_SendCommand(Entry_Mode_Set_1);
        hLCD_SendCommand(Entry_Mode_Set_2);
        _delay_ms(MILLI_Second );
 
}
void hLCD_SendCommand(uint8_t Command)
{
    mDIO_WritePin(LCD_Group, RS_Pin, Low);
   
    LCD_Port|= Upper_Nibble_Masking(Command);
    
    mDIO_WritePin(LCD_Group, EN_Pin, High);
    
    _delay_us( Enable_Pulse_Wait);
    mDIO_WritePin(LCD_Group, EN_Pin, Low);
    
    mDIO_WritePin(LCD_Group, RS_Pin, Low);
    
    LCD_Port|= Lower_Nibble_Masking(Command);

    mDIO_WritePin(LCD_Group, EN_Pin, High);

    _delay_us( Enable_Pulse_Wait);

    mDIO_WritePin(LCD_Group, EN_Pin, Low);
}

void hLCD_Clear(uint8_t ClearLine)
{
    hLCD_SetCursor(ClearLine, 0); //Start of the line
    hLCD_WriteString(Clear_Line_String);
}
void hLCD_SetCursor(uint8_t Line, uint8_t Digits)
{
    uint8_t address = LinesOfDisplay[Line-1]+Digits;
    
    hLCD_SendCommand(SET_DDRAM+address);
    
}
void hLCD_WriteChar(char Character)
{
    mDIO_WritePin(LCD_Group, RS_Pin, High);
   
    LCD_Port|= Upper_Nibble_Masking(Character);
    
    mDIO_WritePin(LCD_Group, EN_Pin, High);
    
    _delay_us( Enable_Pulse_Wait);
    mDIO_WritePin(LCD_Group, EN_Pin, Low);
    
    mDIO_WritePin(LCD_Group, RS_Pin, High);
    
    LCD_Port|= Lower_Nibble_Masking(Character);

    mDIO_WritePin(LCD_Group, EN_Pin, High);

    _delay_us( Enable_Pulse_Wait);

    mDIO_WritePin(LCD_Group, EN_Pin, Low);
    
}
void hLCD_WriteString(const char *str)
{

    if (str!=Null){
        uint8_t index=0;
        while (str[index]!=NullChar) {
        
         hLCD_WriteChar(str[index]);
         index++;
        }
    }else {
    
    }
}
void hLCD_WriteNumber(int32_t Number)
{
   if (Number==0) {
        hLCD_WriteChar('0');
        
}else if (Number<0) {
        Number*=-1;
        hLCD_WriteChar('-');
   } 
    
    uint8_t NumArr[Array_Num_Of_Digits];
    uint8_t index=0;
    while (index<Array_Num_Of_Digits) {
                NumArr[index]=Number%10;
                NumArr[index]+='0';
                Number/=10;
        if (Number==0) {
        
                index++;
                break;
        }
    }
    
    while (index--)
     {
       
        hLCD_WriteChar(NumArr[index]);
        
    }
}