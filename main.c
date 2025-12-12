/**
 * @file       main.c
 * @brief      Main application entry point for the Smart Energy Management System.
 * @details    The main function orchestrates the initialization of MCAL, HAL, and Application modules.
 *             It executes the main superloop, handling measurement updates, display, energy logging,
 *             and communication tasks.
 * @version    1.0
 * @date       2025-12-12
 * @author     Gestell Team
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#include "Common/Config.h"
#include "Common/Macros.h"
#include "Mcal/GIE/GIE_Interface.h"
#include "App/MeasurementEngine/MeasurementEngine_Interface.h"
#include "App/EnergyLogger/EnergyLogger_Interface.h"
#include "App/CommunicationManager/App_CommManager.h"
#include "App/DM_Driver/DisplayManager_Interface.h"
#include "App/ProtectionManager/ProtectionManager_Interface.h"
#include <util/delay.h>

int main(void)
{
    /* 1. Global Interrupt Enable */
    mGIE_Enable();

    /* 2. Initialize Application Modules */
    
    /* Measurement Engine: Configures ADC, Voltage and Current Sensors */
    ME_Init();
    
    /* Energy Logger: Configures buffers and EEPROM management */
    /* Note: Internal timer and sensor sampling in Logger disabled to avoid conflict with ME */
    App_EnergyLogger_Init();
    
    /* Protection Manager: Configures safety checks and relay control */
    PM_Init();
    
    /* Display Manager: Initializes LCD and display state */
    DM_Init();
    
    /* Communication Manager: Initializes Buffer and State machines */
    App_CommManager_Init();

    /* 3. Main Superloop */
    while (1)
    {
        /* --- Measure --- */
        /* Update electrical measurements (V, I, P, E) */
        ME_Update();
        
        /* Retrieve latest values */
        float V = ME_GetVoltageRMS();
        float I = ME_GetCurrentRMS();
        float P = ME_GetPower();
        float E_Joules = ME_GetEnergy();
        
        /* Convert Joules to kWh for Logging and Display */
        float E_kWh = E_Joules / 3600000.0f;

        /* --- Protection --- */
        /* Check for over-current/voltage/etc. */
        PM_Update();
        
        /* --- Display --- */
        /* Update Display with latest values */
        DM_ShowMeasurements(V, I, P, E_kWh); 
        DM_Update();
        
        /* --- Logging --- */
        /* Feed buffer to the logger */
        EnergyLog_t currentLog;
        currentLog.timestamp = timestampCounter; /* Use counter from logger interface extern if available, or 0 */
        currentLog.voltage = V;
        currentLog.current = I;
        currentLog.power = P;
        currentLog.energy_kwh = E_kWh;
        
        App_EnergyLogger_Update(&currentLog);
        
        /* Run Logger Task (Flushes to EEPROM periodically) */
        App_EnergyLogger_Task();
        
        /* --- Communication --- */
        /* Handle incoming commands (Bluetooth) and outgoing responses */
        App_CommManager_Task();

        /* Stability delay */
        _delay_ms(100);
    }
    
    return 0;
}
