/**
 * @file       HC05_Config.h
 * @brief      Configuration file for the HC-05 Bluetooth driver.
 * @details    This file contains user-defined configuration macros for the HC-05 Bluetooth driver,
 *             including UART settings and parameters.
 * @version    1.0
 * @date       2025-10-28
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer: Ahmed Ashraf
 * @copyright  Copyright (c) 2025, Gestell Company
 */


#ifndef HC05_CONFIG_H_
#define HC05_CONFIG_H_

#include <stdint.h>
#include "HC05_Interface.h"

/**
 * @def BT_BAUDRATE
 * @brief Defines the baud rate for HC-05 Bluetooth communication.
 * @note Default = 9600 bps for AT command and normal mode.
 */
#define BT_BAUDRATE   9600UL



#endif /* HC05_CONFIG_H_ */
