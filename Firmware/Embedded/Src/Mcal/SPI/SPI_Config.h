/**
 * @file       SPI_Config.h
 * @brief      Configuration file for the SPI driver.
 * @details    This file contains all user-defined configuration parameters for the SPI module,
 *             including mode selection, data order, clock mode, prescaler, and interrupt settings.
 *             These configuration macros determine how the SPI peripheral will operate in 
 *             the application. Make sure to adjust them carefully according to your system requirements.
 * @version    1.0
 * @date       2025-12-05
 * @author     Developer: Ahmed Twap (ahmedtwap2@gmail.com)
 * @author     Reviewer: ENG.Bassma Khaled 
 * @copyright  Copyright (c) 2025, Gestell
 */

#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H

/*================================= Includes =================================*/
#include <stdint.h>
#include "SPI_Private.h"
#include "../Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"
/*=============================== Configuration ===============================*/

/*=========================== Group: SPI Configuration ========================*/
/**
 * @defgroup SPI_Config SPI Configuration
 * @brief     SPI configuration parameters.
 * @details   Use these macros to configure the SPI peripheral behavior. 
 *            All available options for each parameter can be found in @ref SPI_Private.h.
 * @{
 */

/*============================================================================*/
/**
 * @def SPI_MODE
 * @brief Selects the SPI operating mode (Master or Slave).
 * @note  Options: Master_Mode, Slave_Mode.
 */
#define SPI_MODE           Master_Mode

/*============================================================================*/
/**
 * @def SPI_DATA_ORDER
 * @brief Selects the bit order of transmitted data.
 * @note  Options: MSB_First, LSB_First.
 */
#define SPI_DATA_ORDER     MSB_First

/*============================================================================*/
/**
 * @def SPI_CLOCK_MODE
 * @brief Selects the SPI clock polarity and phase configuration.
 * @note  Options: SPI_Mode0, SPI_Mode1, SPI_Mode2, SPI_Mode3.
 */
#define SPI_CLOCK_MODE     SPI_Mode0

/*============================================================================*/
/**
 * @def SPI_PRESCALER
 * @brief Defines the SPI clock prescaler value.
 * @note  Options: F_CPU_4, F_CPU_16, F_CPU_64, F_CPU_128, F_CPU_2, F_CPU_8, F_CPU_32.
 */
#define SPI_PRESCALER      F_CPU_8

/*============================================================================*/
/**
 * @def SPI_INTERRUPT
 * @brief Enables or disables SPI interrupt mode.
 * @note  Options: Enable, Disable.
 * @warning If interrupt is enabled, ensure an ISR(SPI_STC_vect) is implemented.
 */
#define SPI_INTERRUPT      Disable_SPI_Interrupt

/** @} */   /* End of SPI_Config group */

#endif /* SPI_CONFIG_H */
