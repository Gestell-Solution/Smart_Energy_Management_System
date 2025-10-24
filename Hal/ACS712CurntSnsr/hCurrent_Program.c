/**
 * @file        hCurrent_program.c
 * @brief       ACS712 30A driver implementation.
 * @details     Implements initialization, reading, and conversion functions for
 * the ACS712 current sensor using ADC.
 * @version     1.0.0
 * @author      Mohamed Diaa <mohammediaato@gmail.com> @Mohamed-Diaa-ES
 * @date        2025-10-23
 * @copyright   Copyright (c) 2025, Gestell Company
 */
/**                     File Inclusion                           */
#include "../../Mcal\DIO\DIO_Interface.h"
#include "../../Mcal\ADC\ADC_Interface.h"
#include "../../Mcal\ADC\ADC_Program.C"
#include "hCurrent_Conifg.h"
#include "hCurrent_Interface.h"
#include "hCurrent_Private.h"
#include <math.h>
#include <stdint.h>
/**                     File Inclusion                           */

/**
 * @var isADC_Initialized
 * @brief Tracks whether the ADC module has been initialized.
 * @details Used to prevent redundant initialization of the ADC hardware.
 */
extern uint8_t isADC_Initialized;
/**
 * @var ACS712_ZERO_OFFSET
 * @brief Stores the zero-current voltage offset for the ACS712 sensor.
 * @details Initialized to a default value and updated during calibration to improve accuracy.
 */
static float ACS712_ZERO_OFFSET = ACS712_Initial_ZERO_OFFSET;


void hCurrent_Init(void)
{
    if (isADC_Initialized==0) {
        
        mADC_Init();
        isADC_Initialized=1;
    }
    mDIO_SetDirectionForPin(ADC_Group ,ACS712_PIN ,Input);
    
}

float hCurrent_ReadInstant(void)
{
    float ADC_Val =(float)mADC_Read(ACS712_PIN);
    float Vout=(ADC_Val / ADC_MAX) * Vref;
    float Current =(Vout - ACS712_ZERO_OFFSET) / ACS712_SENSITIVITY;
    return Current;
} // Instantaneous current (A)

float hCurrent_ReadRMS(uint16_t samples) // RMS current over N samples
{
    float Sum_Of_Squares=0;
    uint16_t N=samples;
    while (N--) 
    {
        float Cur_VAL=hCurrent_ReadInstant();
        Sum_Of_Squares+=Cur_VAL*Cur_VAL;
    }
    Sum_Of_Squares/=samples;
    
    float RMS_Current=sqrt(Sum_Of_Squares);

    return RMS_Current;
}

void hCurrent_Calibrate(void) // Adjust zero offset at no load
{
    uint16_t SamplesNum=Num_OF_Calibration_Samples;
    float ADC_Avrg_samples=0;
    while (SamplesNum) {
    ADC_Avrg_samples+= mADC_Read(ACS712_PIN);
    }
    ADC_Avrg_samples/=Num_OF_Calibration_Samples;

    float VoltageConversion=(ADC_Avrg_samples/ADC_MAX)*Vref;

    ACS712_ZERO_OFFSET=VoltageConversion;

}