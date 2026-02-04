/**
 * @file       PowerManager_Interface.h
 * @brief      Power-Down Detection and Emergency Save Interface
 * @details    This module monitors system power and triggers emergency EEPROM save
 *             on power-down detection to prevent data loss.
 * @version    1.0
 * @date       2026-02-04
 * @author     Gestell Team
 * @copyright  Copyright (c) 2026, Gestell Company
 */

#ifndef _POWER_MANAGER_INTERFACE_H_
#define _POWER_MANAGER_INTERFACE_H_

#include <stdint.h>
#include "PowerManager_Config.h"

/**
 * @brief Initialize Power-Down Detection
 * @details Sets up voltage monitoring using ADC or Analog Comparator
 *          Configures interrupt for emergency save
 */
void PowerManager_Init(void);

/**
 * @brief Emergency save triggered on power-down
 * @details Called from interrupt when power drop detected
 *          Saves critical data to EEPROM immediately
 */
void PowerManager_EmergencySave(void);

/**
 * @brief Check if power is stable
 * @return 1 if power is stable, 0 if power-down detected
 */
uint8_t PowerManager_IsPowerStable(void);

#endif /* _POWER_MANAGER_INTERFACE_H_ */
