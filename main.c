#include "Common/Macros.h"
#include "Mcal/Atmega32RegistersAddress.h"
#include "Mcal/Timer1/TIMER1_Interface.h"
#include <util/delay.h>
#define GIE_Enable()  (SREG_Reg |= (1u << 7))

void Task1()
{
    SetBit(PORTA_Reg, 1);
    

}
void Task2()
{
    ClearBit(PORTA_Reg, 1);


}

int  main()
{

    DDRA_Reg=0xFF;
    mTIMER1_Init();
    GIE_Enable();
    mTIMER1_RegisterCallback(Task1);
    while(1)
    {
        _delay_ms(500);
        mTIMER1_RegisterCallback(Task2);
        _delay_ms(500);
        mTIMER1_RegisterCallback(Task1);
    }
}