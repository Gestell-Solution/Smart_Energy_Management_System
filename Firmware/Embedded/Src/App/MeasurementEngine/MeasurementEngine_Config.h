/**
 * @file       MeasurementEngine_Config.h
 * @brief      Configuration definitions for the Measurement Engine module.
 * @details    This file defines configuration macros for the Measurement Engine,
 *             including sampling intervals.
 *             These values can be adjusted to match the specific hardware setup
 *             or measurement requirements.
 * @version    1.0
 * @date       2025-11-11
 * @author     Developer: Ahmed Twap (ahmedtwap2@gmail.com)
 * @author     Reviewer: Mohamed Diaa (mohammediaato@gmail.com)
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#ifndef MEASUREMENT_ENGINE_CONFIG_H
#define MEASUREMENT_ENGINE_CONFIG_H

/**
 * @def ME_SAMPLE_INTERVAL
 * @brief Sampling period between consecutive ME_Update() calls in seconds.
 * @details Determines how often the Measurement Engine reads new ADC samples.
 */
#define ME_SAMPLE_INTERVAL   0.1f   /* Match actual ME_Update() call period (e.g. 100 ms) */

/**
 * @def Resistive_Load_PF
 * @brief Power Factor for Resistive Loads.
 */
#define Resistive_Load_PF 1.0f
/**
 * @def AVG_Residential_Load_PF
 * @brief Average Power Factor for Residential Loads, Assumed to be 0.85.
 */
#define AVG_Residential_Load_PF 0.85f

/**
 * @def Load_Type
 * @brief Type of load connected to the system.
 */
#define Load_Type AVG_Residential_Load

/**
 * @def Resistive_Load
 * @brief Identifier for Resistive Load type.
 */
#define Resistive_Load 1
/**
 * @def AVG_Residential_Load
 * @brief Identifier for Average Residential Load type.
 */
#define AVG_Residential_Load 2

#endif /* MEASUREMENT_ENGINE_CONFIG_H */
