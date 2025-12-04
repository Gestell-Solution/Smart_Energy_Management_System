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
