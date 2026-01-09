/**
 * @file       Atmega32RegistersAddress.h
 * @brief      
 * @details    
 * @version    0.1
 * @author     Hesham Ahmed (Hisham4Ahmed@gmail.com)
 * @date       2025-10-01
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#ifndef _ATMEGA32_REGISTERS_ADDRESS_H_
#define _ATMEGA32_REGISTERS_ADDRESS_H_

#include <stdint.h>


/*Status Register*/
#define SREG_Reg    *(volatile uint8_t*)0x5F
/*Stack Pointer Register*/
#define SPH_Reg     *(volatile uint8_t*)0x5E
#define SPL_Reg     *(volatile uint8_t*)0x5D
#define SP_Reg      *(volatile uint16_t*)0x5D
/*System Clock and Clock Control Register*/
#define OSCCAL_Reg  *(volatile uint8_t*)0x51
/*Timer0 Registers*/
#define OCR0_Reg    *(volatile uint8_t*)0x5C
#define TIMSK_Reg   *(volatile uint8_t*)0x59
#define TIFR_Reg    *(volatile uint8_t*)0x58
#define TCCR0_Reg   *(volatile uint8_t*)0x53
#define TCNT0_Reg   *(volatile uint8_t*)0x52
/*Timer1 Registers */
#define TCCR1A_Reg  *(volatile uint8_t*)0x4F
#define TCCR1B_Reg  *(volatile uint8_t*)0x4E
#define TCNT1H_Reg  *(volatile uint8_t*)0x4D
#define TCNT1L_Reg  *(volatile uint8_t*)0x4C
#define TCNT1_Reg   *(volatile uint16_t*)0x4C
#define OCR1AH_Reg  *(volatile uint8_t*)0x4B
#define OCR1AL_Reg  *(volatile uint8_t*)0x4A
#define OCR1A_Reg   *(volatile uint16_t*)0x4A
#define OCR1BH_Reg  *(volatile uint8_t*)0x49
#define OCR1BL_Reg  *(volatile uint8_t*)0x48
#define OCR1B_Reg   *(volatile uint16_t*)0x48
#define ICR1H_Reg   *(volatile uint8_t*)0x47
#define ICR1L_Reg   *(volatile uint8_t*)0x46
#define ICR1_Reg    *(volatile uint16_t*)0x46
/*Timer2 Registers*/
#define TCCR2_Reg   *(volatile uint8_t*)0x45
#define TCNT2_Reg   *(volatile uint8_t*)0x44
#define OCR2_Reg    *(volatile uint8_t*)0x43
#define ASSR_Reg    *(volatile uint8_t*)0x42
/*EXTI Registers & Power Management and Sleep Mode Control Register*/
#define GICR_Reg    *(volatile uint8_t*)0x5B
#define GIFR_Reg    *(volatile uint8_t*)0x5A
#define MCUCR_Reg   *(volatile uint8_t*)0x55  /*for EXTI & PMSMC*/
#define MCUCSR_Reg  *(volatile uint8_t*)0x54 /*for EXIT & JTAG & System Control Register*/
/*JTAG Interface Registers*/
#define OCDR_Reg    *(volatile uint8_t*)0x51
#define MCUCSR_Reg  *(volatile uint8_t*)0x54
/*Bootloader Support Registes */
#define SPMCR_Reg   *(volatile uint8_t*)0x57
/*TWI Registers */
#define TWBR_Reg    *((volatile uint8_t*)0x20)
#define TWSR_Reg    *((volatile uint8_t*)0x21)
#define TWAR_Reg    *((volatile uint8_t*)0x22)
#define TWDR_Reg    *((volatile uint8_t*)0x23)
#define TWCR_Reg    *((volatile uint8_t*)0x56)  
/*ADC Registers and Analog Comparator */
#define ADMUX_Reg   *((volatile uint8_t*)0x27)
#define ADCSRA_Reg  *((volatile uint8_t*)0x26)
#define ADCData_Reg *((volatile uint16_t*)0x24)
#define ADCH_Reg    *((volatile uint8_t*)0x25)
#define ADCL_Reg    *((volatile uint8_t*)0x24)
#define SFIOR_Reg   *((volatile uint8_t*)0x50)
#define ACSR_Reg    *((volatile uint8_t*)0x28)
/*USART Registers */
#define UBRRL_Reg   *((volatile uint8_t*)0x29)
#define UCSRB_Reg   *((volatile uint8_t*)0x2A)
#define UCSRA_Reg   *((volatile uint8_t*)0x2B)
#define UDR_Reg     *((volatile uint8_t*)0x2C)
#define UBRRH_Reg   *((volatile uint8_t*)0x40)
#define UCSRC_Reg   *((volatile uint8_t*)0x40)
/*SPI Registers */
#define SPCR_Reg    *((volatile uint8_t*)0x2D)
#define SPSR_Reg    *((volatile uint8_t*)0x2E)
#define SPDR_Reg    *((volatile uint8_t*)0x2F)
/*DIO Registers */
#define PORTA_Reg   *((volatile uint8_t*)0x3B)
#define DDRA_Reg    *((volatile uint8_t*)0x3A)
#define PINA_Reg    *((volatile uint8_t*)0x39)
#define PORTB_Reg   *((volatile uint8_t*)0x38)
#define DDRB_Reg    *((volatile uint8_t*)0x37)
#define PINB_Reg    *((volatile uint8_t*)0x36)
#define PORTC_Reg   *((volatile uint8_t*)0x35)
#define DDRC_Reg    *((volatile uint8_t*)0x34)
#define PINC_Reg    *((volatile uint8_t*)0x33)
#define PORTD_Reg   *((volatile uint8_t*)0x32)
#define DDRD_Reg    *((volatile uint8_t*)0x31)
#define PIND_Reg    *((volatile uint8_t*)0x30)
/*EEPROM Registers*/
#define EEARH_Reg   *(volatile uint8_t*)0x3F
#define EEARL_Reg   *(volatile uint8_t*)0x3E
#define EEAR_Reg    *(volatile uint16_t*)0x3E
#define EEDR_Reg    *(volatile uint8_t*)0x3D
#define EECR_Reg    *(volatile uint8_t*)0x3C
/*Watch Dog Timer Registers*/
#define WDTCSR_Reg  *(volatile uint8_t*)0x41

#endif /* _ATMEGA32_REGISTERS_ADDRESS_H_ */