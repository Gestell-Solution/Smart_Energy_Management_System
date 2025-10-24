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
 * @brief Enumeration of UART register bit positions for ATmega32.
 */



 
/**
 * @defgroup UCSRA_bits 
 * @brief Define the bits in UCSRA register
 * @{
 */
#define uart_MPCM   0/** Multi-processor Communication Mode */
#define uart_U2X    1/** Double Transmission Speed */
#define uart_PE     2/** Parity Error*/
#define uart_DOR    3/** Data OverRun */
#define uart_FE     4/** Frame Error  */
#define uart_UDRE   5/** USART Data Register Empty */
#define uart_TXC    6/** Transmit Complete*/
#define uart_RXC    7/** Receive Complete*/

/**
 * @defgroup UCSRB_bits 
 * @brief Define the bits in UCSRB register
 * @{
 */
#define uart_TXB8   0/** Transmit Data Bit 8 */
#define uart_RXB8   1 /** Receive Data Bit 8*/
#define uart_UCSZ2  2/** Character Size */
#define uart_TXEN   3/** Transmitter Enable*/
#define uart_RXEN   4/** Receiver Enable*/
#define uart_UDRIE  5/** Data Register Empty Interrupt Enable*/
#define uart_TXCIE  6/** TX Complete Interrupt Enable*/
#define uart_RXCIE  7/** RX Complete Interrupt Enable*/

/**
 * @defgroup UCSRC_bits 
 * @brief Define the bits in UCSRC register
 * @{
 */
#define uart_UCPOL  0/** Clock Polarity*/
#define uart_UCSZ0  1/** Character Size bit 0*/
#define uart_UCSZ1  2/** Character Size bit 1 */
#define uart_USBS   3/** Stop Bit Select*/
#define uart_UPM0   4/** Parity Mode bit 0*/
#define uart_UPM1   5/** Parity Mode bit 1*/
#define uart_UMSEL  6/** Mode Select*/
#define uart_URSEL  7 /** Register Select*/


#endif 