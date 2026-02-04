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
#include "App/System_Controller/System_Controller_Interface.h"
#include "App/CommunicationManager/App_CommManager.h"
#include "App/DM_Driver/DisplayManager_Interface.h"
#include "App/ProtectionManager/ProtectionManager_Interface.h"
#include "Mcal/Timer1/TIMER1_Interface.h"
#include "Common/SystemDataManager/SystemDataManager.h"
#include <util/delay.h>

int main(void)
{
    /* 1. Initialize Mcal Modules */
    
    /* Timer 1 Initialization */
    mTIMER1_Init();
    /* Global Interrupt Enable */
    mGIE_Enable();
    
    /* 2. Initialize Application via System Controller (central orchestrator) */
    App_SystemController_Init();

    /* 3. Main Superloop (delegates to System Controller) */

while (1)
    {
        /* Centralized update (Measurement -> Protection -> UI -> Log) */
        App_SystemController_Update();

        /* Communication processing remains in main loop */
        App_CommManager_Task();
        /* Periodic SystemData save to reduce EEPROM wear (every 60 seconds) */
        static uint16_t s_saveCounter = 0;
        s_saveCounter++;
        if (s_saveCounter >= 600u) /* 600 * 100ms = 60s */
        {
            SystemData_PeriodicSaveIfDirty();
            s_saveCounter = 0u;
        }

        /* Stability delay */
        _delay_ms(100);
    }
    
    return 0;
}
