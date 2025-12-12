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
#if System_Controller_Module == Enable
#include "System_Controller_Interface.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
extern EnergyLogBuffer_t EnergyRAM;
extern SystemData_t g_SystemData;
SystemEvent_t SystemController;
SystemState_t Status;

uint8_t RecoveryTimer=0; 
uint8_t minutes=0;

void Update_Global_SystemData(void)
{
    g_SystemData.Voltage_RMS = (uint16_t)Status.RamData.voltage;
    g_SystemData.Current_RMS = (uint16_t)Status.RamData.current;
    g_SystemData.Power       = (uint16_t)Status.RamData.power;

    static uint32_t StartupEnergyOffset = 0;
    static uint8_t  IsFirstRun = 1;

    if (IsFirstRun == 1)
    {
        StartupEnergyOffset = g_SystemData.EnergyCounter;
        IsFirstRun = 0;
    }

    g_SystemData.EnergyCounter = StartupEnergyOffset + (uint32_t)Status.RamData.energy_kwh;
}

void FloatNumber_to_string(float Num,char res[])
{

    res[0]=((uint32_t)Num / 10) % 10 + '0';
    res[1]=((uint32_t)Num ) % 10 + '0';
    res[2] = '.';
    res[3] = ((uint32_t)(Num * 10)) % 10+'0';
    res[4] = ((uint32_t)(Num * 100)) % 10+'0';


    res[5]=NullChar;
}
void Update_Rms_Data()
{
    Status.Data[0]=NullChar;
    char res[6];
    strcat(Status.Data,"I= " );
    FloatNumber_to_string(Status.RamData.current,res);
    strcat(Status.Data,res);
    strcat(Status.Data,"V= " );
    FloatNumber_to_string(Status.RamData.voltage,res);
    strcat(Status.Data,res);
    strcat(Status.Data,"P= " );
    FloatNumber_to_string(Status.RamData.power,res);
    strcat(Status.Data,res);
    strcat(Status.Data,"E= " );
    FloatNumber_to_string(Status.RamData.energy_kwh,res);
    strcat(Status.Data,res);

}
void App_SystemController_Init(void)
{
    Status.SysState = INIT_State;
    DM_Init();
    PM_Init();
    ME_Init();
    SystemData_Init();
    App_CommManager_Init();
    App_EnergyLogger_Init();
    App_EnergyLogger_ReadLog(EnergyRAM.front, &Status.RamData);
    Status.Data[0] =NullChar ;
    Status.SysState = NORMAL_State;
    Status.SystemMode = Automatic;
    mTIMER0_StartDelay(Scheduling_Time_sysController, App_SystemController_Update);
}
/* Initialize all dependent modules and set initial system state */

void App_SystemController_Update(void)
{

    if (Status.SysState == RECOVERY_State)
    {
        RecoveryTimer++; 
        
        if (RecoveryTimer>=60)
        {
            minutes++;
            RecoveryTimer=0;
        }
        
        if (minutes >= RecoveryTime) 
        {
            Status.SysState = NORMAL_State; 
        }
    }

    DM_Update();
    Status.RamData.energy_kwh = ME_GetEnergy();
    Status.RamData.power = ME_GetPower();
    Status.RamData.current = ME_GetCurrentRMS();
    Status.RamData.voltage = ME_GetVoltageRMS();

    if (PM_IsTripped() && Status.SysState == NORMAL_State)
    {
        SystemData_SaveToEEPROM();
        SystemController.CmdID = CuttOFF;
        SystemController.Event = EVENT_OVERLOAD_DETECTED;
        App_SystemController_HandleEvent(SystemController);

    }
    if (!PM_IsTripped()&& Status.SysState == OVERLOAD_State)
    {
        SystemController.Event = EVENT_OVERLOAD_CLEARED;
        App_SystemController_HandleEvent(SystemController);
        App_EnergyLogger_Update(&Status.RamData);

    }
    Status.SysState = NORMAL_State;

    if (hBtn_GetStatus() != Status.SystemMode)
    {
        Status.SystemMode = hBtn_GetStatus(); // both are uint8_t
        SystemController.CmdID = ShowModeState;
        SystemController.Event = EVENT_MODE_TOGGLE;
        App_SystemController_HandleEvent(SystemController);
    }
}
/* Called periodically (e.g., every 100ms) to handle system tasks and transitions */

void App_SystemController_HandleEvent(SystemEvent_t Action)
{

    switch (Action.Event)
    {
    case EVENT_OVERLOAD_DETECTED:
        Status.SysState = OVERLOAD_State;

        DM_ShowProtectionState(PM_IsTripped());
        App_CommManager_SendFrame(DangerMessage, Action.CmdID, DangerMessage_length);
        PM_Reset();

        break;
    case EVENT_OVERLOAD_CLEARED:
        Status.SysState = RECOVERY_State;
        RecoveryTimer=0;
        minutes=0;

        break;
    case EVENT_MODE_TOGGLE:
        DM_ShowMode(Status.SystemMode);
        if (Status.SystemMode == Automatic)
        {

            App_CommManager_SendFrame(Change_Mode_To_AutoMatic, Action.CmdID, Automatic_length);
        }
        else
        {
            App_CommManager_SendFrame(Change_Mode_To_Manual, Action.CmdID, Manual_Length);
        }

        break;
    case EVENT_CALIBRATION_DONE:
        g_SystemData.CurrentCalib;
        g_SystemData.VoltageCalib;
        g_SystemData.MagicNumber++;
        break;
    case EVENT_SENSOR_FAULT:
        App_SystemController_Init();
        mTIMER0_Delay_ms(RecoveryTime);
        break;
    case EVENT_Power_Down:
        SetBit(MCUCR_Reg, SE_bit);
        SetBit(MCUCR_Reg, SM1_bit);
        // SLEEP();
        break;
    case EVENT_Reset_event:
        App_CommManager_SendFrame(PLEASE_RESET_Message,SHUTDOWN_Device,PLEASE_RESET_Message_length);
        App_SystemController_Init();
        mTIMER0_Delay_ms(RecoveryTime);
        break;

    default:
        return;
        break;
    }
}
/* Receive and process asynchronous events from modules (Protection, Comm, etc.) */

SystemState_t App_SystemController_GetState(void)
{
    Update_Rms_Data();
    return Status;
}
/* Return current system state for monitoring or debugging */

#endif
