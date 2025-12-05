<<<<<<< HEAD
// AVR peripheral libraries
//#include <avr/interrupt.h> // Required for sei()
#include <util/delay.h>    // Required for _delay_ms()
=======
#include "App/CommunicationManager/App_CommManager.h"
#include "Mcal/Timer0/TIMER0_Interface.h"
// Include your HAL headers here (e.g., hBT.h, mTIMER.h)
>>>>>>> fafca73 (Fixing Some logic of using static data logic)

int main(void)
{

<<<<<<< HEAD
    mGIE_Enable();
        SetBit(SREG_Reg,7); 
=======
    App_CommManager_Init(); 
>>>>>>> fafca73 (Fixing Some logic of using static data logic)


    uint8_t boot_msg[] = { 'R', 'e', 'a', 'd', 'y' };
    App_CommManager_SendFrame( boot_msg,0x00, 5);

    while (1)
    {
        App_CommManager_Task();


        mTIMER0_Delay_ms(50);
    }
}