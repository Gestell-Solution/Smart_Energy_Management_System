<<<<<<< HEAD
<<<<<<< HEAD
#include "Tests/Uart/uartTest.h"
void main()
{
    UartTestMain();
    while(1)
    {
        
=======
<<<<<<< HEAD
=======
>>>>>>> 8d69fa8498a4864fee1dcf3307c94d1d8e86d50d
// AVR peripheral libraries
//#include <avr/interrupt.h> // Required for sei()
#include <util/delay.h>    // Required for _delay_ms()
=======
#include "App/CommunicationManager/App_CommManager.h"
#include "Mcal/Timer0/TIMER0_Interface.h"
#include "Mcal/GIE/GIE_Interface.h"
#include "Hal/HC05/HC05_Interface.h"
// Include your HAL headers here (e.g., hBT.h, mTIMER.h)
>>>>>>> fafca73 (Fixing Some logic of using static data logic)

int main(void)
{

<<<<<<< HEAD
<<<<<<< HEAD
    mGIE_Enable();
        SetBit(SREG_Reg,7); 
=======
=======
    mGIE_Enable();
    mTIMER0_Init();
>>>>>>> 3bedcf6 (Testing)
    App_CommManager_Init(); 
>>>>>>> fafca73 (Fixing Some logic of using static data logic)



    while (1)
    {
        hBT_ReadString("hello");

        mTIMER0_Delay_ms(50);
<<<<<<< HEAD
>>>>>>> 6660dab (adding the changes of the Master)
=======
>>>>>>> 8d69fa8498a4864fee1dcf3307c94d1d8e86d50d
    }
}