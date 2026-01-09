/**
 * @file       TWI_Private.h
 * @brief      This file contains Private for Master/Slave (I2C) TWI driver.
 * @details    Private definitions including Status Codes, Bit definitions, 
 *             Driver states, and external buffer declarations.
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer  : Eng/Basma
 * @date       2025-12-3
 * @copyright  Copyright (c) 2025 , Gestell Company 
 */

#include <stdint.h>
#include "../Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"
#include "TWI_Config.h"

#ifndef   _TWI_PRIVATE_H_
#define   _TWI_PRIVATE_H_


/* ==================================================================================== */
/*                       Status Codes                                                   */
/* ==================================================================================== */

/**
 * @defgroup TWI_Status_Codes TWI Status Codes
 * @brief Status codes returned by the TWI hardware status register (TWSR).
 */

/* MASTER Tx */
#define TWI_MTx_StartCond_Sent                               0x08
#define TWI_MTx_RepeatStart_Sent                             0x10
#define TWI_MTx_SLAW_ACK_Recieved                            0x18
#define TWI_MTx_SLAW_NACK_Recieved                           0x20
#define TWI_MTx_Write_DataByte_ACK_Recieved                  0x28
#define TWI_MTx_Write_DataByte_NACK_Recieved                 0x30
#define TWI_MTx_ARB_Lost                                     0x38
              
/* MASTER Rx */               
#define TWI_MRx_SLAR_ACK_Recieved                            0x40
#define TWI_MRx_SLAR_NACK_Recieved                           0x48
#define TWI_MRx_Read_DataByte_ACK_Recieved                   0x50
#define TWI_MRx_Read_DataByte_NACK_Recieved                  0x58

/* SLAVE Rx */
#define TWI_SRx_SLAW_ACK_Recieved                            0x60
#define TWI_SRx_ARB_Lost_SLAW_R_ACK_Recieved                 0x68
#define TWI_SRx_Read_DataByte_ACK_Recieved                   0x80
#define TWI_SRx_Read_DataByte_NACK_Recieved                  0x88
#define TWI_SRx_GeneralCall_DataRecieved_ACK_Recieved        0x90
#define TWI_SRx_Stop_or_Repeat_Recieved_NoAction             0xA0

/* SLAVE Tx */
#define TWI_STx_SLAR_ACK_Recieved                            0xA8
#define TWI_STx_Write_DataByte_ACK_Recieved                  0xB8
#define TWI_STx_Write_DataByte_NACK_Recieved                 0xC0
#define TWI_STx_Last_DataByteRecieved_ACK_Recieved           0xC8



/* ====================================================================
 * BIT DEFINITIONS
 * ==================================================================== */

/**
 * @defgroup TWI_Bit_Definitions TWI Bit Definitions
 * @brief Bit positions for TWI control registers (TWCR, TWAR).
 */

#define TWIE    0
#define TWEN    2
#define TWWC    3
#define TWSTO   4
#define TWSTA   5
#define TWEA    6
#define TWINT   7

#define TWGCE   0  /* Bit 0 in TWAR register enables General Call Recognition */


/* ==================================================================================== */
/*                                     Buffers                                          */
/* ==================================================================================== */

/**
 * @defgroup TWI_Buffers TWI Buffers
 * @brief External buffer declarations for TWI transmission and reception.
 */

extern uint8_t  MasterTx_Data[TWI_Master_Tx_BufferSize];
extern uint8_t  MasterTx_Len;
extern uint8_t  MasterTx_Index;

extern uint8_t  MasterRx_Data[TWI_Master_Rx_BufferSize];
extern uint8_t  MasterRx_Len;
extern uint8_t  MasterRx_Index;

extern uint8_t  SlaveRx_Data[TWI_Slave_Rx_BufferSize];
extern uint8_t  SlaveRx_Len;

extern uint8_t  SlaveTx_Data[TWI_Slave_Tx_BufferSize];
extern uint8_t  SlaveTx_Len;
extern uint8_t  SlaveTx_Index;


/* ==================================================================================== */
/*                                   Driver State                                       */
/* ==================================================================================== */

/**
 * @defgroup TWI_Driver_State TWI Driver State
 * @brief Internal state machine definitions for the TWI driver.
 * @{
 */

#define TWI_STATE_IDLE        0
#define TWI_STATE_MASTER_TX   1
#define TWI_STATE_MASTER_RX   2
#define TWI_STATE_SLAVE_RX    3
#define TWI_STATE_SLAVE_TX    4

extern uint8_t TWI_State;

/* ==================================================================================== */
/*                                  Callbacks                                           */
/* ==================================================================================== */

/**
 * @defgroup TWI_Callbacks TWI Callbacks
 * @brief Function pointers for Slave operations (RX/TX).
 */

extern void (*slaveRxCallback)(uint8_t *data, uint8_t len);
extern uint8_t (*slaveTxProvider)(uint8_t *buffer, uint8_t maxLen);

#endif /*_TWI_PRIVATE_H_*/