/**
 * @file ADC_Program.c
 * @brief This file contains the implementation for the ADC (Analog-to-Digital Converter) module.
 * @version 2.0
 * @author Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Eng/Hesham Ahmed 
 * @author Reviewer  : Eng/Alaa Emad
 * @date 2025-10-14
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#include "../../Common/Config.h"

#if ADC_Module == Enable
#include "ADC_Interface.h"

/*-------------------------------------------------------------
 *                   Private Global Variables
 *-------------------------------------------------------------*/

/** @brief Callback for ADC0 channel */
static void (*ADC0_Callback)(uint16_t) = Null;

/** @brief Callback for ADC1 channel */
static void (*ADC1_Callback)(uint16_t) = Null; //-> Note if used 8 channels we need 8 callbacks ?!! 

/** @brief Tracks which channel is currently being converted */
static uint8_t current_channel = ADC0_Channel;

/*-------------------------------------------------------------
 *               Private Helper Functions
 *-------------------------------------------------------------*/

/**
 * @brief Sets callback for a specific ADC channel (ADC0 or ADC1 only).
 * @param[in] callback Pointer to callback function.
 * @param[in] channel  Channel number (ADC0_Channel or ADC1_Channel).
 */
void ADC_SetCallback(void (*callback)(uint16_t), uint8_t channel)
{
    if (callback == Null)
        return;

    switch (channel)
    {
        case ADC0_Channel:
            ADC0_Callback = callback;
            break;

        case ADC1_Channel:
            ADC1_Callback = callback;
            break;

        default:
            // Invalid channel (ignored)
            break;
    }
}

/*-------------------------------------------------------------
 *                ADC Initialization Function
 *-------------------------------------------------------------*/

void mADC_Init(const ADC_ConfigType *config)
{
    if (config == Null)
        return;

    /* Right-adjust result (clear ADLAR) */
    ClearBit(ADMUX_Reg, ADLAR_bit);

    /* Set voltage reference (REFS1:0 bits in ADMUX) */
    ADMUX_Reg = (ADMUX_Reg & 0x3F) | ((config->voltageRef & 0x03) << REFS0_bit);

    /* Set ADC prescaler (ADPS2:0 bits in ADCSRA) */
    ADCSRA_Reg = (ADCSRA_Reg & 0xF8) | (config->prescaler & 0x07);

    /* Enable/Disable Auto Trigger */
    if (config->autoTrigger == ADC_AUTO_TRIGGER_Enable)
        SetBit(ADCSRA_Reg, ADATE_bit);
    else
        ClearBit(ADCSRA_Reg, ADATE_bit);

    /* Enable/Disable Interrupt */
    if (config->interrupt == ADC_INTERRUPT_Enable)
        SetBit(ADCSRA_Reg, ADIE_bit);
    else
        ClearBit(ADCSRA_Reg, ADIE_bit);

    /* Set Auto Trigger Source (ADTS2:0 bits in SFIOR) */
    SFIOR_Reg = (SFIOR_Reg & 0x1F) | ((config->triggerSource & 0x07) << ADTS0_bit);

    /* Enable/Disable ADC */
    if (config->enable == ADC_ENABLE)
        SetBit(ADCSRA_Reg, ADEN_bit);
    else
        ClearBit(ADCSRA_Reg, ADEN_bit);
}

/*-------------------------------------------------------------
 *                Synchronous (Blocking) Read
 *-------------------------------------------------------------*/

uint16_t mADC_Read(uint8_t channel)
{
    /* Select channel (0–7) */
    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | (channel & ADC_Channel_LowerNibble_Mask);

    /* Start conversion */
    SetBit(ADCSRA_Reg, ADSC_bit);

    /* Wait until conversion finishes */
    while (GetBit(ADCSRA_Reg, ADSC_bit));

#if ADC_INTERRUPT == ADC_INTERRUPT_Disable
    /* Clear flag manually if interrupts disabled */
    ClearFlag(ADCSRA_Reg, ADIF_bit);
#endif

    /* Return ADC value */
    return ADCData_Reg;
}

/*-------------------------------------------------------------
 *          Asynchronous (Auto Trigger / Round Robin)
 *-------------------------------------------------------------*/

/**
 * @brief Register callback for a specific channel.
 */
void mADC_RegisterChannel(uint8_t channel, void (*callback)(uint16_t value))
{
    ADC_SetCallback(callback, channel);
}

/**
 * @brief Start group sampling (round-robin between ADC0 and ADC1).
 */
void mADC_StartGroup(void)
{
    /* Start from ADC0 */
    current_channel = ADC0_Channel;
    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | ADC0_Channel;

    /* Clear flag and enable auto trigger */
    ClearFlag(ADCSRA_Reg, ADIF_bit);
    SetBit(ADCSRA_Reg, ADATE_bit);

    /* Start first conversion */
    SetBit(ADCSRA_Reg, ADSC_bit);
}

/**
 * @brief Stop auto-trigger / group sampling.
 */
void mADC_Stop(void)
{
    ClearBit(ADCSRA_Reg, ADATE_bit);
    ClearBit(ADCSRA_Reg, ADSC_bit);
}

/*-------------------------------------------------------------
 *                    ADC Interrupt Service Routine
 *-------------------------------------------------------------*/

void __vector_16(void)
{
    uint16_t adc_value = ADCData_Reg;

    /* Call the corresponding callback */
    if (current_channel == ADC0_Channel && ADC0_Callback != Null)
        ADC0_Callback(adc_value);
    else if (current_channel == ADC1_Channel && ADC1_Callback != Null)
        ADC1_Callback(adc_value);

    /* Switch channel for next conversion */
    current_channel = (current_channel == ADC0_Channel) ? ADC1_Channel : ADC0_Channel;

    /* Update ADMUX to select next channel */
    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | (current_channel & ADC_Channel_LowerNibble_Mask);

#if ADC_AUTO_TRIGGER == ADC_AUTO_TRIGGER_Disable
    /* Start next conversion manually if auto-trigger disabled */
    SetBit(ADCSRA_Reg, ADSC_bit);
#endif
}

/*-------------------------------------------------------------
 *                    Global Configurations
 *-------------------------------------------------------------*/

#if ADC_Mode == Asynchronous_Mode
ADC_ConfigType ADC_Configuration = 
{
    .prescaler = ADC_PRESCALER,
    .voltageRef = ADC_VOLTAGE_REF,
    .autoTrigger = ADC_AUTO_TRIGGER_Enable,
    .interrupt = ADC_INTERRUPT_Enable,
    .triggerSource = ADC_TRIGGER_SOURCE,
    .enable = ADC_ENABLE_state
};

#elif ADC_Mode == Synchronous_Mode
ADC_ConfigType ADC_Configuration =
{
    .prescaler = ADC_PRESCALER,
    .voltageRef = ADC_VOLTAGE_REF,
    .autoTrigger = ADC_AUTO_TRIGGER_Disable,
    .interrupt = ADC_INTERRUPT,
    .triggerSource = ADC_FREE_RUNNING_MODE,
    .enable = ADC_ENABLE_state
};
#endif

#endif /* ADC_Module == Enable */
