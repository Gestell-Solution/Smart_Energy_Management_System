/**
 * @file       DIO_Private.h
 * @brief      This file contains The Private header file for DIO (Digital Input/output) Module.    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer  : Eng/Hesham Ahmed 
 * @date       2025-10-16
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#include <stdint.h>
#include "../Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"



#ifndef   _DIO_PRIVATE_H_
#define   _DIO_PRIVATE_H_

/**
*@defgroup      DIO_Port_Groups DIO Port Group Identifiers  
*         
*/

#define  GroupA    1

#define  GroupB    2

#define  GroupC    3

#define  GroupD    4

/**
*@defgroup          DIO_Pin_Numbers DIO Pin Number Definitions.
*
*/

#define  PIN0     0

#define  PIN1     1

#define  PIN2     2

#define  PIN3     3

#define  PIN4     4

#define  PIN5     5

#define  PIN6     6

#define  PIN7     7



#endif/*_DIO_PRIVATE_H_*/