/**
 * @file       EnergyLogger_Program.c
 * @brief      Implementation of the Energy Logger Module.
 * @details    This module manages the storage of electrical measurements (Voltage, Current, Power, Energy).
 *             It utilizes a two-stage storage mechanism:
 *             1. **RAM Circular Buffer**: Temporary storage for high-frequency samples.
 *             2. **EEPROM Storage**: Persistent storage where RAM data is flushed periodically.
 *             
 *             The logger handles buffer overflows by overwriting the oldest data in RAM, ensuring
 *             the most recent history is always preserved before committing to slower EEPROM.
 * @version    1.0
 * @date       2025-12-04
 * @author     Developer: Basma Khaled     (basmak55@gmail.com)
 * @author     Reviewer:  Ahmed Ashraf     (ahmedashraf2022222@gmail.com)
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#include "EnergyLogger_Interface.h"
#include "../../Mcal/EEPROM/EEPROM_Interface.h"
#include "../../Mcal/Timer1/TIMER1_Interface.h"  
#include "../../Hal/Voltage/Voltage_Interface.h"
#include "../../Hal/ACS712CurntSnsr/hCurrent_Interface.h" 
#include "../../Mcal/UART/UART_Tx.h"
#include "../../Mcal/UART/UART_Init.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "../../Hal/LCD/LCD_Interface.h"

/*============================================================================
 *                                 Global Variables
 *============================================================================*/

/** @brief RAM Circular Buffer instance. */
EnergyLogBuffer_t EnergyRAM;

/** @brief Write index for EEPROM (Next available slot). */
uint16_t EEPROM_head = 0;

/** @brief Total count of logs securely stored in EEPROM. */
uint16_t EEPROM_count = 0;

/** @brief Global Timestamp counter (externally updated or internal). */
uint32_t timestampCounter = 0;

/* Addresses to persist logger metadata just before EEPROM_LOG_BASE */
enum {
    EEPROM_LOG_META_HEAD_ADDR = (uint16_t)(EEPROM_LOG_BASE - 4),
    EEPROM_LOG_META_COUNT_ADDR = (uint16_t)(EEPROM_LOG_BASE - 2)
};

/*============================================================================
 *                                 Function Definitions
 *============================================================================*/

/**
 * @brief      Initializes the Energy Logger.
 * @details    Resets the RAM circular buffer indices and EEPROM counters.
 *             (Note: In a real scenario, EEPROM_head might need to be recovered from EEPROM itself).
 * @return     void
 */
void App_EnergyLogger_Init(void)
{
    /* Initialize RAM circular buffer indexes to conventional 0-based values */
    EnergyRAM.front = 0;
    EnergyRAM.rear  = 0;
    EnergyRAM.count = 0;

    /* Recover EEPROM head/count from dedicated metadata area if valid */
    uint8_t tmp[2];
    /* Read head (uint16_t) */
    mEEPROM_ReadBlock(EEPROM_LOG_META_HEAD_ADDR, tmp, 2);
    uint16_t head = (uint16_t)((tmp[0] << 8) | tmp[1]);
    if (head < EEPROM_MAX_LOGS)
    {
        EEPROM_head = head;
    }
    else
    {
        EEPROM_head = 0;
    }

    /* Read count (uint16_t) */
    mEEPROM_ReadBlock(EEPROM_LOG_META_COUNT_ADDR, tmp, 2);
    uint16_t cnt = (uint16_t)((tmp[0] << 8) | tmp[1]);
    if (cnt <= EEPROM_MAX_LOGS)
    {
        EEPROM_count = cnt;
    }
    else
    {
        EEPROM_count = 0;
    }
}

/**
 * @brief      Adds a new log entry to the RAM Circular Buffer.
 * @details    - If buffer is full, it overwrites the oldest data (Front moves forward).
 *             - If buffer is empty, it initializes Front and Rear.
 *             - Otherwise, it increments Rear.
 * @param[in]  newLog  Pointer to the new `EnergyLog_t` data to add.
 * @return     void
 */
void App_EnergyLogger_Update(const EnergyLog_t *newLog)
{
    if (EnergyRAM.count == 0)
    {
        /* First element */
        EnergyRAM.front = EnergyRAM.rear = 0;
        EnergyRAM.buffer[EnergyRAM.rear] = *newLog;
        EnergyRAM.count = 1;
        return;
    }

    if (EnergyRAM.count < ENERGY_LOGGER_RAM_BUFFER_SIZE)
    {
        EnergyRAM.rear = (EnergyRAM.rear + 1) % ENERGY_LOGGER_RAM_BUFFER_SIZE;
        EnergyRAM.buffer[EnergyRAM.rear] = *newLog;
        EnergyRAM.count++;
        return;
    }

    /* Buffer full: overwrite oldest */
    EnergyRAM.front = (EnergyRAM.front + 1) % ENERGY_LOGGER_RAM_BUFFER_SIZE;
    EnergyRAM.rear = (EnergyRAM.rear + 1) % ENERGY_LOGGER_RAM_BUFFER_SIZE;
    EnergyRAM.buffer[EnergyRAM.rear] = *newLog;
}

/**
 * @brief      Moves the oldest log from RAM to EEPROM.
 * @details    Takes the element at `EnergyRAM.front`, writes it to the active EEPROM address,
 *             and updates the indices.
 *             - Circular writes to EEPROM prevent wear leveling issues (basic implementation).
 *             - Decrements RAM count.
 * @return     void
 */
void App_EnergyLogger_StoreToEEPROM(void)
{
     if (EnergyRAM.count == 0)
     {
        return;  
     }

    EnergyLog_t logToeeprom = EnergyRAM.buffer[EnergyRAM.front];

    /* Calculate byte address in EEPROM */
    uint16_t addr = EEPROM_LOG_BASE + (uint16_t)(EEPROM_head * sizeof(EnergyLog_t));
    mEEPROM_WriteBlock(addr, (uint8_t*)&logToeeprom, sizeof(EnergyLog_t));

    /* Advance EEPROM head and keep log count bounded to valid metadata range. */
    EEPROM_head = (EEPROM_head + 1) % EEPROM_MAX_LOGS;
    if (EEPROM_count < EEPROM_MAX_LOGS)
    {
        EEPROM_count++;
    }
    else
    {
        EEPROM_count = EEPROM_MAX_LOGS;
    }

    /* Remove from RAM */
    EnergyRAM.front = (EnergyRAM.front + 1) % ENERGY_LOGGER_RAM_BUFFER_SIZE;
    EnergyRAM.count--;

    /* Persist metadata (head,count) so we can recover after reboot */
    uint8_t meta[2];
    meta[0] = (uint8_t)(EEPROM_head >> 8);
    meta[1] = (uint8_t)(EEPROM_head & 0xFF);
    mEEPROM_WriteBlock(EEPROM_LOG_META_HEAD_ADDR, meta, 2);
    meta[0] = (uint8_t)(EEPROM_count >> 8);
    meta[1] = (uint8_t)(EEPROM_count & 0xFF);
    mEEPROM_WriteBlock(EEPROM_LOG_META_COUNT_ADDR, meta, 2);

    if (EnergyRAM.count == 0)
    {
        EnergyRAM.front = 0;
        EnergyRAM.rear  = 0;
    }
}

/**
 * @brief      Reads a specific log entry from EEPROM.
 * @param[in]  logIndex Index of the log to retrieve (0 to EEPROM_count-1).
 * @param[out] readLog  Pointer to store the retrieved log data.
 * @return     void
 */
void App_EnergyLogger_ReadLog(uint16_t logIndex, EnergyLog_t *readLog)
{
    if (logIndex >= EEPROM_count)
    {
        return; 
    }

    uint16_t addr = (uint16_t)(logIndex * sizeof(EnergyLog_t)) + EEPROM_LOG_BASE;
    mEEPROM_ReadBlock(addr, (uint8_t*)readLog, sizeof(EnergyLog_t));
}

/**
 * @brief      Periodic task to manage data flushing.
 * @details    Checks if enough samples have accumulated or if a time period has passed,
 *             then triggers a batch write to EEPROM. This reduces the frequency of 
 *             EEPROM write cycles compared to writing every single sample.
 * @return     void
 */
void App_EnergyLogger_Task(void)
{
    /*
     * Write at a controlled rate:
     * one EEPROM write every N task calls when data exists.
     * This avoids continuous backlog flush that can add timing pressure.
     */
    static uint8_t sampleCounter = 0u;

    if (EnergyRAM.count == 0u)
    {
        sampleCounter = 0u;
        return;
    }

    sampleCounter++;
    if (sampleCounter >= N_SAMPLES_TO_EEPROM)
    {
        sampleCounter = 0u;
        App_EnergyLogger_StoreToEEPROM();
    }
}
