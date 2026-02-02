/**
 * @file       SystemDataManager.c
 * @brief      
 * @details    
 * @version    0.1
 * @author     Hesham Ahmed (Hisham4Ahmed@gmail.com)
 * @date       2025-10-01
 * @copyright  Copyright (c) 2025 , Gestell Company 
 */
#include "SystemDataManager.h"
#include "../../Hal/RelayControl/RELAY_Config.h"
#include "../../Mcal/EEPROM/EEPROM_Interface.h"
SystemData_t g_SystemData;

void SystemData_Init(void)
{
    SystemData_LoadFromEEPROM();
    if (!SystemData_Validate())
    {
        SystemData_SetDefaults();
        SystemData_SaveToEEPROM();
    }
}

void SystemData_SaveToEEPROM(void)
{
    mEEPROM_WriteBlock(Device_ID_Add, (uint8_t *)&g_SystemData, sizeof(SystemData_t));
}

void SystemData_LoadFromEEPROM(void)
{
    mEEPROM_ReadBlock(Device_ID_Add, (uint8_t *)&g_SystemData, sizeof(SystemData_t));
}

void SystemData_SetDefaults(void)
{
    g_SystemData.MagicNumber       = SYSTEMDATA_MAGIC;
    g_SystemData.DeviceID          = DEFAULT_DEVICE_ID;
    g_SystemData.VoltageCalib      = DEFAULT_VOLTAGE_CALIB;
    g_SystemData.CurrentCalib      = DEFAULT_CURRENT_CALIB;
    g_SystemData.EnergyCounter     = DEFAULT_ENERGY_COUNTER;
    g_SystemData.OvervoltageLimit  = DEFAULT_OVERVOLTAGE_LIMIT;
    g_SystemData.OvercurrentLimit  = DEFAULT_OVERCURRENT_LIMIT;
    g_SystemData.RelayStates       = DEFAULT_RELAY_STATES;
    g_SystemData.Voltage_RMS       = DEFAULT_VOLTAGE_RMS;
    g_SystemData.Current_RMS       = DEFAULT_CURRENT_RMS;
    g_SystemData.Power             = DEFAULT_POWER;
}

bool SystemData_Validate(void)
{
    return (g_SystemData.MagicNumber == SYSTEMDATA_MAGIC);
}

void SystemData_SetRelayState(uint8_t relayId, uint8_t isOn)
{
    if (relayId >= hRELAY_NUM)
    {
        return;
    }

    uint8_t mask = (uint8_t)(1U << relayId);
    uint8_t newStates = g_SystemData.RelayStates;

    if (isOn)
    {
        newStates |= mask;
    }
    else
    {
        newStates &= (uint8_t)~mask;
    }

    /* Persist only if there is an actual change to reduce EEPROM wear */
    if (newStates != g_SystemData.RelayStates)
    {
        g_SystemData.RelayStates = (uint8_t)(newStates & SYSTEMDATA_RELAY_STATE_MASK);
    }
}

uint8_t SystemData_GetRelayStatesMask(void)
{
    return (uint8_t)(g_SystemData.RelayStates & SYSTEMDATA_RELAY_STATE_MASK);
}
