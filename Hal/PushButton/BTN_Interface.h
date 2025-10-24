/**
 * @file       BTN_Interface.h
 * @brief      This file contains The Interface for Push Button Module.    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Mohamed Diaa   (                              )
 * @date       2025-10-19
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
*/

#ifndef _HBUTTON_INTERFACE_H_
#define _HBUTTON_INTERFACE_H_

/* --------------------------------------------------------------------------
*                               Include LIB
* --------------------------------------------------------------------------*/
#include <stdint.h>
#include "../../Common/Macros.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "../../Mcal/EXTI/EXTI_Interface.h"
#include "../../Mcal/GIE_Interface.h"
#include "BTN_Private.h"
#include "BTN_Config.h"

/* --------------------------------------------------------------------------
*                             Functions ProtoType
* --------------------------------------------------------------------------*/

/**
 * @fn      void BTN_Init(void)
 * @brief   Initializes the Push Button Module.
 * @details 
 *  - Configure button pin (PD2) as input.  
 *  - Enable internal pull-up resistor.  
 *  - Initialize EXTI0 interrupt on falling edge.  
 *  - Enable global interrupt.
 */

void hBtn_Init(void);


/**
 * @fn      void BTN_ISRAction(void)
 * @brief   Function executed when the button interrupt occurs.
 * @details 
 *  - Handles debounce (if enabled).  
 *  - Toggles the system mode between Automatic and Manual.  
 */

void hBtn_ISRAction(void);

/**
 * @fn      uint8_t BTN_GetStatus(void)
 * @brief   Returns the current button state.
 * @return  
 *   - BTN_Pressed  
 *   - BTN_NotPressed  
 */

uint8_t hBtn_GetStatus(void);


/* --------------------------------------------------------------------------
*                           Global Variable
* --------------------------------------------------------------------------*/

/**
 * @brief Global variable storing current system mode.
 * @details 
 * - 0 → Automatic Mode  
 * - 1 → Manual Mode  
 * @note Declared here so that application (main) can read the current mode.
*/
extern uint8_t BTN_SystemMode;



#endif /*_HBUTTON_INTERFACE_H_*/