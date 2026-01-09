/**
 * @file UART_Config.h
 * @brief Configuration file for UART driver (shared between Tx and Rx modules).
 * @details This file defines the common data structures, types, and buffer configurations
 * used by both UART transmission and reception modules.
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2025-10-08
 * @copyright Copyright (c) 2025 , Gestell Company
 */


#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_

/**
 * @addtogroup Mcal
 * @{
 * @addtogroup UART
 * @{
 */


#include"../UART/UART_Private.h"
#include <stdint.h>
#include"../Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"
#include <stdbool.h>  

/**
 * @def UART_TX_BUFFER_SIZE
 * @brief Defines the size of the UART transmission (Tx) circular buffer.
 */
#define UART_TX_BUFFER_SIZE    64

/**
 * @def UART_RX_BUFFER_SIZE
 * @brief Defines the size of the UART reception (Rx) circular buffer.
 */
#define UART_RX_BUFFER_SIZE    64

/**
 * @struct UART_Buffer_t
 * @brief Circular buffer structure for UART data handling.
 * This structure provides an efficient, non-blocking mechanism for storing
 * bytes being transmitted or received by the UART driver.
 */
typedef struct
{
    /**
     * @var buffer[UART_TX_BUFFER_SIZE]
     * @brief Internal buffer array
     */
    volatile uint8_t buffer[UART_TX_BUFFER_SIZE]; 
    /**
     * @var front
     * @brief  next byte to to be read
     */
    volatile int16_t front;
    /**
     * @var rear
     * @brief  next byte to be written 
     */
    volatile int16_t rear;

    /**
     * @var size
     * @brief Total buffer capacity
     */
    volatile uint16_t size;                     
} UART_Buffer_t;

/**
 * @defgroup Buffer
 * @brief Uart buffer fuctions to make circular communication
 * @{
  */
 
/**
 * @brief Initializes a UART buffer structure.
 * @details function resets the head and tail pointers, preparing the buffer
 * for operation. Should be called once before use.
 * @param buf Pointer to the UART buffer structure.
 * @param buffer_size Size of the buffer (Tx or Rx).
 */
static inline void UART_Buffer_Init(UART_Buffer_t *buf, uint16_t buffer_size)
{
    buf->front= -1;
    buf->rear = -1;
    buf->size = buffer_size;
}


/**
 * @brief Checks if a circular buffer is empty.
 * @param buf Pointer to the UART buffer.
 * @return true if empty, false otherwise.
 * @warning UART_Buffer_Init() must be called before this function.
 */
static inline bool UART_Buffer_IsEmpty(const UART_Buffer_t *buf)
{
    return (buf->front == -1 && buf->rear == -1);

}



/**
 * @brief Checks if a circular buffer is full.
 * @param buf Pointer to the UART buffer.
 * @return true if full, false otherwise.
 * @warning UART_Buffer_Init() must be called before this function.

 */
static inline bool UART_Buffer_IsFull(const UART_Buffer_t *buf)
{
    return ((buf->rear + 1) % buf->size == buf->front);

}
//
/**
 * @brief Inserts one byte into the buffer.
 * @param buf Pointer to the UART buffer.
 * @param data Byte to be inserted.
 * @warning UART_Buffer_Init() must be called before this function.
 */
static inline bool UART_Buffer_Put(UART_Buffer_t *buf, uint8_t data)
{
    if (UART_Buffer_IsFull(buf)){
        return false;
    }
    else {
        if (UART_Buffer_IsEmpty(buf)){
            buf->front=buf->rear=0;
            buf->buffer[buf->rear]=data;
        }
        else if (buf->rear==buf->size-1){
            buf->rear=0;
            buf->buffer[buf->rear]=data;
        }
        else{
            buf->buffer[++(buf->rear)]=data;
        }
        return true;
    }
}

/**
 * @brief Reads one byte from the buffer.
 * @param buf Pointer to the UART buffer.
 * @return Byte read from the buffer.
 * @warning UART_Buffer_Init() must be called before this function.
 */
static inline bool UART_Buffer_Get(UART_Buffer_t *buf, uint8_t *data)
{
    if (UART_Buffer_IsEmpty(buf)){
        return false;
    }
    else{
        *data = buf->buffer[buf->front];
        if(buf->front==buf->rear){
            buf->front =buf->rear=-1;  /**<one element in the buffer  */
        }
        else if(buf->front==buf->size-1){
            buf->front=0;          /**< Wrap around to beginning */
        } 
        else{
            buf->front++;           /**<Normal mode */
        }
        return true;
    }
}


/**
 * @}
 * @}
 */

#endif /* UART_CONFIG_H_ */


