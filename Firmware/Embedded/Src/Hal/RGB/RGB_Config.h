/**
 * @file       RGB_Config.h
 * @brief      Configuration file for the RGB LED driver.
 * @details    This file contains user-defined configuration macros for the RGB LED driver,
 *             including pin assignments, port groups, and driver enable/disable options.
 *             Modify these values according to your hardware connections.
 * @version    1.0
 * @date       2025-10-19
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer: bassma khaled
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#ifndef _RGB_CONFIG_H_
#define _RGB_CONFIG_H_

/*================================= Includes =================================*/
#include <stdint.h>
#include "../../Mcal/DIO/DIO_Interface.h"
#include "RGB_Interface.h"
#include "RGB_Private.h"

/*=========================== Group: RGB Configuration =======================*/
/**
 * @defgroup RGB_Config RGB Configuration
 * @brief     Configuration macros for RGB LED driver.
 * @{
 */

/*============================================================================*/
/**
 * @def RGB_Driver
 * @brief Enable or disable the RGB driver module.
 * @note  Set to `Enable` to include the RGB driver code in the build, or `Disable` to exclude it.
 */
#define RGB_Driver             Enable

/*============================================================================*/
/**
 * @def RGB_RED_GROUP
 * @brief Defines the DIO group (port) used for the red LED.
 */
#define RGB_RED_GROUP          GroupD

/*============================================================================*/
/**
 * @def RGB_GREEN_GROUP
 * @brief Defines the DIO group (port) used for the green LED.
 */
#define RGB_GREEN_GROUP        GroupC

/*============================================================================*/
/**
 * @def RGB_RED_PIN
 * @brief Defines the pin number for the red LED connection.
 */
#define RGB_RED_PIN            PIN7

/*============================================================================*/
/**
 * @def RGB_GREEN_PIN
 * @brief Defines the pin number for the green LED connection.
 */
#define RGB_GREEN_PIN          PIN7

/** @} */ /* End of RGB_Config group */

#endif /* _RGB_CONFIG_H_ */
