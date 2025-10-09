/**
 * @file       TIMER1_Config.h
 * @brief      Configuration settings for Timer1 specifications.
 * @details    This header file defines macros and constants for configuring Timer1
 *             based on project requirements, including prescaler, waveform mode,
 *             compare output behavior, and interrupt settings.
 * @version    0.3
 * @author     Developer: Mohamed Diaa (mohammeddiaato@gmail.com)
 * @author     Reviewer:  Basma
 * @date       2025-10-09
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#ifndef _TIMER1_CONFIG_H_
#define _TIMER1_CONFIG_H_

#include "../../Common/Macros.h"
#include "../Atmega32RegistersAddress.h"
#include "TIMER1_Private.h"
#include "../../Common/ProjectCfg.h"

/**
 * @def CompareMatch_InterruptEnableA
 * @brief Enables Timer1 Compare Match A interrupt.
 * @details Sets the OCIE1A bit in the TIMSK register.
 */
#define CompareMatch_InterruptEnableA SetBit(TIMSK_Reg, OCIE1A)

/**
 * @def CompareMatch_InterruptEnableB
 * @brief Enables Timer1 Compare Match B interrupt.
 * @details Sets the OCIE1B bit in the TIMSK register.
 */
#define CompareMatch_InterruptEnableB SetBit(TIMSK_Reg, OCIE1B)

/**
 * @def Timer1_Prescaler
 * @brief Defines the prescaler value for Timer1.
 * @details Sets the clock division factor for Timer1. This affects the timer tick frequency.
 * @note Must be one of the values defined in the Timer1_Prescallers enum.
 */
#define Timer1_Prescaler T1_ClkPrescalling_64

/**
 * @def Timer1_WaveGenMode
 * @brief Selects the waveform generation mode for Timer1.
 * @details Configures Timer1 to operate in CTC mode with ICR1 as TOP.
 * @note Other modes include Normal and PWM.
 */
#define Timer1_WaveGenMode T1_CTC_ICR1_is_TOP

/**
 * @def Timer1_CompareOutputModeA
 * @brief Sets the compare output behavior for Timer1 Channel A.
 * @details In Normal Port mode, the OC1A pin is disconnected and used as a regular I/O.
 */
#define Timer1_CompareOutputModeA T1_NormalPort

/**
 * @def Timer1_CompareOutputModeB
 * @brief Sets the compare output behavior for Timer1 Channel B.
 * @details In Normal Port mode, the OC1B pin is disconnected and used as a regular I/O.
 */
#define Timer1_CompareOutputModeB T1_NormalPort

/**
 * @def Timer1_Resolution
 * @brief Defines the time resolution for Timer1 ticks.
 * @details Sets the base time unit for scheduling tasks, typically in milliseconds.
 */
#define Timer1_Resolution T1_MilliSecond

/**
 * @def IsCOMA_FlagSet
 * @brief Checks if the Timer1 Compare Match A flag is set.
 * @details Reads the OCF1A bit from the TIFR register to determine if a compare match occurred.
 * @return Boolean value: 1 if set, 0 if not.
 */
#define IsCOMA_FlagSet GetBit(TIFR_Reg, OCF1A)

/**
 * @def IsCOMB_FlagSet
 * @brief Checks if the Timer1 Compare Match B flag is set.
 * @details Reads the OCF1B bit from the TIFR register to determine if a compare match occurred.
 * @return Boolean value: 1 if set, 0 if not.
 */
#define IsCOMB_FlagSet GetBit(TIFR_Reg, OCF1B)

/**
 * @def T1_ClkTime
 * @brief Calculates the time per Timer1 tick.
 * @details Based on the selected prescaler and system clock frequency.
 * @note Used in determining the compare match value for CTC mode.
 */
#define T1_ClkTime ((float)Timer1_Prescaler / F_CPU)

/**
 * @def T1_NUMBER_OF_CTC
 * @brief Defines the number of CTC channels used in Timer1.
 * @note Adjust this value based on the required number of CTC channels for your application.
 */
#define T1_NUMBER_OF_CTC 1

/**
 * @def CompareMatchValueA
 * @brief Calculates the OCR1A value for Timer1 in CTC mode.
 * @details Determines the value to load into OCR1A to achieve the desired tick resolution.
 * @note Depends on clock frequency, prescaler, and desired time resolution.
 * @warning Ensure that the result fits within 16-bit range (0–65535).
 */
#define CompareMatchValueA ((Timer1_Resolution / (T1_ClkTime * T1_NUMBER_OF_CTC)) - 1)

/**
 * @def CompareMatchValueB
 * @brief Calculates the OCR1B value for Timer1 in CTC mode.
 * @details Determines the value to load into OCR1B to achieve the desired tick resolution.
 * @note Depends on clock frequency, prescaler, and desired time resolution.
 * @warning Ensure that the result fits within 16-bit range (0–65535).
 */
#define CompareMatchValueB ((Timer1_Resolution / (T1_ClkTime * T1_NUMBER_OF_CTC)) - 1)



#endif