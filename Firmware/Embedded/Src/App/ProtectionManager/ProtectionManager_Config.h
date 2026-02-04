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
 * @def PM_TRIP_DELAY_TICKS
 * @brief Number of update ticks required to confirm an Overload fault (Debounce).
 * @details Assuming update rate of 10ms: 10 ticks * 10ms = 100ms tolerance.
 */
#define PM_TRIP_DELAY_TICKS      10

/**
 * @def PM_SHORT_CIRCUIT_MULTIPLIER
 * @brief Multiplier for Immediate Trip threshold.
 * @details If Current > (Irms_Threshold * 3.0), trip instantly ignoring delay.
 */
#define PM_SHORT_CIRCUIT_MULTIPLIER  3

/**
 * @def PM_RESET_DELAY_TICKS
 * @brief Number of ticks required for system stability before reset is allowed.
 * @details 20 ticks * 10ms = 200ms. Ensures the fault condition has truly passed or stabilized.
 */
#define PM_RESET_DELAY_TICKS     20      

/**
 * @def PM_CURRENT_HYSTERESIS
 * @brief Current drop value (in Amperes) required to stop the fault counter.
 * @details 1.0f means the current must drop 1A below the threshold to exit the fault counting state.
 */
#define PM_CURRENT_HYSTERESIS    1.0f    

/**
 * @def PM_RESET_STABLE_TICKS
 * @brief Number of stable-safe ticks required before allowing a reset.
 * @details With SYSTEM_TICK_MS=100, 5 ticks = 500ms stability.
 */
#define PM_RESET_STABLE_TICKS    5

#endif /* _PM_CONFIG_H_ */
