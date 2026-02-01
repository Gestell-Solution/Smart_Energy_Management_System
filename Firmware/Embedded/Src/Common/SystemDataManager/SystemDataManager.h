/**
 * @file       SystemDataManager.h
 * @brief      
 * @details    
 * @version    0.1
 * @author     Hesham Ahmed (Hisham4Ahmed@gmail.com)
 * @date       2025-10-01
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#ifndef _SYSTEM_DATA_MANAGER_H_
#define _SYSTEM_DATA_MANAGER_H_

#include <stdint.h>
#include <stdbool.h>
#include "../../Mcal/EEPROM/EEPROM_Interface.h"



/**
 * @brief Structure holding all system configuration and counters
 */
typedef struct
{
    uint8_t  MagicNumber;
    uint8_t  DeviceID;
    uint16_t VoltageCalib;
    uint16_t CurrentCalib;
    uint32_t EnergyCounter;
    uint16_t OvervoltageLimit;
    uint16_t OvercurrentLimit;
    uint8_t  Checksum;   /**< Optional data validation field */
    uint16_t Voltage_RMS;
    uint16_t Current_RMS;
    uint16_t Power;
} SystemData_t;

// 1- assign adderss different values  

// 2- reset 
// 3- read by EERPOM function 


int main()
{
    if(button == pressed)
    {
        readEEPROM(0x00);
        lcd 
        wait 3 
        readEEPROM(0x01);
        lcd 
        wait 1 
        
    }
    
}

/**
 * @brief Default values
 */
#define DEFAULT_DEVICE_ID            0x06
#define DEFAULT_VOLTAGE_CALIB        9999
#define DEFAULT_CURRENT_CALIB        5555
#define DEFAULT_ENERGY_COUNTER       0x00000000
#define DEFAULT_OVERVOLTAGE_LIMIT    250
#define DEFAULT_OVERCURRENT_LIMIT    10
#define DEFAULT_VOLTAGE_RMS          0
#define DEFAULT_CURRENT_RMS          0
#define DEFAULT_POWER                0
#define SYSTEMDATA_MAGIC  0xA5

/**
 * @brief Global RAM copy of system data
 */
extern SystemData_t g_SystemData;
/**
 * @brief Load data from EEPROM or set defaults.
 */
void SystemData_Init(void);

/**
 * @brief Save RAM shadow to EEPROM.
 */
void SystemData_SaveToEEPROM(void);

/**
 * @brief Reload struct from EEPROM.
 */
void SystemData_LoadFromEEPROM(void);

/**
 * @brief Reset to factory defaults.
 */
void SystemData_SetDefaults(void);

/**
 * @brief Optional checksum/validation.
 */
bool SystemData_Validate(void);
#endif /* _SYSTEM_DATA_MANAGER_H_ */