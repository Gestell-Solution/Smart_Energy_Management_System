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
/**
 * @fn         mTIMER0_Init
 * @brief      Initializes Timer0 peripheral.
 * @details    Configures Timer0 with the required prescaler and mode settings.
 *             This function must be called before using any Timer0 delay functions.
 * @return     void
 * @note       This function does not start the timer; it only configures it.
 */
void mTIMER0_Init(void);
/**
 * @fn         mTIMER0_Delay_ms
 * @brief      Generates a blocking delay using Timer0.
 * @details    Uses Timer0 to create a delay in milliseconds. The function blocks
 *             execution until the delay is complete.
 * @param      delay_ms Duration of the delay in milliseconds.
 * @return     void
 * @warning    This function blocks the CPU. Use non-blocking delay for multitasking.
 * @note       Timer0 must be initialized using mTIMER0_Init() before calling this.
 */
void mTIMER0_Delay_ms(uint32_t delay_ms); 
/**
 * @fn         mTIMER0_StartDelay
 * @brief      Starts a non-blocking delay using Timer0 with a callback.
 * @details    Configures Timer0 to generate a delay in milliseconds. When the delay
 *             is complete, the provided callback function is executed from the ISR context.
 * @param      delay_ms Duration of the delay in milliseconds.
 * @param      callback Pointer to the function to be called when the delay ends.
 * @return     void
 * @warning    The callback function must be short and fast, as it runs inside the ISR.
 * @note       Timer0 must be initialized using mTIMER0_Init() before calling this.
 */
void mTIMER0_StartDelay(uint32_t delay_ms, void (*callback)(void));

/**
 * @fn         mTIMER0_TickHandler
 * @brief      Updates internal delay counters for Timer0.
 * @details    This function should be called inside the Timer0 ISR to manage active
 *             non-blocking delays and trigger callbacks when needed.
 * @return     void
 * @note       This function is part of the non-blocking delay mechanism.
 */
void mTIMER0_TickHandler(void);
/**
 * @fn         __vector_10(void)
 * @brief      ISR Function 
 * @details    This Function Calls the Tick handler to do the ISR Action for 
 *             non-blocking delays and trigger callbacks when needed.
 * @return     void
 */
       
void __vector_10(void) __attribute__((signal));
#endif
