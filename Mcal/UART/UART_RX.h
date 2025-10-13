/**
 * @file UART_Rx.h
 * @brief UART Reception (Rx) interface for ATmega32.
 * @details Provides non-blocking APIs for reading bytes, buffers, and strings
 * from the UART receive circular buffer managed by the RX interrupt.* @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2025-10-08
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#ifndef UART_RX_H_
#define UART_RX_H_

#include "../UART/UART_Config.h"

void UART_Rx_Init(void);

/**
 * @brief Retrieves a single received byte from the Rx buffer (non-blocking).
 * @param data Pointer to a variable where the received byte will be stored.
 * @retval 1 Successfully read one byte from buffer,0 No data available in buffer.
 */
uint8_t mUART_ReceiveByte(uint8_t *data);

/**
 * @brief Receive multiple bytes from the UART buffer.
 * @param data Pointer to destination buffer to store received bytes.
 * @param maxLength Maximum number of bytes to read.
 * @return Number of bytes actually read from the receive buffer.
 */
uint16_t mUART_ReceiveBuffer(uint8_t *data, uint16_t maxLength);

/**
 * @brief Reads characters into a string until a terminator is found or maxLength reached.
 * @param str Pointer to character array where the received string will be stored.
 * @param maxLength Maximum number of characters to store (including null terminator).
 * @param terminator Character that indicates the end of the string.
 
 * @retval 1 if terminator was found and string completed, 0 if terminator not yet received.
 */
uint8_t mUART_ReceiveString(char *str, uint16_t maxLength, char terminator);


/**
 * @brief Set the user callback function for UART reception.
 * @param LocalPtr Pointer to a user function that takes a received byte as argument.
 */
void UART_Rx_SetCallback(void (*LocalPtr)(uint8_t));

#endif /* UART_RX_H_ */
