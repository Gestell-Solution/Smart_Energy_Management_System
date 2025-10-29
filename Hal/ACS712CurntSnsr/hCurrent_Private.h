/**
 * @file       hCurrent_private.h
 * @brief       Internal definitions for ACS712 30A driver.
 * @details     Provides static declarations and internal helper macros used only within the ACS712 driver implementation.
 * @version     1.0.0
 * @author      Mohamed Diaa <mohammediaato@gmail.com> @Mohamed-Diaa-ES
 * @date        2025-10-23
 * @copyright   Copyright (c) 2025, Gestell Company
 */
 #ifndef _H_CURRENT_PRIVATE_H_
 #define _H_CURRENT_PRIVATE_H_

/**
 * @def ADC_Group
 * @brief Defines the ADC group used for reading the ACS712 sensor.
 */
#define ADC_Group GroupA

/**
 * @def Vref
 * @brief Defines the reference voltage used for ADC conversion.
 */
#define Vref 5.0

/**
 * @def ADC_MAX
 * @brief Defines the maximum ADC value for a 10-bit ADC resolution.
 */
#define ADC_MAX 1023.0

/**
 * @def Num_OF_Calibration_Samples
 * @brief Number of samples used during zero offset calibration.
 */
#define Num_OF_Calibration_Samples 500

#endif