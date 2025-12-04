/**
 * @file EnergyLogger_Program.c
 * @brief Implements the Energy Logger logic: sampling, buffering, and EEPROM storage.
 * @author Developer : Basma khaled     (basmak55@gmail.com)
 * @author Reviewer  : Ahmed Ashraf     (ahmedashraf2022222@gmail.com)
 * @date 2025-12-4
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#include "EnergyLogger_Interface.h"
#include "../../MCAL/EEPROM/EEPROM_Interface.h"
#include "../../Mcal/Timer1/TIMER1_Interface.h"  

#include "../../Hal/Voltage/Voltage_Interface.h"
#include "../../Hal/ACS712CurntSnsr/hCurrent_Interface.h" 
#include "../../Mcal/UART/UART_Tx.h"
#include "../../Mcal/UART/UART_Init.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "../../Hal/LCD/LCD_Interface.h"
void App_EnergyLogger_Init(void){
    EnergyRAM.front = -1;
    EnergyRAM.rear = -1;
    EnergyRAM.count = 0;

    EEPROM_head = 0;
    EEPROM_count = 0;
    mTIMER1_Init();
    mTIMER1_RegisterCallback(EnergyLogger_TimerCallback);
    mTIMER1_Start();
    hVoltage_Init();
    hCurrent_Init();

}

void App_EnergyLogger_Update(const EnergyLog_t *newLog)
{
    if ((EnergyRAM.rear + 1) % ENERGY_LOGGER_RAM_BUFFER_SIZE != EnergyRAM.front)
    {
        if (EnergyRAM.count == 0)
        {
            EnergyRAM.front = EnergyRAM.rear = 0;
            EnergyRAM.buffer[EnergyRAM.rear] = *newLog;
        }
        else
        {
            EnergyRAM.rear = (EnergyRAM.rear + 1) % ENERGY_LOGGER_RAM_BUFFER_SIZE;
            EnergyRAM.buffer[EnergyRAM.rear] = *newLog;
        }

        EnergyRAM.count++;
    }

    else
    {
        EnergyRAM.front = (EnergyRAM.front + 1) % ENERGY_LOGGER_RAM_BUFFER_SIZE;

        EnergyRAM.rear = (EnergyRAM.rear + 1) % ENERGY_LOGGER_RAM_BUFFER_SIZE;

        EnergyRAM.buffer[EnergyRAM.rear] = *newLog;
    }
}

void App_EnergyLogger_StoreToEEPROM(void){
     if (EnergyRAM.count == 0)
        return;  
    EnergyLog_t logToeeprom = EnergyRAM.buffer[EnergyRAM.front];

    uint16_t addr = EEPROM_head * sizeof(EnergyLog_t); /*gives the address to start write the new log*/
    mEEPROM_WriteBlock(addr, (uint8_t*)&logToeeprom, sizeof(EnergyLog_t));

    EEPROM_head = (EEPROM_head + 1) % EEPROM_MAX_LOGS;
    EEPROM_count++;

    EnergyRAM.front = (EnergyRAM.front + 1) % ENERGY_LOGGER_RAM_BUFFER_SIZE;
    EnergyRAM.count--;

    if (EnergyRAM.count == 0)
    {
        EnergyRAM.front = -1;
        EnergyRAM.rear = -1;
    }
}


void App_EnergyLogger_ReadLog(uint16_t logIndex, EnergyLog_t *readLog)
{
    if (logIndex >= EEPROM_count){
                return; 
    }

    uint16_t addr = logIndex * sizeof(EnergyLog_t);
    mEEPROM_ReadBlock(addr, (uint8_t*)readLog, sizeof(EnergyLog_t));
}
void EnergyLogger_TimerCallback(void)
{
    static uint8_t sampleCounter = 0;
    static float accumulatedEnergy = 0;  

    EnergyLog_t log;

    log.timestamp = timestampCounter++; 
    log.voltage = hVoltage_ReadRMS(); 
    log.current = hCurrent_ReadRMS();  
    log.power   = log.voltage * log.current;
    accumulatedEnergy += log.power * SAMPLE_INTERVAL_HOURS; 
    log.energy_kwh  = accumulatedEnergy; 

    App_EnergyLogger_Update(&log);

    sampleCounter++;
    if(sampleCounter >= N_SAMPLES_TO_EEPROM)
    {
        sampleCounter = 0;
        while(EnergyRAM.count > 0){
            App_EnergyLogger_StoreToEEPROM();
        }
    }
}
#define LED_COUNT0  PIN0
#define LED_COUNT1  PIN1
#define LED_COUNT2  PIN2

/* LED pins for front/rear (single bit each) */
#define LED_FRONT   PIN3
#define LED_REAR0    PIN4
#define LED_REAR1    PIN5
#define LED_REAR2    PIN6

#define LED_PORT    GroupB
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
    hLCD_Clear(LCD_CLEAR_ALL);
    for(uint16_t i = 0; i < EEPROM_count; i++)
    {
        App_EnergyLogger_ReadLog(i, &readLog);
        hLCD_Clear(LCD_CLEAR_ALL);  // clear display
        hLCD_SetCursor(1, 0);
        hLCD_WriteString("V:");
        hLCD_WriteNumber((int)readLog.voltage);
        hLCD_WriteString(" I:");
        hLCD_WriteNumber((int)readLog.current);
        hLCD_WriteString(" P:");
        hLCD_WriteNumber((int)readLog.power);
    }
}
int main(void)
{
    /* Initialize all LEDs as output */
    for(uint8_t i = 0; i <= LED_REAR2; i++)
        mDIO_SetDirectionForPin(LED_PORT, i, 1);

    // Initialize LCD
    hLCD_Init();
    hLCD_Clear(LCD_CLEAR_ALL);

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