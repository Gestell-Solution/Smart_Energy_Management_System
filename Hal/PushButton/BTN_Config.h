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
 * @def   BTN_Sense_Control
 * @brief Defines the EXTI trigger mode for button input (PD2 → EXTI0).
 * @details 
 * The button interrupt will trigger only on the falling edge.
 */

#define BTN_Sense_Control  EXT_FALLING_EDGE

/* --------------------------------------------------------------------------
*              BTN Debouncing , Debouncing Delay
* --------------------------------------------------------------------------*/

/**
 * @def   BTN_DebounceEnable
 * @brief Enables or disables software debounce filtering.
 * @details
 * Mechanical buttons can generate multiple interrupts due to contact vibration.  
 * Debouncing adds a short delay to filter false triggers.
 * 
 * **Options:**
 * - `Enable` → Activate software debounce.  
 * - `Disable` → No debounce filtering.
 */

#define BTN_DebounceEnable    Enable

/**
 * @def   BTN_DebounceDelay
 * @brief Defines the debounce delay time in milliseconds.
 * @note  Used only if `BTN_DebounceEnable` is set to `Enable`.
 */

#define BTN_DebounceDelay   50




#endif /*_HBUTTON_CONFIG_H_*/