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
#include "../../Common/SystemDataManager/SystemDataManager.h"
#include "../../Mcal/Timer0/TIMER0_Interface.h"

/*============================================================================
 *                                 Private Variables
 *============================================================================*/

/** @brief Latest calculated RMS Voltage (Volts). */
static float ME_Vrms = 0.0f;

/** @brief Latest calculated RMS Current (Amperes). */
static float ME_Irms = 0.0f;

/** @brief Latest calculated Apparent Power (VA). */
static float ME_Apparent_Power = 0.0f;

/** @brief Accumulated Energy (Joules/Watt-seconds). */
static float ME_Energy = 0.0f;
/** @brief Latest calculated Active Power (Watts). */
static float ME_Active_Power = 0.0f;
/** @brief Last integration interval in milliseconds. */
static uint32_t ME_Last_Delta_Ms = 0u;
/** @brief Maximum observed integration interval in milliseconds. */
static uint32_t ME_Max_Delta_Ms = 0u;
/** @brief Timestamp of last integration update in milliseconds. */
static uint32_t ME_Last_Update_Ms = 0u;
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
    ME_Last_Update_Ms = mTIMER0_GetMillis();
    ME_Last_Delta_Ms = 0u;
    ME_Max_Delta_Ms = 0u;
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

    ME_Apparent_Power = ME_Vrms * ME_Irms;
#if Load_Type == Resistive_Load
    ME_Active_Power = ME_Apparent_Power * Resistive_Load_PF;
#elif Load_Type == AVG_Residential_Load
    ME_Active_Power = ME_Apparent_Power * AVG_Residential_Load_PF ;
#endif

    uint32_t nowMs = mTIMER0_GetMillis();
    uint32_t deltaMs = nowMs - ME_Last_Update_Ms; /* Unsigned wrap-safe */
    if (deltaMs > ME_MAX_DT_MS)
    {
        deltaMs = ME_MAX_DT_MS;
    }
    ME_Last_Update_Ms = nowMs;
    ME_Last_Delta_Ms = deltaMs;
    if (deltaMs > ME_Max_Delta_Ms)
    {
        ME_Max_Delta_Ms = deltaMs;
    }

    /* Energy accumulation: Energy (J) = Power (W) * Time (s) */
    ME_Energy += ME_Active_Power * ((float)deltaMs / 1000.0f);
g_SystemData.Voltage_RMS = ME_Vrms;
g_SystemData.Current_RMS = ME_Irms;
g_SystemData.Power= ME_Active_Power;
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
float ME_GetActivePower(void)      
{ 
    return ME_Active_Power; 
}

/**
 * @brief      Getter for the accumulated Energy.
 * @return     float Energy in Joules (J).
 */
float ME_GetEnergy(void)     
{ 
    return ME_Energy; 
}

uint32_t ME_GetLastDeltaMs(void)
{
    return ME_Last_Delta_Ms;
}

uint32_t ME_GetMaxDeltaMs(void)
{
    return ME_Max_Delta_Ms;
}

/**
 * @brief      Resets the internal energy counter to zero.
 * @details    This is useful for starting a new metering session or after clearing stored data.
 * @return     void
 */
void ME_ResetEnergy(void)
{
    ME_Energy = 0.0f;
    ME_Last_Update_Ms = mTIMER0_GetMillis();
    ME_Last_Delta_Ms = 0u;
    ME_Max_Delta_Ms = 0u;
}

float ME_GetApparentPower(void)
{
    return ME_Apparent_Power;
}
