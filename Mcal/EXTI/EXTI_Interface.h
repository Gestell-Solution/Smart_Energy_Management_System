/**
 * @file       EXTI_Interface.h
 * @brief      This file contains External interrupt Interface .
 * @details    File Contains Implementations of APIs:
 *                 - mEXTI_Init(uint8_t source, uint8_t senseControl);
 *                 - mEXTI_Enable(uint8_t source);
 *                 - mEXTI_Disable(uint8_t source);
 *                 - mEXTI_setCallback(uint8_t source, void (*PF)(void));
 *                  
 * @version    2.0
 * @author     Moahmed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @date       2025-10-09
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#include <stdint.h>
#include "..\Atmega32RegistersAddress.h"
#include "..\..\common\Macros.h"
#include "EXTI_Private.h"
#include "EXTI_Config.h"


 #ifndef   _EXTI_INTERFACE_H_
 #define   _EXTI_INTERFACE_H_

/**
*@defgroup          Implement APIs
*@brief             Function -->void mEXTI_Init(uint8_t source, uint8_t senseControl)
*                   ---needs two parameters-->(Source if it EXT0,EXT1,EXT2 , Sense Control if it LOW,Falling,Rising,AnyCHANGE)
*                   --- it determines The region will interrupt Works on
*                   Function -->void mEXTI_setCallback(uint8_t source, void (*PF)(void))
*                   ---needs two parameters-->(Source if it EXT0,EXT1,EXT2 , Function That will be Done in ISR)
*                   --- Determine The ISR (Interrupt Action)
*                   Function -->void mEXTI_Enable(uint8_t source)
*                    ---needs One parameters-->(Source if it EXT0,EXT1,EXT2)
*                    --- Enable The GICR -->Prephiral(EXT) Interrupt Enable 
*                   Function -->void mEXTI_Disable(uint8_t source)
*                    ---needs One parameters-->(Source if it EXT0,EXT1,EXT2)
*                    --- Disable The GICR -->Prephiral(EXT) Interrupt Disable
*/

void mEXTI_Init(uint8_t source, uint8_t senseControl);
void mEXTI_setCallback(uint8_t source, void (*PF)(void));
void mEXTI_Enable(uint8_t source);
void mEXTI_Disable(uint8_t source);




 #endif /* _EXTI_INTERFACE_H_ */

