/**
 * @file       TIMER1_Program.c
 * @brief      Implementation of Timer1 driver functions for ATmega32.
 * @details    This source file contains the function definitions for initializing,
 *             starting, stopping, and handling Timer1 in CTC mode. It supports
 *             interrupt-driven operation via callback registration and uses configuration
 *             macros defined in TIMER1_Config.h.
 *
 *             The driver is designed to be modular and reusable across different
 *             applications requiring precise timing, task scheduling, or delay generation.
 *
 * @author     Developer: Mohamed Diaa (mohammeddiaato@gmail.com)
 * @author     Reviewer:  Basma
 * @version    0.3
 * @date       2025-10-09
 * @copyright  Copyright (c) 2025, Gestell Company
 *
 * @note       Ensure TIMER1_Config.h is properly configured before using this driver.
 * @warning    Global interrupts must be enabled for callback functionality to work.
 */
#include "TIMER1_Config.h"
#include "TIMER1_Interface.h"
#include "TIMER1_Private.h"

static void (*Timer1_Global_Callback)(void)=Null;
void mTIMER1_Init(void)
{
        uint8_t TCCR1A_Temp=0;
        uint8_t TCCR1B_Temp=0;
        /**TCCR1A Reg setting the bits */
        //clear the Compare output mode to be zero so the pin to be on normal operation 
        ClearBit(TCCR1A_Temp,T1_COM1A0_Bit);
        ClearBit(TCCR1A_Temp,T1_COM1A1_Bit);
        
        //clear the Compare output mode to be zero so the pin to be on normal operation 
        ClearBit(TCCR1A_Temp,T1_COM1B0_Bit);
        ClearBit(TCCR1A_Temp,T1_COM1B1_Bit);
        
        //clear the wave gen mode to be zero so the timer will be on CTC having the top on ICR1 
        ClearBit(TCCR1A_Temp,T1_WGM10_Bit);
        ClearBit(TCCR1A_Temp,T1_WGM11_Bit);
        
        /**TCCR1B Register */
        ClearBit(TCCR1B_Temp,T1_ICNC1_Bit);
        ClearBit(TCCR1B_Temp,T1_ICES1_Bit);
        //wave generation mode 
        SetBit(TCCR1B_Temp,T1_WGM12_Bit);
        SetBit(TCCR1B_Temp,T1_WGM13_Bit);
        //
        TCCR1B_Temp|=Timer1_Prescaler;
        
        // enabling the interrupt
        CompareMatch1B_InterruptEnable;
        CompareMatch1A_InterruptEnable;
        //setting the registers
        OCR1A_Reg=Timer1_Sampling_Clks;
        OCR1B_Reg=Timer1_Sampling_Clks;
        ICR1_Reg=Timer1_TOPvalue;
        TCCR1A_Reg=TCCR1A_Temp;
        TCCR1B_Reg=TCCR1B_Temp;
}


void mTIMER1_Start(void)
{
        
        TCCR1B_Reg|= (TCCR1B_Reg& ~0x07u) |(Timer1_Prescaler&0x7u);

}


void mTIMER1_Stop(void)
{
    TCCR1B_Reg &= T1_StopMask;
}


void mTIMER1_RegisterCallback(void (*callback)(void))
{
        if(callback!=Null)
        {
                Timer1_Global_Callback=callback;
        }
        else {
                
        }

} 


void _vector_7(void) __attribute_((signal));

void __vector_7()
{
        if(Timer1_Global_Callback!=Null)
        {
                Timer1_Global_Callback();
        }
        else {
                
        }

}
void _vector_8(void) __attribute_((signal));
void __vector_8()
{
        if(Timer1_Global_Callback!=Null)
        {
                Timer1_Global_Callback();
        }
        else {
                
        }

}