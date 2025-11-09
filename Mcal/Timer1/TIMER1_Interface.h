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
#ifndef _TIMER1_INTERFACE_H_
#define _TIMER1_INTERFACE_H_
#include <stdint.h>
#include "TIMER1_Config.h"

/**
 * @fn      mTIMER1_Init
 * @brief   Initializes Timer1 in CTC (Clear Timer on Compare Match) mode.
 * @details This function configures Timer1 registers with predefined settings for CTC mode.
 *          It sets the waveform generation mode, compare match behavior, and prescaler options.
 * @return  void
 * @note    This function must be called before starting the timer or registering a callback.
 */
void mTIMER1_Init(void);

/**
 * @fn      mTIMER1_Start
 * @brief   Starts Timer1 operation.
 * @details Enables the clock source for Timer1 based on the configured prescaler,
 *          allowing the timer to begin counting.
 * @return  void
 * @note    Timer must be initialized using mTIMER1_Init() before calling this function.
 */
void mTIMER1_Start(void);

/**
 * @fn      mTIMER1_Stop
 * @brief   Stops Timer1 operation.
 * @details Disables the clock source for Timer1, halting its counting operation.
 * @return  void
 * @note    This function can be used to pause or disable Timer1 temporarily.
 */
void mTIMER1_Stop(void);

/**
 * @fn      mTIMER1_RegisterCallback
 * @brief   Registers a callback function for Timer1 Compare Match interrupt.
 * @details This function allows the user to provide a custom function that will be called
 *          when a Compare Match interrupt occurs (e.g., OCR1A match).
 * @param   callback Pointer to the user-defined function to be executed in the ISR.
 * @return  void
 * @retval  None
 * @warning Ensure that global interrupts are enabled and Timer1 is properly configured
 *          before relying on the callback mechanism.
 */
void mTIMER1_RegisterCallback(void (*callback)(void));


#endif
