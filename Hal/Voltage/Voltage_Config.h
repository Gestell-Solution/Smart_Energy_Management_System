/**
 * @file Voltage_Config.h
 * @brief This file contains the configuration for the Voltage module.
 * @version 2.0
 * @date 2024-11-6
 * @author Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author Reviewer : Mohamed Diaa   (mohammeddiaato@gmail.com)
 * @copyright Copyright (c) 2025 ,Gestell Company
 */



#ifndef Voltage_Config_H
#define Voltage_Config_H
#include"../../Common/Config.h"
#include"../../Common/Macros.h"
/**
 * @def Voltage_Group
 * @brief Refers to the ADC Group (GroupA) 
 */
#define Voltage_Group     GroupA           /**< Select the ADC Group connected to Voltage Sensor */
/**
 * @def Voltage_Pin
 * @brief Refers to the ADC Pin (PIN1)
 */
#define Voltage_Pin       PIN1            /**< Select the ADC Pin connected to Voltage Sensor */
/**
 * @def Voltage_REF
 * @brief Refers to the maximum Voltage can be input in the ADC (5V)
 */
#define Voltage_REF       5UL            /**< Reference Voltage for ADC in Volts (V) */

/**
 * @def R_WITH_ADC
 * @brief The Resistor between Vcc and the adc input
 */
#define R_WITH_Vcc     590000.0f          /**< The Resistor between Vcc and the adc input  */
/**
 * @def R_With_GND
 * @brief The Resistor between GND and the adc input
 */
#define R_With_GND     10000.0f         /**< the Resisotr between GND And ADC Input  */
/**
 * @def VCC_Voltage
 * @brief The Vcc Voltage Which needs to be measured
 */
#define VCC_Voltage   300            /**< The Vcc Voltage Value in Volts (V) */

#endif