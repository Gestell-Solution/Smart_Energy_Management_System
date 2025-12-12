/**
 * @file       System_Controller_Config.h
 * @brief      it it Defines Configurable macros for the system Macros
 * @details
 * @version    1.0
 * @date       2025-12-4
 * @author     Developer:   Mohammed Diaa   Mohammeddiaato@gmail.com
 * @author     Reviewer:    Ahmed Ashraf
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#ifndef SYSTEM_CONTROLLER_CONFIG_H
#define SYSTEM_CONTROLLER_CONFIG_H
#include <stdint.h>

/**
 * @def     SchedulingTime
 * @brief
 */
#define Scheduling_Time_sysController 100
/**
 * @def     SchedulingTime
 * @brief
 */
#define RecoveryTime 1//miutes -> in real time =3
/**
 * @def     Second
 * @brief   500ms
 */
#define Second 1000
/**
 * @def     TimeThreshold
 * @brief   5*second
 */
#define TimeThreshold 5 * Second

#define RMS_Message_length 35

/**
 * @defgroup Modes
 * @brief
 */
#define Automatic 0
#define Manual 1
/**
 * @defgroup SystemControllerMacros
 * @brief    Macros defines the System Commands to different Section
 * @{
 */

/**
 * @def     INIT_State
 * @brief   Startup and module initialization State
 */
#define INIT_State 0x00

/**
 * @def     NORMAL_State
 * @brief   Continuous monitoring & data processing State
 */
#define NORMAL_State 0x01

/**
 * @def     OVERLOAD_State
 * @brief   Protection triggered isolate loads.
 */
#define OVERLOAD_State 0x02

/**
 * @def     RECOVERY_State
 * @brief   Wait until system stabilizes.
 */
#define RECOVERY_State 0x03
/**
 * @def     ERROR_State
 * @brief    Critical fault handling.
 */
#define ERROR_State 0x04

/**
 * @def     SHUTDOWN_State
 * @brief   System halt/reset condition.
 */
#define SHUTDOWN_State 0x05
/**
 * @}
 */

/**
 * @defgroup SystemControllerEvents
 * @brief    Events Handled by the SystemController
 * @{
 */

#define EVENT_OVERLOAD_DETECTED 0x01
#define EVENT_OVERLOAD_CLEARED 0x02
#define EVENT_MODE_TOGGLE 0x03
#define EVENT_CALIBRATION_DONE 0x04
#define EVENT_COMM_RECEIVED_CMD 0x05
#define EVENT_SENSOR_FAULT 0x07
#define EVENT_Reset_event 0x08
#define EVENT_Power_Down 0x09

/**
 * @}
 */
#define DangerMessage "OVERLOAD!"
#define DangerMessage_length 10
#define Change_Mode_To_AutoMatic "Mode Changed to Automatic"
#define Automatic_length 26
#define Change_Mode_To_Manual "Mode Changed to Manual"
#define Manual_Length 23
#define PLEASE_RESET_Message "PLEASE RESET"
#define PLEASE_RESET_Message_length 13
#define CuttoFF_Message "Relays are cutoff"
#define Cutoff_message_length 18
#define UpdatedEEPROM_Message "EEPROM_Updated"
#define UpdatedEEPROM_Message_length 15

/**
 * SLEEP DOWN BITS
 */
#define SE_bit  7
#define SM2_bit 6
#define SM1_bit 5
#define SM0_bit 4



#endif