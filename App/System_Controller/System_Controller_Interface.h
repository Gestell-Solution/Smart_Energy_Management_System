/**
 * @file       System_Controller_interface.h
 * @brief      it it declares the interface of the System Controller.
 * @details    
 * @version    1.0
 * @date       2025-12-4
 * @author     Developer:   Mohammed Diaa   Mohammeddiaato@gmail.com
 * @author     Reviewer:    Ahmed Ashraf    
 * @copyright  Copyright (c) 2025, Gestell Company
 */


#ifndef SYSTEM_CONTROLLER_INTERFACE_H
#define SYSTEM_CONTROLLER_INTERFACE_H
/**
 * @defgroup File_Inclusion
 * @brief    File Inclusion Section
 * @{ 
 */
#include "System_Controller_Config.h"
#include "../DM_Driver/DisplayManager_Interface.h"
#include "../MeasurementEngine/MeasurementEngine_Interface.h"
#include "../ProtectionManager/ProtectionManager_Interface.h"
#include "../EnergyLogger/EnergyLogger_Interface.h"
#include "../CommunicationManager/App_CommManager.h"
#include "Common\SystemDataManager\SystemDataManager.h"
/**
 * @}
 */



 /**
 * @defgroup SystemControllerStructures
 * @brief    Structure of data used by the SystemController
 * @{
 */

 typedef struct
{
    volatile uint8_t SysState;
    volatile SystemData_t Data;
    
} SystemState_t;

typedef struct
{
    volatile uint8_t CmdID;
    volatile uint8_t Event;
    volatile uint8_t* data;

} SystemEvent_t;

/**
 * @}
 */
void App_SystemController_Init(void);
/* Initialize all dependent modules and set initial system state */

void App_SystemController_Update(void);
/* Called periodically (e.g., every 100ms) to handle system tasks and transitions */

void App_SystemController_HandleEvent(SystemEvent_t event);
/* Receive and process asynchronous events from modules (Protection, Comm, etc.) */

SystemState_t App_SystemController_GetState(void);
/* Return current system state for monitoring or debugging */

#endif