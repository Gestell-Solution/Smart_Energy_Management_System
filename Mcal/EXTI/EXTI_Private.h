/**
 * @file       EXTI_Private.h
 * @brief      This file contains External interrupt Interface .
 * @details    File Contains 
 * 
 *                 - Each MCUCR , MCUCSR , GICR , GIFR Registers and Each Bitname of Each Register
 *                 - Implementations of ISR Prototyping
 *                 - SenseControl Definition       
 * @version    0.1
 * @author     Moahmed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @date       2025-10-09
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#include <stdint.h>
#include "D:\Gestell Projects\Smart_Energy_Management_System\Mcal\Atmega32RegistersAddress.h"
#include "D:\Gestell Projects\Smart_Energy_Management_System\Common\Macros.h"

#ifndef _EXTI_PRIVATE_H
#define _EXTI_PRIVATE_H

/*
*@defgroup          Name of Each Bit inside MCUCR Register
*@brief             MCUCR Reg Has 4 bits related With External Interrupt (Bit No (0-->3)) 
*                      - Bits Name ISC00 ,ISC01 , ISC10 ,ISC11 
*                      - INT0 bits --> ISC00 , ISC01
*                      - INT1 bits --> ISC10 , ISC11 
*/


#define      ISC00_Bit    0
#define      ISC01_Bit    1
#define      ISC10_Bit    2
#define      ISC11_Bit    3

/*
*@defgroup          Name of Each Bit inside MCUCSR Register
*@brief             MCUSCR Reg Has 1 bit related With External Interrupt (BIT NO 6)
*                      - Bits Name ISC2 
*                      - INT2 bit --> ISC2                
*/

#define      ISC2_Bit    6

/*
*@defgroup          Name of Each Bit inside GICR Register -- (External interrupt Enable and disable Bit )
*@brief             GICR Reg Has 3 bits related With External Interrupt (BIT NO (5-->7)
*                      - Bits Name INT0 ,INT1,INT2                                         
*/

#define      INT0_Bit        6
#define      INT1_Bit        7
#define      INT2_Bit        5

/*
*@defgroup          Name of Each Bit inside GIFR Register -- (External interrupt FLAG Enable and disable Bit )
*@brief             GICR Reg Has 3 bits related With External Interrupt (BIT NO (5-->7)
*                      - Bits Name INTF0 ,INTF1,INTF2                                         
*/

#define      INTF0_Bit        6
#define      INTF1_Bit        7
#define      INTF2_Bit        5         

/*Interrupt surface routine (ISR) Declaration\ prototype */

void __vector_1__(void) __attribute__((signal)) ; // Related with INT0 
void __vector_2__(void) __attribute__((signal)) ; // Related With INT1
void __vector_3__(void) __attribute__((signal)) ; // Related with INT2

/*SenseControl*/
/*sense mode (Low / Any Change / Falling / Rising)*/

#define     EXT_LOW_LEVEL       0 
#define     EXT_RISING_EDGE     1 
#define     EXT_FALLING_EDGE   2
#define     EXT_AnyCHANGE       3 

/*External Interrupt Source Selection*/

#define     EXTI0               0
#define     EXTI1               1
#define     EXTI2               2



#endif /*_EXTI_PRIVATE_H*/