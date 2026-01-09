/**
 * @file       RELAY_Program.c
 * @brief      Implementation of the Relay Control Driver.
 * @details    This module handles the low-level activation and deactivation of relays.
 *             It maps abstract relay IDs (e.g., hRELAY_0) to specific physical ports and pins.
 *             It abstracts the active-high/active-low logic defined in the configuration.
 * @version    1.0
 * @author     Developer: Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer:  Eng - Alaa Emad
 * @date       2025-10-21
 * @copyright  Copyright (c) 2025, Gestell Company
 */

/*============================================================================
 *                                 Includes
 *============================================================================*/
#include <stdint.h>
#include "../../Mcal/Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"
#include "../../Common/Config.h"

#if Relay_Module == Enable

#include "RELAY_Interface.h"
#include "RELAY_Config.h"
#include "RELAY_Private.h"
#include "../../Mcal/DIO/DIO_Interface.h"

/*============================================================================
 *                                 Function Definitions
 *============================================================================*/

/**
 * @brief      Initializes the specified relay.
 * @details    Sets the direction of the relay pin to OUTPUT and writes the initial state.
 * @param[in]  relayId  Legacy ID of the relay (hRELAY_0 ... hRELAY_3).
 * @return     void
 */
void hRelay_Init(uint8_t relayId)
{
    switch (relayId)
    {
    case hRELAY_0:
       mDIO_SetDirectionForPin(hRELAY_0_PORT, hRELAY_0_PIN, Output);
       mDIO_WritePin(hRELAY_0_PORT, hRELAY_0_PIN, hRELAY_0_INIT_STATE);
       break;
    case hRELAY_1:
       mDIO_SetDirectionForPin(hRELAY_1_PORT, hRELAY_1_PIN, Output);
       mDIO_WritePin(hRELAY_1_PORT, hRELAY_1_PIN, hRELAY_1_INIT_STATE);   
       break; 
    case hRELAY_2:
       mDIO_SetDirectionForPin(hRELAY_2_PORT, hRELAY_2_PIN, Output);
       mDIO_WritePin(hRELAY_2_PORT, hRELAY_2_PIN, hRELAY_2_INIT_STATE);
       break;  
    case hRELAY_3:
       mDIO_SetDirectionForPin(hRELAY_3_PORT, hRELAY_3_PIN, Output);
       mDIO_WritePin(hRELAY_3_PORT, hRELAY_3_PIN, hRELAY_3_INIT_STATE);  
       break;

    default:
        /* Error: Relay Id Out of Range */
        break;
    }
}

/**
 * @brief      Turns the relay ON.
 * @details    Writes the ACTIVE state (inverse of INIT if INIT is OFF, or typically High/Low depending on circuit).
 *             Logic assumes '!' initial state is ON, or needs explicit Active State config.
 *             Here, it toggles from Init State.
 * @param[in]  relayId  Relay ID.
 * @return     void
 */
void hRelay_On(uint8_t relayId)
{
    switch (relayId)
    {
    case hRELAY_0:
        mDIO_WritePin(hRELAY_0_PORT, hRELAY_0_PIN, !hRELAY_0_INIT_STATE);
        break;
    case hRELAY_1:
        mDIO_WritePin(hRELAY_1_PORT, hRELAY_1_PIN, !hRELAY_1_INIT_STATE);   
        break; 
    case hRELAY_2:
        mDIO_WritePin(hRELAY_2_PORT, hRELAY_2_PIN, !hRELAY_2_INIT_STATE);
        break;  
    case hRELAY_3:
        mDIO_WritePin(hRELAY_3_PORT, hRELAY_3_PIN, !hRELAY_3_INIT_STATE);  
        break;
    default:
        break;
    }    
}

/**
 * @brief      Turns the relay OFF.
 * @details    Restores the relay pin to its Initial (Inactive) state.
 * @param[in]  relayId  Relay ID.
 * @return     void
 */
void hRelay_Off(uint8_t relayId)
{
    switch (relayId)
    {
    case hRELAY_0:
        mDIO_WritePin(hRELAY_0_PORT, hRELAY_0_PIN, hRELAY_0_INIT_STATE);
        break;
    case hRELAY_1:
        mDIO_WritePin(hRELAY_1_PORT, hRELAY_1_PIN, hRELAY_1_INIT_STATE);   
        break; 
    case hRELAY_2:
        mDIO_WritePin(hRELAY_2_PORT, hRELAY_2_PIN, hRELAY_2_INIT_STATE);
        break;  
    case hRELAY_3:
        mDIO_WritePin(hRELAY_3_PORT, hRELAY_3_PIN, hRELAY_3_INIT_STATE);  
        break;
    default:
        break;
    }    
}

/**
 * @brief      Toggles the relay state.
 * @param[in]  relayId  Relay ID.
 * @return     void
 */
void hRelay_Toggle(uint8_t relayId)
{
    switch (relayId)
    {
    case hRELAY_0:
        mDIO_TogglePin(hRELAY_0_PORT, hRELAY_0_PIN);
        break;
    case hRELAY_1:
        mDIO_TogglePin(hRELAY_1_PORT, hRELAY_1_PIN);   
        break; 
    case hRELAY_2:
        mDIO_TogglePin(hRELAY_2_PORT, hRELAY_2_PIN);
        break;  
    case hRELAY_3:
        mDIO_TogglePin(hRELAY_3_PORT, hRELAY_3_PIN);  
        break;
    default:
        break;
    }
}

/**
 * @brief      Reads the physical state of the relay pin.
 * @param[in]  relayId  Relay ID.
 * @return     uint8_t  Low(0) or High(1).
 */
uint8_t hRelay_GetStatus(uint8_t relayId)
{
    uint8_t hRELAY_PinVal = Low;
    switch (relayId)
    {
    case hRELAY_0:
        mDIO_ReadPin(hRELAY_0_PORT, hRELAY_0_PIN, &hRELAY_PinVal);
        break;
    case hRELAY_1:
        mDIO_ReadPin(hRELAY_1_PORT, hRELAY_1_PIN, &hRELAY_PinVal);   
        break; 
    case hRELAY_2:
        mDIO_ReadPin(hRELAY_2_PORT, hRELAY_2_PIN, &hRELAY_PinVal);
        break;  
    case hRELAY_3:
        mDIO_ReadPin(hRELAY_3_PORT, hRELAY_3_PIN, &hRELAY_PinVal);  
        break;
    default:
        break;
    }    
    return hRELAY_PinVal;
}

#endif /* Relay_Module == Enable */


