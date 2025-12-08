/**
 * @file       TWI_Program.c
 * @brief      This file contains Program for Master/Slave (I2C) TWI driver.   
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer  : Eng/Basma
 * @date       2025-12-3 --> 2025-12-8
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
*/

#include <stdint.h>
#include <string.h>         
#include <avr/interrupt.h>  
#include "../../Common/Macros.h"
#include "../../Common/Config.h"
#include "../Atmega32RegistersAddress.h"
#include "TWI_Interface.h"  
#include "TWI_Config.h"
#include "TWI_Private.h"

/* ==================================================================================== */
/*                                Block(1)  Buffers / State                             */
/* ==================================================================================== */
/**
 * @defgroup    Block(1):Buffer&State
 * @brief       It is Important for:
 *                  1- Stores Data in  a buffer either master or slave.
 *                  2- Stores Twi State (Bus State) That the ISR will take Action on it.
*/

/*Buffer stores The Data that the Master will Send */
uint8_t  MasterTx_Data[TWI_Master_Tx_BufferSize];
uint8_t  MasterTx_Len=0;
uint8_t  MasterTx_Index=0;

/*Buffer stores The Data that the Master will Read\Recieve */
uint8_t  MasterRx_Data[TWI_Master_Rx_BufferSize];
uint8_t  MasterRx_Len=0;
uint8_t  MasterRx_Index=0;

/*Buffer stores The Data that the Slave will Recieve\Read */
uint8_t  SlaveRx_Data[TWI_Slave_Rx_BufferSize];
uint8_t  SlaveRx_Len=0;

/*Buffer stores The Data that the Slave will Send*/
uint8_t  SlaveTx_Data[TWI_Slave_Tx_BufferSize];
uint8_t  SlaveTx_Len=0;
uint8_t  SlaveTx_Index=0;

/*Gives The Intiale State is IDLE in the bus (Very Important Variable as The ISR Will Take Action on it) */
uint8_t TWI_State=TWI_STATE_IDLE;

/*Call back functions used to make the user to handle the data that slave will send or recieve*/
void (*slaveRxCallback)(uint8_t *data, uint8_t len)=0;
uint8_t (*slaveTxProvider)(uint8_t *buffer, uint8_t maxLen)=0;

/*Stores the 7_bit Address*/
static uint8_t master_addr7=0;
/*Stores State of the master Data Mode (write\Read) */
static uint8_t master_is_read=0;

/* ==================================================================================== */
/*                               Block(2) Helper Functions                              */
/* ==================================================================================== */
/**
 * @defgroup    Block(2): Helper Functions
 * @brief       It is Important for reducing the number of commands repeat.
 *                 
*/

/*This Function is used to reflect the status of the TWI logic and the Two-wire Serial Bus */
/* The application designer should mask the prescaler bits to zero when checking the Status bits. 
This makes status checking independent of prescaler setting. (Page:176 DataSheet Approach)*/
static uint8_t twi_status(void){
    return (TWSR_Reg & 0xF8); 
}

/*Page 175 DataSheet(Function used to clear flag after every single operation)*/
static void TWI_ClearTWINT(void){
    SetBit(TWCR_Reg,TWINT);
}
/*Page 176 DataSheet(Function used to enable Twi module)*/
static void TWI_Enable(void){
    SetBit(TWCR_Reg,TWEN);
}
/*Page 175 DataSheet(Function used to enable Acknowledge pulse)*/
static void TWI_EnableACK(void){
    SetBit(TWCR_Reg,TWEA);
}
/*Page 175 DataSheet(Function used to Disable Acknowledge pulse)*/
static void TWI_DisableACK(void){
    ClearBit(TWCR_Reg,TWEA);
}
/*Page 175 DataSheet(Function used to enable Interrupts for ISR action)*/
static void TWI_EnableInterrupt(void){
    SetBit(TWCR_Reg,TWIE);
}
/*Page 175 DataSheet(Function used to Disable Interrupts)*/
static void TWI_DisableInterrupt(void){
    ClearBit(TWCR_Reg,TWIE);
}

/*Page 175 DataSheet(Very importatnt Function used to Start Condition in Master mode)*/
static void TWI_SendSTART(void){
    ClearBit(TWCR_Reg,TWSTO); // make sure stop cond. is cleared from the bus.
    SetBit(TWCR_Reg,TWSTA); // Start condition bit
    TWI_Enable();
    TWI_EnableInterrupt();
    TWI_EnableACK();
    
}

/*Page 176 DataSheet( Function used to Stop Condition in Master mode)*/
static void TWI_SendSTOP(void){
    SetBit(TWCR_Reg,TWSTO); 
    TWI_ClearTWINT();
    TWI_Enable();
}
/*( Function used to make device in listen mode (Slave mode) waits if slave address is in bus and prepare for ack)*/
/*(Used after Arbitration Lost or Making Slave ready to recieve new data)*/
static void TWI_ListenWithACK(void){
    TWI_ClearTWINT();
    TWI_Enable();
    TWI_EnableACK();
    
    TWI_EnableInterrupt();
}



/* ==================================================================================== */
/*                      Block(3) Initialization                                         */
/* ==================================================================================== */

void mTWI_Init(void) {
    /*Bit Rate Caclulation Page 173 DataSheet*/
    uint32_t twbr_calc = 0;
    if (TWI_Bit_Rate != 0) {
        twbr_calc = ((F_CPU / TWI_Bit_Rate) - 16UL) / 2UL;
        if (twbr_calc > 255) twbr_calc = 255;
    }
    TWBR_Reg = (uint8_t)twbr_calc;

    TWSR_Reg =TWSR_Reg & 0xFC; //(And Masking To make sure Prescalar bits are zero) 0b1111 1100 last two prescalar bits  are 0 (Prescalar Value 1)

    /*Resets all internal buffers*/
    MasterTx_Len = 0;
    MasterTx_Index = 0;
    MasterRx_Len = 0;
    MasterRx_Index = 0;
    SlaveRx_Len = 0;
    SlaveTx_Len = 0;
    SlaveTx_Index = 0;
    TWI_State = TWI_STATE_IDLE;
    master_addr7 = 0;
    master_is_read = 0;

    /*Clear TWCR_Reg bits*/
    ClearBit(TWCR_Reg, TWSTA);
    ClearBit(TWCR_Reg, TWSTO);
    ClearBit(TWCR_Reg, TWIE); 
    ClearBit(TWCR_Reg, TWEA); 
    ClearBit(TWCR_Reg, TWINT); 

    /* Enable TWI operation , Enable TWI interrupt (TWIE), Enable ACK (TWEA) for Slave reception.*/
    SetBit(TWCR_Reg, TWEN);
    SetBit(TWCR_Reg, TWIE);
    SetBit(TWCR_Reg, TWINT);
    if (TWI_Enable_ACK){
        SetBit(TWCR_Reg, TWEA);
    }
}
/* ==================================================================================== */
/*                                Block(4) Master APIs                                  */
/* ==================================================================================== */

uint8_t mTWI_MasterSendRequest(uint8_t addr, const uint8_t *data, uint8_t len) {
    // Check if length is valid (not zero and not bigger than TX buffer)
    if (len == 0 || len > TWI_Master_Tx_BufferSize)
        return 0;
    // Check if TWI is already busy (sending or receiving)   
    if (TWI_State==TWI_STATE_MASTER_TX || TWI_State==TWI_STATE_MASTER_RX )
        return 0;
    // Copy the data byte-by-byte into the internal TX buffer
    for (uint8_t i=0; i<len ;i++)
    {
        MasterTx_Data[i]=data[i];
    }
    // Save the length and reset the index to the first byte
    MasterTx_Len=len;
    MasterTx_Index=0;
    // Store the 7-bit slave address (mask out any extra bits)
    master_addr7=addr & 0x7F;
    master_is_read=0; // This is a WRITE operation (not read)

    // Change state to Master send
    TWI_State=TWI_STATE_MASTER_TX;

    // Send the START condition to begin communication
    TWI_SendSTART();

    return 1;

    
}

uint8_t mTWI_MasterReadResponse(uint8_t addr, uint8_t len) {
    // Check if requested length is valid
    if (len == 0 || len > TWI_Master_Rx_BufferSize) 
        return 0;
    // Check if TWI is already busy
    if (TWI_State == TWI_STATE_MASTER_TX || TWI_State == TWI_STATE_MASTER_RX) 
        return 0;

    // Save the number of bytes we expect to receive
    MasterRx_Len = len;
    MasterRx_Index = 0;
    // Store the 7-bit slave address
    master_addr7 = addr & 0x7F;

    // This is a Read operation
    master_is_read = 1;
    // Change state to Master Receive
    TWI_State = TWI_STATE_MASTER_RX;
    // Send START condition to begin reading
    TWI_SendSTART();

    return 1;
}

/* ==================================================================================== */
/*                                Block(5) Slave  APIs                                  */
/* ==================================================================================== */
void mTWI_EnableSlave(uint8_t slaveAddr) {
    
    // Mask to 7-bit address and shift left because TWAR stores address in bits [7:1]
    uint8_t addr_reg = (slaveAddr & 0x7F) << 1 ;

    // If general call is enabled, set the TWGCE bit (bit0 in TWAR) (Not enabled in this Configuration)
    if (TWI_General_Call_Enable){
        SetBit(TWAR_Reg,TWGCE);
    }
    // Assigning the prepared address into the TWAR Register
    TWAR_Reg=addr_reg;

    ClearBit(TWCR_Reg,TWSTA); // Make sure that  start bit is Cleared (Slave must not send Start Cond.)
    ClearBit(TWCR_Reg,TWSTO); // Make sure that  stop bit is Cleared (Slave must not send Stop Cond.)
    ClearBit(TWCR_Reg,TWIE);  // Disable interrupt temporary.
    ClearBit(TWCR_Reg,TWINT); // Clear interrupt flag


    SetBit(TWCR_Reg,TWEN); // Start Twi module
    SetBit(TWCR_Reg,TWIE); // enable Interrupt so slave can respond when addressing it
    SetBit(TWCR_Reg,TWINT);// clear flag and start listening


    TWI_EnableACK();
    

    // Driver state is IDLE (slave ready and waiting )
    TWI_State=TWI_STATE_IDLE;


}

void mTWI_DisableSlave(void) {
    TWAR_Reg = 0;// Clear the slave address register (slave will no longer respond to any address)

    SetBit(TWCR_Reg, TWEN);
    SetBit(TWCR_Reg, TWINT);
    ClearBit(TWCR_Reg, TWIE);// No ISR triggering
    ClearBit(TWCR_Reg, TWEA); // Slave will NACK any Recieved byte
}

/* ==================================================================================== */
/*                                   Block(6) Call back                                 */
/* ==================================================================================== */
void mTWI_SetSlaveRxCallback(void (*cb)(uint8_t *data, uint8_t len)){
     slaveRxCallback = cb; // Store the callback function that will be called when the slave receives data
    }
void mTWI_SetSlaveTxProvider(uint8_t (*provider)(uint8_t *buf, uint8_t maxLen)){
     slaveTxProvider = provider;// Store the provider function that will supply data when the master requests a read 
    }

/* ==================================================================================== */
/*                               Block(7) ISR_(StateMachine)                            */
/* ==================================================================================== */
ISR(TWI_vect) {
    uint8_t status = twi_status();

    switch (status) {

        /* MASTER TRANSMITTER (Sending Data as Master) */
        // START or REPEATED START sent
        case TWI_MTx_StartCond_Sent: 
        case TWI_MTx_RepeatStart_Sent:
            // Prepare the I2C address byte: shift 7-bit slave address left and set R/W bit (0 = write, 1 = read)
            TWDR_Reg = (uint8_t)((master_addr7 << 1) | (master_is_read ? 1 : 0 ));

            TWI_ClearTWINT(); 
            TWI_Enable(); 
            TWI_EnableInterrupt();
            break;
        // SLA+W transmitted, ACK received
        case TWI_MTx_SLAW_ACK_Recieved:
            MasterTx_Index = 0;
            if (MasterTx_Len > 0) {
                TWDR_Reg = MasterTx_Data[MasterTx_Index++];
                TWI_ClearTWINT(); 
                TWI_Enable(); 
                TWI_EnableInterrupt();
            } else {
                TWI_SendSTOP();
                MasterTx_Len = 0;
                TWI_State = TWI_STATE_IDLE;
            }
            break;
        // SLA+W NACK OR data byte NACK    
        case TWI_MTx_SLAW_NACK_Recieved: 
        case TWI_MTx_Write_DataByte_NACK_Recieved:
            TWI_SendSTOP();
            MasterTx_Len = 0;
            TWI_State = TWI_STATE_IDLE;
            break;
        // Data byte ACK received (send next)
        case TWI_MTx_Write_DataByte_ACK_Recieved:
            if (MasterTx_Index < MasterTx_Len) {
                TWDR_Reg = MasterTx_Data[MasterTx_Index++];
                TWI_ClearTWINT(); 
                TWI_Enable(); 
                TWI_EnableInterrupt();
            } else {
                TWI_SendSTOP();
                MasterTx_Len = 0;
                TWI_State = TWI_STATE_IDLE;
            }
            break;
        // Arbitration lost
        case TWI_MTx_ARB_Lost:
            MasterTx_Len = 0;
            MasterRx_Len = 0;
            TWI_State = TWI_STATE_IDLE;
            TWI_ListenWithACK();
            break;

        /* MASTER RECEIVER (Receiving Data as Master)  */
        // SLA+R ACK received --> prepare to receive bytes
        case TWI_MRx_SLAR_ACK_Recieved:
            MasterRx_Index = 0;
            if (MasterRx_Len > 1) {
                SetBit(TWCR_Reg, TWEA);}
            else{ 
                ClearBit(TWCR_Reg, TWEA);
            }
            TWI_ClearTWINT(); 
            TWI_Enable(); 
            TWI_EnableInterrupt();
            break;
        // SLA+R NACK --> device not responding
        case TWI_MRx_SLAR_NACK_Recieved :
            MasterRx_Len = 0;
            TWI_State = TWI_STATE_IDLE;
            TWI_SendSTOP();
            break;
        // Data byte received (ACK or NACK)
        case TWI_MRx_Read_DataByte_ACK_Recieved: 
        case TWI_MRx_Read_DataByte_NACK_Recieved:
        {
            uint8_t receivedByte = TWDR_Reg;
            MasterRx_Data[MasterRx_Index++] = receivedByte;

            if (status == TWI_MRx_Read_DataByte_NACK_Recieved) {
                MasterRx_Len = MasterRx_Index;
                MasterRx_Index = 0;
                TWI_State = TWI_STATE_IDLE;
                TWI_SendSTOP();
            } else {
                if (MasterRx_Index < (MasterRx_Len - 1)){
                    SetBit(TWCR_Reg, TWEA);}  
                else {
                    ClearBit(TWCR_Reg, TWEA);}
                TWI_ClearTWINT();
                TWI_Enable();
                TWI_EnableInterrupt();
            }
            break;
        }

        /*SLAVE RECEIVER (Slave being written to by Master)*/
        // Master sent SLA+W and ACK received --> slave ready to receive
        case TWI_SRx_SLAW_ACK_Recieved: 
        case TWI_SRx_ARB_Lost_SLAW_R_ACK_Recieved:
            SlaveRx_Len = 0;
            TWI_ClearTWINT(); 
            TWI_Enable(); 
            TWI_EnableACK();  
            TWI_EnableInterrupt();
            TWI_State = TWI_STATE_SLAVE_RX;
            break;
        // Receiving data byte (ACK)
        case TWI_SRx_Read_DataByte_ACK_Recieved: 
        case TWI_SRx_GeneralCall_DataRecieved_ACK_Recieved:
            SlaveRx_Data[SlaveRx_Len++] = TWDR_Reg;
            TWI_ClearTWINT(); 
            TWI_Enable(); 
            TWI_EnableACK();
            TWI_EnableInterrupt();
            break;
        // Receiving last byte (NACK)
        case TWI_SRx_Read_DataByte_NACK_Recieved:
            SlaveRx_Data[SlaveRx_Len++] = TWDR_Reg;
            TWI_ClearTWINT();
            TWI_Enable();
            TWI_EnableACK();
            TWI_EnableInterrupt();
            break;
        // STOP or repeated START --> end of slave RX transfer
        case TWI_SRx_Stop_or_Repeat_Recieved_NoAction:
            if (slaveRxCallback && SlaveRx_Len){
                slaveRxCallback(SlaveRx_Data, SlaveRx_Len);
            }
            SlaveRx_Len = 0;
            TWI_ClearTWINT();
            TWI_Enable();
            TWI_EnableACK();
            TWI_EnableInterrupt();
            TWI_State = TWI_STATE_IDLE;
            break;
        /*SLAVE TRANSMITTER (Master is reading FROM the slave)*/
        // Master sent SLA+R ACK --> prepare data to send        
        case TWI_STx_SLAR_ACK_Recieved:
            SlaveTx_Len = SlaveTx_Index = 0;
            if (slaveTxProvider){
                uint8_t bytesToSend = slaveTxProvider(SlaveTx_Data, TWI_Slave_Tx_BufferSize);
                SlaveTx_Len = bytesToSend;
            }
            TWDR_Reg = (SlaveTx_Len > 0) ? SlaveTx_Data[SlaveTx_Index++] : 0xFF;
            TWI_ClearTWINT();
            TWI_Enable();
            TWI_EnableInterrupt();
            TWI_State = TWI_STATE_SLAVE_TX;
            break;

        // Master ACKs --> send next byte
        case TWI_STx_Write_DataByte_ACK_Recieved:
            TWDR_Reg = (SlaveTx_Index < SlaveTx_Len) ? SlaveTx_Data[SlaveTx_Index++] : 0xFF;
            TWI_ClearTWINT();
            TWI_Enable();
            TWI_EnableInterrupt();
            break;
        // Master done reading --> reset TX state
        case TWI_STx_Write_DataByte_NACK_Recieved: 
        case TWI_STx_Last_DataByteRecieved_ACK_Recieved:
            SlaveTx_Len = SlaveTx_Index = 0;
            TWI_State = TWI_STATE_IDLE;
            TWI_ClearTWINT();
            TWI_Enable();
            TWI_EnableACK();
            TWI_EnableInterrupt();
            break;
        /*DEFAULT — Any undefined state */
        default:
            TWI_ClearTWINT(); 
            TWI_Enable(); 
            TWI_EnableACK();
            TWI_EnableInterrupt();
            TWI_State = TWI_STATE_IDLE;
            break;
    }
}

