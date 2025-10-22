/**
 * @file       RELAY_Interface.h
 * @brief      This file contains Interface for Relay Control Module.    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Alaa Emad      
 * @date       2025-10-21   
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#ifndef _HRELAY_INTERFACE_H_
#define _HRELAY_INTERFACE_H_

/* --------------------------------------------------------------------------
*                               Include LIB
* --------------------------------------------------------------------------*/

#include <stdint.h>
#include "../../Common/Macros.h"
#include "../../Mcal/DIO/DIO_Interface.h"  

/* --------------------------------------------------------------------------
*                           Functions Prototype
* --------------------------------------------------------------------------*/

/**
 * @brief   Initializes the specified relay.
 * @param   relayId   Relay identifier (hRELAY_0 → hRELAY_3).
 * @details This function configures the corresponding relay pin as output
 *          and sets its initial state as defined in RELAY_Config.h.
 */

void hRelay_Init(uint8_t relayId);

/**
 * @brief   Turns ON the specified relay.
 * @param   relayId   Relay identifier (hRELAY_0 → hRELAY_3).
 * @details Activates the relay according to its active state
 *          (HIGH or LOW) defined in RELAY_Config.h.
 */
void hRelay_On(uint8_t relayId);

/**
 * @brief   Turns OFF the specified relay.
 * @param   relayId   Relay identifier (hRELAY_0 → hRELAY_3).
 * @details Deactivates the relay according to its active state
 *          (HIGH or LOW) defined in RELAY_Config.h.
 */
void hRelay_Off(uint8_t relayId);

/**
 * @brief   Toggles the current state of the specified relay.
 * @param   relayId   Relay identifier (hRELAY_0 → hRELAY_3).
 * @details If the relay is ON, it will turn OFF and vice versa.
 */
void hRelay_Toggle(uint8_t relayId);

/**
 * @brief   Returns the current logical state of the specified relay.
 * @param   relayId   Relay identifier (hRELAY_0 → hRELAY_3).
 * @details This function reads the relay pin state and returns
 *          its logical representation (ON/OFF).
 * @return  uint8_t  Relay state (0 = OFF, 1 = ON)
 * 
 */
uint8_t hRelay_GetStatus(uint8_t relayId);






#endif /*_HRELAY_INTERFACE_H_*/