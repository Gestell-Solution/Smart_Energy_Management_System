#include "Mcal/Timer0/TIMER0_Interface.h"


void Task1(void) { PORTB_Reg ^= (1 << 0); } 
void Task2(void) { PORTB_Reg ^= (1 << 1); } 

int main(void)
{
    mTIMER0_Init();
    DDRB_Reg |= (1 << 0) | (1 << 1); // Set PB0 and PB1 as output
    SetBit(SREG_Reg,7);//Global interrupt

    
    mTIMER0_StartDelay(500, Task1);  // Task1 after 500 ms
    mTIMER0_StartDelay(1000, Task2); // Task2 after 1000 ms
    while (1)
    {
        // Main loop continues running
    }
}
