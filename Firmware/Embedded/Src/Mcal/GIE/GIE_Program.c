/**
 * @file       GIE_Program.c
 * @brief      
 * @details    
 * @version    0.1
 * @author     Hesham Ahmed (Hisham4Ahmed@gmail.com)
 * @date       2025-10-13
 * @copyright  Copyright (c) 2025 , Gestell Company 
 *   
 */

#include "GIE_Interface.h"



void mGIE_Enable(void)
{
    SetBit(SREG_Reg,I_Bit);
}
void mGIE_Disable(void)
{
    ClearBit(SREG_Reg,I_Bit);
}