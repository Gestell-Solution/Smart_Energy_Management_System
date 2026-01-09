/**
 * @file       HC05_Config.h
 * @brief      Configuration file for the HC-05 Bluetooth driver.
 * @details    This file contains user-defined configuration macros for the HC-05 Bluetooth driver,
 *             including UART parameters and communication settings.
 *             These configurations can be adjusted based on system requirements.
 * @version    1.0
 * @date       2025-10-28
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer :Ahmed ashraf (ahmedashraf2022222@gmail.com)
 * @copyright  Copyright (c) 2025, Gestell Company
 */


#ifndef HC05_CONFIG_H_
#define HC05_CONFIG_H_

#include <stdint.h>
#include "HC05_Interface.h"

/*=========================== Group: HC05 Configuration ======================*/
/**
 * @defgroup HC05_Config HC05 Configuration
 * @brief     Configuration macros for the HC-05 Bluetooth driver.
 * @details   These macros define the default parameters used for UART-based Bluetooth communication.
 * @{
 */

/**
 * @def BT_BAUDRATE
 * @brief Defines the baud rate for HC-05 Bluetooth communication.
 * @details The baud rate determines the speed of UART communication between 
 *          the microcontroller and the HC-05 module.
 * @note    Default: 9600 bps — suitable for both AT command mode and normal mode.
 */
#define BT_BAUDRATE   9600UL

/** @} */ /* End of HC05_Config group */

#endif /* HC05_CONFIG_H_ */
