/**
 * @file       RELAY_Program.c
 * @brief      This file contains Program for Relay Control Module.    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Alaa Emad      
 * @date       2025-10-21   
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */




/* --------------------------------------------------------------------------
*                               Include LIB
* --------------------------------------------------------------------------*/

#include <stdint.h>
#include "../../Mcal/Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"
#include "../../Common/Config.h"

#if Relay_Module == Enable

#include "RELAY_Interface.h"
#include "RELAY_Config.h"
#include "RELAY_Private.h"
#include "../../Mcal/DIO/DIO_Interface.h"

/* --------------------------------------------------------------------------
*                               Functions Definitions
* --------------------------------------------------------------------------*/

void hRelay_Init(uint8_t relayId)
{
    switch (relayId)
    {
    case hRELAY_0 :
       mDIO_SetDirectionForPin(hRELAY_0_PORT,hRELAY_0_PIN,Output);
       mDIO_WritePin(hRELAY_0_PORT,hRELAY_0_PIN,hRELAY_0_INIT_STATE);
        break;
    case hRELAY_1 :
       mDIO_SetDirectionForPin(hRELAY_1_PORT,hRELAY_1_PIN,Output);
       mDIO_WritePin(hRELAY_1_PORT,hRELAY_1_PIN,hRELAY_1_INIT_STATE);   
       break; 
    case hRELAY_2 :
       mDIO_SetDirectionForPin(hRELAY_2_PORT,hRELAY_2_PIN,Output);
       mDIO_WritePin(hRELAY_2_PORT,hRELAY_2_PIN,hRELAY_2_INIT_STATE);
       break;  
    case hRELAY_3 :
       mDIO_SetDirectionForPin(hRELAY_3_PORT,hRELAY_3_PIN,Output);
       mDIO_WritePin(hRELAY_3_PORT,hRELAY_3_PIN,hRELAY_3_INIT_STATE);  
       break;

    default:
    /**
     * @todo Relay Id Out of Range
    */
        break;
    }
}


void hRelay_On(uint8_t relayId)
{
    switch (relayId)
    {
    case hRELAY_0 :
        mDIO_WritePin(hRELAY_0_PORT,hRELAY_0_PIN,!hRELAY_0_INIT_STATE);
        break;
    case hRELAY_1 :
        mDIO_WritePin(hRELAY_1_PORT,hRELAY_1_PIN,!hRELAY_1_INIT_STATE);   
        break; 
    case hRELAY_2 :
        mDIO_WritePin(hRELAY_2_PORT,hRELAY_2_PIN,!hRELAY_2_INIT_STATE);
        break;  
    case hRELAY_3 :
        mDIO_WritePin(hRELAY_3_PORT,hRELAY_3_PIN,!hRELAY_3_INIT_STATE);  
        break;

    default:
    /**
     * @todo Relay Id Out of Range
    */
        break;
    }    
}

void hRelay_Off(uint8_t relayId)

{

    switch (relayId)
    {
    case hRELAY_0 :
        mDIO_WritePin(hRELAY_0_PORT,hRELAY_0_PIN,hRELAY_0_INIT_STATE);
        break;
    case hRELAY_1 :
        mDIO_WritePin(hRELAY_1_PORT,hRELAY_1_PIN,hRELAY_1_INIT_STATE);   
        break; 
    case hRELAY_2 :
        mDIO_WritePin(hRELAY_2_PORT,hRELAY_2_PIN,hRELAY_2_INIT_STATE);
        break;  
    case hRELAY_3 :
        mDIO_WritePin(hRELAY_3_PORT,hRELAY_3_PIN,hRELAY_3_INIT_STATE);  
        break;

    default:
    /**
     * @todo Relay Id Out of Range
    */
        break;
    }    
  
}

void hRelay_Toggle(uint8_t relayId)
{
    switch (relayId)
    {
    case hRELAY_0 :
        
        mDIO_TogglePin(hRELAY_0_PORT,hRELAY_0_PIN);
        break;
    case hRELAY_1 :
        mDIO_TogglePin(hRELAY_1_PORT,hRELAY_1_PIN);   
        break; 
    case hRELAY_2 :
        mDIO_TogglePin(hRELAY_2_PORT,hRELAY_2_PIN);
        break;  
    case hRELAY_3 :
        mDIO_TogglePin(hRELAY_3_PORT,hRELAY_3_PIN);  
        break;

    default:
    /**
     * @todo Relay Id Out of Range
    */
        break;
    }
}

uint8_t hRelay_GetStatus(uint8_t relayId)
{
    uint8_t hRELAY_PinVal=Low ;
    switch (relayId)
    {
    case hRELAY_0 :
        mDIO_ReadPin(hRELAY_0_PORT,hRELAY_0_PIN,&hRELAY_PinVal);
        break;
    case hRELAY_1 :
        mDIO_ReadPin(hRELAY_1_PORT,hRELAY_1_PIN,&hRELAY_PinVal);   
        break; 
    case hRELAY_2 :
        mDIO_ReadPin(hRELAY_2_PORT,hRELAY_2_PIN,&hRELAY_PinVal);
        break;  
    case hRELAY_3 :
        mDIO_ReadPin(hRELAY_3_PORT,hRELAY_3_PIN,&hRELAY_PinVal);  
        break;

    default:
    /**
     * @todo Relay Id Out of Range
    */
        break;
    }    
    return hRELAY_PinVal;
}

#endif /*Relay_Module == Enable*/


