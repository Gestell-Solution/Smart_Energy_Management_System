/**
 * @file       MeasurementEngine_Progarm.c
 * @brief      Implementation of the Measurement Engine Module.
 * @details    This module is responsible for acquiring raw sensor data (Voltage and Current),
 *             performing RMS calculations, and deriving Power and Energy values.
 *             It acts as the central hub for all electrical measurements in the system.
 *             
 *             All hardware-specific interactions are abstracted via HAL functions.
 * @version    1.0
 * @date       2025-11-11
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer:  Mohamed Diaa (mohammediaato@gmail.com)
 * @copyright  Copyright (c) 2025, Gestell Company
 */

/*============================================================================
 *                                 Includes
 *============================================================================*/
#include "MeasurementEngine_Interface.h"
#include "MeasurementEngine_Config.h"
#include "MeasurementEngine_Private.h"

/*============================================================================
 *                                 Private Variables
 *============================================================================*/

/** @brief Latest calculated RMS Voltage (Volts). */
static float ME_Vrms = 0.0f;

/** @brief Latest calculated RMS Current (Amperes). */
static float ME_Irms = 0.0f;

/** @brief Latest calculated Active Power (Watts). */
static float ME_Power = 0.0f;

/** @brief Accumulated Energy (Joules/Watt-seconds). */
static float ME_Energy = 0.0f;

/*============================================================================
 *                                 Function Definitions
 *============================================================================*/

/**
 * @brief      Initializes the Measurement Engine.
 * @details    Calls the initialization routines for the underlying Voltage and Current sensor drivers.
 *             Ensures that ADC and other necessary peripherals are ready for conversion.
 * @return     void
 */
void ME_Init(void)
{
    hVoltage_Init();
    hCurrent_Init();
}

/**
 * @brief      Updates the electrical measurements.
 * @details    Triggers a read operation from the sensors to get the latest RMS values.
 *             Calculates:
 *             - Power = Vrms * Irms (assuming Unity Power Factor for simplicity, or sensor provides real power).
 *             - Energy = Energy + (Power * Sample_Interval).
 *             This function should be called periodically at a fixed interval (`ME_SAMPLE_INTERVAL`).
 * @return     void
 */
void ME_Update(void)
{
    ME_Vrms = hVoltage_ReadRMS();
    ME_Irms = hCurrent_ReadRMS();

    ME_Power = ME_Vrms * ME_Irms;

    /* Energy accumulation: Energy (J) = Power (W) * Time (s) */
    ME_Energy += ME_Power * ME_SAMPLE_INTERVAL;
}

/**
 * @brief      Getter for the last measured Voltage RMS.
 * @return     float Voltage in Volts (V).
 */
float ME_GetVoltageRMS(void) 
{ 
    return ME_Vrms; 
}

/**
 * @brief      Getter for the last measured Current RMS.
 * @return     float Current in Amperes (A).
 */
float ME_GetCurrentRMS(void) 
{ 
    return ME_Irms; 
}

/**
 * @brief      Getter for the calculated Active Power.
 * @return     float Power in Watts (W).
 */
float ME_GetPower(void)      
{ 
    return ME_Power; 
}

/**
 * @brief      Getter for the accumulated Energy.
 * @return     float Energy in Joules (J).
 */
float ME_GetEnergy(void)     
{ 
    return ME_Energy; 
}

/**
 * @brief      Resets the internal energy counter to zero.
 * @details    This is useful for starting a new metering session or after clearing stored data.
 * @return     void
 */
void ME_ResetEnergy(void)
{
    ME_Energy = 0.0f;
}
