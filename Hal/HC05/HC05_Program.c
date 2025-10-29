/**
 * @file       HC05_Config.h
 * @brief      Configuration file for the HC-05 Bluetooth driver.
 * @details    This file contains user-defined configuration macros for the HC-05 Bluetooth driver,
 *             including UART settings and parameters.
 * @version    1.0
 * @date       2025-10-28
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer: Ahmed Ashraf
 * @copyright  Copyright (c) 2025, Gestell Company
 */



#include "HC05_Config.h"
#include "HC05_Interface.h"
#include "HC05_Private.h"

/**
 * @brief Local variable to store HC-05 operational state.
 */

/**
 * @brief UART configuration structure for HC-05.
 */
static const UART_Config_t BT_Config = {
    .BaudRate = BT_BAUDRATE,
    .Parity   = UART_PARITY_NONE,
    .StopBits = UART_1_STOP,
    .DataBits = UART_8_BIT
};

/**
 * @brief Initializes the HC-05 Bluetooth module using UART0.
 */
void hBT_Init(void)
{
    mUART_Init(&BT_Config);
    UART_Tx_Init();
    UART_Rx_Init();
}

/**
 * @brief Sends a null-terminated string to HC-05.
 */
void hBT_SendString(const uint8_t *string)
{
    mUART_SendString((const char *)string);
}

/**
 * @brief Sends a data buffer via HC-05.
 */
void hBT_SendBuffer(const uint8_t *data, uint16_t len)
{
    mUART_SendBuffer(data, len);
}

/**
 * @brief Reads a single byte from the HC-05 receive buffer.
 */
uint8_t hBT_ReadByte(uint8_t *data)
{
    return mUART_ReceiveByte(data);
}

/**
 * @brief Reads a null-terminated string from the HC-05 receive buffer.
 */
void hBT_ReadString(uint8_t *string)
{
    while (!mUART_ReceiveString((char *)string, 64, '\n'))//wait until string is received
        ;
}

/**
 * @brief Reads multiple bytes from the HC-05 receive buffer.
 */
void hBT_ReadBuffer(uint8_t *data, uint16_t len)
{
    mUART_ReceiveBuffer(data, len); 
}

