/*
 * Smart Energy Management Project Schematic Test
 */

#include <stdint.h>
#include <util/delay.h>
#include "../../Src/Common/Macros.h"
#include "../../Src/Mcal/DIO\DIO_Interface.h"
#include "../../Src/Mcal/DIO/DIO_Private.h"
#include "../../Src/Mcal/GIE/GIE_Interface.h"
#include "../../Src/Hal/PushButton/BTN_Interface.h"
#include "../../Src/Hal/RGB/RGB_Config.h" 
#include "../../Src/Mcal/Atmega32RegistersAddress.h"


void BTN_Test2_Init()
{
    /* 1. Initialization */
    hBtn_Init();       //Mode Initailize
    mGIE_Enable();

    mDIO_SetDirectionForPin(RGB_RED_GROUP, RGB_RED_PIN, Output);

    while(1)
    {
        /* Reads CurrentMode*/
        uint8_t currentMode = hBtn_GetStatus();

        /*
         * if 
         * - Manual Mode (1) -> LedAlert ON
         * - Auto Mode (0)   -> LedAlert OFF
         */
        if(currentMode == 1)
        {
            mDIO_WritePin(RGB_RED_GROUP, RGB_RED_PIN, High);
        }
        else
        {
            mDIO_WritePin(RGB_RED_GROUP, RGB_RED_PIN, Low);
        }
    }
    return 0;
}