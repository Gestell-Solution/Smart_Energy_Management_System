/**
 * @file       TWI_Config.h
 * @brief      This file contains Config for Master/Slave (I2C) TWI driver.
 * @details    Configuration file for TWI driver including Bit rate, Slave address, 
 *             Arbitration, and Buffer sizes settings.
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer  : Eng/Basma
 * @date       2025-12-3
 * @copyright  Copyright (c) 2025 , Gestell Company 
 */

#include <stdint.h>
#include "../Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"

#ifndef   _TWI_CONFIG_H_
#define   _TWI_CONFIG_H_

/**
 * @def F_CPU
 * @brief Defines the CPU frequency used for baud rate calculations.
 */
#define F_CPU 8000000UL



/* ==================================================================================== */
/*                                      BIT Rate                                        */
/* ==================================================================================== */

/**
 * @def TWI_BitRate_Config TWI Bit Rate Configuration
 * @brief Settings for TWI communication speed.
 */

#define TWI_Bit_Rate   100000UL  // e.g (100 KHz or 400KHz)--> 100KHz has been chosen Normal and Suitable speed rate. 

/* ==================================================================================== */
/*                                   Slave Address                                      */
/* ==================================================================================== */

/**
 * @def TWI_Address_Config TWI Slave Address Configuration
 * @brief Settings for the device's own slave address.
 */

#define TWI_Slave_Address  0x50  // Standard 7 bit slave address and Non reserved (0x00 , 1111 xxx) out from TWI reserved 9 slave adresses


/* ==================================================================================== */
/*                                    Multi-Master                                      */
/* ==================================================================================== */

/**
 * @defgroup TWI_MultiMaster_Config TWI Multi-Master Configuration
 * @brief Configuration for Multi-Master arbitration handling.
 */

#define TWI_Enable_MasterArbitration    1   // Atmega32 supports master arbitration (Multi master can talk arbitration happens)

#define TWI_MAX_Master_Retries          3  //  3 retry is suitable and standard to handle arbitration for master loser to waiting until it sends its data packet

/* ==================================================================================== */
/*                                  ACK/General Call                                    */
/* ==================================================================================== */

/**
 * @defgroup TWI_Control_Config TWI Control Configuration
 * @brief Settings for ACK and General Call recognition.
 */

#define TWI_Enable_ACK           1      // important to set ACK for Slave after Recieving data or to Master after Reading Data.
#define TWI_General_Call_Enable  0      // No Broadcasting in this driver uptill now.


/* ==================================================================================== */
/*                                   Buffers Size                                       */
/* ==================================================================================== */

/**
 * @defgroup TWI_Buffer_Config TWI Buffer Configuration
 * @brief Size definitions for TWI Transmit and Receive buffers.
 */

/* Asynchrounus TWI Driver need */

#define TWI_Master_Tx_BufferSize  64

#define TWI_Master_Rx_BufferSize  64

#define TWI_Slave_Tx_BufferSize   64

#define TWI_Slave_Rx_BufferSize   64



#endif /*_TWI_CONFIG_H_*/