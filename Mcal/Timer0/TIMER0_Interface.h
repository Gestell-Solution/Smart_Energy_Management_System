/**
 * @file       TIMER0_Interfacd.h
 * @brief      This file contains APIs Interfacing settings for Timer0 specifications.
 * @details    This header file declare Several functions that are needed for the Functionality of Timer0.
 *             It includes The declarations of the APIs for Timer0 .
 *             It includes The Documentations of the APIs for Timer0.
 * @version    0.2
 * @author     Developer: Mohamed Diaa (mohammeddiaato@gmail.com)
 * @author     Reviewer:  Basma        ()
 * @date       2025-10-07 
 * @copyright  Copyright (c) 2025 , Gestell Company 
 */
#ifndef _TIMER0_Interface_H_
#define _TIMER0_Interface_H_
#include <stdint.h>
#include "TIMER0_Config.h"
#include "TIMER0_Private.h"

void mTIMER0_Init(void);
void mTIMER0_Delay_ms(uint32_t delay_ms);                           //→ blocking delay.
void mTIMER0_StartDelay(uint32_t delay_ms, void (*callback)(void)); // → non-blocking delay with callback.
void mTIMER0_TickHandler(void);                                     //→ called inside ISR to update active delays.

#endif

