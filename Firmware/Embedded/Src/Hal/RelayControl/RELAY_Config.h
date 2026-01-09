/**
 * @file       RELAY_Config.h
 * @brief      This file contains Config for Relay Control Module.    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Alaa Emad      
 * @date       2025-10-21   
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#ifndef _HRELAY_CONFIG_H_
#define _HRELAY_CONFIG_H_

/* --------------------------------------------------------------------------
*                               Include LIB
* --------------------------------------------------------------------------*/

#include <stdint.h>
#include "../../Common/Macros.h"
#include "../../Mcal/DIO/DIO_Interface.h"  


/* --------------------------------------------------------------------------
*                               Relay Number
* --------------------------------------------------------------------------*/


/**
 * @def hRELAY_NUM
 * @brief Defines the number of relays used in the project.
 * @note Adjust this value according to the number of relays connected.
 */

#define hRELAY_NUM    4

/* --------------------------------------------------------------------------
*                 Relay Identifiers , Active State Logic (High,Low)
* --------------------------------------------------------------------------*/

/**
 * @defgroup        hRELAY_X
 * @brief      Defines Relay identifiers (Relay 0 → Relay 3).
 * @details    These macros are used as indexes to refer to specific relays in
 *             the application code or relay configuration arrays.
*/

#define hRELAY_0    0
#define hRELAY_1    1
#define hRELAY_2    2
#define hRELAY_3    3



/* --------------------------------------------------------------------------
*                           Pin Mapping
* --------------------------------------------------------------------------*/

/**
 * @defgroup   hRELAY_PinMapping  Relay Pin Mapping Configuration
 * @brief      Defines hardware mapping for each relay.
 * @details    
 * This section maps each relay to its corresponding MCU port and pin.
 *  its initial state after system initialization.
 * 
 *
 * - hRELAY_X_PORT         -->The MCU GPIO port connected to the relay.
 * - hRELAY_X_PIN          -->The specific pin number controlling the relay.
 * - hRELAY_X_INIT_STATE   --> Sets the default relay state (ON/OFF) at startup.
*/


/*Relay 0*/
#define hRELAY_0_PORT                  GroupB
#define hRELAY_0_PIN                   PIN0
#define hRELAY_0_INIT_STATE            Low

/*Relay 1*/
#define hRELAY_1_PORT                  GroupB
#define hRELAY_1_PIN                   PIN1
#define hRELAY_1_INIT_STATE            Low

/*Relay 2*/
#define hRELAY_2_PORT                  GroupB
#define hRELAY_2_PIN                   PIN2
#define hRELAY_2_INIT_STATE            Low

/*Relay 3*/
#define hRELAY_3_PORT                  GroupB
#define hRELAY_3_PIN                   PIN3
#define hRELAY_3_INIT_STATE            Low




#endif /*_HRELAY_CONFIG_H_*/