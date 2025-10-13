/**
 * @file ADC_Interface.h
 * @brief This file contains the interface for the ADC (Analog-to-Digital Converter) module.
 * @version 1.0
 * @author  Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author  Reviewer  :
 * @date 2025-10-11
 * @copyright Copyright (c) 2025 , Gestell Company
 */



#ifndef ADC_INTERFACE_H
#define ADC_INTERFACE_H

#include <stdint.h>
#include "ADC_Private.h"
#include "ADC_Config.h"
#include "../../Common/Macros.h"
/**
 * @fn mADC_Init(const ADC_ConfigType *config)
 * @brief   This Function Initialize the ADC Module
 * @param *config : this is a struct contains the configuration parameters for the ADC module.
 * @return  void
 */
void mADC_Init(const ADC_ConfigType *config);

/**
 * @fn uint16_t mADC_Read(uint8_t channel)
 * @brief  This Function reads the ADC value from the specified channel.
 * @return returns the 10-bit ADC value.
 * @warning to use this function the ADC must be initialized in Synchronous_Mode from the config.h
 */
uint16_t mADC_Read(uint8_t channel);

/**
 * @fn mADC_RegisterChannel(uint8_t channel, void (*callback)(uint16_t value))
 * @brief This Function registers a callback function for a specific ADC channel.
 * @param channel : the ADC channel number (0-7) in our project (0-1).
 * @param callback : pointer to the callback function that will be called with the ADC value when conversion is complete.
 * @return void
 */
void mADC_RegisterChannel(uint8_t channel, void (*callback)(uint16_t value));


/**
 * @fn mADC_StartGroup(void)
 * @brief enables auto-triggering and starts the ADC conversion process for a group of channels in round-robin fashion.
 * @note this function starts conversions starting from channel0
 * @warning to use this function the ADC must be initialized in Asynchronous_Mode from the config.h
 */
void mADC_StartGroup(void);

/**
 * @fn mADC_Stop(void)
 * @brief This Function stops the ADC auto-triggering and conversion process.
 * @warning to use this function the ADC must be initialized in Asynchronous_Mode from the config.h
 */
void mADC_Stop(void);
/**
 * @fn __vector_16(void)
 * @brief ISR Action for ADC Conversion
 * @warning this function won't work except if the ADC Interrupt is enabled from the config.h
 */
void __vector_16(void)__attribute__((signal)); // ADC Conversion Complete ISR

#endif