/**
 * @file       DIO_Interface.h
 * @brief      This file contains The Interface header file for DIO (Digital Input/output) Module.    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer  : Eng/Hesham Ahmed 
 * @author     Reviewer  : Eng/Alaa Emad
 * @date       2025-10-14
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#include <stdint.h>
#include "..\Atmega32RegistersAddress.h"
#include "..\..\common\Macros.h"
#include "DIO_Private.h"
#include "DIO_Config.h"

#ifndef   _DIO_INTERFACE_H_
#define   _DIO_INTERFACE_H_

/**
 * @fn          mDIO_SetDirectionForPin(uint8_t GroupName, uint8_t PinNumber, uint8_t DirectionState)
 * @brief       Sets the direction of a specific DIO pin as input or output.
 * @details     
 *              This function configures the direction of a single pin within a specific DIO port.  
 *              It allows you to select the desired port (GroupName), the target pin number within that port,  
 *              and the direction state (input or output).  
 *              
 *
 * @param       GroupName       The name of the Port group (PORTA, PORTB, PORTC,PORTD).
 * @param       PinNumber       The pin number within the specified port (PIN0--->PIN7).
 * @param       DirectionState  Desired direction for the pin:
 *                              == 0: Input  
 *                              == 1: Output
 *
 * @return      void
 */

void mDIO_SetDirectionForPin(uint8_t GroupName,uint8_t PinNumber,uint8_t DirectionState);

/**
 * @fn          mDIO_WritePin(uint8_t GroupName,uint8_t PinNumber,uint8_t OutputValue)
 * @brief       Writes the Value of a specific Output DIO pin as High or Low.
 * @details     
 *              This function configures the Value of a single Output pin within a specific DIO port.  
 *              It allows you to select the desired port (GroupName), the target pin number within that port,  
 *              and Output Value (High or Low).  
 *              
 *
 * @param       GroupName       The name of the Port group (PORTA, PORTB, PORTC,PORTD).
 * @param       PinNumber       The pin number within the specified port (PIN0--->PIN7).
 * @param       DirectionState  Desired OutputValue for the pin:
 *                              == 0: Low 
 *                              == 1: High
 *
 * @return      void
 */

void mDIO_WritePin(uint8_t GroupName,uint8_t PinNumber,uint8_t OutputValue);

/**
 * @fn          mDIO_TogglePin(uint8_t GroupName,uint8_t PinNumber)
 * @brief       Toggle the Value of a specific DIO pin .
 * @details     
 *              This function Toggles the Value of a single Output pin within a specific DIO port.  
 *              It allows you to select the desired port (GroupName), the target pin number within that port(PinNumber)
 *              It Toggles Specific Pin 
 *                     if it is  0 ---> 1
 *                     if it is  1 ---> 0  
 *               
 *              
 *
 * @param       GroupName       The name of the Port group (PORTA, PORTB, PORTC,PORTD).
 * @param       PinNumber       The pin number within the specified port (PIN0--->PIN7).
 *
 * @return      void
 */

void mDIO_TogglePin(uint8_t GroupName,uint8_t PinNumber);

/**
 * @fn          mDIO_ReadPin(uint8_t GroupName, uint8_t PinNumber, uint8_t *InputValue)
 * @brief       Reads the state of a specific DIO pin.
 * @details     
 *              This function retrieves the digital value (HIGH or LOW) of a given pin 
 *              within a specified DIO port. 
 *              The read value is stored in the variable pointed to by the InputValue pointer.  
 *              It accesses the corresponding PIN register to get the actual logic level
 *              present at the pin (useful for input pins).
 *
 * @param       GroupName     The name of the Port group (PORTA, PORTB, PORTC,PORTD).
 * @param       PinNumber     The pin number within the specified port (PIN0--->PIN7).
 * @param       InputValue    Pointer to a variable where the reading level will be stored:
 *                            == 0: LOW  
 *                            == 1: HIGH
 *
 * @return      void
 */

void mDIO_ReadPin(uint8_t GroupName,uint8_t PinNumber,uint8_t * InputValue);

/**
 * @fn          mDIO_SetDirectionForGroup(uint8_t GroupName,uint8_t DirectionState)
 * @brief       Sets the direction of a specific Group Port as input or output. 
 * @details     
 *              This function configures the direction of Specific Group Port.  
 *              It allows you to select the desired port (GroupName), the direction state of the group (input or output). 
 *              All pins in the Specific group takes the dirction of the Port. 
 *              
 * @param       GroupName       The name of the Port group (PORTA, PORTB, PORTC,PORTD).
 * @param       DirectionState  Desired direction for all Pins of The Specific Port
 *                              == 0: Input  
 *                              == 1: Output
 *
 * @return      void
 */

void mDIO_SetDirectionForGroup(uint8_t GroupName,uint8_t DirectionState);

/**
 * @fn          mDIO_WriteGroup(uint8_t GroupName,uint8_t OutputValue)
 * @brief       Writes the Value of a specific Group (Port) as High or Low.
 * @details        
 * 
 *              This function configures the Output Value of Specific Group Port.  
 *              It allows you to select the desired port (GroupName), the Output value of the Port as High or Low (OutputValue). 
 *              All pins in the Specific group takes the Value of the Port. 
 *              
 * @param       GroupName       The name of the Port group (PORTA, PORTB, PORTC,PORTD).
 * @param       OutputValue     Desired OutputValue for all Pins of The Specific Port
 *                              == 0: Low  
 *                              == 1: High
 *
 * @return      void
 */

void mDIO_WriteGroup(uint8_t GroupName,uint8_t OutputValue);

/**
 * @fn          mDIO_ReadGroup(uint8_t GroupName,uint8_t *InputValue)
 * @brief       Read the Value of a specific Group (Port) as High or Low.
 * @details        
 *              This function retrieves the digital value (HIGH or LOW) of a given Port
 *              The read value is stored in the variable pointed to by the InputValue pointer.
 *              It accesses the corresponding Port register to get the actual logic level
 *              
 * @param       GroupName       The name of the Port group (PORTA, PORTB, PORTC,PORTD).
 * @param       InputValue      Pointer to a variable where the reading level will be stored:
 *                              == 0: LOW  
 *                              == 1: HIGH
 *
 * @return      void
 */
void mDIO_ReadGroup(uint8_t GroupName,uint8_t *InputValue);


#endif/*_DIO_INTERFACE_H_*/

