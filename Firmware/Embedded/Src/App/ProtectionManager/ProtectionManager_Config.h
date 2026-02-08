/**
 * @file ProtectionManager_Config.h
 * @brief This File Contains the Protection Manager Configuration Parameters
 * @version 2.0
 * @date 11-11-2025
 * @author Developer : Ahmed Ashraf (ahmedashraf2022222@gmail.com)
 * @author Developer(Bug Fixing) : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author Reviewer  : Mohamed Diaa (mohammeddiaato@gmail.com)
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#ifndef _PM_CONFIG_H_
#define _PM_CONFIG_H_

#include "../../Common/ProjectCfg.h"
/**
 * @def Vrms_Threshold
 * @brief Voltage RMS Threshold which if exceeded System Takes a Protection Action
 */
#define Vrms_Threshold 250
/**
 * @def Irms_Threshold
 * @brief Current RMS Threshold which if exceeded System Takes a Protection Action
 */
#define Irms_Threshold 11
/**
 * @def P_Threshold
 * @brief Power Threshold which if exceeded System Takes a Protection Action
 */
#define P_Threshold 7500

/**
 * @def PM_POWER_LIMIT_MODE_FIXED
 * @brief Use fixed configured power threshold (`P_Threshold`).
 */
#define PM_POWER_LIMIT_MODE_FIXED   0U

/**
 * @def PM_POWER_LIMIT_MODE_PRODUCT
 * @brief Derive power threshold from current*voltage configured limits.
 */
#define PM_POWER_LIMIT_MODE_PRODUCT 1U

/**
 * @def PM_POWER_LIMIT_MODE
 * @brief Active power limit mode selector.
 * @details
 * - `PM_POWER_LIMIT_MODE_FIXED`   -> uses `P_Threshold`.
 * - `PM_POWER_LIMIT_MODE_PRODUCT` -> uses `OvercurrentLimit * OvervoltageLimit`.
 */
#define PM_POWER_LIMIT_MODE PM_POWER_LIMIT_MODE_FIXED
/**
 *@def Reset_BTN_Group
 *@brief Reset Button Group/Port
 */
#define Reset_BTN_Group GroupD
/**
 *@def Reset_BTN_Group
 *@brief Reset Button Pin
 */
#define Reset_BTN_Pin PIN3
/* ================================================= */
/*     FIX-007 ProtectionManager Debounce Setting    */
/* ================================================= */

/**
 * @def PM_TICKS_FROM_MS
 * @brief Convert milliseconds to scheduler ticks (rounded up).
 */
#define PM_TICKS_FROM_MS(ms) (((ms) + (SYSTEM_TICK_MS - 1U)) / (SYSTEM_TICK_MS))

/**
 * @def PM_TRIP_DELAY_MS
 * @brief Overload confirmation time before tripping.
 */
#define PM_TRIP_DELAY_MS         100U

/**
 * @def PM_RESET_DELAY_MS
 * @brief Stable-safe time required before reset is accepted.
 */
#define PM_RESET_DELAY_MS        200U

/**
 * @def PM_TRIP_DELAY_TICKS
 * @brief Number of update ticks required to confirm an Overload fault (Debounce).
 * @details Derived from `PM_TRIP_DELAY_MS` and `SYSTEM_TICK_MS`.
 */
#define PM_TRIP_DELAY_TICKS      PM_TICKS_FROM_MS(PM_TRIP_DELAY_MS)

/**
 * @def PM_ENABLE_POWER_TRIP
 * @brief Enable/disable active-power trip condition.
 * @details
 * - `1` -> power trip enabled.
 * - `0` -> ignore power trip (debug mode).
 */
#define PM_ENABLE_POWER_TRIP     1U

/**
 * @def PM_POWER_TRIP_DELAY_MS
 * @brief Overpower confirmation time before tripping.
 */
#define PM_POWER_TRIP_DELAY_MS   300U

/**
 * @def PM_POWER_TRIP_DELAY_TICKS
 * @brief Number of ticks required to confirm overpower fault.
 */
#define PM_POWER_TRIP_DELAY_TICKS PM_TICKS_FROM_MS(PM_POWER_TRIP_DELAY_MS)

/**
 * @def PM_POWER_HYSTERESIS_W
 * @brief Power drop value required to decay overpower counter.
 */
#define PM_POWER_HYSTERESIS_W    150.0f

/**
 * @def PM_SHORT_CIRCUIT_MULTIPLIER
 * @brief Multiplier for Immediate Trip threshold.
 * @details If Current > (Irms_Threshold * 3.0), trip instantly ignoring delay.
 */
#define PM_SHORT_CIRCUIT_MULTIPLIER  3

/**
 * @def PM_MIN_OVERVOLTAGE_LIMIT
 * @brief Minimum accepted overvoltage setting from EEPROM/communication.
 */
#define PM_MIN_OVERVOLTAGE_LIMIT  100U

/**
 * @def PM_MAX_OVERVOLTAGE_LIMIT
 * @brief Maximum accepted overvoltage setting from EEPROM/communication.
 */
#define PM_MAX_OVERVOLTAGE_LIMIT  400U

/**
 * @def PM_MIN_OVERCURRENT_LIMIT
 * @brief Minimum accepted overcurrent setting from EEPROM/communication.
 */
#define PM_MIN_OVERCURRENT_LIMIT  1U

/**
 * @def PM_MAX_OVERCURRENT_LIMIT
 * @brief Maximum accepted overcurrent setting from EEPROM/communication.
 */
#define PM_MAX_OVERCURRENT_LIMIT  100U

/**
 * @def PM_USE_TEST_CURRENT
 * @brief Select current source for PM_Update.
 * @details 0: use sensor (`ME_GetCurrentRMS`), 1: use `PM_Test_Current`.
 */
#define PM_USE_TEST_CURRENT       0

/**
 * @def PM_RESET_DELAY_TICKS
 * @brief Number of ticks required for system stability before reset is allowed.
 * @details Derived from `PM_RESET_DELAY_MS` and `SYSTEM_TICK_MS`.
 */
#define PM_RESET_DELAY_TICKS     PM_TICKS_FROM_MS(PM_RESET_DELAY_MS)

/**
 * @def PM_CURRENT_HYSTERESIS
 * @brief Current drop value (in Amperes) required to stop the fault counter.
 * @details 1.0f means the current must drop 1A below the threshold to exit the fault counting state.
 */
#define PM_CURRENT_HYSTERESIS    1.0f    

/**
 * @def PM_STARTUP_CONNECT_LOADS
 * @brief Startup relay policy.
 * @details 1: connect loads at startup, 0: keep loads disconnected.
 */
#define PM_STARTUP_CONNECT_LOADS  0

/**
 * @def PM_RESET_STABLE_TICKS
 * @brief Backward-compatible alias for reset stability ticks.
 */
#define PM_RESET_STABLE_TICKS    PM_RESET_DELAY_TICKS

#endif /* _PM_CONFIG_H_ */
