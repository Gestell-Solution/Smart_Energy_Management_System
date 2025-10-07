/**
 * @file       TIMER0_Private.h
 * @brief      This file contains Private Data that is constant settings for Timer0 registers and other concepts.
 * @details    This header file defines several macros that allow for defining D based Timer0/Atmega32 data sheet specifications.
 * @version    0.2
 * @author     Developer: Mohamed Diaa (mohammeddiaato@gmail.com)
 * @author     Reviewer:  Basma        ()
 * @date       2025-10-07 
 * @copyright  Copyright (c) 2025 , Gestell Company 
 */
#ifndef _TIMER0_PRIVATE_H_
#define _TIMER0_PRIVATE_H_
#include "../Atmega32RegistersAddress.h"

/**
 * @enum    Timer0_Prescallers
 * @brief   defining Prescaller options for Timer0 to control the speed of the clk
 */
typedef enum
{
    /**
     * @var   T0_NoClkSource
     * @brief Defined for no clk source of the Timer so Timer/counter0 is stopped
     */
    T0_NoClkSource,
    /**
     * @var   T0_ClkPrescalling_1
     * @brief Defined for clk source of the Timer to have same clk (no prescalling)
     */
    T0_ClkPrescalling_1,
    /**
     * @var   T0_ClkPrescalling_8
     * @brief Defined for clk source of the Timer to have clk /8 speed 
     */
    T0_ClkPrescalling_8,
    /**
     * @var   T0_ClkPrescalling_64
     * @brief Defined for clk source of the Timer to have clk /64 speed 
     */
    T0_ClkPrescalling_64,
    /**
     * @var   T0_ClkPrescalling_256
     * @brief Defined for clk source of the Timer to have clk /256 speed 
     */
    T0_ClkPrescalling_256,
    /**
     * @var   T0_ClkPrescalling_1024
     * @brief Defined for clk source of the Timer to have clk /1024 speed 
     */
    T0_ClkPrescalling_1024,
    /**
     * @var   T0_EXT_ClkOnFallingEdge,
     * @brief Defined for clk source of the Timer to External Clock .
     * @note  Clock is counted on Falling edge
     */
    T0_EXT_ClkOnFallingEdge,
    /**
     * @var   T0_EXT_ClkOnRisingEdge,
     * @brief Defined for clk source of the Timer to External Clock .
     * @note  Clock is counted on rising edge
     */
    T0_EXT_ClkOnRisingEdge,

}Timer0_Prescallers;
/**
 * @enum    Timer0_Bits
 * @brief   defining Bits Of TCCR0 to control the Timer0 Options.
 */
typedef enum
{

    /**
     * @var   T0_CS00_Bit
     * @brief Defined for Chossing Prescaller Options
     */
    T0_CS00_Bit,
    /**
     * @var   T0_CS01_Bit
     * @brief Defined for Chossing Prescaller Options
     */
    T0_CS01_Bit,
    /**
     * @var   T0_CS02_Bit
     * @brief Defined for Chossing Prescaller Options
     */
    T0_CS02_Bit,
    /**
     * @var   TT0_WGM01_Bit
     * @brief Defined To control Which Mode the Timer Operate at.
     */
    T0_WGM01_Bit,
    /**
     * @var   TT0_COM00_Bit
     * @brief Defined To control the Compare match behaviour for the PWM modes and Non PWM modes 
     */
    T0_COM00_Bit,
    /**
     * @var   TT0_COM01_Bit
     * @brief Defined To control the Compare match behaviour for the PWM modes and Non PWM modes
     */
    T0_COM01_Bit,
    /**
     * @var   TT0_WGM00_Bit
     * @brief Defined To control Which Mode the Timer Operate at.
     */
    T0_WGM00_Bit,
    /**
     * @var   T0_FOC0_Bit
     * @brief It forces an immediate compare match between the TCNT0 and OCR0 regardless of their actual values. and Do the Options of the COM bits
     */
    T0_FOC0_Bit,

} TCCR0_Bits;

/**
 * @enum    WaveGenModes
 * @brief   Defines the waveform generation modes for Timer0.
 * @details This enumeration specifies the available waveform generation modes for Timer0 operation.
 */
typedef enum
{
    /**
     * @var   T0_Normal
     * @brief Normal mode (Timer counts up to 0xFF, then overflows).
     */
    T0_Normal = 0,
    /**
     * @var   T0_CTC
     * @brief Clear Timer on Compare Match (CTC) mode.
     */
    T0_CTC = 2,
    /**
     * @var   T0_FastPWM
     * @brief Fast PWM mode.
     */
    T0_FastPWM = 3,
} WaveGenModes;

/**
 * @def   OCIE0_Bit
 * @brief This Bit is responisble for the Enabling the Interrupt on the Case of the Output Compare match Enabling
 */
#define  OCIE0_Bit  1
/**
 * @def   OCFO0_Bit
 * @brief This Bit is just about the Flag risen by hard ware on Compare Output Match
 */
#define  OCFO0_Bit  1


#endif

