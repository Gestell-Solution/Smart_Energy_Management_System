/**
 * @file       MeasurementEngine_Private.h
 * @brief      Private variables for the Measurement Engine module.
 * @details    This header defines internal variables used by the Measurement Engine
 *             to store RMS voltage, RMS current, instantaneous power, and total energy.
 *             These variables are not exposed outside the module and should only
 *             be accessed via the Measurement Engine API.
 * @version    1.0
 * @date       2025-11-11
 * @author     Developer: Ahmed Twap (ahmedtwap2@gmail.com)
 * @author     Reviewer: Mohamed Diaa (mohammediaato@gmail.com)
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#ifndef MEASUREMENT_ENGINE_PRIVATE_H
#define MEASUREMENT_ENGINE_PRIVATE_H

/* Internal RMS voltage value (Volts) */
static float ME_Vrms = 0.0f;

/* Internal RMS current value (Amperes) */
static float ME_Irms = 0.0f;

/* Internal instantaneous power (Watts) */
static float ME_Power = 0.0f;

/* Internal accumulated energy (Watt-seconds / Joules) */
static float ME_Energy = 0.0f;

#endif /* MEASUREMENT_ENGINE_PRIVATE_H */
