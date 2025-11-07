/**
 * @file UART_Tx.h
 * @brief UART Transmission (Tx) driver for ATmega32 - Non-blocking with interrupts.
 *@details
 * This module implements UART transmission functionality 
 * It uses interrupt-driven transmission and a circular buffer to send data without
 * blocking CPU time, ensuring smooth operation alongside ADC sampling and control logic.
 * Features:
 * - Send single byte
 * - Send null-terminated string
 * - Send raw data buffer
 * - Non-blocking transmission using interrupts and circular buffer
 * - Configurable buffer size 
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2025-10-08
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#ifndef UART_TX_H_
#define UART_TX_H_

#include "../UART/UART_Init.h"


void UART_Tx_Init(void);

/**
 * @brief Sends a single byte using non-blocking UART transmission.
 * @details Update the tx Buffer with data will be Sent and Waiting until Data Transimate
 * @param data The byte to send.
 */
void mUART_SendByte(uint8_t data);

/**
 * @brief Sends a null-terminated string using non-blocking UART transmission.
 * @details Update the tx Buffer with data will be Sent and Waiting until Data Transimate
 * @param str Pointer to the string to send.
 */
void mUART_SendString(const char *str);

/**
 * @brief Sends a raw data buffer using non-blocking UART transmission.
 * @details Update the tx Buffer with data of any length bytes of will be Sent and Waiting until Data Transimate
 * @param data Pointer to data buffer.
 * @param length Number of bytes to send.
 */
void mUART_SendBuffer(const uint8_t *data, uint16_t length);

/**
 * @brief Set callback function for transmission complete event.
 * @param LocalPtr Pointer to user function .
 */

#endif /* UART_TX_H_ */



