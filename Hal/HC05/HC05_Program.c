/**
 * @file       HC05_Program.c
 * @brief      Program file for the HC-05 Bluetooth driver.
 * @details    This file contains the implementation of all APIs used for initializing and 
 *             communicating with the HC-05 Bluetooth module via UART0. 
 *             It includes functions to send and receive data, either as single bytes, 
 *             strings, or buffers. 
 *             The HC-05 driver provides a high-level interface for Bluetooth serial communication.
 * @version    1.0
 * @date       2025-10-28
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer: Ahmed Ashraf (ahmedashraf2022222@gmail.com)
 * @copyright  Copyright (c) 2025, Gestell Company
 */

/*================================= Includes =================================*/
#include "HC05_Config.h"
#include "HC05_Interface.h"
#include "HC05_Private.h"

/*============================== Static Objects ==============================*/


/**
 * @brief UART configuration structure for HC-05.
 */
static const UART_Config_t BT_Config = {
    .BaudRate = BT_BAUDRATE,
    .Parity   = UART_PARITY_NONE,
    .StopBits = UART_1_STOP,
    .DataBits = UART_8_BIT
};

/*=============================== API Functions ==============================*/


void hBT_Init(void)
{
    mUART_Init(&BT_Config);
    UART_Tx_Init();
    UART_Rx_Init();
}

/*============================================================================*/

void hBT_SendString(const uint8_t *string)
{
    mUART_SendString((const char *)string);
}

/*============================================================================*/

void hBT_SendBuffer(const uint8_t *data, uint16_t len)
{
    mUART_SendBuffer(data, len);
}

/*============================================================================*/

uint8_t hBT_ReadByte(uint8_t *data)
{
    return mUART_ReceiveByte(data);
}


/*============================================================================*/

uint8_t hBT_ReadString(uint8_t *string)
{
    return mUART_ReceiveString((char *)string, 64, '\n');
}

/*============================================================================*/

uint16_t hBT_ReadBuffer(uint8_t *data, uint16_t len)
{
    return mUART_ReceiveBuffer(data, len);
}

/*============================================================================*/


