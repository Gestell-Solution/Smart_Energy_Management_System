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

} Timer0_Prescallers;
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
 * @enum    CompareOutputModes
 * @brief   Defines the waveform generation modes for Timer0.
 * @details This enumeration specifies the available waveform generation modes for Timer0 operation.
 */
typedef enum
{
    /**
     * @var   T0_NormalPort
     * @brief Normal port operation, OC0 disconnected.
     */
    T0_NormalPort = 0,
    /**
     * @var   T0_OC0_Toggle
     * @brief Toggle OC0 on compare match
     */
    T0_OC0_Toggle = 1,
    /**
     * @var   T0_OC0_Clear
     * @brief  Clear OC0 on compare match
     */
    T0_OC0_Clear = 2,
    /**
     * @var    T0_OC0_Set
     * @brief  Set OC0 on compare match
     */
    T0_OC0_Set = 3,
} CompareOutputModes_NonPWM;

/**
 * @def   OCIE0_Bit
 * @brief This Bit is responisble for the Enabling the Interrupt on the Case of the Output Compare match Enabling
 */
#define OCIE0_Bit 1
/**
 * @def   OCF0_Bit
 * @brief This Bit is just about the Flag risen by hard ware on Compare Output Match
 */
#define OCF0_Bit 1
/**
 * @def Second
 * @brief Represents one second as a base time unit.
 */
#define T0_Second          1

/**
 * @def MilliSecond
 * @brief Represents one millisecond as a fraction of a second.
 *        Calculated as 0.0001 times the value of Second.
 */
#define T0_MilliSecond .001*T0_Second
/**
 * @def NUMBER_OF_CTC
 * @brief Defines the number of CTC (Clear Timer on Compare Match) channels or instances used in Timer0.
 * @note Adjust this value based on the required number of CTC channels for your application.
 */
#define T0_NUMBER_OF_CTC      1

/**
 * @struct Timer0_ScheduledTasks
 * @brief Represents a scheduled task managed by Timer0.
 * @details This structure holds the necessary information for a non-blocking delay task,
 *          including its callback function, remaining time in ticks, and active status.
 *          It is packed with 1-byte alignment to optimize memory usage in embedded systems.
 * @note Used internally by the Timer0 scheduler to manage multiple concurrent tasks.
 */

typedef struct
{
    /**
     * @brief Pointer to the callback function to be executed when the delay expires.
     */
    void (*Callback)(void);

    // /**
    //  * @brief Optional Task ID for identifying or managing specific tasks.
    //  */
    // uint8_t TaskID;

    /**
     * @brief Remaining time in ticks before the task is triggered.
     */
    uint32_t Remaining_Ticks;
    /**
     * @brief Remaining time in ticks before the task is triggered.
     */
    uint32_t  Delaying;

    /**
     * @brief Indicates whether the task is currently active (1) or inactive (0).
     */
    uint8_t Active;
} Timer0_ScheduledTasks;


#endif
