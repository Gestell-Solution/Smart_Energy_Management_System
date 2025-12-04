/**
 * @file       SPI_Interface.h
 * @brief      Public interface for the SPI driver.
 * @details    This file contains all API functions used to initialize and operate
 *             the Serial Peripheral Interface (SPI) module. The driver supports
 *             blocking and interrupt-driven data transmission for both single-byte
 *             and multi-byte buffers.
 * @version    1.0
 * @date       2025-12-05
 *
 * @author     Developer: Ahmed Twap (ahmedtwap2@gmail.com)
 * @author     Reviewer: ENG.Bassma Khaled
 * @copyright  Copyright (c) 2025
 */

#ifndef SPI_INTERFACE_H
#define SPI_INTERFACE_H

/*==============================================================================
 *                                   Includes
 *============================================================================*/
#include <stdint.h>
#include "SPI_Private.h"
#include "SPI_Config.h"

/*==============================================================================
 *                              Public API Functions
 *============================================================================*/

/**
 * @brief Initializes the SPI peripheral.
 * @details Configures SPI mode, data order, clock mode, prescaler,
 *          and interrupt settings based on SPI_Config.h.
 */
void mSPI_Init(void);

/**
 * @brief Transmits a single byte through SPI (Blocking).
 * @param data The byte to be transmitted.
 * @return The received byte during the transfer.
 * @note This function waits until transmission is complete.
 */
uint8_t mSPI_TransmitByte(uint8_t data);

/**
 * @brief Transmits and receives a buffer over SPI (Blocking).
 * @param txBuffer Pointer to the transmit buffer.
 * @param rxBuffer Pointer to the receive buffer (stores received data).
 * @param length   Number of bytes to transfer.
 * @note Both txBuffer and rxBuffer must be valid memory locations.
 */
void mSPI_TransmitBuffer(uint8_t* txBuffer, uint8_t* rxBuffer, uint16_t length);

/**
 * @brief Transmits a single byte via SPI interrupt mode.
 * @param data The byte to be transmitted.
 * @param callback Function pointer executed when transmission completes.
 * @warning ISR(SPI_STC_vect) must be implemented in the program file.
 */
void mSPI_TransmitByte_IT(uint8_t data, void (*callback)(uint8_t rxData));

/**
 * @brief Transmits and receives a buffer using SPI interrupt mode.
 * @param txBuffer Pointer to the transmit buffer.
 * @param rxBuffer Pointer to the receive buffer.
 * @param length   Number of bytes to transfer.
 * @param callback Function pointer called when full buffer transfer completes.
 * @warning Ensure SPI_INTERRUPT is set to Enable in SPI_Config.h.
 */
void mSPI_TransmitBuffer_IT(uint8_t* txBuffer, uint8_t* rxBuffer, uint16_t length, void (*callback)(void));

/**
 * @brief Enables the SPI peripheral.
 * @note This sets the SPE bit in SPCR.
 */
void mSPI_Enable(void);

/**
 * @brief Disables the SPI peripheral.
 * @note This clears the SPE bit in SPCR.
 */
void mSPI_Disable(void);

#endif /* SPI_INTERFACE_H */
