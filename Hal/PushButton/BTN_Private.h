/**
 * @file       BTN_Private.h
 * @brief      This file contains The Private for Push Button Module.    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Mohamed Diaa   (                              )
 * @date       2025-10-19
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#ifndef _HBUTTON_PRIVATE_H_
#define _HBUTTON_PRIVATE_H_


/* --------------------------------------------------------------------------
 *                           PORT & PIN Mapping
 * --------------------------------------------------------------------------*/

/**
 * @defgroup BTN_Port_Pin Button Hardware Mapping
 * @brief    Defines the port and pin connection for the push button.
 * @{
*/

#define BTN        1                //Related With EXTI0
#define BTN_PORT GroupD 
#define BTN_PIN  PIN2

/** @} */ // End of PORT & Pin Mapping

/* --------------------------------------------------------------------------
*                           BTN State
* --------------------------------------------------------------------------*/

/**
 * @defgroup BTN_States Button Logic States
 * @brief    Defines the logical states of the push button.
 * @{
*/

#define BTN_Pressed     0
#define BTN_NotPressed  1



#endif /*_HBUTTON_PRIVATE_H_*/