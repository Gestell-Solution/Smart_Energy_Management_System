/**
 * @file       ADC_Program.c
 * @brief      Implementation of the Analog-to-Digital Converter (ADC) Driver.
 * @details    This module configures the ATmega32 ADC peripheral. It supports:
 *             - **Synchronous Mode**: Blocking read (`mADC_Read`).
 *             - **Asynchronous Mode**: Interrupt-driven round-robin sampling (`mADC_StartGroup`).
 *             - **Configuration**: configurable Reference voltage, Prescaler, and Trigger sources via `ADC_Config.h`.
 * @version    2.0
 * @author     Developer: Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author     Reviewer:  Eng/Hesham Ahmed 
 * @author     Reviewer:  Eng/Alaa Emad
 * @date       2025-10-14
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#include "../../Common/Config.h"

#if ADC_Module == Enable
#include "ADC_Interface.h"

/*============================================================================
 *                                 Global Variables
 *============================================================================*/

/** @brief Guard flag to prevent double initialization. */
uint8_t isADC_Initialized = 0;

/*============================================================================
 *                                 Private Global Variables
 *============================================================================*/

/** @brief Array of function pointers for ADC conversion complete callbacks (Per Channel). */
static void (*ADC_Callbacks[2])(uint16_t) = {Null};

/** @brief Internal index to track the current channel in round-robin mode. */
static uint8_t Channel_Index = 0;

/*============================================================================
 *                                 Private Helper Functions
 *============================================================================*/

/**
 * @brief      Registers a callback for a specific ADC channel.
 * @details    Called internally by `mADC_RegisterChannel`.
 * @param[in]  callback Pointer to the function to execute on conversion complete.
 * @param[in]  channel  Channel ID (0 or 1).
 * @return     void
 */
void ADC_SetCallback(void (*callback)(uint16_t), uint8_t channel)
{
    if (callback == Null || channel > ADC_Max_used_Channel)
    {
        return;
    }
    ADC_Callbacks[channel] = callback;
}

/*============================================================================
 *                                 Function Definitions
 *============================================================================*/

/**
 * @brief      Initializes the ADC peripheral.
 * @details    
 * - Configures **ADMUX**: Reference Voltage, Adjustment.
 * - Configures **ADCSRA**: Prescaler, Auto-Trigger Enable, Interrupt Enable.
 * - Configures **SFIOR**: Trigger Source (if Auto-Trigger enabled).
 * - Enables the ADC hardware.
 * @return     void
 */
void mADC_Init()
{
    /**
     * Configuration Map:
     * 1. ADMUX  (Ref + LeftAdjust + Channel)
     * 2. ADCSRA (Enable + Start + AutoTrig + IntFlag + IntEnable + Prescaler)
     * 3. SFIOR  (Special Function IO, for Trigger Source)
     */
    
    /* Guard Clause */
    if (isADC_Initialized == 0) {
        isADC_Initialized = 1;
    } else {
        return;
    }

    /* Right-adjust result (clear ADLAR) - 10-bit resolution */
    ClearBit(ADMUX_Reg, ADLAR_bit);

    /* Set voltage reference (REFS1:0 bits in ADMUX) */
    ADMUX_Reg = (ADMUX_Reg & 0x3F) | ((ADC_VOLTAGE_REF & 0x03) << REFS0_bit);

    /* Set ADC prescaler (ADPS2:0 bits in ADCSRA) */
    ADCSRA_Reg = (ADCSRA_Reg & 0xF8) | (ADC_PRESCALER & 0x07);

    /* Enable/Disable Auto Trigger */
    if (ADC_AUTO_TRIGGER == ADC_AUTO_TRIGGER_Enable){
        SetBit(ADCSRA_Reg, ADATE_bit);
    }
    else{
        ClearBit(ADCSRA_Reg, ADATE_bit);
    }
    
    /* Enable/Disable Interrupt */
    if (ADC_INTERRUPT == ADC_INTERRUPT_Enable){
        SetBit(ADCSRA_Reg, ADIE_bit);
    }
    else{
        ClearBit(ADCSRA_Reg, ADIE_bit);
    }
    
    /* Set Auto Trigger Source (ADTS2:0 bits in SFIOR) */
    SFIOR_Reg = (SFIOR_Reg & 0x1F) | ((ADC_TRIGGER_SOURCE & 0x07) << ADTS0_bit);

    /* Enable/Disable ADC Global State */
    if (ADC_ENABLE_state == ADC_ENABLE)
        SetBit(ADCSRA_Reg, ADEN_bit);
    else
        ClearBit(ADCSRA_Reg, ADEN_bit);
}

/*============================================================================
 *                                 Synchronous (Blocking) Read
 *============================================================================*/

/**
 * @brief      Performs a blocking read operation on a specific channel.
 * @details    1. Selects the channel in ADMUX.
 *             2. Starts Conversion (ADSC).
 *             3. Busy-waits until ADSC clears (or ADIF sets).
 *             4. Returns the 10-bit result.
 *             
 *             **Note**: Disabling interrupts is recommended during this call if mixed mode is used,
 *             though this implementation manually clears the flag.
 * @param[in]  channel Channel Number (0-7).
 * @return     uint16_t 10-bit ADC result.
 */
uint16_t mADC_Read(uint8_t channel)
{
    /* Select channel (Mask protects Ref bits) */
    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | (channel & ADC_Channel_LowerNibble_Mask);

    /* Start conversion */
    SetBit(ADCSRA_Reg, ADSC_bit);

    /* Wait until conversion finishes (ADSC becomes 0) */
    while (GetBit(ADCSRA_Reg, ADSC_bit))
    {
        /* Busy wait */
    }
    
#if ADC_INTERRUPT == ADC_INTERRUPT_Disable
    /* Clear flag manually if interrupts disabled */
    ClearFlag(ADCSRA_Reg, ADIF_bit);
#endif

    /* Return ADC value (Low + High byte combined) */
    return ADCData_Reg;
}

/*============================================================================
 *          Asynchronous (Auto Trigger / Round Robin)
 *============================================================================*/

/**
 * @brief      Registers a user callback function for a specific channel's data.
 * @param[in]  channel  ADC Channel to listen to.
 * @param[in]  callback Function pointer `void func(uint16_t value)`.
 * @return     void
 */
void mADC_RegisterChannel(uint8_t channel, void (*callback)(uint16_t value))
{
    ADC_SetCallback(callback, channel);
}

/**
 * @brief      Starts the Group Sampling mode (Round Robin).
 * @details    Beings conversion on Channel 0. The ISR will then automatically
 *             rotate to the next channel upon completion.
 * @return     void
 */
void mADC_StartGroup(void)
{
    /* Start from ADC0 */
    Channel_Index = ADC0_Channel;
    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | ADC0_Channel;

    /* Enable Auto Trigger mechanism if configured */
    SetBit(ADCSRA_Reg, ADATE_bit);

    /* Start first conversion */
    SetBit(ADCSRA_Reg, ADSC_bit);
}

/**
 * @brief      Stops the auto-trigger / group sampling.
 * @return     void
 */
void mADC_Stop(void)
{
    ClearBit(ADCSRA_Reg, ADATE_bit);
    ClearBit(ADCSRA_Reg, ADSC_bit);
}

/*============================================================================
 *                    ADC Interrupt Service Routine
 *============================================================================*/

/** @brief Declaration of ADC Conversion Complete ISR vector. */
void __vector_16(void) __attribute__((signal)); 

/**
 * @brief      ADC Conversion Complete ISR.
 * @details    
 * 1. Reads the converted value.
 * 2. Executes the callback for the current channel.
 * 3. Increments the `Channel_Index` (Round Robin).
 * 4. Switches ADMUX to the new channel.
 * 5. Re-triggers conversion (if strict timing/triggering isn't fully automatic for MUX switching).
 */
void __vector_16(void)
{
    uint16_t ADC_Value = ADCData_Reg;
    
    /* Call current channel callback if valid */
    if (ADC_Callbacks[Channel_Index] != Null)
    {
        ADC_Callbacks[Channel_Index](ADC_Value);
    }

    /* Increment and wrap around channel index */
    Channel_Index++;
    if (Channel_Index >= ADC_Max_used_Channel)
    {
        Channel_Index = 0;
    }
    
    /* Switch Channel */
    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | Channel_Index;

    /* Start Next Conversion */
    SetBit(ADCSRA_Reg, ADSC_bit);
}


#endif /* ADC_Module == Enable */