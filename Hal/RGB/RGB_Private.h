/**
 * @file       RGB_Private.h
 * @brief      This file contains RGB LED private functions and macros.
 * @version    1.0
 * @author     Developer : Ahmed twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer : bassma khaled
 * @date       2025-10-19
 * @copyright  Copyright (c) 2025 , Gestell Company
 *
 */
#ifndef _RGB_PRIVATE_H_
#define _RGB_PRIVATE_H_
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


#endif /* _RGB_PRIVATE_H_ */
