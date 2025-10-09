/**
 * @file       EXTI_Interface.h
 * @brief      This file contains External interrupt Interface .
 * @details    File Contains Implementations of APIs:
 *                 - mEXTI_Init(uint8_t source, uint8_t senseControl);
 *                 - mEXTI_Enable(uint8_t source);
 *                 - mEXTI_Disable(uint8_t source);
 *                 - mEXTI_setCallback(uint8_t source, void (*PF)(void));
 *                  
 * @version    0.1
 * @author     Moahmed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @date       2025-10-09
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */
#include <stdint.h>
#include "D:\Gestell Projects\Smart_Energy_Management_System\Mcal\Atmega32RegistersAddress.h"
#include "D:\Gestell Projects\Smart_Energy_Management_System\Common\Macros.h"
#include "EXTI_Private.h"
#include "EXTI_Config.h"


 #ifndef   _EXTI_INTERFACE_H_
 #define   _EXTI_INTERFACE_H_


void mEXTI_Init(uint8_t source, uint8_t senseControl);
void mEXTI_setCallback(uint8_t source, void (*PF)(void));
void mEXTI_Enable(uint8_t source);
void mEXTI_Disable(uint8_t source);




 #endif /* _EXTI_INTERFACE_H_ */

