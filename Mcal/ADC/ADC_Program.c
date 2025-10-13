/**
 * @file ADC_Program.c
 * @brief This file contains the implementation for the ADC (Analog-to-Digital Converter) module.
 * @version 1.0
 * @author  Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author  Reviewer  :
 * @date 2025-10-11
 * @copyright Copyright (c) 2025 , Gestell Company
 */


#include "../../Common/Config.h"


#if ADC_Module == Enable 
#include "ADC_Interface.h"

static void (*ADC_Callback)(uint16_t) = Null; // Global callback function pointer

void ADC_SetCallback(void (*callback)(uint16_t))
{
    if (callback == Null)
    {
        return; // Do not set if the callback is NULL
    }
    ADC_Callback = callback;
}

/* Initialize ADC with the runtime configuration */
void mADC_Init(const ADC_ConfigType *config)
{ /**
     * psuedo code
     * 1-Set The voltage reference
     * 2-Set the ADC prescaler
     * 3-Enable/Disable Auto Trigger
     * 4-Enable/Disable Interrupt
     * 5-Set the Auto Trigger Source
     * 6-Enable/Disable the ADC
     * 7-Clear ADLAR_bit in ADMUX to right adjust the result
     */
    if(config == Null)
    {
        return;
    }

    ClearBit(ADMUX_Reg, ADLAR_bit); // Right adjust the result by clearing ADLAR bit in ADMUX
    /* Set the voltage reference (REFS1:0 bits in ADMUX) */
    ADMUX_Reg = (ADMUX_Reg & 0x3F) | ((config->voltageRef & 0x03) << REFS0_bit); // Set voltage reference (REFS1:0)
    /* Set the ADC prescaler (ADPS2:0 bits in ADCSRA) */
    ADCSRA_Reg = (ADCSRA_Reg & 0xF8) | (config->prescaler & 0x07); // Set prescaler (ADPS2:0)
   
    /* Enable/Disable Auto Trigger (ADATE bit in ADCSRA) */
    if (config->autoTrigger == ADC_AUTO_TRIGGER_Enable)
    {
        SetBit(ADCSRA_Reg, ADATE_bit); // Enable Auto Trigger
    }
    else
    {
        ClearBit(ADCSRA_Reg, ADATE_bit); // Disable Auto Trigger
    }
    /* Enable/Disable Interrupt (ADIE bit in ADCSRA) */
    if (config->interrupt == ADC_INTERRUPT_Enable)
    {
        SetBit(ADCSRA_Reg, ADIE_bit); // Enable Interrupt
    }
    else
    {
        ClearBit(ADCSRA_Reg, ADIE_bit); // Disable Interrupt
    }
    /* Set the Auto Trigger Source (ADTS2:0 bits in SFIOR) */
    SFIOR_Reg = (SFIOR_Reg & 0x1F) | ((config->triggerSource & 0x07) << ADTS0_bit); // Set Auto Trigger Source (ADTS2:0)
    /* Enable/Disable the ADC (ADEN bit in ADCSRA) */
    if (config->enable == ADC_ENABLE)
    {
        SetBit(ADCSRA_Reg, ADEN_bit); // Enable ADC
    }
    else
    {
        ClearBit(ADCSRA_Reg, ADEN_bit); // Disable ADC
    }


}

    


/* Blocking (synchronous) read from a single channel */
uint16_t mADC_Read(uint8_t channel)
{
    /*psuedo code 
    * Select the ADC channel
    * Start the conversion
    * Wait for the conversion to complete
    * Return the ADC value
     */
    /* Select the ADC channel (0-7) */
    
    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | (channel & ADC_Channel_LowerNibble_Mask);

    /* Start the conversion by setting the ADSC bit in ADCSRA */
    SetBit(ADCSRA_Reg, ADSC_bit);

    /* Wait for the conversion to complete (ADSC becomes 0) */
    while(GetBit(ADCSRA_Reg, ADSC_bit))
    {
        // Busy wait
    }

    /* Read and return the ADC value from ADC data registers */
    uint16_t adc_value = ADCData_Reg; // Assuming right-adjusted result
    return adc_value;
}

/* Register a callback for a specific channel (callback receives raw 10-bit ADC value) */
void mADC_RegisterChannel(uint8_t channel, void (*callback)(uint16_t value))
{
    /**
     * psuedo code
     * Store the callback function pointer
     * Validate the channel number (0-7)
     * Do not register if the callback is NULL
     * don't allow registering more than one channel
     */
    if( channel > ADC_Max_used_Channel)
    {
        return; // Invalid channel, do not register
    }
    if (callback == Null)
    {
        return; // Do not register if the callback is NULL
    }
    ADC_Callback = callback;
}


/* Start group sampling: round-robin using Auto Trigger (Timer1 Compare Match) */
void mADC_StartGroup(void)
{
    /**
     * psuedo code
     * Set initial channel (e.g., ADC0) -> start the conversion round-robin from Channel 0
     * Clear the interrupt flag (ADIF) by writing 1 to it
     * Enable Auto Trigger (if not already)
     * Start first conversion manually — after that, hardware trigger takes over
     */
    // Set initial channel (e.g., ADC0)
    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | (ADC0_Channel & ADC_Channel_LowerNibble_Mask);
    
    // Clear the interrupt flag (ADIF) by writing 1 to it
    ClearFlag(ADCSRA_Reg, ADIF_bit);
    
    // Enable Auto Trigger (if not already)
    SetBit(ADCSRA_Reg, ADATE_bit);

    // Start first conversion manually — after that, hardware trigger takes over
    SetBit(ADCSRA_Reg, ADSC_bit);
}

/* Stop Auto Trigger / sampling */
void mADC_Stop(void)
{
    /**
     * psuedo code
     *  disable auto trigger
     * 
     */
    // Disable Auto Trigger
    ClearBit(ADCSRA_Reg, ADATE_bit);

}

void __vector_16(void)
{
    /**
     * psuedo code
     * Call the registered callback with the ADC value
     * Switch to the next channel (0 -> 1 -> 0 ...)
     * Select the next channel in ADMUX
     */
   static uint8_t currently_channel = ADC0_Channel;

    if (ADC_Callback != Null)
    {
        uint16_t adc_value = ADCData_Reg; 
        ADC_Callback(adc_value);
    }

    /* Switch between channel 0 and 1 */
    if (currently_channel == ADC0_Channel)
    {
        currently_channel = ADC1_Channel;
    }
    else
    {
        currently_channel = ADC0_Channel;
    }

    /* Select next channel for next conversion */
    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | (currently_channel & ADC_Channel_LowerNibble_Mask);
}

#endif