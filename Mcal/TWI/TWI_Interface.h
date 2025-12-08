/**
 * @file       TWI_Interface.h
 * @brief      This file contains Interface for Master/Slave (I2C) TWI driver.   
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer  : Eng/Basma
 * @date       2025-12-3
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#include <stdint.h>
#include "../Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"

#ifndef   _TWI_INTERFACE_H_
#define   _TWI_INTERFACE_H_

/* ==================================================================================== */
/* ==============================      Initialization       ============================ */
/* ==================================================================================== */

/**
 * @fn      mTWI_Init
 * @brief   Initializes the TWI (I2C) peripheral as Master and/or Slave.
 * @details This function performs the following tasks:
 *          1. Sets the TWI bit rate according to the configuration macro `TWI_Bit_Rate`.
 *          2. Resets all internal buffers (Master Tx/Rx, Slave Tx/Rx) to initial state.
 *          3. Sets the TWI peripheral control register (TWCR) to:
 *             - Enable TWI operation.
 *             - Enable TWI interrupt (TWIE).
 *             - Enable ACK (TWEA) for Slave reception.
 *          4. Sets the initial driver state to IDLE.
 *          5. Prepares the device to operate as Master and/or Slave according to configuration.
 *
 * @note    This function should be called once at system startup before any TWI operation.
 *          The actual data transmission/reception is handled asynchronously via ISR.
 *
 * @param   None
 * @return  void
 */
void mTWI_Init(void);


/* ==================================================================================== */
/* ==============================      Master APIs       ============================== */
/* ==================================================================================== */

/**
 * @fn mTWI_MasterSendRequest
 * @brief  Sends a write request to a Slave device.
 * @details This function copies the provided data to the internal Tx buffer. 
 * Transmission happens asynchronously via ISR. 
 * If the buffer is full, the request will fail.
 * @param addr The 7-bit I2C address of the slave device.
 * @param data Pointer to the data array to be sent.
 * @param len  Number of bytes to send (0–255).
 * @return uint8_t  (1 = request accepted, 0 = buffer full / invalid)
 */
uint8_t mTWI_MasterSendRequest(uint8_t addr, const uint8_t *data, uint8_t len);


/**
 * @fn mTWI_MasterReadResponse
 * @brief  Requests to read data from a Slave device asynchronously.
 * @details The received data will be stored in the internal Rx buffer. After the 
 * read operation is complete (via ISR), the buffer will contain the result.
 * @param addr The 7-bit I2C address of the slave device.
 * @param len  Number of bytes to read (0–255).
 * @return uint8_t  (1 = request accepted, 0 = buffer full / invalid)
 */
uint8_t mTWI_MasterReadResponse(uint8_t addr, uint8_t len);


/* ==================================================================================== */
/* ==============================       Slave APIs       ============================== */
/* ==================================================================================== */

/**
 * @fn mTWI_EnableSlave
 * @brief  Enables this device to operate as a Slave.
 * @param  slaveAddr The 7-bit address for this device.
 * @return void
 */
void mTWI_EnableSlave(uint8_t slaveAddr);


/**
 * @fn mTWI_DisableSlave
 * @brief  Disables Slave operation.
 * @return void
 */
void mTWI_DisableSlave(void);


/* ==================================================================================== */
/* ==============================  Buffer/Callback APIs  ============================== */
/* ==================================================================================== */

/**
 * @fn mTWI_SetSlaveRxCallback
 * @brief Sets the callback executed when the slave receives data (SLA+W).
 * @param cb Pointer to a function: void myCallback(uint8_t *data, uint8_t len)
 */
void mTWI_SetSlaveRxCallback(void (*cb)(uint8_t *data, uint8_t len));


/**
 * @fn mTWI_SetSlaveTxProvider
 * @brief Sets the provider function called when a Master reads from the Slave (SLA+R).
 * @param provider Pointer to a function returning data length (0–255):
 *        uint8_t myProvider(uint8_t *buf, uint8_t maxLen)
 */
void mTWI_SetSlaveTxProvider(uint8_t (*provider)(uint8_t *buf, uint8_t maxLen));

#endif /*_TWI_INTERFACE_H_*/
