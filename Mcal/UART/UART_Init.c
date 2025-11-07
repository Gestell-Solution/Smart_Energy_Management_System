/**
 * @file UART_Init.c
 * @brief UART initialization implementation for ATmega32.
 * @details This source file configures the UART peripheral for interrupt-driven
 * non-blocking transmission and reception using circular buffers.
 * It sets baud rate, data bits, stop bits, and parity mode based on
 * user-defined configuration in UART_Init.h.
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2025-10-08
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#include "../UART/UART_Init.h"
#include"../UART/UART_RX.h"
#include"../UART/UART_Tx.h"
#include"../Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"

#define F_CPU 8000000UL  /**< Define the CPU frequency as 8 MHz */
UART_Buffer_t UART_TxBuffer;
UART_Buffer_t UART_RxBuffer;

/**
* @fn mUART_Init
* @brief Initializes UART peripheral with user-defined settings.
* @details 
* @param ubrr_value Calculate UBRR value for normal speed.
* @param ucsrc_temp
* @return void
*/
void mUART_Init(const UART_Config_t *Config)
{
    uint16_t ubrr_value;

    ubrr_value = (uint16_t)((F_CPU / (16UL * Config->BaudRate)) - 1);

    /* Set baud rate registers */
    UBRRH_Reg = (uint8_t)(ubrr_value >> 8);
    UBRRL_Reg = (uint8_t)ubrr_value;

    /* UCSRA: Normal speed mode */
   
    ClearBit(UCSRA_Reg,uart_U2X);
    /* UCSRB: Enable Rx/Tx  */
    SetBit(UCSRB_Reg, uart_RXEN);   /**<Enable Receiver */
    SetBit(UCSRB_Reg, uart_TXEN);   /**<Enable Transmitter */

    /* UCSRC: Configure frame format */
    uint8_t ucsrc_temp = 0;
    SetBit(ucsrc_temp, uart_URSEL); 

    /* Asynchronous mode */
    ClearBit(ucsrc_temp, uart_UMSEL);

    /* Parity setting */
    switch (Config->Parity) {
        case UART_PARITY_EVEN:
            SetBit(ucsrc_temp, uart_UPM1);
            ClearBit(ucsrc_temp, uart_UPM0);
            break;
        case UART_PARITY_ODD:
            SetBit(ucsrc_temp, uart_UPM1);
            SetBit(ucsrc_temp, uart_UPM0);
            break;
        default: // None
            ClearBit(ucsrc_temp, uart_UPM1);
            ClearBit(ucsrc_temp, uart_UPM0);
            break;
    }

    /* Stop bits */
    if (Config->StopBits == UART_2_STOP)
        SetBit(ucsrc_temp, uart_USBS);
    else
        ClearBit(ucsrc_temp, uart_USBS);

    /* Data bits */
    switch (Config->DataBits) {
        case UART_6_BIT:
            SetBit(ucsrc_temp, uart_UCSZ0);
            ClearBit(ucsrc_temp, uart_UCSZ1);
            ClearBit(UCSRB_Reg, uart_UCSZ2);
            break;
        case UART_7_BIT:
            ClearBit(ucsrc_temp, uart_UCSZ0);
            SetBit(ucsrc_temp, uart_UCSZ1);
            ClearBit(UCSRB_Reg, uart_UCSZ2);
            break;
        default: // 8-bit
            SetBit(ucsrc_temp, uart_UCSZ0);
            SetBit(ucsrc_temp, uart_UCSZ1);
            ClearBit(UCSRB_Reg, uart_UCSZ2);
            break;
    }

    /* Write final UCSRC configuration */
    UCSRC_Reg = ucsrc_temp;
}
