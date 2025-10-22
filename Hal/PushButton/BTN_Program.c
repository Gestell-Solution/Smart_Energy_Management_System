/**
 * @file       BTN_Program.c
 * @brief      This file contains The Program for Push Button Module.    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Mohamed Diaa   (                              )
 * @date       2025-10-19
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
*/

#include <stdint.h>
#include <util/delay.h>
#include "../../Mcal/Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"
#include "../../Common/Config.h"


#if Btn_Module==Enable

/* --------------------------------------------------------------------------
*                               Include LIB
* --------------------------------------------------------------------------*/

#include "BTN_Private.h"
#include "BTN_Config.h"
#include "BTN_Interface.h"
#include "../../Mcal/GIE_Interface.h"
#include "../../Mcal/EXTI/EXTI_Interface.h"
#include "../../Mcal/DIO/DIO_Interface.h"

/* --------------------------------------------------------------------------
*                           Global Variable 
* --------------------------------------------------------------------------*/

uint8_t BTN_SystemMode = 0;  // 0 = Auto Mode , 1 = Manual Mode


/* --------------------------------------------------------------------------
*                           Functions Definitions
* --------------------------------------------------------------------------*/

void hBtn_Init(void)
{
    mDIO_SetDirectionForPin(BTN_PORT,BTN_PIN,Input);
    mDIO_WritePin(BTN_PORT,BTN_PIN,High);
    mEXTI_Init(EXTI0,BTN_Sense_Control);
    mEXTI_setCallback(EXTI0, hBtn_ISRAction); 
    mEXTI_Enable(EXTI0);
    GIE_Enable();

}

void hBtn_ISRAction(void)
{
    #if BTN_DebounceEnable == Enable
    
    _delay_ms(BTN_DebounceDelay);
    
    #endif /*BTN_DebounceEnable == Enable*/

    uint8_t BTNState ;
    mDIO_ReadPin(BTN_PORT,BTN_PIN,&BTNState);

    if (BTNState==BTN_Pressed)
    {
        if ( BTN_SystemMode == 1)
            BTN_SystemMode = 0 ; // Manaul -->Automatic Mode
        else if ( BTN_SystemMode == 0)
            BTN_SystemMode = 1 ; // Automatic -->Manual Mode

    }
   
}

uint8_t hBtn_GetStatus(void)
{
    uint8_t BTNSTATE=0;
    mDIO_ReadPin(BTN_PORT,BTN_PIN,&BTNSTATE);

    return BTNSTATE ;
}

#endif /*Btn_Module==Enable */