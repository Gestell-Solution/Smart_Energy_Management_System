/**
 * @file       HC05_Interface.h
 * @brief      Interface file for the HC-05 Bluetooth driver.
 * @details    This file provides the public function prototypes for initializing and 
 *             communicating with the HC-05 Bluetooth module through UART0. 
 *             It includes functions for sending and receiving data buffers, strings, 
 *             and single bytes. All APIs in this file must be called after 
 *             initializing the driver via @ref hBT_Init.
 * @version    1.0
 * @date       2025-10-28
 * 
 * @author     Developer: Ahmed Twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer : Ahmed Ashraf (ahmedashraf2022222@gmail.com)
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#ifndef HC05_INTERFACE_H_
#define HC05_INTERFACE_H_

/**
 * @addtogroup Hal
 * @{
 * @addtogroup HC05_Bluetooth
 * @{
 */


/*================================= Includes =================================*/
#include "../../Mcal/UART/UART_Init.h"
#include "../../Mcal/UART/UART_Tx.h"
#include "../../Mcal/UART/UART_Rx.h"
#include "../../Common/Macros.h"
#include <string.h>
#include <stdint.h>

/*============================== Group: HC05 Driver ==========================*/
/**
 * @defgroup HC05_Driver HC05 Bluetooth Driver
 * @brief     APIs for controlling and communicating with the HC-05 Bluetooth module.
 * @details   This group provides the main interface functions used by the application 
 *            to send and receive data via Bluetooth using the HC-05 module.
 * @{
 */

/*============================================================================*/
/**
 * @fn         void hBT_Init(void)
 * @brief      Initializes the HC-05 Bluetooth module.
 * @details    Configures the UART peripheral with predefined settings to establish 
 *             communication with the HC-05 module.
 * @note       This function must be called once before any other HC-05 function.
 */
void hBT_Init(void);

/*============================================================================*/
/**
 * @fn         void hBT_SendString(const uint8_t *string)
 * @brief      Sends a null-terminated string via the HC-05 Bluetooth module.
 * @details    Transmits ASCII characters from the provided string until the null terminator (`'\0'`) is reached.
 * @param[in]  string  Pointer to the null-terminated string to be transmitted.
 * @warning    Ensure Bluetooth pairing is complete before calling this function.
 */
void hBT_SendString(const uint8_t *string);

/*============================================================================*/
/**
 * @fn         void hBT_SendBuffer(const uint8_t *data, uint16_t len)
 * @brief      Sends a raw data buffer to the HC-05 module.
 * @details    Transmits a specific number of bytes from a given data buffer via UART.
 * @param[in]  data  Pointer to the buffer containing data to send.
 * @param[in]  len   Number of bytes to send from the buffer.
 * @note       This function does not automatically append a null terminator.
 */
void hBT_SendBuffer(const uint8_t *data, uint16_t len);

/*============================================================================*/
/**
 * @fn         uint8_t hBT_ReadByte(uint8_t *data)
 * @brief      Reads a single byte from the HC-05 receive buffer.
 * @details    Waits until a byte is available in the UART RX buffer, then stores it 
 *             in the provided pointer variable.
 * @param[out] data  Pointer to variable where the received byte will be stored.
 * @return     uint8_t  Returns 1 if data was received successfully, or 0 if no data is available.
 * @warning    This function should be called periodically when expecting incoming data.
 */
uint8_t hBT_ReadByte(uint8_t *data);

/*============================================================================*/
/**
 * @fn         uint8_t hBT_ReadString(uint8_t *string)
 * @brief      Reads a null-terminated string from the HC-05 module (non-blocking).
 * @details    Receives characters over UART until a newline (`'\n'`) is detected or 
 *             the internal buffer limit is reached.
 * @param[out] string  Pointer to buffer where the received string will be stored.
 * @return     uint8_t  Status of the receive operation (0: Success, 1: Timeout/Error).
 * @note       The buffer size should be large enough to store the expected string.
 */
uint8_t hBT_ReadString(uint8_t *string);

/*============================================================================*/
/**
 * @fn         void hBT_ReadBuffer(uint8_t *data, uint16_t len)
 * @brief      Reads multiple bytes from the HC-05 receive buffer.
 * @details    Receives a defined number of bytes from UART and stores them in the provided buffer.
 * @param[out] data  Pointer to buffer where the received data will be stored.
 * @param[in]  len   Number of bytes to read.
 * @warning    Ensure the receive buffer size is sufficient for the requested length.
 */
uint16_t hBT_ReadBuffer(uint8_t *data, uint16_t len);



/** @} */ /* End of HC05_Driver group */


/**
 * @}
 * @}
 */

#endif /* HC05_INTERFACE_H_ */
