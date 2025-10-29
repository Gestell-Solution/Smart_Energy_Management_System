/**
 * @file        hCurrent_interface.h
 * @brief       ACS712 30A driver interface.
 * @details     Declares public APIs for initializing and reading current values from the ACS712 sensor.
 * @version     1.0.0
 * @author      Mohamed Diaa <mohammediaato@gmail.com> @Mohamed-Diaa-ES
 * @date        2025-10-23
 * @copyright   Copyright (c) 2025, Gestell Company
 */
#ifndef _H_CURRENT_INTERFACE_H_
#define _H_CURRENT_INTERFACE_H_
#include <stdint.h>

typedef struct
{

    float Previous_ADC_Avrg_Value;
    float Current_ADC_Avrg_Value;
    uint16_t Callibration_Samples_Num;
    float   ADC_Readings_Sum;
} Calibration_Data;
typedef struct
{
    float Summing_Squares;
    float Current_RMS_Value;
    float Previous_RMS_Value;
    uint16_t RMS_Samples_Num;

} RMS_Data;

/**
 * @fn void hCurrent_Init(void)
 * @brief Initializes the ACS712 current sensor driver.
 * @details Configures the ADC and sets the direction of the sensor pin for input.
 *          Ensures ADC is initialized only once.
 * @warning Must be called before any current reading functions.
 */
void hCurrent_Init(void);

/**
 * @fn float hCurrent_ReadInstant(void)
 * @brief Reads the instantaneous current from the ACS712 sensor.
 * @details Converts the ADC value to voltage, subtracts the zero offset, and applies sensitivity to calculate current.
 * @return float Current value in amperes.
 * @note Ensure the sensor is properly calibrated for accurate readings.
 */
float hCurrent_ReadInstant(void);

/**
 * @fn float hCurrent_ReadRMS()
 * @brief Calculates the RMS current over a number of samples.
 * @return float RMS current value in amperes.
 * @retval Returns the calculated RMS current.
 * @note Larger sample sizes improve accuracy but increase computation time.
 */
float hCurrent_ReadRMS();
/**
 * @fn void hCurrent_Calibrate(void)
 * @brief Calibrates the zero-current offset of the ACS712 sensor.
 * @details Averages multiple ADC readings while no current is flowing to determine the baseline voltage offset.
 * @warning Ensure no current is flowing through the sensor during calibration.
 * @note This function should be called once during setup or when environmental conditions change.
 */
void hCurrent_Calibrate(void);
#endif