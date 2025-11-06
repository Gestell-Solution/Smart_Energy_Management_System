/**
 * @file Voltage_Interface.h
 * @brief This file contains the Private Definitions for the Voltage module.
 * @version 2.0
 * @date 2025-11-6
 * @author Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author Reviewer : Mohamed Diaa   (mohammeddiaato@gmail.com)
 * @copyright Copyright (c) 2025 ,Gestell Company
 */




#ifndef Voltage_Private_H
#define Voltage_Private_H
/**
 * @{
 * @struct Voltage_RMS_Data
 * @brief This structure holds the data for RMS voltage calculation.
 * @var sumOfSquares
 * @brief Accumulates the sum of squares of voltage samples.
 * @var Voltage_RMS_Value
 * @brief Stores the calculated RMS voltage value.
 * @var Voltage_Previous_RMS
 * @brief Stores the previous RMS voltage value for comparison.
 * @var sampleCount
 * @brief Counts the number of samples taken for RMS calculation.
 */
typedef struct 
{
    
    float sumOfSquares;
    float Voltage_RMS_Value;
    float Voltage_Previous_RMS;
    uint16_t sampleCount;

}Voltage_RMS_Data;
/**@} */
/**
 * @{
 * @struct Voltage_Calibration_Data
 * @brief This structure holds the data for voltage calibration.
 * @var Voltage_Prev_Value
 * @brief Stores the previous voltage value during calibration.
 * @var Voltage_Current_Value
 * @brief Stores the current voltage value during calibration.
 * @var Callibration_Count
 * @brief Counts the number of calibration samples taken.
 * @var ADC_Reading_Sum
 * @brief Accumulates the sum of ADC readings for calibration.
 */
typedef struct {
    float Voltage_Prev_Value ;
    float Voltage_Current_Value ;
    uint16_t Callibration_Count ;
    float ADC_Reading_Sum ;
} Voltage_Calibration_Data;
/**@} */

/**
 * @def Voltage_Calibration_Samples
 * @brief Number of samples to take for voltage calibration.
 */
#define Voltage_Calibration_Samples 200
#endif 