/**
 * @file UART_Program.c
 * @brief intialize the uart and simulate it .
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2025-10-08
 * @copyright Copyright (c) 2025 , Gestell Company
 */
#include "../UART/UART_Tx.h"
#include "../UART/UART_Rx.h"
#include"../UART/UART_Init.h"
#include"../UART/UART_Config.h"
void UART_ReceivedAction(uint8_t data)
{
    mUART_SendByte(data); 
}

void UART_TransmitComplete(void)
{

}

/* UART Configuration  */
UART_Config_t UART_Config = {
    .BaudRate = 9600,
    .DataBits = UART_8_BIT,
    .Parity = UART_PARITY_NONE,
    .StopBits = UART_1_STOP
};
int main(void)
{
    mUART_Init(&UART_Config);
    UART_Tx_Init();
    UART_Rx_Init();
    UART_Rx_SetCallback(UART_ReceivedAction);
    UART_Tx_SetCallback(UART_TransmitComplete);

    
    SetBit(SREG_Reg, 7); /*  Enable Global Interrupts  */

    mUART_SendString("UART Interrupt Test Started!\r\n");

    while (1)
    {
        
    }
}
