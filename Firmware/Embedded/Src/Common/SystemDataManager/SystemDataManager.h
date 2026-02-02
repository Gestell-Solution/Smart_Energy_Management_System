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
    uint8_t  RelayStates;   /**< Bitmask: bit0..bit3 represent relays 0..3 (1=ON,0=OFF) */
    uint8_t  Checksum;   /**< Optional data validation field */
    uint16_t Voltage_RMS;
    uint16_t Current_RMS;
    uint16_t Power;
} SystemData_t;


/**
 * @brief Default values
 */
#define DEFAULT_DEVICE_ID            0x01
#define DEFAULT_VOLTAGE_CALIB        1000
#define DEFAULT_CURRENT_CALIB        1000
#define DEFAULT_ENERGY_COUNTER       0x00000000
#define DEFAULT_OVERVOLTAGE_LIMIT    250
#define DEFAULT_OVERCURRENT_LIMIT    33
#define DEFAULT_RELAY_STATES         0x00
#define DEFAULT_VOLTAGE_RMS          0
#define DEFAULT_CURRENT_RMS          0
#define DEFAULT_POWER                0
#define SYSTEMDATA_MAGIC  0xA5

/**
 * @brief Relay bitmask helpers
 */
#define SYSTEMDATA_RELAY_STATE_MASK  0x0F

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

/**
 * @brief Update persisted relay state (bit per relay) and save to EEPROM if changed.
 * @param relayId Relay index (0..3)
 * @param isOn    Logical ON state (1 = ON, 0 = OFF)
 */
void SystemData_SetRelayState(uint8_t relayId, uint8_t isOn);

/**
 * @brief Get current relay states bitmask (bits 0..3 correspond to relays 0..3).
 */
uint8_t SystemData_GetRelayStatesMask(void);

#endif /* _SYSTEM_DATA_MANAGER_H_ */