/**
 * @file       HC05_Interface.h
 * @brief      Interface file for the HC-05 Bluetooth driver.
 * @details    This file contains the function prototypes for the HC-05 Bluetooth driver,
 *             including initialization, sending, and receiving data.
 * @version    1.0
 * @date       2025-10-28
 *
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer: Ahmed Ashraf
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#ifndef HC05_INTERFACE_H_
#define HC05_INTERFACE_H_


#include "../../MCAL/UART/UART_Init.h"
#include "../../MCAL/UART/UART_Tx.h"
#include "../../MCAL/UART/UART_RX.h"
#include "../../Common/Macros.h"
#include <string.h>
#include <stdint.h>

/**
 * @brief Initializes the HC-05 Bluetooth module.
 */
void hBT_Init(void);

/**
 * @brief Sends a null-terminated string via HC-05.
 * @param string Pointer to the string to be sent.
 */
void hBT_SendString(const uint8_t *string);

/**
 * @brief Sends a raw data buffer via HC-05.
 * @param data Pointer to data buffer.
 * @param len  Length of data in bytes.
 */
void hBT_SendBuffer(const uint8_t *data, uint16_t len);

/**
 * @brief Reads one byte from the HC-05 receive buffer.
 * @param data Pointer to variable where received byte will be stored.
 * @return 1 if data available, 0 if buffer is empty.
 */
uint8_t hBT_ReadByte(uint8_t *data);

/**
 * @brief Reads a null-terminated string from HC-05.
 * @param string Pointer to buffer to store received string.
 */
void hBT_ReadString(uint8_t *string);

/**
 * @brief Reads multiple bytes from HC-05 buffer.
 * @param data Pointer to buffer to store received bytes.
 * @param len  Number of bytes to read.
 */
void hBT_ReadBuffer(uint8_t *data, uint16_t len);

#endif /* HC05_INTERFACE_H_ */
