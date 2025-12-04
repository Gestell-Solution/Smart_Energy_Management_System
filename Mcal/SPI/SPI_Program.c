#include <stdint.h>
#include "../Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"
#include "../../Common/Config.h"
#include "../../MCAL/DIO/DIO_Interface.h"
    
#include "SPI_Private.h"
#include "SPI_Interface.h"
#include <avr/interrupt.h>  
#include "SPI_Config.h"
//------------------------------------------------------------------------------------------



// Static Variables for Interrupt Handling
static volatile uint8_t SPI_TxByte;
static volatile uint8_t SPI_RxByte;
static void (*SPI_ByteCallback)(uint8_t) = Null;

static volatile uint8_t* SPI_TxBufferPtr = Null;
static volatile uint8_t* SPI_RxBufferPtr = Null;
static volatile uint16_t SPI_BufferLength = 0;
static volatile uint16_t SPI_BufferIndex = 0;
static void (*SPI_BufferCallback)(void) = Null;
static SPI_State_t SPI_State = SPI_State_Idle;

//------------------------------------------------------------------------------------------



void mSPI_Init(void)
{
    // ----------------------
    // Master / Slave
    // ----------------------
    if (SPI_MODE == Master_Mode)
    {
        SetBit(SPCR_REG, MSTR_Bit4); // Master mode

        // Configure pins for Master
        mDIO_SetDirectionForPin(MOSI_GROUP, MOSI_PIN, Output);
        mDIO_SetDirectionForPin(MISO_GROUP, MISO_PIN, Input);
        mDIO_SetDirectionForPin(SCK_GROUP, SCK_PIN, Output);
        mDIO_SetDirectionForPin(SS_GROUP, SS_PIN, Output);
        mDIO_WritePin(SS_GROUP, SS_PIN, Low); // Set SS Pin Low


    }
    else if (SPI_MODE == Slave_Mode)
    {
        ClearBit(SPCR_REG, MSTR_Bit4); // Slave mode

        // Configure pins for Slave
        mDIO_SetDirectionForPin(MOSI_GROUP, MOSI_PIN, Input);
        mDIO_SetDirectionForPin(MISO_GROUP, MISO_PIN, Output);
        mDIO_SetDirectionForPin(SCK_GROUP, SCK_PIN, Input);
        mDIO_SetDirectionForPin(SS_GROUP, SS_PIN, Input);
    }

    // ----------------------
    // Data Order
    // ----------------------
    if (SPI_DATA_ORDER == LSB_First)
    {
        SetBit(SPCR_REG, DORD_Bit5);
    }
    else if (SPI_DATA_ORDER == MSB_First)
    {
        ClearBit(SPCR_REG, DORD_Bit5);
    }
    // ----------------------
    // Clock Mode (CPOL and CPHA)
    // ----------------------
    if (SPI_CLOCK_MODE == SPI_Mode0)    
    {
        ClearBit(SPCR_REG, CPOL_Bit3);
        ClearBit(SPCR_REG, CPHA_Bit2);
    }
    else if (SPI_CLOCK_MODE == SPI_Mode1)
    {
        ClearBit(SPCR_REG, CPOL_Bit3);
        SetBit(SPCR_REG, CPHA_Bit2);
    }
    else if (SPI_CLOCK_MODE == SPI_Mode2)
    {
        SetBit(SPCR_REG, CPOL_Bit3);
        ClearBit(SPCR_REG, CPHA_Bit2);
    }
    else if (SPI_CLOCK_MODE == SPI_Mode3)
    {
        SetBit(SPCR_REG, CPOL_Bit3);
        SetBit(SPCR_REG, CPHA_Bit2);
    }

    // ----------------------
    // Clock Prescaler
    // ----------------------
    switch(SPI_PRESCALER)
    {
        case F_CPU_4:
            ClearBit(SPCR_REG, SPCR0_Bit0);
            ClearBit(SPCR_REG, SPCR1_Bit1);
            ClearBit(SPSR_REG, SPI2x_Bit0);
            break;

        case F_CPU_16:
            SetBit(SPCR_REG, SPCR0_Bit0);
            ClearBit(SPCR_REG, SPCR1_Bit1);
            ClearBit(SPSR_REG, SPI2x_Bit0);
            break;

        case F_CPU_64:
            ClearBit(SPCR_REG, SPCR0_Bit0);
            SetBit(SPCR_REG, SPCR1_Bit1);
            ClearBit(SPSR_REG, SPI2x_Bit0);
            break;

        case F_CPU_128:
            SetBit(SPCR_REG, SPCR0_Bit0);
            SetBit(SPCR_REG, SPCR1_Bit1);
            ClearBit(SPSR_REG, SPI2x_Bit0);
            break;

        case F_CPU_2:
            ClearBit(SPCR_REG, SPCR0_Bit0);
            ClearBit(SPCR_REG, SPCR1_Bit1);
            SetBit(SPSR_REG, SPI2x_Bit0);
            break;

        case F_CPU_8:
            SetBit(SPCR_REG, SPCR0_Bit0);
            ClearBit(SPCR_REG, SPCR1_Bit1);
            SetBit(SPSR_REG, SPI2x_Bit0);
            break;

        case F_CPU_32:
            ClearBit(SPCR_REG, SPCR0_Bit0);
            SetBit(SPCR_REG, SPCR1_Bit1);
            SetBit(SPSR_REG, SPI2x_Bit0);
            break;
    }

    // ----------------------
    // SPI Interrupt
    // ----------------------
    #if SPI_INTERRUPT == Enable
        SetBit(SPCR_REG, SPIE_Bit7);
    #elif SPI_INTERRUPT == Disable
            ClearBit(SPCR_REG, SPIE_Bit7);
    #endif
    // ----------------------
    
}
//------------------------------------------------------------------------------------------
// Transmit Byte (Blocking)
uint8_t mSPI_TransmitByte(uint8_t data)
{
    SPDR_REG =data;// Start transmission

    while(GetBit(SPSR_REG, SPIF_Bit7) == 0);
    return SPDR_REG;// Return received byte
}

//------------------------------------------------------------------------------------------
// Transmit Buffer (Blocking)
void mSPI_TransmitBuffer(uint8_t* txBuffer, uint8_t* rxBuffer, uint16_t length)
{
    for(uint16_t i = 0; i < length; i++)
    {
        SPDR_REG = txBuffer[i];

        while(GetBit(SPSR_REG, SPIF_Bit7) == 0);

        if(rxBuffer !=Null) 
        {
            rxBuffer[i] = SPDR_REG;
        }
    }


}
//------------------------------------------------------------------------------------------
// Transmit Byte using Interrupt
void mSPI_TransmitByte_IT(uint8_t data, void (*callback)(uint8_t rxData))
{
    // Check if SPI is idle
    if(SPI_State == SPI_State_Idle)
    {
        SPI_State = SPI_State_Busy;
        SPI_TxByte = data;
        SPI_ByteCallback = callback;

        // Start transmission
        SPDR_REG = SPI_TxByte;
    }

}
//------------------------------------------------------------------------------------------
// Transmit Buffer using Interrupt
void mSPI_TransmitBuffer_IT(uint8_t* txBuffer, uint8_t* rxBuffer, uint16_t length, void (*callback)(void))
{
    // Check if SPI is idle
    if(SPI_State == SPI_State_Idle)
    {
        SPI_State = SPI_State_Busy;
        SPI_TxBufferPtr = txBuffer;
        SPI_RxBufferPtr = rxBuffer;
        SPI_BufferLength = length;
        SPI_BufferIndex = 0;
        SPI_BufferCallback = callback;

        // Start transmission
        SPDR_REG = SPI_TxBufferPtr[SPI_BufferIndex];
    }



}
//------------------------------------------------------------------------------------------
// Enable and Disable SPI Peripheral
void mSPI_Enable(void)
{
    SetBit(SPCR_REG, SPE_Bit6);
}
void mSPI_Disable(void)
{
    ClearBit(SPCR_REG, SPE_Bit6);

}

//------------------------------------------------------------------------------------------


// SPI Interrupt Service Routine
//void __vector_12(void) __attribute__((signal));
//void __vector_12(void)
ISR(SPI_STC_vect)
{
    // Check if Byte Transmission
    if(SPI_BufferLength == 0)
    {
        SPI_RxByte = SPDR_REG;// Receive byte

        // Call the callback function if provided
        if(SPI_ByteCallback !=Null) 
        {
            SPI_ByteCallback(SPI_RxByte);
        }

        // Set SPI state to idle
        SPI_State = SPI_State_Idle;
    }
    else // Buffer Transmission
    {
        // Receive byte
        if(SPI_RxBufferPtr !=Null)
        {
            SPI_RxBufferPtr[SPI_BufferIndex] = SPDR_REG;
        }

        SPI_BufferIndex++;

        // Check if more data to send
        if(SPI_BufferIndex < SPI_BufferLength)
        {
            // Send next byte
            SPDR_REG = SPI_TxBufferPtr[SPI_BufferIndex];
        }
        else
        {
            // Transmission complete
            SPI_BufferLength = 0;
            SPI_BufferIndex = 0;

            // Call the callback function if provided
            if(SPI_BufferCallback !=Null)
            {
                SPI_BufferCallback();
            }

            // Set SPI state to idle
            SPI_State = SPI_State_Idle;
        }
    }
   
}