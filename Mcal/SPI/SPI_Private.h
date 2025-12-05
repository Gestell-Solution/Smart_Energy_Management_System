/**
 * @file       SPI_Private.h
 * @brief      Private definitions and internal macros for the SPI driver.
 * @details    This file contains register definitions, bit positions, enumerations, 
 *             and internal configurations used by the SPI driver. 
 *             These elements are not exposed to the user and should only be accessed
 *             by the SPI driver implementation.
 * @version    1.0
 * @date       2025-12-05
 *
 * @author     Developer: Ahmed Twap (ahmedtwap2@gmail.com)
 * @author     Reviewer: ENG.Bassma Khaled
 * @copyright  Copyright (c) 2025, Gestell
 */

#ifndef SPI_PRIVATE_H
#define SPI_PRIVATE_H

/*==============================================================================
 *                               Includes
 *============================================================================*/
#include <stdint.h>
#include "../../Common/Macros.h"
#include "../../Mcal/Atmega32RegistersAddress.h"

/*==============================================================================
 *                          SPI Register Definitions
 *============================================================================*/

/**
 * @brief SPI Control Register.
 */
#define SPCR_REG   (*(volatile uint8_t*)0x2D)

/**
 * @brief SPI Status Register.
 */
#define SPSR_REG   (*(volatile uint8_t*)0x2E)

/**
 * @brief SPI Data Register.
 */
#define SPDR_REG   (*(volatile uint8_t*)0x2F)

/*==============================================================================
 *                   SPI Control Register Bit Definitions
 *============================================================================*/

/**
 * @enum SPCR_Bits_t
 * @brief Bit positions for the SPI Control Register (SPCR).
 */
typedef enum
{
    SPCR0_Bit0 = 0,   /**< Clock rate bit 0. */
    SPCR1_Bit1,       /**< Clock rate bit 1. */
    CPHA_Bit2,        /**< Clock phase select. */
    CPOL_Bit3,        /**< Clock polarity select. */
    MSTR_Bit4,        /**< Master/Slave mode select. */
    DORD_Bit5,        /**< Data order (MSB/LSB first). */
    SPE_Bit6,         /**< SPI enable. */
    SPIE_Bit7         /**< SPI interrupt enable. */
} SPCR_Bits_t;

/*==============================================================================
 *                   SPI Status Register Bit Definitions
 *============================================================================*/

/**
 * @enum SPSR_Bits_t
 * @brief Bit positions for the SPI Status Register (SPSR).
 */
typedef enum
{
    SPI2x_Bit0 = 0,  /**< Double SPI speed. */
    WCOL_Bit6 = 6,   /**< Write collision flag. */
    SPIF_Bit7 = 7    /**< SPI interrupt flag. */
} SPSR_Bits_t;

/*==============================================================================
 *                        SPI Configuration Enumerations
 *============================================================================*/

/**
 * @enum SPI_Mode_t
 * @brief Selects SPI operating mode (Master/Slave).
 */
typedef enum
{
    Slave_Mode,    /**< SPI operates as Slave. */
    Master_Mode    /**< SPI operates as Master. */
} SPI_Mode_t;

/**
 * @enum SPI_DataOrder_t
 * @brief Selects MSB-first or LSB-first data order.
 */
typedef enum
{
    MSB_First = 0, /**< Most Significant Bit first. */
    LSB_First = 1  /**< Least Significant Bit first. */
} SPI_DataOrder_t;

/**
 * @enum SPI_ClockMode_t
 * @brief Clock polarity and phase configuration (SPI Modes 0–3).
 */
typedef enum
{
    SPI_Mode0 = 0, /**< CPOL=0, CPHA=0 (Idle Low, Sample Leading). */
    SPI_Mode1,     /**< CPOL=0, CPHA=1 (Idle Low, Sample Trailing). */
    SPI_Mode2,     /**< CPOL=1, CPHA=0 (Idle High, Sample Leading). */
    SPI_Mode3      /**< CPOL=1, CPHA=1 (Idle High, Sample Trailing). */
} SPI_ClockMode_t;

/**
 * @enum SPI_ClockRate_t
 * @brief Clock prescaler selection.
 */
typedef enum
{
    F_CPU_4 = 0,
    F_CPU_16,
    F_CPU_64,
    F_CPU_128,
    F_CPU_2,
    F_CPU_8,
    F_CPU_32
} SPI_ClockRate_t;

/**
 * @enum SPI_Interrupt_t
 * @brief Enable or disable SPI interrupt functionality.
 */
typedef enum
{
    Disable_SPI_Interrupt = 0,  /**< Disable interrupt. */
    Enable_SPI_Interrupt  = 1   /**< Enable interrupt. */
} SPI_Interrupt_t;

/*==============================================================================
 *                              SPI Pin Definitions
 *============================================================================*/

/**
 * @brief SPI pin mapping (as per ATmega32 hardware).
 */
#define SS_GROUP     GroupA
#define MOSI_GROUP   GroupA
#define MISO_GROUP   GroupA
#define SCK_GROUP    GroupA

#define SS_PIN       PIN4
#define MOSI_PIN     PIN5
#define MISO_PIN     PIN6
#define SCK_PIN      PIN7

/*==============================================================================
 *                              SPI State Machine
 *============================================================================*/

/**
 * @enum SPI_State_t
 * @brief Represents the internal SPI driver state.
 */
typedef enum
{
    SPI_State_Idle = 0, /**< No transmission is active. */
    SPI_State_Busy      /**< SPI is currently transmitting or receiving. */
} SPI_State_t;

#endif /* SPI_PRIVATE_H */
