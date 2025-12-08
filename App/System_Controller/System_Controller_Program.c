/**
 * @file       System_Controller_Program.c
 * @brief      it it Defines the APIs Used by the System Controller
 * @details    
 * @version    1.0
 * @date       2025-12-4
 * @author     Developer:   Mohammed Diaa   Mohammeddiaato@gmail.com
 * @author     Reviewer:    Ahmed Ashraf    
 * @copyright  Copyright (c) 2025, Gestell Company
 */
#include "../../Common/Config.h"
#if System_Controller_Module ==Enable
#include "System_Controller_Interface.h"
extern SystemData_t g_SystemData;

SystemEvent_t SystemController;
SystemState_t Status;
void App_SystemController_Init(void)
{
    SystemController.SysState.State=INIT_State;
    DM_Init();
    PM_Init();
    ME_Init();
    SystemData_Init();
    App_CommManager_Init();
    App_EnergyLogger_Init();
    SystemController.SysState.State=NORMAL_State;
    
}
/* Initialize all dependent modules and set initial system state */

void App_SystemController_Update(void)
{
    
}
/* Called periodically (e.g., every 100ms) to handle system tasks and transitions */

void App_SystemController_HandleEvent(SystemEvent_t event)
{
    
}
/* Receive and process asynchronous events from modules (Protection, Comm, etc.) */

SystemState_t App_SystemController_GetState(void)
{
    
}
/* Return current system state for monitoring or debugging */
#endif


