/*
 * Purpose: Simulating (Counting Clicks)
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

/* Global variable to be easily visible in the Watch Window */
volatile uint8_t Counter = 0;

void BTN_Test1_Init()
{
    /* Initialization */
    hBtn_Init();       
    mGIE_Enable(); 
    
    // Visual Feedback
    mDIO_SetDirectionForPin(RGB_RED_GROUP, RGB_RED_PIN, Output);

    // Variables to monitor changes
    uint8_t Previous_Mode = hBtn_GetStatus(); // Initial state
    uint8_t Current_Mode = 0;

    while(1)
    {
        /* Check for Mode Change (Simulation of "Move") */
        Current_Mode = hBtn_GetStatus();

        if (Current_Mode != Previous_Mode)
        {
            Counter++;

            
            // We assume PORTA is unused and not connected to anything
            PORTA_Reg = Counter; 

            Previous_Mode = Current_Mode;

            // Update LED for visual feedback
            if(Current_Mode == 1)
                mDIO_WritePin(RGB_RED_GROUP, RGB_RED_PIN, High);
            else
                mDIO_WritePin(RGB_RED_GROUP, RGB_RED_PIN, Low);
        }
    }
    return 0;
}