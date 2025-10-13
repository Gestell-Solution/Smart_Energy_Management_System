/**
 * @file UART_Private.h
 * @brief UART private bits .
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2025-10-08
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#ifndef MCAL_UART_UART_PRIVATE_H_
#define MCAL_UART_UART_PRIVATE_H_

/**
 * @enum  UartBits
 * @brief Enumeration of UART register bit positions for ATmega32.
 */
typedef enum
{
    /**
     * @defgroup UCSRA_bits 
     * @brief Define the bits in UCSRA register
     * @{
     */
    
    uart_MPCM = 0,    /** Multi-processor Communication Mode */
    uart_U2X,         /** Double Transmission Speed */
    uart_PE,          /** Parity Error*/
    uart_DOR,         /** Data OverRun */
    uart_FE,          /** Frame Error  */
    uart_UDRE,        /** USART Data Register Empty */
    uart_TXC,         /** Transmit Complete*/
    uart_RXC,         /** Receive Complete*/

    
    /**
     * @defgroup UCSRB_bits 
     * @brief Define the bits in UCSRB register
     * @{
     */
    
    uart_TXB8,        /** Transmit Data Bit 8 */
    uart_RXB8,        /** Receive Data Bit 8*/
    uart_UCSZ2,       /** Character Size */
    uart_TXEN,        /** Transmitter Enable*/
    uart_RXEN,        /** Receiver Enable*/
    uart_UDRIE,       /** Data Register Empty Interrupt Enable*/
    uart_TXCIE,       /** TX Complete Interrupt Enable*/
    uart_RXCIE,       /** RX Complete Interrupt Enable*/

    
    /**
     * @defgroup UCSRC_bits 
     * @brief Define the bits in UCSRC register
     * @{
     */
    
  
    uart_UCPOL,        /** Clock Polarity*/
    uart_UCSZ0,        /** Character Size bit 0*/
    uart_UCSZ1,        /** Character Size bit 1 */
    uart_USBS,         /** Stop Bit Select*/
    uart_UPM0,         /** Parity Mode bit 0*/
    uart_UPM1,         /** Parity Mode bit 1*/
    uart_UMSEL,        /** Mode Select*/
    uart_URSEL         /** Register Select*/
    
} UartBits;

#endif 