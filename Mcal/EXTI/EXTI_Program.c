/**
 * @file       EXTI_Progranm.c
 * @brief      This file contains External interrupt Interface .
 * @details    File Contains Functions Definitions-->
 *                      -(EXTI_Init , CallBack ,Enable and Disable of EXTI) 
 *                      - ISR Handling Code  
 * @version    0.1
 * @author     Moahmed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @date       2025-10-09
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#include <stdint.h>
#include "D:\Gestell Projects\Smart_Energy_Management_System\Mcal\Atmega32RegistersAddress.h"
#include "D:\Gestell Projects\Smart_Energy_Management_System\Common\Macros.h"
#include "EXTI_Interface.h"
#include "EXTI_Private.h"
#include "EXTI_Config.h"

#ifndef _EXTI_PROGRAM_C
#define _EXTI_PROGRAM_C

static void(*EXTI0_CallbackFunction_and_ISR_Handler)(void)=Null;
static void(*EXTI1_CallbackFunction_and_ISR_Handler)(void)=Null;
static void(*EXTI2_CallbackFunction_and_ISR_Handler)(void)=Null;

void mEXTI_Init(uint8_t source, uint8_t senseControl)
{
    // waits Source --> if it EXTI0 or EXTI1 or EXTI2
    // Check on SenseControl if it is ---> Low Level , Rising edge , Falling Edge , Any Logical Change
    /*----------------------------------The Code Flow--------------------------------*/
    // if its EXT0 --> (MCUCR REG)
    // -- SenseControl is Low Level ---------->(ISC00 AND ISC01) bits Are Zeros (ClearBit)
    // -- SenseControl is AnyLogical Level --->(ISC00 is one (setBit) and ISC01 is zero (ClearBit)
    // -- SenseControl is Falling Edge ------->(ISC00 is zero (ClearBit) and ISC01 is one (SetBit)
    // -- SenseControl is Rising Edge -------->(ISC00 AND ISC01) bits Are ones (SetBit)
    // if its EXT1 --> (MCUCR REG)
    // -- SenseControl is Low Level ---------->(ISC10 AND ISC11) bits Are Zeros (ClearBit)
    // -- SenseControl is AnyLogical Level --->(ISC10 is one (setBit) and ISC11 is zero (ClearBit)
    // -- SenseControl is Falling Edge ------->(ISC10 is zero (ClearBit) and ISC11 is one (SetBit)
    // -- SenseControl is Rising Edge -------->(ISC10 AND ISC11) bits Are ones (SetBit)
    // if its EXT2 --> (MCUCSR REG)
    // -- SenseControl is Falling Edge ------->ISC2 is zero (ClearBit) 
    // -- SenseControl is Rising Edge -------->ISC2 is one (SetBit)



    if(source==EXTI0){
        if(senseControl==EXT_LOW_LEVEL){
            ClearBit(MCUCR_Reg,ISC00_Bit);
            ClearBit(MCUCR_Reg,ISC01_Bit);
            
        }
        else if(senseControl==EXT_AnyCHANGE){
            SetBit(MCUCR_Reg,ISC00_Bit);
            ClearBit(MCUCR_Reg,ISC01_Bit);
            
        } 
        else if(senseControl==EXT_FALLING_EDGE){
            ClearBit(MCUCR_Reg,ISC00_Bit);
            SetBit(MCUCR_Reg,ISC01_Bit);
          
        }
        else if(senseControl==EXT_RISING_EDGE){
            SetBit(MCUCR_Reg,ISC00_Bit);
            SetBit(MCUCR_Reg,ISC01_Bit);
            
        }
        else{
            /*
            *
            *@todo SenseControl is OutofRange
            * 
            */
        
        }
    }
    else if(source==EXTI1){
        if(senseControl==EXT_LOW_LEVEL){
            ClearBit(MCUCR_Reg,ISC10_Bit);
            ClearBit(MCUCR_Reg,ISC11_Bit);
            
        }
        else if(senseControl==EXT_AnyCHANGE){
            SetBit(MCUCR_Reg,ISC10_Bit);
            ClearBit(MCUCR_Reg,ISC11_Bit);
            
        } 
        else if(senseControl==EXT_FALLING_EDGE){
            ClearBit(MCUCR_Reg,ISC10_Bit);
            SetBit(MCUCR_Reg,ISC11_Bit);
          
        }
        else if(senseControl==EXT_RISING_EDGE){
            SetBit(MCUCR_Reg,ISC10_Bit);
            SetBit(MCUCR_Reg,ISC11_Bit);
            
        }
        else{
            /*
            *
            *@todo SenseControl is OutofRange
            * 
            */
        }
    }
    else if(source==EXTI2){
        if(senseControl==EXT_FALLING_EDGE){
            ClearBit(MCUCSR_Reg,ISC2_Bit);
        }
        else if(senseControl==EXT_RISING_EDGE){
            SetBit(MCUCSR_Reg,ISC2_Bit); 
        }
        else{
            /*
            *
            *@todo SenseControl is OutofRange
            * 
            */
        }
    }
}
void mEXTI_setCallback(uint8_t source, void (*PF)(void))
{
if(PF==Null)
{
    // The user inserts a wrong input (Not a function)
    return; // Leave The Function Without Doing AnyThing
}
switch (source)
{
case EXTI0:
    EXTI0_CallbackFunction_and_ISR_Handler=PF;
    break;
case EXTI1:
    EXTI1_CallbackFunction_and_ISR_Handler=PF;
    break;
case EXTI2:    
    EXTI2_CallbackFunction_and_ISR_Handler=PF;
    break; 

default:
/*
*@todo Source is Out of Range
*
*/
    break;
}
}
void mEXTI_Enable(uint8_t source)
{
    if(source==EXTI0){
        SetBit(GICR_Reg,INT0_Bit);
    }
    else if(source==EXTI1){
        SetBit(GICR_Reg,INT1_Bit);
    }
    else if(source==EXTI2){
        SetBit(GICR_Reg,INT2_Bit);
    }
    else{
        /*
        *@to do Out of Range
        *
        */
    }
}
void mEXTI_Disable(uint8_t source)
{
    if(source==EXTI0){
        ClearBit(GICR_Reg,INT0_Bit);
    }
    else if(source==EXTI1){
        ClearBit(GICR_Reg,INT1_Bit);
    }
    else if(source==EXTI2){
        ClearBit(GICR_Reg,INT2_Bit);
    }
    else{
        /*
        *@to do Out of Range
        *
        */
    }
}

void __vector_1(void)
{  // Check for Global Pointer is not qual to Null
    if(EXTI0_CallbackFunction_and_ISR_Handler !=Null){
    // Call for Global Pointer to Function
    EXTI0_CallbackFunction_and_ISR_Handler();
    }    
    else{
    /*
    *@toddo Handle EXTI0_CallbackFunction_and_ISR_Handler =Null
    *
    */
    }
}
void __vector_2(void)
{   // Check for Global Pointer is not qual to Null
    if(EXTI1_CallbackFunction_and_ISR_Handler !=Null){
    // Call for Global Pointer to Function
    EXTI1_CallbackFunction_and_ISR_Handler();
    }  
    else {
    /*
    *@toddo Handle EXTI1_CallbackFunction_and_ISR_Handler =Null
    *
    */
    }
}
void __vector_3(void)
{   // Check for Global Pointer is not qual to Null
    if(EXTI2_CallbackFunction_and_ISR_Handler !=Null){
    // Call for Global Pointer to Function
    EXTI2_CallbackFunction_and_ISR_Handler();
    }
    else {
    /*
    *@toddo Handle EXTI2_CallbackFunction_and_ISR_Handler =Null
    *
    */
    }
}








#endif/*_EXTI_PROGRAM_C*/
