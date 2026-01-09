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
#define ME_SAMPLE_INTERVAL   0.01f   



#endif /* MEASUREMENT_ENGINE_CONFIG_H */
