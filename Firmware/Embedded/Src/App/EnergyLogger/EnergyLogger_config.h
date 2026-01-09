/**
 * @file EnergyLogger_config.h
 * @brief Configuration constants for the Energy Logger module.
 * @author Developer : Basma khaled     (basmak55@gmail.com)
 * @author Reviewer  : Ahmed Ashraf     (ahmedashraf2022222@gmail.com)
 * @date 2025-12-4
 * @copyright Copyright (c) 2025 , Gestell Company 
 */
#ifndef ENERGY_LOGGER_CONFIG_H
#define ENERGY_LOGGER_CONFIG_H

#define ENERGY_LOGGER_RAM_BUFFER_SIZE 32 /** Size of RAM circular buffer */

#define EEPROM_MAX_LOGS 128 /** Maximum number of logs to store in EEPROM */

#define EEPROM_LOG_BASE 0x100 /** Base EEPROM address for the first log */

#define N_SAMPLES_TO_EEPROM 10 /** Number of samples to accumulate before writing to EEPROM */

#define SAMPLE_INTERVAL_HOURS (0.01f / 3600.0f)  /** Sampling interval in hours (converted from seconds) */


#endif