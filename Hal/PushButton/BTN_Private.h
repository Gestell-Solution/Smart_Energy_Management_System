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
*                               Include LIB
* --------------------------------------------------------------------------*/
#include <stdint.h>
#include "../../Mcal/EXTI/EXTI_Interface.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "../../Mcal/GIE_Interface.h"

/* --------------------------------------------------------------------------
*                           PORT,PIN Mapping
* --------------------------------------------------------------------------*/

/**
 * @defgroup BTN_Port , BTN_Pin
 * @brief    Defines the Button where it will be connected at What Port & What Pin
*/

#define BTN        1                //Related With EXTI0
#define BTN_PORT GroupD 
#define BTN_PIN  PIN2

/* --------------------------------------------------------------------------
*                           BTN State
* --------------------------------------------------------------------------*/

#define BTN_Pressed     0
#define BTN_NotPressed  1

/* --------------------------------------------------------------------------
*                    Variable Stores System Mode 
* --------------------------------------------------------------------------*/

/**
 * @def   static uint8_t BTN_SystemMode
 * @brief Internal variable storing the system mode.
 * @details
 *  0 → Automatic Mode  
 *  1 → Manual Mode  
*/

extern uint8_t BTN_SystemMode ;


#endif /*_HBUTTON_PRIVATE_H_*/