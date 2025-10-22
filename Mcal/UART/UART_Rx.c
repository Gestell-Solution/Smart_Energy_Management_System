/**
 * @file UART_Rx.c
 * @brief UART Reception (Rx) implementation for ATmega32.
 * @details Implements non-blocking read functions for UART using circular buffer.
 *          Data is filled via interrupt (RXC_vect) and accessed by the application
 *          through these APIs.
 * @version 2.0.0
 * @author Developer : Basma khaled     (basmak55@gmail.com)
 * @author Developer : Ahmed Ashraf     (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Hesham Ahmed     (Hisham4Ahmed@gmail.com)
 * @date 2025-10-21
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#include "../../Common/Config.h"
#if UART_Module == Enable 
#include "../UART/UART_Rx.h"

extern UART_Buffer_t UART_RxBuffer;
static void (*UART_Rx_Callback)(uint16_t) = Null;

uint8_t mUART_ReceiveByte(uint8_t *data)
{
    if (UART_Buffer_IsEmpty(&UART_RxBuffer))
    {
        return 0;
    }
    return UART_Buffer_Get(&UART_RxBuffer, data); //modification : 2 arguments function
}

uint16_t mUART_ReceiveBuffer(uint8_t *data, uint16_t maxLength)
{
    uint16_t count = 0;

    while (count < maxLength && UART_Buffer_Get(&UART_RxBuffer, &data[count]))
    {
        count++;
    }

    return count;
}

uint8_t mUART_ReceiveString(char *str, uint16_t maxLength, char terminator)
{
    static uint16_t index = 0;
    uint8_t c;
   

    while (UART_Buffer_Get(&UART_RxBuffer, (uint8_t*)&c))
    {
        if (c == terminator ||c== '\r' ||c== '\n')
        {
            str[index] = '\0';
            index = 0;
            return 1;
        }

       else if (index < (maxLength - 1))
        {
            str[index++] = c;
        }
        else
        {
            str[index] = '\0';
            index = 0;
            return 0;
        }
    }

    return 0;
}

void UART_Rx_Init(void)
{
    UART_Buffer_Init((UART_Buffer_t *)&UART_RxBuffer, UART_RX_BUFFER_SIZE);
    SetBit(UCSRB_Reg, uart_RXCIE);
}

void UART_Rx_SetCallback(void (*LocalPtr)(uint8_t))
{
    if (LocalPtr != 0)
        UART_Rx_Callback = LocalPtr;
}

/**
 * @brief UART Receive Complete ISR .
 * @details Reads received byte, stores in buffer, and triggers user callback.
 * __vector_13 corresponds to USART_RXC_vect on ATmega32.
 */

void __vector_13(void) __attribute__((signal));
void __vector_13(void)
{
    uint16_t data = UDR_Reg;

    if (!UART_Buffer_IsFull((UART_Buffer_t *)&UART_RxBuffer)){
        UART_Buffer_Put((UART_Buffer_t *)&UART_RxBuffer, data);
    }

    if (UART_Rx_Callback != Null){
        UART_Rx_Callback(data);
    }
}

#endif