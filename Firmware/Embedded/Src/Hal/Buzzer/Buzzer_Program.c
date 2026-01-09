/**
 * @file Buzzer_Program.c
 * @brief Implementation of Buzzer control driver (On, Off, Beep, Alert, and Periodic modes).
 * @details 
 * This module controls the buzzer connected to the MCU. It provides multiple operating modes:
 * ON, OFF, single beep, periodic beep, and alert beep. 
 * The buzzer timing is handled using Timer0 delay callbacks.
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @copyright Copyright (c) 2025 , Gestell Company
 */
#include <stdint.h>
#include"../../Common/Macros.h"
#include"../../Mcal/DIO/DIO_Interface.h"
#include"../Buzzer/Buzzer_Interface.h"
#include"../../Mcal/Timer0/TIMER0_Interface.h"
static Buzzer_State_t buzzer_state= {BUZZER_OFF, 0};

void Buzzer_Init(void){
    mDIO_SetDirectionForPin(BUZZER_PORT,BUZZER_PIN,Output);
    Buzzer_Off();
}

void Buzzer_On(void){
    mDIO_WritePin(BUZZER_PORT, BUZZER_PIN, High);
    buzzer_state.is_on=1;
}
void Buzzer_Off(void){
    mDIO_WritePin(BUZZER_PORT, BUZZER_PIN, Low);
    buzzer_state.is_on=0;
}
void Buzzer_Beep(uint16_t duration){
    Buzzer_On();
    mTIMER0_StartDelay(duration, Buzzer_Off);
}
void Buzzer_Alert_Callback(void)
{
    Buzzer_SetMode(BUZZER_ALERT);
}
void Buzzer_Periodic_Callback(void) 
{ 
    Buzzer_SetMode(BUZZER_BEEP_PERIODIC);
}

void Buzzer_SetMode(Buzzer_Mode_t mode){
    buzzer_state.mode=mode;
    switch (mode){
        case BUZZER_OFF:
            Buzzer_Off();
            break;

        case BUZZER_ON:
            Buzzer_On();
            break;

        case BUZZER_BEEP_ONCE:
            Buzzer_Beep(BUZZER_BEEP_DURATION);
            break;

        case BUZZER_BEEP_PERIODIC:
            Buzzer_Beep(BUZZER_BEEP_DURATION);    
            mTIMER0_StartDelay(BUZZER_PERIODIC_DURATION, Buzzer_Periodic_Callback);
            break;

        case BUZZER_ALERT:
            Buzzer_Beep(BUZZER_ALERT_DURATION);   
            mTIMER0_StartDelay(BUZZER_ALERT_DURATION, Buzzer_Alert_Callback);
            break;
        default:
            Buzzer_Off();
            break;

    }
}
