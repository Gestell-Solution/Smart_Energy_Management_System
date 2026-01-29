/**
 * @file Persistence_Program.c
 * @brief Implementation of the persistence module for energy data storage.
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2026-1-29
 * @copyright Copyright (c) 2025 , Gestell Company
 */
#include "Persistance_Interface.h"
#include "../../Mcal/EEPROM/EEPROM_Interface.h"
#include"../../App/MeasurementEngine/MeasurementEngine_Interface.h"
#include "../../Common/Macros.h"
#include <stdbool.h>
#include <stdint.h>

static uint32_t TickCounter = 0;
static float LastSavedEnergy = 0.0f;
uint32_t EEPROM_WriteCount = 0;   

void Persistence_Update(void)
{
    ME_Init();
    TickCounter++;
    float current_energy = ME_GetEnergy();
    uint8_t save_required = 0;

    if (TickCounter >= SAVE_INTERVAL_TICKS)
    {
        save_required = 1;
    }

    if ((current_energy - LastSavedEnergy) >= 0.1f)
    {
        save_required = 1;
    }
    if (save_required)
    {
        Persistence_Save(current_energy);
        LastSavedEnergy = current_energy;
        TickCounter = 0;
    }
}



static uint32_t CalcCRC(const void *data, uint32_t len)
{
    const uint8_t *p = (const uint8_t *)data;
    uint32_t crc = 0xFFFFFFFF;

    for (uint32_t i = 0; i < len; i++)
    {
        crc ^= p[i];
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }

    return ~crc;
}



void Persistence_Save(float energy)
{
    EnergyRecord rec;

    rec.energy = energy;
    rec.crc    = CalcCRC(&energy, sizeof(float));
    mEEPROM_WriteBlock(0x00, (uint8_t*)&rec, sizeof(rec));

    mEEPROM_WriteBlock(0x10, (uint8_t*)&rec, sizeof(rec));
    EEPROM_WriteCount++;

}
float Persistence_Load(void)
{
    EnergyRecord Primary, backup;

    mEEPROM_ReadBlock(0x00, (uint8_t*)&Primary, sizeof(Primary));
    mEEPROM_ReadBlock(0x10, (uint8_t*)&backup, sizeof(backup));

    bool prim_ok = (CalcCRC(&Primary.energy, sizeof(float)) == Primary.crc);
    bool back_ok = (CalcCRC(&backup.energy, sizeof(float)) == backup.crc);

    if (prim_ok) return Primary.energy;
    if (back_ok) return backup.energy;

    return 0.0f; 
}

void Persistence_Reset(void)
{
    TickCounter = 0;
    LastSavedEnergy = 0.0f;

}
