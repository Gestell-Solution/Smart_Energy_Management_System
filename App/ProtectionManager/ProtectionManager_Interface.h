/**
 * @file ProtectionManager_Interface.h
 * @brief This File Contains the Protection Manager Interfacing
 * @version 1.0
 * @date 11-11-2025
 * @author Developer : Ahmed Ashraf (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Mohamed Diaa (mohammeddiaato@gmail.com)
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#ifndef _PROTECTIONMANAGER_iNTERFACE_H_
#define _PROTECTIONMANAGER_iNTERFACE_H_

/**
 * @addtogroup App
 * @{
 * @addtogroup Protection_Manager
 * @{
 */

#include "ProtectionManager_Config.h"
#include "ProtectionManager_Private.h"
#include "../../Hal/Buzzer/Buzzer_Interface.h"
#include "../../Hal/PushButton/BTN_Interface.h"
#include "../../Hal/RGB/RGB_Interface.h"
#include "../../Hal/RelayControl/RELAY_Interface.h"
#include "../../Hal/RelayControl/RELAY_Config.h"
#include "../DM_Driver/DisplayManager_Interface.h"
#include "../../Hal/LCD/LCD_Interface.h"

#include "../MeasurementEngine/MeasurementEngine_Interface.h"
#include "../../Common/SystemDataManager/SystemDataManager.h"
/**
 * @defgroup PM API'S
 * @brief    PM Functions
 * {@
 */

/**
 * @fn PM_Init
 * @brief Initialize all of the needed Modules in this Driver
 */

void PM_Init(void);
/**
 * @fn PM_Update
 * @brief Checks Periodically all the Protection Conditions
 */

void PM_Update(void); // Periodic check of all protection conditions
/**
 * @fn PM_IsTripped
 * @brief Check if System Is Currently in Protection Mode
 * @return returns Danger or
 */

uint8_t PM_IsTripped(void); // Check if system is currently in protection mode

/**
 * @fn PM_Reset
 * @brief Resets the Normal operation after fixing the problem
 * @details this function is triggered by the system automatically,
 *          when the protection conditions are exceeded system stucks in a loop till problem is fixed
 *          , to reset the Operation Press the Reset button
 *
 * @warning Please Make Sure not to press the button till the problem is fixed
 *
 */
void PM_Reset(void); // Restore normal operation

/** @} */


/**
 * @}
 * @}
 */

#endif