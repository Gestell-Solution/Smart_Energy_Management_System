/**
 * @file Voltage_Interface.h
 * @brief This file contains the interface for the Voltage module.
 * @version 1.0
 * @date 2024-10-24
 * @author Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author Reviewer : Mohamed Diaa   (mohammeddiaato@gmail.com)
 * @copyright Copyright (c) 2025 ,Gestell Company
 */





#ifndef Voltage_Interface_H
#define Voltage_Interface_H
#include "../../Mcal/Adc/Adc_Interface.h"
#include "Voltage_Config.h"
#include "Voltage_Private.h"
/**
 * @defgroup Voltage Sensor Api's
 * @brief This section contains the APIs for the Voltage Sensor module.
 * @{
 */

/**
 * @fn hVoltage_Init
 * @brief Initializes the voltage measurement module.
 */

void hVoltage_Init(void);

/**
 * @fn hVoltage_ReadInstant
 * @brief Reads the instantaneous voltage value.
 * @details This function reads the instantaneous voltage value from the ADC Voltage Pin 
 * @return Returns a Floating Value of the Instantaneous Voltage Value in Volts (V)
 */

float hVoltage_ReadInstant(void); // Instantaneous voltage (V)

/**
 * @fn hVoltage_ReadRMS
 * @brief Reads the RMS voltage value over a specified number of samples.
 * @param samples The number of samples to average for RMS calculation.
 * @return Returns a Floating Value of the RMS Voltage Value in Volts (V)
 */

float hVoltage_ReadRMS(uint16_t samples); // RMS voltage (V) over N samples

/**
 * @fn hVoltage_Calibrate
 * @brief Calibrates the voltage measurement scaling factor.
 * @param ref The reference voltage value for calibration.
 */

void hVoltage_Calibrate(float ref); // Calibrate scaling factor

/** @} */

#endif