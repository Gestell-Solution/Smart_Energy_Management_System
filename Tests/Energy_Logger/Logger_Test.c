#include "../../App/EnergyLogger/EnergyLogger_Interface.h"
#include "../../Mcal/EEPROM/EEPROM_Interface.h"
#include "../../Mcal/Timer1/TIMER1_Interface.h"  

#include "../../Hal/Voltage/Voltage_Interface.h"
#include "../../Hal/ACS712CurntSnsr/hCurrent_Interface.h" 

#include "../../Mcal/DIO/DIO_Interface.h"
#include "../../Hal/LCD/LCD_Interface.h"




#define LED_COUNT0  PIN0
#define LED_COUNT1  PIN1
#define LED_COUNT2  PIN2

/* LED pins for front/rear (single bit each) */
#define LED_FRONT   PIN3
#define LED_REAR0    PIN4
#define LED_REAR1    PIN5
#define LED_REAR2    PIN6

#define LED_PORT    GroupB
#define LCD_CLEAR_ALL  0xFF  // arbitrary value to represent "clear all"
void hLCD_Clear1(uint8_t ClearLine)
{
    if (ClearLine == LCD_CLEAR_ALL)
    {
        for (uint8_t i = 0; i < NumOfRows; i++)
        {
            hLCD_SetCursor(i, 0);
            hLCD_WriteString(Clear_Line_String);
        }
    }
    else
    {
        hLCD_SetCursor(ClearLine, 0);
        hLCD_WriteString(Clear_Line_String);
    }
}


void Test_RAM_Buffer(void)
{
    EnergyLog_t log;

    for(int i = 0; i < 5; i++)
    {
        log.timestamp = i;
        log.voltage = 10 + i;
        log.current = 1 + i;
        log.power = log.voltage * log.current;
        log.energy_kwh = i * 0.001;

        App_EnergyLogger_Update(&log);
    }

}

void Display_LED_Status(void)
{
    uint8_t count = EnergyRAM.count;
    uint8_t rear = EnergyRAM.rear;

    mDIO_WritePin(LED_PORT, LED_COUNT0, count & 0x01);
    mDIO_WritePin(LED_PORT, LED_COUNT1, (count >> 1) & 0x01);
    mDIO_WritePin(LED_PORT, LED_COUNT2, (count >> 2) & 0x01);

    /* Front LED ON if even, OFF if odd */
    mDIO_WritePin(LED_PORT, LED_FRONT, (EnergyRAM.front % 2 == 0));

    /* Rear LED ON if even, OFF if odd */
    mDIO_WritePin(LED_PORT, LED_REAR0, rear & 0x01);
    mDIO_WritePin(LED_PORT, LED_REAR1, (rear >> 1) & 0x01);
    mDIO_WritePin(LED_PORT, LED_REAR2, (rear >> 2) & 0x01);
}
void Test_EEPROM_LCD(void)
{
    EnergyLog_t readLog;

    // Store all logs from RAM to EEPROM
    while(EnergyRAM.count > 0)
    {
        App_EnergyLogger_StoreToEEPROM();
    }

    // Clear LCD and show EEPROM logs
    hLCD_Clear1(LCD_CLEAR_ALL);
    for(uint16_t i = 0; i < EEPROM_count; i++)
    {
        App_EnergyLogger_ReadLog(i, &readLog);
        hLCD_Clear1(LCD_CLEAR_ALL);
        hLCD_SetCursor(1, 0);
        hLCD_WriteString("V:");
        hLCD_WriteNumber((int)readLog.voltage);
        hLCD_WriteString(" I:");
        hLCD_WriteNumber((int)readLog.current);
        hLCD_WriteString(" P:");
        hLCD_WriteNumber((int)readLog.power);
    }
}
int LoggerTestMain(void)
{
    /* Initialize all LEDs as output */
    for(uint8_t i = 0; i <= LED_REAR2; i++)
        mDIO_SetDirectionForPin(LED_PORT, i, 1);

    // Initialize LCD
    hLCD_Init();
    hLCD_Clear1(LCD_CLEAR_ALL);

    // Initialize EnergyLogger RAM
    App_EnergyLogger_Init();

    // Fill RAM buffer
    Test_RAM_Buffer();

    // Display RAM status on LEDs
    Display_LED_Status();

    // Small delay (simulate wait)
    for(uint32_t i = 0; i < 100000; i++);

    // Store RAM → EEPROM and display on LCD
    Test_EEPROM_LCD();

    while(1)
    {
        Display_LED_Status(); // Keep updating LEDs
    }
}