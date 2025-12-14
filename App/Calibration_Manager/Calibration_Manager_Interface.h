/**
 * @file Calibration_Manager_Interface.h
 * @brief Interface for the Calibration Manager module.
 * @version 1.0
 * @author Developer : Ahmed Ashraf (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Mohamed Diaa (mohammediaato@gmail.com)
 * @date 2025-12-04
 * @copyright Copyright (c) 2025, Gestell Company
 */

#ifndef _CALIBRATION_MANAGER_INTERFACE_H_
#define _CALIBRATION_MANAGER_INTERFACE_H_

/**
 * @addtogroup App
 * @{
 * @addtogroup Calibration_Manager
 * @{
 */

#include "Calibration_Manager_Config.h"
#include "Calibration_Manager_Private.h"
#include "../../Hal/ACS712CurntSnsr/hCurrent_Interface.h"
#include "../../Hal/Voltage/Voltage_Interface.h"
#include "../../Mcal/EEPROM/EEPROM_Interface.h"
#include "../../Hal/LCD/LCD_Interface.h"
#include "../../Hal/HC05/HC05_Interface.h"
#include "../../Mcal/Timer1/TIMER1_Interface.h"
/**
 * @fn App_Calibration_Init
 * @brief Initializes The Calibration Manager Module.
 */
void App_Calibration_Init(void);

/**
 * @fn App_Calibration_Remote
 * @brief Handles Remote Calibration Process.
 */
void App_Calibration_Remote(uint8_t *cmdData);
/**
 * @fn App_Calibration_Apply
 * @brief Applies the Calibration Settings.
 */
void App_Calibration_Apply(void);


/**
 * @}
 * @}
 */

#endif /* _CALIBRATION_MANAGER_INTERFACE_H_ */