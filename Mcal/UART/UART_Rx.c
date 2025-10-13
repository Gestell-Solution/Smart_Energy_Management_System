/**
 * @file UART_Rx.c
 * @brief UART Reception (Rx) implementation for ATmega32.
 * @details Implements non-blocking read functions for UART using circular buffer.
 *          Data is filled via interrupt (RXC_vect) and accessed by the application
 *          through these APIs.
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2025-10-08
 * @copyright Copyright (c) 2025 , Gestell Company
 */


 
#include "../UART/UART_Rx.h"

/* External Rx buffer defined in UART_INT.c */
extern UART_Buffer_t UART_RxBuffer;
static void (*UART_Rx_Callback)(uint8_t) = 0; /**< Pointer to receive callback function */


/**
 * @fn mUART_ReceiveByte
 * @brief Receive a single byte from the UART buffer.
 * @details 
 * Checks if data is available in the receive buffer. 
 * If data exists, it reads one byte and returns success.
 * @param data Pointer to a variable where the received byte will be stored.
 * @retval 1 Successfully read one byte from buffer,0 No data available in buffer.
 */

 
uint8_t mUART_ReceiveByte(uint8_t *data)
{
    if (UART_Buffer_IsEmpty(&UART_RxBuffer))
        return 0;     /**<  No data available in buffer */


    *data = UART_Buffer_Get(&UART_RxBuffer);
    return 1;         /**< Successfully read one byte */
}


/**
 * @fn mUART_ReceiveBuffer
 * @brief Receive multiple bytes from the UART buffer.
 * @details 
 * Reads up to maxLength bytes from the UART receive buffer and stores them
 * into the provided data array. Stops reading when the buffer is empty or
 * when  maxLength bytes are read.
 * @param data Pointer to destination buffer to store received bytes.
 * @param maxLength Maximum number of bytes to read.
 * @return Number of bytes actually read from the receive buffer.
 */

uint16_t mUART_ReceiveBuffer(uint8_t *data, uint16_t maxLength)
{
    uint16_t count = 0;

    while (!UART_Buffer_IsEmpty(&UART_RxBuffer) && count < maxLength)
    {
        data[count++] = UART_Buffer_Get(&UART_RxBuffer);    /**< Read next byte. */
    }

    return count; /**< Return number of bytes successfully read. */ 
}


/**
 * @fn mUART_ReceiveString
 * @brief Reads characters into a string until a terminator is found or maxLength reached.
 * @details 
 * Continuously reads characters from the UART receive buffer until the 
 * specified terminator character is encountered or the maximum length 
 * is reached. The resulting string is null-terminated.
 *
 * @param str Pointer to character array where the received string will be stored.
 * @param maxLength Maximum number of characters to store (including null terminator).
 * @param terminator Character that indicates the end of the string (e.g., '\n').
 *
 * @retval 1 if terminator was found and string completed, 0 if terminator not yet received.
 */

uint8_t mUART_ReceiveString(char *str, uint16_t maxLength, char terminator)
{
    static uint16_t index = 0;

    while (!UART_Buffer_IsEmpty(&UART_RxBuffer))
    {
        char c = UART_Buffer_Get(&UART_RxBuffer);   /**< Get next character. */

        if (c == terminator)
        {
            str[index] = '\0';    /**< Null-terminate the string. */
            index = 0;
            return 1;             /**< Complete string received. */
        }

        if (index < (maxLength - 1))
        {
            str[index++] = c;     /**< Store character in string buffer. */
        }
        else
        {
            /**< Buffer full*/
            str[index] = '\0';
            index = 0;
            return 0;
        }
    }

    return 0; /**< No terminator received yet. */
}


/**
 * @fn UART_Rx_Init
 * @brief Initialize the UART Rx buffer and enable RX interrupt.
 */
void UART_Rx_Init(void)
{
    UART_Buffer_Init((UART_Buffer_t *)&UART_RxBuffer, UART_RX_BUFFER_SIZE);
    SetBit(UCSRB_Reg, uart_RXCIE); /**< Enable Receive Complete interrupt */
}

/**
 * @fn UART_Rx_SetCallback
 * @brief Set the user callback function for UART reception.
 * @param LocalPtr Pointer to a user function that takes a received byte as argument.
 */
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
    uint8_t data = UDR_Reg; /**< Read to clear RX flag */

    if (!UART_Buffer_IsFull((UART_Buffer_t *)&UART_RxBuffer))
        UART_Buffer_Put((UART_Buffer_t *)&UART_RxBuffer, data);

    if (UART_Rx_Callback != 0)
        UART_Rx_Callback(data);
}