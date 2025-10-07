
/**
 * @file       TIMER0_Program.c
 * @brief      This file contains Implementation for the APIs in the Interface.h  for Timer0.
 * @details    This C file defines several Functions that Implement the APIs in defining Specified Actions based on project requirements.
 * @version    0.2
 * @author     Developer: Mohamed Diaa (mohammeddiaato@gmail.com)
 * @author     Reviewer:  Basma        ()
 * @date       2025-10-07 
 * @copyright  Copyright (c) 2025 , Gestell Company 
 */
#include "TIMER0_Interface.h"


void mTIMER0_Init(void)
{

}
void mTIMER0_Delay_ms(uint32_t delay_ms)
{

}                           //→ blocking delay.
void mTIMER0_StartDelay(uint32_t delay_ms, void (*callback)(void))
{

} // → non-blocking delay with callback.
void mTIMER0_TickHandler(void)
{

}                                     //→ called inside ISR to update active delays.
