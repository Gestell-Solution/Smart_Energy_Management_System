/**
 * @file        hCurrent_program.c
 * @brief       ACS712 30A driver implementation.
 * @details     Implements initialization, reading, and conversion functions for
 * the ACS712 current sensor using ADC.
 * @version     1.0.0
 * @author      Mohamed Diaa <mohammediaato@gmail.com> @Mohamed-Diaa-ES
 * @date        2025-10-23
 * @copyright   Copyright (c) 2025, Gestell Company
 */
/**                     File Inclusion                           */
#include "../../Mcal\DIO\DIO_Interface.h"
#include "../../Mcal\ADC\ADC_Interface.h"
#include "hCurrent_Config.h"
#include "hCurrent_Interface.h"
#include "hCurrent_Private.h"
#include <math.h>
#include <stdint.h>

/**
 * @fn void hcurrent_CallBack(uint16_t dummy)
 * @brief ADC interrupt callback for the ACS712 current sensor.
 * @details Invoked from the ADC ISR to notify the current-sensing layer of a new
 *          ADC conversion or to act as a simple conversion-complete signal.
 * @param[in] dummy 16-bit ADC sample.
 * @warning This function runs in interrupt context. 
 */
void hcurrent_CallBack(uint16_t dummy);

/**                     File Inclusion                           */

/**
 * @var isADC_Initialized
 * @brief Tracks whether the ADC module has been initialized.
 * @details Used to prevent redundant initialization of the ADC hardware.
 */
extern uint8_t isADC_Initialized;
/**
 * @var ACS712_ZERO_OFFSET
 * @brief Stores the zero-current voltage offset for the ACS712 sensor.
 * @details Initialized to a default value and updated during calibration to improve accuracy.
 */
static float ACS712_ZERO_OFFSET = ACS712_Initial_ZERO_OFFSET;
/**
 * @var ADC_Current_Value
 * @brief Last ADC-derived current value / accumulator used for RMS calculations.
 * @details
 * File-scope (static) floating-point value initialized to 0.0f. Holds the most recent
 * ADC measurement or an accumulated/summed value that is consumed by the RMS and
 * calibration routines. The physical meaning (raw ADC counts vs. converted amperes)
 * depends on the active calibration parameters stored in Calibration_Actions.
 * @note
 * This variable has static linkage (visible only within the defining compilation unit).
 * If it is accessed from multiple execution contexts (e.g., main code and an ISR),
 * access must be synchronized to avoid data races.
 */
static float ADC_Current_Value = 0;



/**
 * @var RMS_Actions
 * @brief Container for RMS-related accumulators and results.
 * @details
 * Holds the data used to compute root-mean-square (RMS) values for the current
 * measurement channel. Typically includes accumulators, sample counts and the
 * computed RMS result. Initialized to zeros. The exact fields and semantics are
 * defined by the RMS_Data type.
 * @note
 * This variable is file-scoped (static) and persists for the lifetime of the program.
 * Concurrent access must be protected if used across threads/interrupts.
 */
static RMS_Data RMS_Actions = {0, 0, 0,0};


/**
 * @var Calibration_Actions
 * @brief Calibration parameters and state for current measurement conversion.
 * @details
 * Stores calibration data such as offsets, scale factors and any runtime state
 * required to convert raw ADC readings into physical units (e.g., amperes) or to
 * correct systematic measurement errors. Initialized to zeros. See the
 * Calibration_Data type for the exact field layout and semantics.
 * @note
 * As a static file-scope object, access should be synchronized when modified or
 * read from multiple contexts to ensure consistency.
 */
static Calibration_Data Calibration_Actions = {0, 0, 0,0};

void hCurrent_Init(void)
{
    if (isADC_Initialized == 0)
    {

        mADC_Init();
        isADC_Initialized = 1;
    }
    mDIO_SetDirectionForPin(ADC_Group, ACS712_PIN, Input);
    mADC_RegisterChannel(ACS712_PIN,hcurrent_CallBack);
}

float hCurrent_ReadInstant(void)
{
    float Vout = (ADC_Current_Value / ADC_MAX) * Vref;
    float Current = (Vout - ACS712_ZERO_OFFSET) / ACS712_SENSITIVITY;
    return Current;
} // Instantaneous current (A)


float hCurrent_ReadRMS() // RMS current over N samples using polling
{
    if (RMS_Actions.RMS_Samples_Num < RMS_Nominal_Samples_Num)
    {
        return sqrt(RMS_Actions.Previous_RMS_Value);
    }
    return sqrt(RMS_Actions.Current_RMS_Value);
}

void hCurrent_Calibrate(void) // Adjust zero offset at no load
{

    float VoltageConversion=0;
    if (Calibration_Actions.Callibration_Samples_Num < RMS_Nominal_Samples_Num)
    {

        float VoltageConversion = (Calibration_Actions.Previous_ADC_Avrg_Value/ ADC_MAX) * Vref;
        
        
    }else
    {
        
        float VoltageConversion = (Calibration_Actions.Current_ADC_Avrg_Value/ ADC_MAX) * Vref;
        
    }
    
    
    ACS712_ZERO_OFFSET = VoltageConversion;
}

void hcurrent_CallBack(uint16_t dummy) // setting the callback of the current sensor
{
    ADC_Current_Value = dummy;

    // Upadting RMS
    RMS_Actions.RMS_Samples_Num++;
    float Current_Val = hCurrent_ReadInstant();
    RMS_Actions.Summing_Squares += Current_Val * Current_Val;

    if (RMS_Actions.RMS_Samples_Num == RMS_Nominal_Samples_Num)
    {
        RMS_Actions.RMS_Samples_Num = 0;
        RMS_Actions.Previous_RMS_Value = RMS_Actions.Current_RMS_Value;
        RMS_Actions.Current_RMS_Value = RMS_Actions.Summing_Squares / RMS_Nominal_Samples_Num;
        RMS_Actions.Summing_Squares = 0;
    }
    else
    {
    }

    // Updating callibration 
    Calibration_Actions.Callibration_Samples_Num++;
    Calibration_Actions.ADC_Readings_Sum += ADC_Current_Value;
    if (Calibration_Actions.Callibration_Samples_Num == Num_OF_Calibration_Samples)
    {
        Calibration_Actions.Callibration_Samples_Num = 0;
        Calibration_Actions.Previous_ADC_Avrg_Value = Calibration_Actions.Current_ADC_Avrg_Value;
        Calibration_Actions.Current_ADC_Avrg_Value = Calibration_Actions.ADC_Readings_Sum / Num_OF_Calibration_Samples;
        Calibration_Actions.Current_ADC_Avrg_Value=0;
    }
    else
    {
    }
    
}
