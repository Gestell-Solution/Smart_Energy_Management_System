#include "../../../../Src/Common/Config.h"

#if CommunicationManager_TestModule == Enable

#include "CommunicationManager.h"
#include "../../../../Src/Mcal/UART/UART_Tx.h"
#include "../../../../Src/Hal/HC05/HC05_Interface.h"
#include "../../../../Src/Mcal/UART/UART_Rx.h"
#include "../../../../Src/Mcal/UART/UART_Init.h"
#include "../../../../Src/Mcal/GIE/GIE_Interface.h"
#include "../../../../Src/Mcal/DIO/DIO_Interface.h"
#include "../../../../Src/App/CommunicationManager/App_CommManager.h"
#include "../../../../Src/Mcal/Timer0/TIMER0_Interface.h"
#include <util/delay.h>

void toggle()
{
         mDIO_TogglePin(GroupD,PIN3);

}
void CommManagerTestsMain()
{
    App_CommManager_Init();
    mGIE_Enable();
    mDIO_SetDirectionForPin(GroupD,PIN0,Input);
    mDIO_SetDirectionForPin(GroupD,PIN1,Output);
    mDIO_SetDirectionForPin(GroupD,PIN3,Output);
    // mTIMER0_StartDelay(500,toggle);
    while (1)
    {

        App_CommManager_SendFrame("HELLO, IAM: ",0x1,13)   ;     
        App_CommManager_SendFrame(" والنبي اشتغل يا سعييد",0xAA,7)   ;     
        App_CommManager_ReceiveHandler("والنبي اشتغل يا سعييد ");
        _delay_ms(500);
            
    }
    /// message 1 "ª5345"
    


}
#endif