#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H

#include <stdint.h>
#include "SPI_Private.h"
#include "../Atmega32RegistersAddress.h"


/**
 * @defgroup SPI Module Configuration Parameters
 * @def SPI_MODE
 * @brief Represents the SPI operating mode (Master/Slave).
 * @def SPI_DATA_ORDER
 * @brief Represents the data order (MSB first/LSB first).
 * @def SPI_CLOCK_MODE
 * @brief Represents the clock polarity and phase.
 * @def SPI_PRESCALER
 * @brief Represents the SPI clock rate prescaler.
 * @def SPI_INTERRUPT
 * @brief Represents the SPI interrupt state (Enable/Disable).
 * @note to see the options for each parameter check SPI_Private.h
 * @warning Make sure to configure these parameters according to your application requirements.
 * @{
 */

#define SPI_MODE           Master_Mode 


#define SPI_DATA_ORDER     MSB_First

#define SPI_CLOCK_MODE     SPI_Mode0

#define SPI_PRESCALER      F_CPU_8

#define SPI_INTERRUPT      Enable







#endif  /* SPI_CONFIG_H */