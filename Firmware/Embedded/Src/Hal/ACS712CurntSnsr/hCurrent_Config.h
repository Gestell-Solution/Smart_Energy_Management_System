/**
 * @file        hCurrent__config.h
 * @brief       Configuration file for ACS712 30A current sensor.
 * @details     Contains macros and settings to configure the ACS712 sensor parameters such as sensitivity,
                 voltage reference, and ADC channel.
 * @version     1.0.0
 * @author      Mohamed Diaa <mohammediaato@gmail.com> @Mohamed-Diaa-ES
 * @date        2025-10-23
 * @copyright   Copyright (c) 2025, Gestell Company
 */
#ifndef _H_CURRENT_CONFIG_H_
#define _H_CURRENT_CONFIG_H_
#include "hCurrent_Private.h"

/**
 * @def ACS712_PIN
 * @brief Defines the ADC pin connected to the ACS712 sensor.
 */
#define ACS712_PIN  0ul 

/**
 * @def ACS712_SENSITIVITY
 * @brief Defines the sensitivity of the ACS712 30A sensor in volts per ampere.
 */
#define ACS712_SENSITIVITY 0.066 // volts per ampere

/**
 * @def Vref
 * @brief Defines the reference voltage used for ADC conversion.
 */
#define Vref 5.0

/**
 * @def ADC_MAX
 * @brief Defines the maximum ADC value for a 10-bit ADC.
 */
#define ADC_MAX 1023.0

/**
 * @def ACS712_Initial_ZERO_OFFSET
 * @brief Initial zero-current voltage offset (typically Vref/2).
 */
#define ACS712_Initial_ZERO_OFFSET (Vref / 2.0)

#define RMS_Nominal_Samples_Num  200

/**
 * @def Num_OF_Calibration_Samples
 * @brief Number of samples used during zero offset calibration.
 */
#define Num_OF_Calibration_Samples 100
#endif