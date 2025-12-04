#ifndef SPI_INTERFACE_H
#define SPI_INTERFACE_H


#include <stdint.h>
#include "SPI_Private.h"
#include "SPI_Config.h"
//------------------------------------------------------------------------------------------



void mSPI_Init(void);
uint8_t mSPI_TransmitByte(uint8_t data);
void mSPI_TransmitBuffer(uint8_t* txBuffer, uint8_t* rxBuffer, uint16_t length);
void mSPI_TransmitByte_IT(uint8_t data, void (*callback)(uint8_t rxData));
void mSPI_TransmitBuffer_IT(uint8_t* txBuffer, uint8_t* rxBuffer, uint16_t length, void (*callback)(void));
void mSPI_Enable(void);
void mSPI_Disable(void);

#endif // SPI_INTERFACE_H