/**
 * @file       Config.h
 * @brief      This file contains configuration settings for enabling or disabling various modules in the project.
 * @details    This header file defines several macros that allow for easy enabling or disabling of specific modules based on project requirements.
 *             It includes configuration settings for both MCAL and HAL modules, as well as system-wide settings.
 * @version    0.1
 * @author     Hesham Ahmed (Hisham4Ahmed@gmail.com)
 * @date       2025-10-01
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

/**
 * @addtogroup Common
 * @{
 */
#include "Macros.h"

/**
 * @defgroup  MCAL Module State Configuration
 * @brief     This section contains configuration settings for enabling or disabling various MCAL modules.
 * @details   The Macros defined in this section allow for easy enabling or disabling of specific MCAL modules based on project requirements
 * @note      The available options for each module are 'Enable' and 'Disable'.
 * @{
 */
#define DIO_Module                  Enable  /**< Enable or Disable the DIO Module */
#define ADC_Module                  Enable  /**< Enable or Disable the ADC Module */
#define GIE_Module                  Enable  /**< Enable or Disable the GIE Module */
#define EEP_Module                  Enable  /**< Enable or Disable the EEPROM Module */
#define EXTI_Module                 Enable  /**< Enable or Disable the EXTI Module */
#define Timer0_Module               Disable /**< Enable or Disable the Timer0 Module */
#define Timer1_Module               Disable /**< Enable or Disable the Timer1 Module */
#define UART_Module                 Enable /**< Enable or Disable the UART Module */
#define SPI_Module                  Disable /**< Enable or Disable the SPI Module */
#define TWI_Module                  Disable /**< Enable or Disable the TWI Module */
#define System_Controller_Module    Enable /**< Enable or Disable the TWI Module */

/** @} */
/*****************************************************************************************************************************************************/
/**
 * @defgroup  HAL Module State Configuration
 * @brief     This section contains configuration settings for enabling or disabling various HAL modules.
 * @details   The Macros defined in this section allow for easy enabling or disabling of specific HAL modules based on project requirements
 * @note      The available options for each module are 'Enable' and 'Disable'.
 * @{ */
#define RGB_Module            Disable  /**< Enable or Disable the RGB Module */
#define LCD_Module            Disable  /**< Enable or Disable the LCD Module */
#define Btn_Module            Enable  /**< Enable or Disable the Button Module */
#define Buzzer_Module         Enable  /**< Enable or Disable the Buzzer Module */
#define Relay_Module          Enable  /**< Enable or Disable the Relay Module */
#define HC05_Module           Disable /**< Enable or Disable the HC05 Module */
#define ESP01_Module          Disable /**< Enable or Disable the ESP01 Module */
#define VoltageSensor_Module  Enable  /**< Enable or Disable the Voltage Sensor Module */
#define CurrentSensor_Module  Enable  /**< Enable or Disable the Current Sensor Module */

/** @} */
/*****************************************************************************************************************************************************/
/**
 * @defgroup  System Configuration
 * @brief     This section contains system-wide configuration settings.
 * @details   The Macros defined in this section allow for easy configuration of system parameters such as CPU frequency and error handling.
 * @{ */
#define ERROR_STATE         Enable   /**< Enable or Disable the Error State Module */
#define SystemDataManager   Enable   /**< Enable or Disable the System Data Manager Module */
#define ProtectionManager   Enable   /**< Enable or Disable The Protection Manager Module */
/**
 * @}
 */

#endif /* _COMFIG_H_*/