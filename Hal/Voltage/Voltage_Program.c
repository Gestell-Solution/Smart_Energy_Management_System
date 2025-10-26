/**
 * @file Voltage_Interface.h
 * @brief This file contains the implementation of Voltage Driver
 * @version 1.0
 * @date 2024-10-24
 * @author Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author Reviewer : Mohamed Diaa   (mohammeddiaato@gmail.com)
 * @copyright Copyright (c) 2025 ,Gestell Company
 */



#include"../../Common/Config.h"
#if VoltageSensor_Module==Enable
#include"Voltage_Interface.h"
extern isADC_Initialized ;
void hVoltage_Init(void)
{
    if (isADC_Initialized == 0)
    {
        mADC_Init();
        isADC_Initialized = 1;
      mDIO_SetDirectionForPin(Voltage_Group, Voltage_Pin, Input);

    }
    else {
    mDIO_SetDirectionForPin(Voltage_Group, Voltage_Pin, Input);
    
}
}

float hVoltage_ReadInstant(void)
{
    uint16_t ADC_Value = mADC_ReadChannel(Voltage_Pin);
    float Voltage = ((float)ADC_Value / 1023.0f) * Voltage_REF * Voltage_Scaling_Factor;
    return Voltage;
}
float hVoltage_ReadRMS(uint16_t samples)
{
    uint32_t sumOfSquares = 0;
    for (uint16_t i = 0; i < samples; i++)
    {
        uint16_t ADC_Value = mADC_ReadChannel(Voltage_Pin);
        float Voltage = ((float)ADC_Value / 1023.0f) * Voltage_REF * Voltage_Scaling_Factor;
        sumOfSquares += (uint32_t)(Voltage * Voltage);
    }
    float meanOfSquares = (float)sumOfSquares / (float)samples;
    float RMS_Voltage = sqrtf(meanOfSquares);
    return RMS_Voltage;
}
void hVoltage_Calibrate(float ref)
{
    float currentVoltage = hVoltage_ReadInstant();
    float calibrationFactor = ref / currentVoltage;
    // Update the scaling factor
    #undef Voltage_Scaling_Factor
    #define Voltage_Scaling_Factor (11.0f * calibrationFactor)
}
#endif