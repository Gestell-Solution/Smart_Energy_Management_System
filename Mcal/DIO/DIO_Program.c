/**
 * @file       DIO_Interface.h
 * @brief      This file contains The Program file for DIO (Digital Input/output) Module.    
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
#include "../../Common/Config.h"

#if DIO_Module == Enable

#include "DIO_Private.h"
#include "DIO_Config.h"
#include "DIO_Interface.h"



void mDIO_SetDirectionForPin(uint8_t GroupName,uint8_t PinNumber,uint8_t DirectionState)
{
    if(PinNumber>=0 && PinNumber<=7)
    {   
        if(DirectionState==Input)
        {
            switch (GroupName)
            {
            case GroupA:
                ClearBit(DDRA_Reg,PinNumber);
                break;
            case GroupB:
                ClearBit(DDRB_Reg,PinNumber);
                break;
            case GroupC:
                ClearBit(DDRC_Reg,PinNumber);
                break;
            case GroupD:
                ClearBit(DDRD_Reg,PinNumber);
                break;
            
            default:
                /**
                 *@todo DirectionSate Out of Range
                 *
                 */  
                break;
            }
            
        }
        else if(DirectionState==Output)
        {
            switch (GroupName)
            {
            case GroupA:
                SetBit(DDRA_Reg,PinNumber);
                break;
            case GroupB:
                SetBit(DDRB_Reg,PinNumber);
                break;
            case GroupC:
                SetBit(DDRC_Reg,PinNumber);
                break;
            case GroupD:
                SetBit(DDRD_Reg,PinNumber);
                break;
            
            default:
                /**
                 *@todo DirectionSate Out of Range
                 *
                 */  
                break;
            }        
        }
        else
        {/**
         *@todo DirectionSate Out of Range
         *
         */

        }

    }
    else 
    {/**
     *@todo Invalid PinNumber Range is From (PIN0--PIN7)
     *
     */
    }

}

void mDIO_SetDirectionForGroup(uint8_t GroupName,uint8_t DirectionState)
{
    if(DirectionState>=0X00 && DirectionState<=0XFF)
    {
        switch (GroupName)
        {
        case GroupA:
            DDRA_Reg=DirectionState;
            break;
        case GroupB:
            DDRB_Reg=DirectionState;
            break;
        case GroupC:
            DDRC_Reg=DirectionState;
            break;
        case GroupD:
            DDRD_Reg=DirectionState;
            break;
        
        default:
        /**
        *@todo DirectionSate Out of Range
        *
        */
            break;
        }
    }    
}

void mDIO_WritePin(uint8_t GroupName,uint8_t PinNumber,uint8_t OutputValue)
{
    if (PinNumber>=0 && PinNumber<=7)
    {
        if (OutputValue==Low)
        {
            switch (GroupName)
            {
            case GroupA:
                ClearBit(PORTA_Reg,PinNumber);
                break;
            case GroupB:
                ClearBit(PORTB_Reg,PinNumber);
                break;
            case GroupC:
                ClearBit(PORTC_Reg,PinNumber);
                break;
            case GroupD:
                ClearBit(PORTD_Reg,PinNumber);
                break;
            default:
            /**
             * @todo GroupName Not Valid
             */
                break;
            }
        }
        else if (OutputValue==High)
        {
            switch (GroupName)
            {
            case GroupA:
                SetBit(PORTA_Reg,PinNumber);
                break;
            case GroupB:
                SetBit(PORTB_Reg,PinNumber);
                break;
            case GroupC:
                SetBit(PORTC_Reg,PinNumber);
                break;
            case GroupD:
                SetBit(PORTD_Reg,PinNumber);
                break;
            default:
            /**
             * @todo GroupName Not Valid
             */
                break;
            }        
        }
        else
        {/**
         * @todo Output value Out of Range
         */
        }
        
    }
    else
    {/**
     *@todo PinNumber Out of Range
     */

    }
}

void mDIO_WriteGroup(uint8_t GroupName,uint8_t OutputValue)
{
    if(OutputValue>=0x00 && OutputValue<=0XFF)
    {
        switch (GroupName)
        {
        case GroupA:
            PORTA_Reg=OutputValue;
            break;
        case GroupB:
            PORTB_Reg=OutputValue;
            break;
        case GroupC:
            PORTC_Reg=OutputValue;
            break;
        case GroupD:
            PORTD_Reg=OutputValue;
            break;

        
        default:
        /**
         * @todo Group Name Not Valid
         */
            break;
        }
    }
    else
    {
    /**
     * @todo Output Value Out of Range
     */

    }
}

void mDIO_TogglePin(uint8_t GroupName,uint8_t PinNumber)
{
    if(PinNumber>=0 && PinNumber<=7)
    {
        switch (GroupName)
        {
        case GroupA:
            ToggleBit(PORTA_Reg,PinNumber);
            break;
        case GroupB:
            ToggleBit(PORTB_Reg,PinNumber);
            break;
        case GroupC:
            ToggleBit(PORTC_Reg,PinNumber);
            break;
        case GroupD:
            ToggleBit(PORTD_Reg,PinNumber);
            break;
        
        default:
        /**
         * @todo Group Name Invalid 
         */
            break;
        }
    }
    else
    {
    /**
     * @todo Pin Number Out of Range
     */
    }
    
}

void mDIO_ReadPin(uint8_t GroupName,uint8_t PinNumber,uint8_t * InputValue)
{
    if(InputValue!=Null)
    {
        if(PinNumber>=0 && PinNumber<=7)
        {
            switch (GroupName)
            {
            case GroupA:
                *InputValue=GetBit(PINA_Reg,PinNumber);
                break;
            case GroupB:
                *InputValue=GetBit(PINB_Reg,PinNumber);
                break;
            case GroupC:
                *InputValue=GetBit(PINC_Reg,PinNumber);
                break;
            case GroupD:
                *InputValue=GetBit(PIND_Reg,PinNumber);
                break;
            
            default:
            /**
             * @todo Group Name Out of Range
             */
                break;
            }
        }
    }
    else
    {
    /**
     * @todo Handle Pointer InputValue=Null
     *  
     * */    
    }
}

void mDIO_ReadGroup(uint8_t GroupName,uint8_t *InputValue)
{
    if(InputValue!=Null)
    {
        switch (GroupName)
        {
        case GroupA:
            *InputValue=PINA_Reg;
            break;
        case GroupB:
            *InputValue=PINB_Reg;
            break;
        case GroupC:
            *InputValue=PINC_Reg;
            break;
        case GroupD:
            *InputValue=PIND_Reg;
            break;
        
        default:
        /**
         * @todo  GroupName Invalid
         */
            break;
        }
    }
    else
    {
    /**
     * @todo  Handle Pointer Inputvalue = Null
     */
    }
}


#endif /*DIO Module Enable*/