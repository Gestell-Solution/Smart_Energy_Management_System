#include "../../Common/Config.h"

#if Uart_TestModule == Enable

#include "../../Mcal/UART/UART_Tx.h"
#include "../../Mcal/UART/UART_Rx.h"
#include "../../Mcal/UART/UART_Init.h"
#include "../../Mcal/GIE/GIE_Interface.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include <util/delay.h>
void UartTestMain()
{
    UART_Config_t config = {
    .BaudRate = 9600,
    .DataBits = UART_8_BIT,
    .StopBits = UART_1_STOP,
    .Parity   = UART_PARITY_NONE
    };
    mDIO_SetDirectionForPin(GroupD,PIN0,Input);
    mDIO_SetDirectionForPin(GroupD,PIN1,Output);
    
    mUART_Init(&config);
    UART_Tx_Init();
    mGIE_Enable();
    while(1){
    mUART_SendString("Hello World!\r\n");
    _delay_ms(500);
    }
    // mUART_SendByte(0x55);
    // mUART_SendByte(0x55);
    // mUART_SendByte(0x55);
    // mUART_SendByte(0x55);
    // mUART_SendByte(0x55);
}
#endif