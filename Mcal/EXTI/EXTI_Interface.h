/**
 * @file       EXTI_Interface.h
 * @brief      This file contains The Interface for External Interrupt Module.    
 * @version    3.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng - Ahmed Ashraf   (ahmedashraf2022222@gmail.com  )
 * @date       2025-10-16
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#include <stdint.h>
#include "../Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"
#include "EXTI_Private.h"
#include "EXTI_Config.h"


 #ifndef   _EXTI_INTERFACE_H_
 #define   _EXTI_INTERFACE_H_

/**
 * @addtogroup Mcal
 * @{
 * @addtogroup EXTI
 * @{
 */




/**
 * @fn          void mEXTI_Init(uint8_t source, uint8_t senseControl)
 * @brief       Initialize the external interrupt configuration.
 * @details     This function configures the selected external interrupt (EXTI0, EXTI1, or EXTI2)
 *              with the desired sense control (LOW LEVEL, ANY CHANGE, FALLING EDGE, RISING EDGE).
 *              It sets the corresponding bits in MCUCR or MCUCSR registers.
 *
 * @param       source        Specifies which EXTI line to configure (EXTI0, EXTI1, EXTI2).
 * @param       senseControl  Specifies the interrupt triggering condition.
 *                            Possible values: EXT_LOW_LEVEL, EXT_AnyCHANGE, EXT_FALLING_EDGE, EXT_RISING_EDGE.
 */

void mEXTI_Init(uint8_t source, uint8_t senseControl);

/**
 * @fn          void mEXTI_setCallback(uint8_t source, void (*PF)(void))
 * @brief       Set the callback function for a specific external interrupt source.
 * @details     This function links a user-defined function (callback) to a specific EXTI line.
 *              When the interrupt occurs, the linked callback function will be executed automatically.
 *
 * @param       source  Specifies which EXTI line to attach the callback (EXTI0, EXTI1, EXTI2).
 * @param       PF  Pointer to the function to be executed when the interrupt occurs.
 *
 * @warning     The callback pointer must not be NULL.
 */

void mEXTI_setCallback(uint8_t source, void (*PF)(void));

/**
 * @fn          void mEXTI_Enable(uint8_t source)
 * @brief       Enable the selected external interrupt.
 * @details     This function sets the corresponding enable bit (INT0, INT1, or INT2) 
 *              in the GICR register to allow the interrupt request.
 *
 * @param   source  Specifies which EXTI line to enable (EXTI0, EXTI1, EXTI2).
 *
 * @note        The interrupt must be initialized first using mEXTI_Init().
 */

void mEXTI_Enable(uint8_t source);

/**
 * @fn          void mEXTI_Disable(uint8_t source)
 * @brief       Disable the selected external interrupt.
 * @details     This function clears the corresponding enable bit (INT0, INT1, or INT2)
 *              in the GICR register to prevent the interrupt request from occurring.
 *
 * @param       source  Specifies which EXTI line to disable (EXTI0, EXTI1, EXTI2).
 *
 * @note        Use this function when you need to temporarily stop interrupt handling.
 */

void mEXTI_Disable(uint8_t source);






/**
 * @}
 * @}
 */

 #endif /* _EXTI_INTERFACE_H_ */

