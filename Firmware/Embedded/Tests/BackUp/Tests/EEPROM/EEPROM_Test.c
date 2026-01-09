/**
 * @file IntegrationTest_LCD.c
 * @brief Integration test for SystemDataManager + EnergyLogger + EEPROM with LCD/LED display.
 * @author Basma khaled
 * @date 2025-12-12
 */
#include "../../Common/Config.h"

#if EEPROM_TestModule == Enable

#include <stdint.h>
#include "../../App/EnergyLogger/EnergyLogger_Interface.h"
#include "../../Common/SystemDataManager/SystemDataManager.h"
#include "../../Hal/LCD/LCD_Interface.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "../../Mcal/EEPROM/EEPROM_Interface.h"



void IntegrationTest1_logging(void)
{
    hLCD_Init();
    

    SystemData_Init();
    App_EnergyLogger_Init();

    /*test SystemData values */
    g_SystemData.DeviceID = 0x55;
    g_SystemData.VoltageCalib = 1100;
    g_SystemData.CurrentCalib = 1050;
    g_SystemData.EnergyCounter = 1234;
    SystemData_SaveToEEPROM();

    /* Add Energy logs */
    EnergyLog_t log;
    for(int i=0;i<5;i++)
    {
        log.timestamp = i;
        log.voltage = 220 + i;
        log.current = 5 + i;
        log.power = log.voltage * log.current;
        log.energy_kwh = 0.01*i;
        App_EnergyLogger_Update(&log);
    }

    /* Store logs to EEPROM */
    while(EnergyRAM.count>0){
        App_EnergyLogger_StoreToEEPROM();
    }
    
    hLCD_SendCommand(0x01);
    for(uint16_t i = 0; i < EEPROM_count; i++)
    {
        App_EnergyLogger_ReadLog(i, &log);
        hLCD_SendCommand(0x01);
        hLCD_SetCursor(1, 0);
        hLCD_WriteString("V:");
        hLCD_WriteNumber((int)log.voltage);
        hLCD_WriteString(" I:");
        hLCD_WriteNumber((int)log.current);
        hLCD_WriteString(" P:");
        hLCD_WriteNumber((int)log.power);
    }
    
    SystemData_LoadFromEEPROM();

    hLCD_SendCommand(0x01);
    hLCD_SetCursor(1, 0);
    hLCD_WriteString("DevID:");   
    hLCD_WriteNumber(g_SystemData.DeviceID);

    hLCD_SendCommand(0x01);
    hLCD_SetCursor(1, 0);
    hLCD_WriteString("VoltCal:");   
    hLCD_WriteNumber(g_SystemData.VoltageCalib);

    hLCD_SendCommand(0x01);
    hLCD_SetCursor(1, 0);
    hLCD_WriteString("CurrCal:");   
    hLCD_WriteNumber(g_SystemData.CurrentCalib);

     hLCD_SendCommand(0x01);
    hLCD_SetCursor(1, 0);
    hLCD_WriteString("Energy:");   
    hLCD_WriteNumber(g_SystemData.EnergyCounter);


    while(1)
    {
        
    }
}
void IntegrationTest2_logging(void)
{
    hLCD_Init();
    

    SystemData_t expectedData = {0x55, 1100, 1050, 1234};
    EnergyLog_t logs[5];

    for(int i=0; i<5; i++)
    {
        logs[i].timestamp = i;
        logs[i].voltage = 220 + i;
        logs[i].current = 5 + i;
        logs[i].power = logs[i].voltage * logs[i].current;
        logs[i].energy_kwh = 0.01*i;
    }

    bool test_pass = true;

    for(int cycle=0; cycle<5; cycle++)
    {
        SystemData_Init();
        App_EnergyLogger_Init();

        /* Write SystemData to EEPROM */
        g_SystemData = expectedData;
        SystemData_SaveToEEPROM();

        for(int i=0; i<5; i++)
            App_EnergyLogger_Update(&logs[i]);

        while(EnergyRAM.count > 0)
            App_EnergyLogger_StoreToEEPROM();

        /* clear RAM */
        g_SystemData.DeviceID = 0;
        g_SystemData.VoltageCalib = 0;
        g_SystemData.CurrentCalib = 0;
        g_SystemData.EnergyCounter = 0;
        EnergyRAM.count = 0;
        EnergyRAM.front = -1;
        EnergyRAM.rear = -1;

        SystemData_LoadFromEEPROM();

        /* Validate SystemData */
        if(g_SystemData.DeviceID != expectedData.DeviceID) test_pass = false;
        if(g_SystemData.VoltageCalib != expectedData.VoltageCalib) test_pass = false;
        if(g_SystemData.CurrentCalib != expectedData.CurrentCalib) test_pass = false;
        if(g_SystemData.EnergyCounter != expectedData.EnergyCounter) test_pass = false;
    }

    /* Display validation result on LCD */
    hLCD_SendCommand(0x01);
    hLCD_SetCursor(1, 0);
    if(test_pass)
        hLCD_WriteString("EEPROM PASS");
    else
        hLCD_WriteString("EEPROM FAIL");


    while(1)
    {

    }
}
#endif