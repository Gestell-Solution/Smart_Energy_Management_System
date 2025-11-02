/**
 * @file UART_Tx.c
 * @brief UART Transmission (Tx) implementation for ATmega32.
 * @details Implements non-blocking UART transmission using interrupts and a circular buffer.
 * Data is added to the Tx buffer, and the ISR (UDRE_vect) handles actual transmission.
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2025-10-08
 * @copyright Copyright (c) 2025 , Gestell Company
 */


#include "../UART/UART_Tx.h"
#include "../../Common/Macros.h"

/* External Tx buffer defined in UART_Interrupt.c */
extern UART_Buffer_t UART_TxBuffer;


/**
 * @fn UART_Tx_Init
 * @brief Initialize the UART Tx buffer.
 */
void UART_Tx_Init(void)
{
    UART_Buffer_Init((UART_Buffer_t *)&UART_TxBuffer, UART_TX_BUFFER_SIZE);
    SetBit(UCSRB_Reg, uart_TXCIE);  /**<Enable Transmit Complete Interrupt */
    /* Enable Data Register Empty Interrupt(interrupt when udr is ready for another byte) */
    //SetBit(UCSRB_Reg, uart_UDRIE); 

}


/**
 * @fn mUART_SendByte
 * @brief Sends a single byte using non-blocking UART transmission.
 * @details Update the tx Buffer with data will be Sent and Waiting until Data Transimate
 * @param data The byte to send.
 */
void mUART_SendByte(uint8_t data)
{
    /* Wait if buffer is full */
    if(UART_Buffer_IsFull(&UART_TxBuffer)) {
        return ;
    }
    UART_Buffer_Put(&UART_TxBuffer, data);
    SetBit(UCSRB_Reg, uart_UDRIE); 

}

/**
 * @fn mUART_SendString
 * @brief Sends a null-terminated string using non-blocking UART transmission.
 * @details Update the tx Buffer with data will be Sent and Waiting until Data Transimate
 * @param str Pointer to the string to send.
 */
void mUART_SendString(const char *str)
{
    if (str==Null)
    {
        return;
    }
    
    while (*str != '\0') 
    {
        mUART_SendByte((uint8_t)*str);
        str++;
    }
}

/**
 * @fn mUART_SendBuffer
 * @brief Sends a raw data buffer using non-blocking UART transmission.
 * @details Update the tx Buffer with data of any length bytes of will be Sent and Waiting until Data Transimate
 * @param data Pointer to data buffer.
 * @param length Number of bytes to send.
 */
void mUART_SendBuffer(const uint8_t *data, uint16_t length)
{
    if (data==Null || length==0)
    {
        return;
    }
    for (uint16_t i = 0; i < length; i++)
    {
        mUART_SendByte(data[i]);
    }
}

/**
 * @brief UART Data Register Empty ISR (manual vector).
 * @details Sends next byte from buffer or disables interrupt if no data.
 * __vector_14 corresponds to USART_UDRE_vect on ATmega32.
 */
void __vector_14(void) __attribute__((signal));
void __vector_14(void)
{
    uint8_t data;

    // If there’s still data in the transmit buffer
    if (UART_Buffer_Get((UART_Buffer_t *)&UART_TxBuffer, &data))
    {
        // Load the next byte into the UART Data Register
        UDR_Reg = data;
    }
    else
    {
        ClearBit(UCSRB_Reg, uart_UDRIE); /**< Disable interrupt */ //don't
    }
}
