/**
 * @file EnergyLogger_Interface.h
 * @brief Public interface for the Energy Logger module.
 * @details 
 * This module handles logging of voltage, current, power, and energy values.
 * It stores data in a RAM circular buffer and periodically saves logs to EEPROM.
 * @author Developer : Basma khaled     (basmak55@gmail.com)
 * @author Reviewer  : Ahmed Ashraf     (ahmedashraf2022222@gmail.com)
 * @date 2025-12-4
 * @copyright Copyright (c) 2025 , Gestell Company
 */


#ifndef ENERGY_LOGGER_INTERFACE_H
#define ENERGY_LOGGER_INTERFACE_H

#include <stdint.h>
#include "EnergyLogger_config.h"


/**
 * @struct EnergyLog_t
 * @brief Represents a single energy measurement log.
 * 
 * @var EnergyLog_t::timestamp
 * @brief   Log timestamp counter.
 * 
 * @var EnergyLog_t::voltage
 * @brief   RMS voltage measurement.
 * 
 * @var EnergyLog_t::current
 * @brief   RMS current measurement.
 * 
 * @var EnergyLog_t::power
 * @brief   Instantaneous power (voltage * current).
 * 
 * @var EnergyLog_t::energy_kwh
 * @brief   Accumulated energy in kilowatt-hours.
 * 
 */
typedef struct
{
    uint32_t timestamp;
    float voltage;
    float current;
    float power;
    float energy_kwh;
} EnergyLog_t;

/**
 * @struct EnergyLogBuffer_t
 * @brief Represents a circular RAM buffer used for temporary storage of logs.
 * 
 * @var EnergyLogBuffer_t::buffer
 * @brief     Array holding logs.
 * 
 * @var EnergyLogBuffer_t::rear
 * @brief    Index where the next log will be written.
 * 
 * @var EnergyLogBuffer_t::front
 * @brief     Index of the oldest stored log.
 * 
 * @var EnergyLogBuffer_t::count
 * @brief     Number of logs currently in the buffer.
 */

typedef struct
{
    EnergyLog_t buffer[ENERGY_LOGGER_RAM_BUFFER_SIZE];
    uint16_t rear;       /* write position*/
    uint16_t front;      /*read position*/ 
    uint16_t count;      /*number of stored logs*/ 
} EnergyLogBuffer_t;



extern EnergyLogBuffer_t EnergyRAM;
extern uint16_t EEPROM_head ; 
extern uint16_t EEPROM_count ; 
extern uint32_t timestampCounter ;

/**
 * @fn App_EnergyLogger_Init
 * @brief Initializes the Energy Logger module, RAM buffer, EEPROM head, voltage/current sensors, and timer.
 */
void App_EnergyLogger_Init(void);

/**
 * @fn App_EnergyLogger_Update
 * @brief Inserts a new log into the RAM circular buffer.
 * @param newLog Pointer to the newly sampled log data.
 */
void App_EnergyLogger_Update(const EnergyLog_t *newLog);

/**
 * @fn App_EnergyLogger_StoreToEEPROM
 * @brief Writes the oldest log from RAM buffer into EEPROM storage.
 */
void App_EnergyLogger_StoreToEEPROM(void);

/**
 * @fn App_EnergyLogger_ReadLog
 * @brief Reads a log entry from EEPROM.
 *
 * @param index Index of the desired log.
 * @param log Pointer where the result will be stored.
 */
void App_EnergyLogger_ReadLog(uint16_t index, EnergyLog_t *log);

/**
 * @fn   App_EnergyLogger_Task
 * @brief Periodic processing task for Energy Logger (e.g., flushing buffer to EEPROM).
 * @details This function should be called periodically from the main loop. It monitors the
 *          log buffer and handles batch writing to EEPROM to minimize write cycles.
 */
void App_EnergyLogger_Task(void);

#endif 
