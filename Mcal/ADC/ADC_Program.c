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
uint8_t isADC_Initialized=0;

/*-------------------------------------------------------------
 *                   Private Global Variables
 *-------------------------------------------------------------*/

/** @brief Callback for ADC0 channel */
static void (*ADC_Callbacks[2])(uint16_t) = {Null};



    static uint8_t Channel_Index =0;


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
    if (callback == Null || channel >ADC_Max_used_Channel)
    {
        return;
    }
    ADC_Callbacks[channel]=callback ;
}

/*-------------------------------------------------------------
 *                ADC Initialization Function
 *-------------------------------------------------------------*/

void mADC_Init()
{
    //ADC Guard
    if (isADC_Initialized==0) {
    
        isADC_Initialized=1;
    }else return;
    /* Right-adjust result (clear ADLAR) */
    ClearBit(ADMUX_Reg, ADLAR_bit);

    /* Set voltage reference (REFS1:0 bits in ADMUX) */
    ADMUX_Reg = (ADMUX_Reg & 0x3F) | (( ADC_VOLTAGE_REF& 0x03) << REFS0_bit);

    /* Set ADC prescaler (ADPS2:0 bits in ADCSRA) */
    ADCSRA_Reg = (ADCSRA_Reg & 0xF8) | (ADC_PRESCALER & 0x07);

    /* Enable/Disable Auto Trigger */
    if (ADC_AUTO_TRIGGER== ADC_AUTO_TRIGGER_Enable)
        SetBit(ADCSRA_Reg, ADATE_bit);
    else
        ClearBit(ADCSRA_Reg, ADATE_bit);

    /* Enable/Disable Interrupt */
    if ( ADC_INTERRUPT == ADC_INTERRUPT_Enable)
        SetBit(ADCSRA_Reg, ADIE_bit);
    else
        ClearBit(ADCSRA_Reg, ADIE_bit);

    /* Set Auto Trigger Source (ADTS2:0 bits in SFIOR) */
    SFIOR_Reg = (SFIOR_Reg & 0x1F) | ((ADC_TRIGGER_SOURCE & 0x07) << ADTS0_bit);

    /* Enable/Disable ADC */
    if (ADC_ENABLE_state == ADC_ENABLE)
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
    while (GetBit(ADCSRA_Reg, ADSC_bit))
    {
        // Busy wait
    }
    

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
    Channel_Index = ADC0_Channel;
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
    /**
     * psuedo code
     * read the conversion
     * set the callback
     * increament the current channel and round - robin
     * switch the ADMUX_Reg
     * Clear the interrupt flag
     */
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

    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | Channel_Index;

    ClearFlag(ADCSRA_Reg, ADIF_bit);

#if ADC_AUTO_TRIGGER == ADC_AUTO_TRIGGER_Disable
    /* If no hardware trigger, start manually */
    SetBit(ADCSRA_Reg, ADSC_bit);
#endif
}


#endif /* ADC_Module == Enable */
