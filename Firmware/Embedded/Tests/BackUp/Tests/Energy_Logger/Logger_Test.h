#ifndef LOGGER_TEST_H
#define LOGGER_TEST_H
#include "../../Mcal/DIO/DIO_Interface.h"

void hLCD_Clear1(uint8_t ClearLine);
void Test_RAM_Buffer(void);
void Display_LED_Status(void);
void Test_EEPROM_LCD(void);
int LoggerTestMain(void);

#endif 
