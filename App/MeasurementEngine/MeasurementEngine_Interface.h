/**
 * @file       MeasurementEngine_Interface.h
 * @brief      Interface file for the Measurement Engine module.
 * @details    This file provides the function prototypes for initializing, updating, 
 *             and retrieving electrical measurement values such as RMS voltage, 
 *             RMS current, power, and total energy consumption.
 *             The Measurement Engine interacts with current and voltage sensor drivers 
 *             to compute real-time and accumulated electrical parameters.
 * @version    1.0
 * @date       2025-10-19
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer: Mohamed Diaa (mohammediaato@gmail.com)
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#ifndef _MeasurementEngine_Interface_h_
#define _MeasurementEngine_Interface_h_

/*================================= Includes =================================*/
#include "../../Hal/ACS712CurntSnsr/hCurrent_Interface.h"
#include "../../Hal/Voltage/Voltage_Interface.h"
#include "../../Common/Macros.h"
#include <string.h>
#include <stdint.h>

/*=========================== Group: Measurement Engine ======================*/
/**
 * @defgroup MeasurementEngine Measurement Engine
 * @brief     APIs for electrical measurement processing.
 * @details   The Measurement Engine is responsible for acquiring, processing, and 
 *            calculating key electrical parameters including RMS voltage/current, 
 *            active power, and energy consumption.
 * @{
 */

/*============================================================================*/
/**
 * @fn         void ME_Init(void)
 * @brief      Initializes the Measurement Engine module.
 * @details    Configures all required peripherals (ADC, sensors, etc.) and 
 *             initializes internal variables used for voltage and current measurement.
 * @note       This function must be called once before using any other Measurement Engine API.
 */
void ME_Init(void);

/*============================================================================*/
/**
 * @fn         void ME_Update(void)
 * @brief      Periodically updates measurement calculations.
 * @details    This function should be called at a fixed sampling rate to process 
 *             new ADC samples for voltage and current, updating the RMS, power, 
 *             and energy calculations.
 * @warning    Must be called periodically (e.g., via timer interrupt or scheduler).
 */
void ME_Update(void);

/*============================================================================*/
/**
 * @fn         float ME_GetVoltageRMS(void)
 * @brief      Returns the RMS value of the measured voltage.
 * @details    Calculates and returns the root mean square (RMS) voltage 
 *             based on recent ADC samples from the voltage sensor.
 * @return     float  RMS voltage in volts (V).
 */
float ME_GetVoltageRMS(void);

/*============================================================================*/
/**
 * @fn         float ME_GetCurrentRMS(void)
 * @brief      Returns the RMS value of the measured current.
 * @details    Calculates and returns the root mean square (RMS) current 
 *             from recent ADC readings of the current sensor.
 * @return     float  RMS current in amperes (A).
 */
float ME_GetCurrentRMS(void);

/*============================================================================*/
/**
 * @fn         float ME_GetPower(void)
 * @brief      Returns the instantaneous active power.
 * @details    Computes the active power based on voltage and current RMS values.
 * @return     float  Active power in watts (W).
 */
float ME_GetPower(void);

/*============================================================================*/
/**
 * @fn         float ME_GetEnergy(void)
 * @brief      Returns the accumulated energy consumption.
 * @details    Calculates total consumed energy over time based on integrated power values.
 * @return     float  Energy consumption in watt-hours (Wh).
 */
float ME_GetEnergy(void);

/*============================================================================*/
/**
 * @fn         void ME_ResetEnergy(void)
 * @brief      Resets the accumulated energy counter.
 * @details    Clears the stored energy value, effectively restarting the measurement cycle.
 * @note       Useful when starting a new measurement session.
 */
void ME_ResetEnergy(void);

/** @} */ /* End of MeasurementEngine group */

#endif /* _MeasurementEngine_Interface_h_ */
