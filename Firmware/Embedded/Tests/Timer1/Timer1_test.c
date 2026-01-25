#include "../../../../Src/Mcal/Timer1/TIMER1_Interface.h"
#include "../../../../Src/Mcal/UART/UART_Tx.h"
#include "../../../../Src/Hal/HC05/HC05_Interface.h"
#include "../../../../Src/Mcal/UART/UART_Rx.h"
#include "../../../../Src/Mcal/UART/UART_Init.h"
#include "../../../../Src/Mcal/GIE/GIE_Interface.h"
#include "../../../../Src/Mcal/DIO/DIO_Interface.h"
#include "Timer1_test_Interface.h"
void toggle()
{

    mDIO_TogglePin(GroupA, PIN3);
}
void Timer1_Test1_CallBack()
{
    toggle();
}
void Timer1_Test2_CallBack()
{
    static short int Num=0;
    if (Num==100)
    {
        mUART_SendByte(Num);
        Num=0;
    }else
    {
        Num++;
    }
}
void Timer1_Test1_Init()
{
    
    mDIO_SetDirectionForPin(GroupA, PIN3, Output);
    mTIMER1_RegisterCallback(Timer1_Test1_CallBack);
}
void Timer1_Test2_Init()
{

    mTIMER1_RegisterCallback(Timer1_Test2_CallBack);
}
// void Timer1_Test3_Init()
// {
//     mDIO_SetDirectionForPin(GroupA, PIN3, Output);
//     mTIMER1_RegisterCallback(Timer1_Test_CallBack);
// }

// 1-  test for 1 ms change to 10 ms
// 2-  if 10ms test the test cases