/**
 * @file       TIMER0_Config.h
 * @brief      This file contains configuration settings for Timer0 specifications.
 * @details    This header file defines several macros that allow for defining Conifgurations based on project requirements.
 *             It includes configuration settings for Timer0 Settings.
 * @version    0.2
 * @author     Developer: Mohamed Diaa (mohammeddiaato@gmail.com)
 * @author     Reviewer:  Basma        ()
 * @date       2025-10-07 
 * @copyright  Copyright (c) 2025 , Gestell Company 
 */
#ifndef _TIMER0_CONFIG_H_
#define _TIMER0_CONFIG_H_
#include "../../Common/Macros.h"
#include "../Atmega32RegistersAddress.h"
#include "TIMER0_Private.h"

/**
 * @def   CompareMatch_InterruptEnable
 * @brief Enble Interrupts on Compare Match
 */
#define CompareMatch_InterruptEnable SetBit(TIMSK_Reg,OCIE0_Bit)


#endif
