/**
 * @file ADC_Config.h
 * @brief This file contains the configuration for the ADC (Analog-to-Digital Converter) module.
 * @version 1.0
 * @author  Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author  Reviewer  :
 * @date 2025-10-11
 * @copyright Copyright (c) 2025 , Gestell Company
 */


 #ifndef ADC_CONFIG_H
 #define ADC_CONFIG_H
 #include "ADC_Private.h"
#include  "../Atmega32RegistersAddress.h"

/**
 * @defgroup ADC Module Configuration Parameters
 * @def ADC_PRESCALER
 * @brief Represents the adc prescaler value.
 * @def ADC_VOLTAGE_REF
 * @brief Represents the adc voltage reference.
 * @def ADC_AUTO_TRIGGER
 * @brief Represents the adc auto trigger state.
 * @def ADC_INTERRUPT
 * @brief represents the adc interrupt state
 * @def ADC_TRIGGER_SOURCE
 * @brief represents the adc trigger source
 * @def ADC_ENABLE_state
 * @brief determines whether the adc is enabled or disabled
 * @def ADC_Mode
 * @brief determines whether the adc works in synchronous mode or asynchronous mode
 * @note to see the options for each parameter check ADC_Private.h
 * @warning Make sure to configure these parameters according to your application requirements.
 */
#define ADC_PRESCALER         ADC_Prescaler_DIV32
#define ADC_VOLTAGE_REF       ADC_AREF
#define ADC_AUTO_TRIGGER      ADC_AUTO_TRIGGER_Enable
#define ADC_INTERRUPT         ADC_INTERRUPT_Enable
#define ADC_TRIGGER_SOURCE    ADC_FREE_RUNNING_MODE
#define ADC_ENABLE_state      ADC_ENABLE
#define ADC_Mode   Asynchronous_Mode

/**at Synchronous mode all prescalares working perfectly , at Async only prescaler above or equal to DIV32*/


 #endif