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
#include <math.h>
#include "../LCD/LCD_Interface.h"
extern isADC_Initialized ;

static float Voltage_Scaling_Factor=(R_With_GND + R_WITH_Vcc) / R_With_GND;

static float Voltage_Value=0.0f;

Voltage_RMS_Data Voltage_RMS ={
.sumOfSquares=0.0f,
.Voltage_RMS_Value=0.0f,
.Voltage_Previous_RMS=0.0f,
.sampleCount=0
};
Voltage_Calibration_Data Voltage_Calibration={
.Voltage_Prev_Value=0.0f,
.Voltage_Current_Value=0.0f,
.Callibration_Count=0,
.ADC_Reading_Sum=0.0f



};
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
    uint16_t ADC_Value = mADC_Read(Voltage_Pin);
    float Voltage = ((float)ADC_Value / 1023.0f) * Voltage_REF * Voltage_Scaling_Factor;
    return Voltage;
}
float hVoltage_ReadRMS()
{
    if (Voltage_RMS.sampleCount<Voltage_Calibration_Samples)
    {
        return sqrt (Voltage_RMS.Voltage_Previous_RMS);
    }
    else {
return sqrt (Voltage_RMS.Voltage_RMS_Value);
  }
}
void hVoltage_Calibrate(float ref)
{
     float currentVoltage;
    if (Voltage_Calibration.Callibration_Count<Voltage_Calibration_Samples)
    {
        currentVoltage = Voltage_Calibration.Voltage_Prev_Value;
    }
   else {
        currentVoltage = Voltage_Calibration.Voltage_Current_Value;
    }
    float calibrationFactor = ref / currentVoltage;
    // Update the scaling factor
   Voltage_Scaling_Factor*= calibrationFactor;
}

void hVoltage_Callback(uint16_t dummy)
{
   
    Voltage_Value=dummy;
     Voltage_Value *= (1 / 1023.0f) * Voltage_REF * Voltage_Scaling_Factor;

     Voltage_RMS.sampleCount++;
     Voltage_RMS.sumOfSquares += Voltage_Value * Voltage_Value;

     if (Voltage_RMS.sampleCount==Voltage_Calibration_Samples)
     {
            Voltage_RMS.Voltage_Previous_RMS=Voltage_RMS.Voltage_RMS_Value;
            Voltage_RMS.Voltage_RMS_Value=Voltage_RMS.sumOfSquares / Voltage_RMS.sampleCount;
            Voltage_RMS.sumOfSquares=0.0f;
            Voltage_RMS.sampleCount=0;
     }
     else {
        // do nothing
     }
    //  update Calibration data
     Voltage_Calibration.Callibration_Count++;
        Voltage_Calibration.ADC_Reading_Sum += Voltage_Value;
        if(Voltage_Calibration.Callibration_Count==Voltage_Calibration_Samples)
        {
            Voltage_Calibration.Voltage_Prev_Value=Voltage_Calibration.Voltage_Current_Value;
            Voltage_Calibration.Voltage_Current_Value=Voltage_Calibration.ADC_Reading_Sum / Voltage_Calibration.Callibration_Count;
            Voltage_Calibration.ADC_Reading_Sum=0.0f;
            Voltage_Calibration.Callibration_Count=0;
        }
        else {
            // do nothing
        }
}
#endif