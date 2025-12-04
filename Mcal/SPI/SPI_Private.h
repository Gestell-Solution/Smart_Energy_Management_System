#ifndef SPI_PRIVATE_H
#define SPI_PRIVATE_H

#include <stdint.h>
#include "../../Common/Macros.h"
#include"../../Mcal/Atmega32RegistersAddress.h"
//-----------------------------------------------------------------------
// SPI Registers Definitions

#define SPCR_REG   (*(volatile uint8_t*)0x2D)
#define SPSR_REG   (*(volatile uint8_t*)0x2E)
#define SPDR_REG   (*(volatile uint8_t*)0x2F)

//-----------------------------------------------------------------------
// SPI Control Register (SPCR) Bit Definitions
typedef enum
{
    SPCR0_Bit0 = 0, //clock rate bit 0
    SPCR1_Bit1,     //clock rate bit 1
    CPHA_Bit2,     //clock phase
    CPOL_Bit3,     //clock polarity
    MSTR_Bit4,    //master/slave select
    DORD_Bit5,    //data order
    SPE_Bit6,     //SPI enable
    SPIE_Bit7     //SPI interrupt enable
}SPCR_Bits_t;
//-----------------------------------------------------------------------
// SPI Status Register (SPSR) Bit Definitions
typedef enum
{
    SPI2x_Bit0 = 0, //double SPI speed bit
    WCOL_Bit6=6,     //write collision flag
    SPIF_Bit7=7     //SPI interrupt flag
}SPSR_Bits_t;
//-----------------------------------------------------------------------

typedef enum
{
    Slave_Mode,
    Master_Mode
}SPI_Mode_t;


//-----------------------------------------------------------------------
typedef enum
{
    MSB_First=0,
    LSB_First=1
}SPI_DataOrder_t;
//-----------------------------------------------------------------------
//clock polarity and clock phase
typedef enum
{
    SPI_Mode0=0, //CPOL=0, CPHA=0 Idel Low, Sample Leading Edge

    SPI_Mode1,   //CPOL=0, CPHA=1 Idel Low, Sample Trailing Edge
    SPI_Mode2,   //CPOL=1, CPHA=0 Idel High, Sample Leading Edge
    SPI_Mode3    //CPOL=1, CPHA=1 Idel High, Sample Trailing Edge   
}SPI_ClockMode_t;
//-----------------------------------------------------------------------
//clock rate prescaler
typedef enum
{
    F_CPU_4=0,
    F_CPU_16,
    F_CPU_64,
    F_CPU_128,
    F_CPU_2,
    F_CPU_8,
    F_CPU_32 
}SPI_ClockRate_t;
//-----------------------------------------------------------------------
// SPI Pins Definitions
#define SS_GROUP     GroupA
#define MOSI_GROUP   GroupA
#define MISO_GROUP   GroupA
#define SCK_GROUP    GroupA


#define SS_PIN      PIN4
#define MOSI_PIN    PIN5
#define MISO_PIN    PIN6
#define SCK_PIN     PIN7
//-----------------------------------------------------------------------
// SPI State Enumeration
typedef enum
{
    SPI_State_Idle=0,
    SPI_State_Busy,
}SPI_State_t;
//-----------------------------------------------------------------------
/*typedef enum
{
    Enable=0,
    Disable=1
}SPI_Interrupt_t;
//-----------------------------------------------------------------------
// SPI Configuration Structure
typedef struct
{
    SPI_Mode_t Mode;
    SPI_DataOrder_t DataOrder;
    SPI_ClockMode_t ClockMode;
    SPI_ClockRate_t ClockRate;
    SPI_Interrupt_t Interrupt;
} SPI_Config_t;
*/

#endif /* SPI_PRIVATE_H */