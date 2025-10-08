
/**
 * @file       TIMER0_Program.c
 * @brief      This file contains Implementation for the APIs in the Interface.h  for Timer0.
 * @details    This C file defines several Functions that Implement the APIs in defining Specified Actions based on project requirements.
 * @version    0.2
 * @author     Developer: Mohamed Diaa (mohammeddiaato@gmail.com)
 * @author     Reviewer:  Basma        ()
 * @date       2025-10-07 
 * @copyright  Copyright (c) 2025 , Gestell Company 
 */
#include "TIMER0_Interface.h"


Timer0_ScheduledTasks Timer0_TasksList[T0_ScheduledTasksNum]={};

void mTIMER0_Init(void)//CTC MODE
{
        uint8_t TCCR0_Temp=0;
        ClearBit(TCCR0_Temp,T0_WGM00_Bit);
        SetBit(TCCR0_Temp,T0_WGM01_Bit);

        ClearBit(TCCR0_Temp,T0_COM00_Bit);
        ClearBit(TCCR0_Temp,T0_COM01_Bit);

        TCCR0_Temp|=Timer0_Prescaller;
        OCR0_Reg=CompareMatchValue;

        CompareMatch_InterruptEnable;
        
        TCCR0_Reg=TCCR0_Temp;

}

void mTIMER0_Delay_ms(uint32_t delay_ms)//Blocking Delay on the Timer
{
        for (int i = 0; i < delay_ms; i++)
        {
                while (!IsCOM_FlagSet);
                ClearFlag(TIFR_Reg,OCF0_Bit);
        }
        
}                           
void mTIMER0_StartDelay(uint32_t delay_ms, void (*callback)(void))
{
        
for (int i = 0; i < T0_ScheduledTasksNum; i++)
{
        if (!Timer0_TasksList[i].Active)
        {
                Timer0_TasksList[i].Callback=callback;
                Timer0_TasksList[i].Remaining_Ticks=delay_ms;
                // Timer0_TasksList[i].TaskID=i;
                Timer0_TasksList[i].Active=1;           
        }
        
}

} 
void mTIMER0_TickHandler(void)
{
        for ( int i = 0; i < T0_ScheduledTasksNum; i++)
        {
                if (Timer0_TasksList[i].Active&&Timer0_TasksList[i].Remaining_Ticks>0)
                {
                                Timer0_TasksList[i].Remaining_Ticks--;

                }
                if (Timer0_TasksList[i].Active&&Timer0_TasksList[i].Remaining_Ticks==0)
                {
                                if (Timer0_TasksList[i].Callback!=Null)
                                {
                                        Timer0_TasksList[i].Callback();
                                }
                                Timer0_TasksList[i].Active=0;
                }
                
                        
        }
                
}                                     

void __vector_10(void) __attribute__((signal));
void __vector_10(void) {
        mTIMER0_TickHandler();
}
