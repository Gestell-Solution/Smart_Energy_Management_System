/**
 * @file       BTN_Config.h
 * @brief      This file contains Config for Push Button Module.    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Mohamed Diaa   (                              )
 * @date       2025-10-19
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#ifndef _HBUTTON_CONFIG_H_
#define _HBUTTON_CONFIG_H_

/* --------------------------------------------------------------------------
*                               Include LIB
* --------------------------------------------------------------------------*/

#include "../../Mcal/EXTI/EXTI_Private.h"  // To Use EXT_FALLING_EDGE


/* --------------------------------------------------------------------------
*                      Button SenseControl Configuration
* --------------------------------------------------------------------------*/

/**
 * @def        BTN_Sense_Control
 * @brief      Defines button (PIND2)-->EXT0 Interrupt triggers only on falling edge .  
 * 
 * 
*/

#define BTN_Sense_Control  EXT_FALLING_EDGE

/* --------------------------------------------------------------------------
*              BTN Debouncing , Debouncing Delay
* --------------------------------------------------------------------------*/

/**
 * @def   BTN_DebounceEnable
 * @brief Enables or disables debounce.
 * @details Debouncing Occurs in Hardware mainly as the button while Pressing on it vibrates mechanicaly and may do more than one interrupt
 * so we handle this by adding a delay between each press and another press.
 * 
 * Options:
 *   - Enable-----> Software Debounce is active
 *   - Disable----> no debounce filtering
*/

#define BTN_DebounceEnable    Enable

/**
 * @def   BTN_DebounceDelay
 * @brief Sets the debounce delay time (in milliseconds).
 * @details Debounce by giving delay between each press and another press
 * 
 * @note Use This When BTN_Debounce------>Enable
 * 
*/

#define BTN_DebounceDelay   50




#endif /*_HBUTTON_CONFIG_H_*/