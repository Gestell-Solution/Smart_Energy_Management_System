/**
 * @file Voltage_Interface.h
 * @brief This file contains the implementation of Voltage Driver
 * @version 1.0
 * @date 2024-10-24
 * @author Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author Reviewer : Mohamed Diaa   (mohammeddiaato@gmail.com)
 * @copyright Copyright (c) 2025 ,Gestell Company
 */

#include "../../Common/Config.h"
#if VoltageSensor_Module == Enable
#include "Voltage_Interface.h"
#include "../LCD/LCD_Interface.h"
#include <math.h>
extern uint8_t isADC_Initialized;


float calibrationFactor = 1.0f;
static float Voltage_Value = 0.0f;

volatile uint8_t New_Sample_Flag = 0;

Voltage_RMS_Data Voltage_RMS = {
    .sumOfSquares = 0.0f,
    .Voltage_RMS_Value = 0.0f,
    .Voltage_Previous_RMS = 0.0f,
    .sampleCount = 0};
Voltage_Calibration_Data Voltage_Calibration = {
    .Voltage_Prev_Value = 0.0f,
    .Voltage_Current_Value = 0.0f,
    .Callibration_Count = 0,
    .ADC_Reading_Sum = 0.0f

};
void hVoltage_Init(void)
{
    if (isADC_Initialized == 0)
    {
        mADC_Init();
        isADC_Initialized = 1;
        mDIO_SetDirectionForPin(Voltage_Group, Voltage_Pin, Input);
        mADC_RegisterChannel(Voltage_Pin, hVoltage_Callback);
    }
    else
    {
        mDIO_SetDirectionForPin(Voltage_Group, Voltage_Pin, Input);
        mADC_RegisterChannel(Voltage_Pin, hVoltage_Callback);
    }
}

float hVoltage_ReadInstant(void)
{
    float analog_voltage = (Voltage_Value / ADC_Max_Resolution) * Voltage_REF ;

    float Voltage = (analog_voltage-V_Offset) * AC_Ratio;
    return Voltage;
}

float hVoltage_ReadRMS(void)
{
    Voltage_RMS.sumOfSquares = 0.0f;
    Voltage_RMS.sampleCount = 0;
float instant_voltage=0;
    while (Voltage_RMS.sampleCount < Voltage_Calibration_Samples)
    {
        while (New_Sample_Flag == 0)
        {
            // Wait for new sample
        }

        New_Sample_Flag = 0;
         instant_voltage = hVoltage_ReadInstant();
         Voltage_RMS.sumOfSquares += (instant_voltage * instant_voltage);
        Voltage_RMS.sampleCount++;
    }

    float meanSquare = Voltage_RMS.sumOfSquares / (float)Voltage_RMS.sampleCount;
    Voltage_RMS.Voltage_RMS_Value = sqrt(meanSquare);

    return Voltage_RMS.Voltage_RMS_Value;
}
void hVoltage_Calibrate(float ref)
{
    // float currentVoltage;
    // if (Voltage_Calibration.Callibration_Count < Voltage_Calibration_Samples)
    // {
    //     currentVoltage = Voltage_Calibration.Voltage_Prev_Value;
    // }
    // else
    // {
    //     currentVoltage = Voltage_Calibration.Voltage_Current_Value;
    // }
    // calibrationFactor = ref / currentVoltage;
    // Update the scaling factor
    // Voltage_Scaling_Factor *= calibrationFactor;
}

void hVoltage_Callback(uint16_t dummy)
{

    Voltage_Value = dummy;
    New_Sample_Flag = 1;
}
#endif