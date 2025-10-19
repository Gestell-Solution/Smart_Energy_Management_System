/**
 * @file       RGB_interface.h
 * @brief      RGB LED driver interface.
 * @details    This header file provides the function prototypes and type definitions 
 *             for controlling an RGB LED module using digital ON/OFF signals 
 *             through the DIO driver on the ATmega32 microcontroller.
 * @version    1.0
 * @date       2025-10-19
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer:bassma khaled
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#ifndef _RGB_INTERFACE_H_
#define _RGB_INTERFACE_H_

/*================================= Includes =================================*/
#include <stdint.h>
#include "../../Mcal/Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "RGB_Private.h"
#include "RGB_Config.h"

/*============================ Group: RGB Driver =============================*/
/**
 * @defgroup RGB_Driver RGB Driver APIs
 * @brief     Functions and definitions for controlling RGB LEDs.
 * @{
 */

/*=============================== Enumerations ===============================*/

/**
 * @enum RGB_State_t
 * @brief Represents the predefined operating states of the RGB LED.
 */
typedef enum
{
    /** @var RGB_OFF
     *  @brief The RGB LED is turned off.
     */
    RGB_OFF,

    /** @var RGB_RED
     *  @brief The RGB LED displays red color.
     */
    RGB_RED,

    /** @var RGB_GREEN
     *  @brief The RGB LED displays green color.
     */
    RGB_GREEN,

    /** @var RGB_ERROR
     *  @brief The RGB LED indicates an error state (custom defined by the application).
     */
    RGB_ERROR,

    /** @var RGB_NORMAL
     *  @brief The RGB LED indicates a normal operating state.
     */
    RGB_NORMAL

} RGB_State_t;

/*=============================== API Functions ==============================*/

/**
 * @fn         void RGB_Init(void)
 * @brief      Initializes the RGB LED module.
 * @details    Configures the RGB LED control pins as output and sets the default state to OFF.
 * @note       This function must be called before using any other RGB functions.
 */
void RGB_Init(void);

/**
 * @fn         void RGB_SetColor(uint8_t red, uint8_t green)
 * @brief      Sets the RGB LED color using digital control.
 * @details    Turns each LED channel (Red, Green) ON or OFF based on the input parameters.
 *             Each parameter represents the digital state of its respective color component.
 * @param[in]  red    Red LED state (0: OFF, 1: ON).
 * @param[in]  green  Green LED state (0: OFF, 1: ON).
 * @warning    RGB_Init() must be called before using this function.
 */
void RGB_SetColor(uint8_t red, uint8_t green);

/**
 * @fn         void RGB_SetState(RGB_State_t state)
 * @brief      Sets the RGB LED to a predefined logical state.
 * @details    Updates the RGB LED to represent system states 
 *             such as NORMAL, ERROR, or OFF.
 * @param[in]  state  The desired LED state (see @ref RGB_State_t).
 */
void RGB_SetState(RGB_State_t state);

/** @} */ /* End of RGB_Driver group */

#endif /* _RGB_INTERFACE_H_ */
