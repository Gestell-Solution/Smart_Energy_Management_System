/**
 * @file       main.c
 * @brief      Main entry point for the Smart Energy Management System.
 * @version    1.0
 * @date       2025-12-12
 * @author     Gestell Team
 * @copyright  Copyright (c) 2025, Gestell Company
 *
 * @mainpage Smart Energy Management System Documentation
 *
 * @section intro_sec Introduction
 * The **Smart Energy Management System** is an embedded solution designed to monitor, control, and log electrical parameters
 * (Voltage, Current, Power, Energy) in real-time. It features a responsive display, wireless communication (Bluetooth/Wi-Fi),
 * and robust protection mechanisms against over-current and over-voltage conditions.
 *
 * @section arch_sec Architecture
 * The software is organized into a layered architecture to ensure modularity and portability:
 *
 * - **@ref App "Application Layer (App)"**: High-level business logic, including:
 *   - @ref Measurement_Engine "Measurement Engine"
 *   - @ref Energy_Logger "Energy Logger"
 *   - @ref Protection_Manager "Protection Manager"
 *   - @ref System_Controller "System Controller"
 *   - @ref Communication_Manager "Communication Manager"
 *   - @ref Display_Manager "Display Manager"
 *
 * - **@ref Hal "Hardware Abstraction Layer (HAL)"**: Drivers for external hardware modules:
 *   - Sensors (Voltage, Current)
 *   - Actuators (Relays, Buzzer, RGB LED)
 *   - HMI (LCD, Buttons)
 *   - Communication Modules (HC-05 Bluetooth, ESP-01 Wi-Fi)
 *
 * - **@ref Mcal "Microcontroller Abstraction Layer (MCAL)"**: Low-level drivers for the ATmega32 peripherals:
 *   - DIO, ADC, Timers, UART, SPI, TWI, EEPROM, EXTI, GIE
 *
 * - **@ref Common "Common Utilities"**: Shared macros, configuration, and data structures.
 *
 * @section usage_sec Usage
 * The system initializes all drivers in `main()` and enters a superloop that:
 * 1. Updates measurements via the Measurement Engine.
 * 2. Checks protection conditions via the Protection Manager.
 * 3. Updates the User Interface (Display).
 * 4. Logs data to EEPROM via the Energy Logger.
 * 5. Handles remote commands via the Communication Manager.
 *
 * @section contact_sec Contact
 * For support, contact the development team at Gestell Company.
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
#include "../Tests/UnitTest/Mcal/Timer1/Timer1_test_Interface.h"

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
    void Timer1_Test_Init();
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
