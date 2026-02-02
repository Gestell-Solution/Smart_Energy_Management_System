/**
 * @file Persistence_Interface.c
 * @brief Interface for the persistence module
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2026-1-29
 * @copyright Copyright (c) 2025 , Gestell Company
 */
#ifndef PERSISTENCE_INTERFACE_H
#define PERSISTENCE_INTERFACE_H

#include "../../Common/Macros.h"
#include <stdint.h>

#define SAVE_INTERVAL_TICKS  (100 * 60 * 10) 
extern uint32_t EEPROM_WriteCount;

typedef struct {
    float energy;
    uint32_t   crc;
} EnergyRecord;
/**
 * @fn Persistence_Update
 * @brief Updates the persistence module.
 */
void Persistence_Update(void);
/**
 * @fn Persistence_Save
 * @brief Saves the given energy value to EEPROM immediately.
 * @param energy Energy value in kWh to be saved.
 */
void Persistence_Save(float energy);
/**
 * @fn CalcCRC
 * @brief Calculates the CRC for a given data block.
 * @param data Pointer to the data block.
 * @param len Length of the data block in bytes. 
 * @return CRC value of the given data block.
 */
static uint32_t CalcCRC(const void *data, uint32_t len);
/**
 * @fn Persistence_Load
 * @brief Loads the last saved energy value from EEPROM.
 * @return Last saved energy value in kWh, or 0.0f if EEPROM is invalid.
 */
float Persistence_Load(void);
/**
 * @fn Persistence_Reset
 * @brief Resets the persistence module.
 */
void Persistence_Reset(void);
#endif 