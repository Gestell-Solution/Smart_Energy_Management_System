/**
 * @file       program.c
 * @brief      Main application for reading electrical measurements.
 * @details    This file contains the main application code that initializes
 *             the voltage and current sensors as well as the Measurement Engine.
 *             It periodically updates the measurements and retrieves values such as:
 *             - RMS voltage
 *             - RMS current
 *             - Instantaneous power
 *             - Total energy consumption
 *             
 *             The program relies on the HAL interfaces for voltage and current sensors
 *             and the Measurement Engine to perform real-time calculations.
 *             
 *             All hardware-specific interactions are abstracted via HAL functions.
 * @version    1.0
 * @date       2025-11-11
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer: Mohamed Diaa (mohammediaato@gmail.com)
 * @copyright  Copyright (c) 2025, Gestell Company
 */
//==============================================================================
//                                Includes
#include "MeasurementEngine_Interface.h"
#include "MeasurementEngine_Config.h"
#include "MeasurementEngine_Private.h"

//==============================================================================
static float ME_Vrms ;
static float ME_Irms ;
static float ME_Power ;
static float ME_Energy ;

//==============================================================================
void ME_Init(void)
{

    hVoltage_Init();
    hCurrent_Init();

   
}
//==============================================================================
/*void ME_Update(void)
{
    ME_Vrms = hVoltage_ReadRMS();
    ME_Irms = hCurrent_ReadRMS();

    ME_Power = ME_Vrms * ME_Irms;

    ME_Energy += ME_Power * ME_SAMPLE_INTERVAL;
}
    */
//==============================================================================
float ME_GetVoltageRMS(void) 
{ 
    ME_Vrms = hVoltage_ReadRMS();

    return ME_Vrms ; 
}

/*//==============================================================================
float ME_GetCurrentRMS(void) 
{ 
    return ME_Irms; 
}
//==============================================================================
float ME_GetPower(void)      
{ 
    return ME_Power; 
}

//==============================================================================
float ME_GetEnergy(void)     
{ 
    return ME_Energy; 
}

//==============================================================================
void ME_ResetEnergy(void)
{
    ME_Energy = 0.0f;
}
//==============================================================================
*/