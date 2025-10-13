/**
 * @file UART_Init.h
 * @brief UART Initialization configuration and interface for ATmega32.
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2025-10-08
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#ifndef UART_INIT_H_
#define UART_INIT_H_

#include "../UART/UART_Config.h"
#include <stdint.h>

/**
 * @def UART_BAUDRATE
 * @brief UART Baud Rate configuration (in bits per second).
 */
#define UART_BAUDRATE        9600UL


/**
 * @enum UART_DataBits_t
 * @brief UART Data Bits configuration.
 */
typedef enum
{
    /**
     * @defgroup DataBits
     * @brief number of bits will be sent and recieve
     * @{
      */
    UART_5_BIT = 0x00,  /**< 5 data bits */
    UART_6_BIT = 0x02,  /**< 6 data bits */
    UART_7_BIT = 0x04,  /**< 7 data bits */
    UART_8_BIT = 0x06,  /**< 8 data bits (default) */
} UART_DataBits_t;

/**
 * @enum UART_StopBits_t
 * @brief UART Stop Bits configuration.
 */
typedef enum
{
    UART_1_STOP = 0x00, /**< 1 stop bit (default) */
    UART_2_STOP = 0x08  /**< 2 stop bits */
} UART_StopBits_t;

/**
 * @enum UART_Parity_t
 * @brief UART Parity configuration.
 */
typedef enum
{
    UART_PARITY_NONE = 0x00, /**< No parity (default) */
    UART_PARITY_EVEN = 0x20, /**< Even parity */
    UART_PARITY_ODD  = 0x30  /**< Odd parity */
} UART_Parity_t;

/**
 *  @def UART_DATA_BITS
 *  @brief Select the number of data bits for UART frame. 
 */
#define UART_DATA_BITS   UART_8_BIT

/** 
 * @def UART_STOP_BITS
 * @brief Select number of stop bits.
 */
#define UART_STOP_BITS   UART_1_STOP

/**
 *  @def UART_PARITY_MODE
 *  @brief Select parity mode. 
 */
#define UART_PARITY_MODE UART_PARITY_NONE

/**
 * @struct UART_Config_t
 * @brief Structure holding UART configuration parameters.
 */
typedef struct
{
    uint32_t BaudRate;         /**< Communication speed (bits per second) */
    UART_DataBits_t DataBits;  /**< Number of data bits */
    UART_StopBits_t StopBits;  /**< Number of stop bits */
    UART_Parity_t Parity;      /**< Parity mode */
} UART_Config_t;
/**
 * @brief Initializes UART peripheral with the defined configuration.
 * @details baud rate, frame format (data bits, stop bits, parity),
 * enables transmitter and receiver.
 */
void mUART_Init(const UART_Config_t *Config);

#endif /* UART_INIT_H_ */
