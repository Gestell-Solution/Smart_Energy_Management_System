/**
 * @file ProtectionManager_Config.h
 * @brief This File Contains the Protection Manager Configuration Parameters
 * @version 1.0
 * @date 11-11-2025
 * @author Developer : Ahmed Ashraf (ahmedashraf2022222@gmail.com)
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
#define Irms_Threshold 10
/**
 * @def P_Threshold
 * @brief Power Threshold which if exceeded System Takes a Protection Action
 */
#define P_Threshold 2500
/**
 *@def Reset_BTN_Group
 *@brief Reset Button Group/Port
 */
#define Reset_BTN_Group GroupD
/**
 *@def Reset_BTN_Group
 *@brief Reset Button Pin
 */
#define Reset_BTN_Group PIN3
#endif