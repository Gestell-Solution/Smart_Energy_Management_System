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

/* External Tx buffer defined in UART_Interrupt.c */
extern UART_Buffer_t UART_TxBuffer;
static void (*UART_Tx_Callback)(void) = 0; /**< Pointer to transmit complete callback */


/**
 * @fn UART_Tx_Init
 * @brief Initialize the UART Tx buffer.
 */
void UART_Tx_Init(void)
{
    UART_Buffer_Init((UART_Buffer_t *)&UART_TxBuffer, UART_TX_BUFFER_SIZE);
    SetBit(UCSRB_Reg, uart_TXCIE);  /**<Enable Transmit Complete Interrupt */

}

/**
 * @fn UART_Tx_SetCallback
 * @brief Set callback function for transmission complete event.
 * @param LocalPtr Pointer to user function .
 */
void UART_Tx_SetCallback(void (*LocalPtr)(void))
{
    if (LocalPtr != 0)
        UART_Tx_Callback = LocalPtr;
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
    while (UART_Buffer_IsFull(&UART_TxBuffer))
        ;  // Could also choose to drop instead of blocking

    UART_Buffer_Put(&UART_TxBuffer, data);

    /* Enable Data Register Empty Interrupt */
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
    if (!UART_Buffer_IsEmpty((UART_Buffer_t *)&UART_TxBuffer))
    {
        UDR_Reg = UART_Buffer_Get((UART_Buffer_t *)&UART_TxBuffer);
    }
    else
    {
        ClearBit(UCSRB_Reg, uart_UDRIE); /**< Disable interrupt */
        if (UART_Tx_Callback != 0)
            UART_Tx_Callback();
    }
}
