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
/**
 * INIT → Startup and module initialization.
NORMAL_State → Continuous monitoring & data processing.
OVERLOAD_State → Protection triggered; isolate loads.
RECOVERY_State → Wait until system stabilizes.
ERROR_State → Critical fault handling.
SHUTDOWN_State → System halt/reset condition.
 */
/**
 * @def     INIT_State
 * @brief   Startup and module initialization State
 */
#define     INIT_State      0x00

/**
 * @def     NORMAL_State 
 * @brief   Continuous monitoring & data processing State
 */
#define     NORMAL_State    0x01

/**
 * @def     OVERLOAD_State 
 * @brief   Protection triggered isolate loads.
 */
#define     OVERLOAD_State  0x02

/**
 * @def     RECOVERY_State 
 * @brief   Wait until system stabilizes.
 */
#define     RECOVERY_State  0x03
/**
 * @def     ERROR_State 
 * @brief    Critical fault handling.
 */
#define     ERROR_State     0x04

/**
 * @def     SHUTDOWN_State  
 * @brief   System halt/reset condition.
 */
#define     SHUTDOWN_State  0x05

#endif