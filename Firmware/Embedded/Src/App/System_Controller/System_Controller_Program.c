/**
 * @file       System_Controller_Program.c
 * @brief      Implementation of the Central System Controller.
 * @details    The System Controller orchestrates the overall application flow.
 *             It manages the system state machine, handles high-level events (Protection trips, Mode changes),
 *             and coordinates data exchange between Measurement, Display, Communication, and Logging modules.
 * @version    1.0
 * @date       2025-12-04
 * @author     Developer: Mohammed Diaa (Mohammeddiaato@gmail.com)
 * @author     Reviewer:  Ahmed Ashraf
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#include "../../Common/Config.h"

#if System_Controller_Module == Enable
#include "System_Controller_Interface.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h> /* For strcat */
#include "../../Mcal/Timer1/TIMER1_Interface.h"

/*============================================================================
 *                                 Global Variables
 *============================================================================*/

/** @brief External reference to the Energy RAM buffer. */
extern EnergyLogBuffer_t EnergyRAM;

/** @brief Global persistent system data (EEPROM-mirrored). */
extern SystemData_t g_SystemData;

/*============================================================================
 *                                 Private Variables
 *============================================================================*/

/** @brief Event object holding the current command ID and Event type. */
SystemEvent_t SystemController;

/** @brief Global status object holding current system state, mode, and transient data. */
SystemState_t Status;

/** @brief Recovery elapsed time in milliseconds. */
static uint32_t RecoveryElapsedMs = 0;

/*============================================================================
 *                                 Private Helper Functions
 *============================================================================*/

/**
 * @brief      Updates the global system data structure with latest RAM values.
 * @details    Syncs voltage, current, power, and accumulated energy to the `g_SystemData`
 *             struct which is intended for EEPROM storage or external access.
 *             - Handles offset calculation for energy on the first run.
 * @return     void
 */
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

/**
 * @brief      Converts a floating-point number to a string representation "XX.XX".
 * @details    Fixed format conversion: 2 digits integer part, 2 digits decimal part.
 *             Used for formatting data for Bluetooth/UART transmission.
 * @param[in]  Num  Float number to convert.
 * @param[out] res  Character buffer to store the result (size >= 6).
 */
void FloatNumber_to_string(float Num, char res[])
{
    /* Integer part (Tens and Units) */
    res[0] = ((uint16_t)Num / 10) % 10 + '0';
    res[1] = ((uint16_t)Num) % 10 + '0';
    
    /* Decimal point */
    res[2] = '.';
    
    /* Fractional part (Tenths and Hundredths) */
    res[3] = ((uint16_t)(Num * 10)) % 10 + '0';
    res[4] = ((uint16_t)(Num * 100)) % 10 + '0';

    /* Null terminator */
    res[5] = NullChar;
}

/**
 * @brief      Updates the string buffer in `Status` struct with formatted measurement data.
 * @details    Constructs a protocol string like "I=00.00V=00.00P=00.00E=00.00"
 *             for transmission via Communication Manager.
 * @return     void
 */
void Update_Rms_Data(void)
{
    /* Build Status.Data safely with snprintf to avoid buffer overflow */
    Status.Data[0] = NullChar;
    char res[6];
    int written = 0;
    int remaining = (int)sizeof(Status.Data);

    FloatNumber_to_string(Status.RamData.current, res);
    written = snprintf((char *)Status.Data, remaining, "I=%s ", res);
    if (written < 0) written = 0;
    if (written >= remaining) return;
    remaining -= written;

    FloatNumber_to_string(Status.RamData.voltage, res);
    int w = snprintf((char *)(Status.Data + strlen(Status.Data)), remaining, "V=%s ", res);
    if (w < 0) w = 0;
    if (w >= remaining) return;
    remaining -= w;

    FloatNumber_to_string(Status.RamData.power, res);
    w = snprintf((char *)(Status.Data + strlen(Status.Data)), remaining, "P=%s ", res);
    if (w < 0) w = 0;
    if (w >= remaining) return;
    remaining -= w;

    FloatNumber_to_string(Status.RamData.energy_kwh, res);
    w = snprintf((char *)(Status.Data + strlen(Status.Data)), remaining, "E=%s", res);
    (void)w;
}

/*============================================================================
 *                                 Function Definitions
 *============================================================================*/

/**
 * @brief      Initializes the System Controller and all dependent subsystems.
 * @details    1. Sets initial state to INIT.
 *             2. Calls Init functions for DM, PM, ME, SystemData, Comm, and EnergyLogger.
 *             3. Loads initial state from EnergyLogger/EEPROM.
 *             4. Sets system to NORMAL state and Automatic mode.
 *             5. Starts the main scheduler task (Timer0).
 * @return     void
 */
void App_SystemController_Init(void)
{
    static uint8_t isScheduled = 0;
    static uint8_t isTimer1Init = 0;
    Status.SysState = INIT_State;
    RecoveryElapsedMs = 0;

    if (!isTimer1Init)
    {
        mTIMER1_Init();
        isTimer1Init = 1;
    }

    /* Initialize Sub-modules */
    DM_Init();
    PM_Init();
    ME_Init();
    SystemData_Init();
    App_CommManager_Init();
    App_EnergyLogger_Init();

    /* Load initial data if logs exist */
    extern uint16_t EEPROM_count;
    if (EEPROM_count > 0u)
    {
        App_EnergyLogger_ReadLog(0, &Status.RamData);
    }

    Status.Data[0]    = NullChar;
    Status.SysState   = NORMAL_State;
    Status.SystemMode = Automatic;

    /* Start Scheduling (only once) */
    if (!isScheduled)
    {
        mTIMER0_StartDelay(Scheduling_Time_sysController, App_SystemController_Update);
        isScheduled = 1;
    }
}

/**
 * @brief      Periodic Update Task for the System Controller.
 * @details    This function is called periodically (scheduled by Timer0).
 *             - Manages the Recovery State timer.
 *             - Updates Measurements and UI.
 *             - Checks Protection status and triggers State Transitions (Normal <-> Overload).
 *             - Checks HMI (Button) for Mode toggling.
 * @return     void
 */
void App_SystemController_Update(void)
{
    /* Recovery State Logic */
    if (Status.SysState == RECOVERY_State)
    {
        RecoveryElapsedMs += SYSTEM_TICK_MS;
        if (RecoveryElapsedMs >= RECOVERY_TIME_MS)
        {
            Status.SysState = NORMAL_State;
            RecoveryElapsedMs = 0;
        }
    }

    /* Update Measurements */
    ME_Update();
    float V = ME_GetVoltageRMS();
    float I = ME_GetCurrentRMS();
    float P = ME_GetActivePower();
    float E_Joules = ME_GetEnergy();
    float E_kWh = E_Joules / 3600000.0f;

    Status.RamData.voltage    = V;
    Status.RamData.current    = I;
    Status.RamData.power      = P;
    Status.RamData.energy_kwh = E_kWh;

    /* Protection Update */
    PM_Update();

    /* Protection Logic */
    if (PM_IsTripped() && Status.SysState == NORMAL_State)
    {
        /* Transition to Overload State */
        SystemData_SaveToEEPROM(); /* Save state before trip handling */
        SystemController.CmdID = CuttOFF;
        SystemController.Event = EVENT_OVERLOAD_DETECTED;
        App_SystemController_HandleEvent(SystemController);
    }
    else if (!PM_IsTripped() && Status.SysState == OVERLOAD_State)
    {
        /* Transition back to Normal (or Recovery) */
        SystemController.Event = EVENT_OVERLOAD_CLEARED;
        App_SystemController_HandleEvent(SystemController);
        App_EnergyLogger_Update(&Status.RamData);
    }

    /* Update Display */
    static uint32_t displayElapsedMs = 0;
    displayElapsedMs += SYSTEM_TICK_MS;
    if (displayElapsedMs >= DISPLAY_UPDATE_MS)
    {
        DM_ShowMeasurements(V, I, P, E_kWh);
        displayElapsedMs = 0;
    }

    /* Update Timestamp (seconds) */
    static uint32_t msAccumulator = 0;
    msAccumulator += ME_GetLastDeltaMs();
    while (msAccumulator >= 1000u)
    {
        timestampCounter++;
        msAccumulator -= 1000u;
    }

    /* Logging */
    EnergyLog_t currentLog;
    currentLog.timestamp  = timestampCounter;
    currentLog.voltage    = V;
    currentLog.current    = I;
    currentLog.power      = P;
    currentLog.energy_kwh = E_kWh;

    App_EnergyLogger_Update(&currentLog);
    App_EnergyLogger_Task();

    /* Mode Toggling Logic (Button) */
    if (hBtn_GetStatus() != Status.SystemMode)
    {
        Status.SystemMode = hBtn_GetStatus(); 
        SystemController.CmdID = ShowModeState;
        SystemController.Event = EVENT_MODE_TOGGLE;
        App_SystemController_HandleEvent(SystemController);
    }
}

/**
 * @brief      Handles system events and executes corresponding actions.
 * @param[in]  Action  SystemEvent_t structure containing Event ID and Command ID.
 * @return     void
 */
void App_SystemController_HandleEvent(SystemEvent_t Action)
{
    switch (Action.Event)
    {
    case EVENT_OVERLOAD_DETECTED:
        Status.SysState = OVERLOAD_State;
        App_CommManager_SendFrame((uint8_t*)DangerMessage, Action.CmdID, DangerMessage_length);
        break;

    case EVENT_OVERLOAD_CLEARED:
        Status.SysState = RECOVERY_State;
        RecoveryElapsedMs = 0;
        break;

    case EVENT_MODE_TOGGLE:
        DM_ShowMode(Status.SystemMode);
        if (Status.SystemMode == Automatic)
        {
            App_CommManager_SendFrame((uint8_t*)Change_Mode_To_AutoMatic, Action.CmdID, Automatic_length);
        }
        else
        {
            App_CommManager_SendFrame((uint8_t*)Change_Mode_To_Manual, Action.CmdID, Manual_Length);
        }
        break;

    case EVENT_CALIBRATION_DONE:
        /* Logic for calibration finalization */
        /* g_SystemData updates seemed incomplete in original code (no assignment) */
        /* Preserving structure but noting emptiness */
        // g_SystemData.CurrentCalib = ...;
        // g_SystemData.VoltageCalib = ...;
        g_SystemData.MagicNumber++;
        break;

    case EVENT_SENSOR_FAULT:
        /* Re-initialize system on sensor fault */
        App_SystemController_Init();
        Status.SysState = RECOVERY_State;
        RecoveryElapsedMs = 0;
        break;

    case EVENT_Power_Down:
        /* Enter Sleep Mode */
        SetBit(MCUCR_Reg, SE_bit);
        SetBit(MCUCR_Reg, SM1_bit);
        // sleep_cpu(); /* Requires proper sleep enable */
        break;

    case EVENT_Reset_event:
        App_CommManager_SendFrame((uint8_t*)PLEASE_RESET_Message, SHUTDOWN_Device, PLEASE_RESET_Message_length);
        App_SystemController_Init();
        Status.SysState = RECOVERY_State;
        RecoveryElapsedMs = 0;
        break;

    default:
        break;
    }
}

/**
 * @brief      Gets the current system state snapshot.
 * @details    Updates the string formatting of data before returning.
 * @return     SystemState_t Current system state structure.
 */
SystemState_t App_SystemController_GetState(void)
{
    Update_Rms_Data();
    return Status;
}

#endif /* System_Controller_Module == Enable */
