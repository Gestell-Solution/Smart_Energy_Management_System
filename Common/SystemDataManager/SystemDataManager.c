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
#include"../../Mcal/EEPROM/EEPROM_Interface.h"
SystemData_t g_SystemData;

void SystemData_Init(void){
    
    SystemData_LoadFromEEPROM();
    if (!SystemData_Validate())
    {
        SystemData_SetDefaults();
        SystemData_SaveToEEPROM();
    }
}
void SystemData_SaveToEEPROM(void){
    mEEPROM_WriteBlock(Device_ID_Add, (uint8_t*)&g_SystemData, sizeof(SystemData_t));


}
void SystemData_LoadFromEEPROM(void){
    mEEPROM_ReadBlock(Device_ID_Add, (uint8_t*)&g_SystemData, sizeof(SystemData_t));

}
void SystemData_SetDefaults(void){
    g_SystemData.MagicNumber       = SYSTEMDATA_MAGIC;
    g_SystemData.DeviceID          = DEFAULT_DEVICE_ID;
    g_SystemData.VoltageCalib      = DEFAULT_VOLTAGE_CALIB;
    g_SystemData.CurrentCalib      = DEFAULT_CURRENT_CALIB;
    g_SystemData.EnergyCounter     = DEFAULT_ENERGY_COUNTER;
    g_SystemData.OvervoltageLimit  = DEFAULT_OVERVOLTAGE_LIMIT;
    g_SystemData.OvercurrentLimit  = DEFAULT_OVERCURRENT_LIMIT;
}
bool SystemData_Validate(void){
    return (g_SystemData.MagicNumber == SYSTEMDATA_MAGIC);

}
// void main(void)
// {
//     SystemData_Init(); 

//     g_SystemData.VoltageCalib = 1020;
//     SystemData_SaveToEEPROM(); 

//     SystemData_LoadFromEEPROM();
// }
