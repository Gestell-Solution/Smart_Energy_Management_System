/**
 * @file       TIMER1_Private.h
 * @brief      This file contains Private Data that is constant settings for Timer1 registers and other concepts.
 * @details    This header file defines several macros that allow for defining Driver based on Timer1/Atmega32 data sheet specifications.
 * @version    0.2
 * @author     Developer: Mohamed Diaa (mohammeddiaato@gmail.com)
 * @author     Reviewer:  Basma        ()
 * @date       2025-10-07
 * @copyright  Copyright (c) 2025 , Gestell Company
 */

#ifndef _TIMER1_PRIVATE_H_
#define _TIMER1_PRIVATE_H_
#include "../Atmega32RegistersAddress.h"

/**
 * @enum    Timer1_Prescallers
 * @brief   Defining Prescaler options for Timer1 to control the speed of the clock.
 */
typedef enum
{
    /**
     * @var   T1_NoClkSource
     * @brief Defined for no clk source of the Timer so Timer/counter1 is stopped
     */
    T1_NoClkSource,
    /**
     * @var   T1_ClkPrescalling_1
     * @brief Defined for clk source of the Timer to have same clk (no prescalling)
     */
    T1_ClkPrescalling_1,
    /**
     * @var   T1_ClkPrescalling_8
     * @brief Defined for clk source of the Timer to have clk /8 speed
     */
    T1_ClkPrescalling_8,
    /**
     * @var   T1_ClkPrescalling_64
     * @brief Defined for clk source of the Timer to have clk /64 speed
     */
    T1_ClkPrescalling_64,
    /**
     * @var   T1_ClkPrescalling_256
     * @brief Defined for clk source of the Timer to have clk /256 speed
     */
    T1_ClkPrescalling_256,
    /**
     * @var   T1_ClkPrescalling_1024
     * @brief Defined for clk source of the Timer to have clk /1024 speed
     */
    T1_ClkPrescalling_1024,
    /**
     * @var   T1_EXT_ClkOnFallingEdge,
     * @brief Defined for clk source of the Timer to External Clock .
     * @note  Clock is counted on Falling edge
     */
    T1_EXT_ClkOnFallingEdge,
    /**
     * @var   T1_EXT_ClkOnRisingEdge,
     * @brief Defined for clk source of the Timer to External Clock .
     * @note  Clock is counted on rising edge
     */
    T1_EXT_ClkOnRisingEdge,

} Timer1_Prescallers;
/**
 * @enum    Timer1_Bits
 * @brief   defining Bits Of TCCR1A to control the Timer1 Options.
 */
typedef enum
{

    /**
     * @var   T1_WGM10_Bit
     * @brief Defined for Chossing Wave Generation Options
     */
    T1_WGM10_Bit,
    /**
     * @var   T1_WGM11_Bit
     * @brief Defined for Chossing Wave Generation Options
     */
    T1_WGM11_Bit,
    /**
     * @var   T1_FOC1B_Bit
     * @brief Defined to Force Output Compare at any state for the OC1B channel -> non pwm
     */
    T1_FOC1B_Bit,
    /**
     * @var   T1_FOC1A_Bit
     * @brief Defined to Force Output Compare at any state for the OC1A channel -> non pwm
     */
    T1_FOC1A_Bit,
    /**
     * @var   T1_COM1B0_Bit
     * @brief Defined To control the Compare match behaviour for the PWM modes and Non PWM modes for the OC1B channel
     */
    T1_COM1B0_Bit,
    /**
     * @var   T1_COM1B1_Bit
     * @brief Defined To control the Compare match behaviour for the PWM modes and Non PWM modes for the OC1B channel
     */
    T1_COM1B1_Bit,
    /**
     * @var   T1_COM1A0_Bit
     * @brief Defined To control the Compare match behaviour for the PWM modes and Non PWM modes for the OC1A channel
     */
    T1_COM1A0_Bit,
    /**
     * @var   T1_COM1A1_Bit
     * @brief Defined To control the Compare match behaviour for the PWM modes and Non PWM modes for the OC1A channel
     */
    T1_COM1A1_Bit,

} TCCR1A_Bits;
typedef enum
{

    /**
     * @var   T1_CS10_Bit
     * @brief Defined for Chossing Prescaller Options
     */
    T1_CS10_Bit = 0,
    /**
     * @var   T1_CS11_Bit
     * @brief Defined for Chossing Prescaller Options
     */
    T1_CS11_Bit = 1,
    /**
     * @var   T1_CS12_Bit
     * @brief Defined for Chossing Prescaller Options
     */
    T1_CS12_Bit = 2,
    /**
     * @var   T1_WGM12_Bit
     * @brief Defined for Chossing Wave Generation Options
     */
    T1_WGM12_Bit = 3,
    /**
     * @var   T1_WGM13_Bit
     * @brief Defined for Chossing Wave Generation Options
     */
    T1_WGM13_Bit = 4,
    /**
     * @var   T1_ICES1_Bit
     * @brief This bit chooses where to capture the input weather at falling edge (write 0)or at the rising edge(write 1)
     */
    T1_ICES1_Bit = 6,
    /**
     * @var   T1_ICNC1_Bit
     * @brief Setting this bit (to one) activates the Input Capture Noise Canceler.
     */
    T1_ICNC1_Bit = 7,

} TCCR1B_Bits;

/**
 * @enum    WaveGenModes
 * @brief   Defines the waveform generation modes for Timer0.
 * @details This enumeration specifies the available waveform generation modes for Timer0 operation.
 */
typedef enum
{
    /**
     * @var   T1_Normal
     * @brief Normal mode (Timer counts up to 0xFF, then overflows).
     */
    T1_Normal = 0,
    /**
     * @var    T1_CTC_OCR1A_is_TOP
     * @brief Clear Timer on Compare Match (CTC) mode. Having the Top Value at OCR1A.
     */
    T1_CTC_OCR1A_is_TOP = 4,
    /**
     * @var   T1_CTC_ICR1_is_TOP
     * @brief Clear Timer on Compare Match (CTC) mode. Having the Top Value at ICR1.
     */
    T1_CTC_ICR1_is_TOP = 12,

} WaveGenModes;
/**
 * @enum    CompareOutputModes
 * @brief   Defines the waveform generation modes for Timer0.
 * @details This enumeration specifies the available waveform generation modes for Timer0 operation.
 */
typedef enum
{
    /**
     * @var   T1_NormalPort
     * @brief Normal port operation, OC0 disconnected.
     */
    T1_NormalPort = 0,
    /**
     * @var   T1_OC0_Toggle
     * @brief Toggle OC0 on compare match
     */
    T1_OC0_Toggle = 1,
    /**
     * @var   T1_OC0_Clear
     * @brief  Clear OC0 on compare match
     */
    T1_OC0_Clear = 2,
    /**
     * @var    T1_OC0_Set
     * @brief  Set OC0 on compare match
     */
    T1_OC0_Set = 3,
} CompareOutputModes_NonPWM;

/**
 * @def Second
 * @brief Represents one second as a base time unit.
 */
#define T1_Second 1

/**
 * @def MilliSecond
 * @brief Represents one millisecond as a fraction of a second.
 *        Calculated as 0.001 times the value of Second.
 */
#define T1_MilliSecond .001 * T1_Second

/**
 * @def OCIE1A_Bit
 * @brief Output Compare A Match Interrupt Enable bit position in TIMSK register.
 */
#define OCIE1A_Bit 4

/**
 * @def OCIE1B_Bit
 * @brief Output Compare B Match Interrupt Enable bit position in TIMSK register.
 */
#define OCIE1B_Bit 3

/**
 * @def OCF1A_Bit
 * @brief Output Compare A Match Flag bit position in TIFR register.
 */
#define OCF1A_Bit 4

/**
 * @def OCF1B_Bit
 * @brief Output Compare B Match Flag bit position in TIFR register.
 */
#define OCF1B_Bit 3

#endif
