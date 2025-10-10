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
#include "../../Common/ProjectCfg.h"

/**
 * @def   CompareMatch_InterruptEnable
 * @brief Enble Interrupts on Compare Match
 */
#define CompareMatch_InterruptEnable SetBit(TIMSK_Reg, OCIE0_Bit)

/**
 * @def Timer0_Prescaller
 * @brief Defines the prescaler value for Timer0.
 * @details Sets the clock division factor for Timer0. This affects the timer tick frequency.
 * @note    Must be one of the values defined in the TIMER0_Prescaler enum.
 */
#define Timer0_Prescaller T0_ClkPrescalling_64
/**
 * @def Timer0_PrescallerValue
 * @brief Defines the prescaler value for Timer0.
 * @details Sets the clock division factor for Timer0. This affects the timer tick frequency.
 * @note    Must be one of the values defined in the TIMER0_Prescaler enum.
 */
#define Timer0_PrescallerValue 64
/**
 * @def Timer0_WaveGenMode
 * @brief Selects the waveform generation mode for Timer0.
 * @details Configures Timer0 to operate in CTC (Clear Timer on Compare Match) mode.
 * @note Other modes include Normal and Fast PWM.
 */
#define Timer0_WaveGenMode T0_CTC

/**
 * @def Timer0_CompareOutputMode
 * @brief Sets the compare output behavior for Timer0.
 * @details In Normal Port mode, the OC0 pin is disconnected and used as a regular I/O.
 * @note Used when no PWM or toggling output is needed.
 */

#define Timer0_CompareOutputMode T0_NormalPort

/**
 * @def Timer0_Resoultion
 * @brief Defines the time resolution for Timer0 ticks.
 * @details Sets the base time unit for scheduling tasks, typically in milliseconds.
 * @note Used in calculating the compare match value.
 */

#define Timer0_Resoultion T0_MilliSecond

/**
 * @def IsCOM_FlagSet
 * @brief Checks if the Timer0 Compare Match flag is set.
 * @details Reads the OCF0 bit from the TIFR register to determine if a compare match occurred.
 * @return Boolean value: 1 if set, 0 if not.
 * @note Used in polling-based delay or status checking.
 */

#define IsCOM_FlagSet GetBit(TIFR_Reg, OCF0_Bit)

/**
 * @def T0_ClkTime
 * @brief Calculates the time per Timer0 tick.
 * @details Based on the selected prescaler and system clock frequency.
 * @note Used in determining the compare match value for CTC mode.
 */

#define T0_ClkTime (Timer0_PrescallerValue / (float)F_CPU)

/**
 * @def CompareMatchValue
 * @brief Calculates the OCR0 value for Timer0 in CTC mode.
 * @details Determines the value to load into OCR0 to achieve the desired tick resolution.
 * @note Depends on clock frequency, prescaler, and desired time resolution.
 * @warning Ensure that the result fits within 8-bit range (0–255).
 */

 #define CompareMatchValue 124 //((1.0 / 1000.0) / T0_ClkTime - 1)  // result ==124 but for future if we want to modify it

/**
 * @def     T0_ScheduledTasksNum
 * @brief   Defines the maximum number of concurrent scheduled tasks for Timer0.
 * @details Limits the number of non-blocking delays that can be managed simultaneously.
 * @note    Increase this value if more tasks need to be scheduled.
 * @warning must be careful with ram possible tasks.
 */

#define T0_ScheduledTasksNum 3

// #define T0_ScheduledTasksDefinintions  {};

#endif
