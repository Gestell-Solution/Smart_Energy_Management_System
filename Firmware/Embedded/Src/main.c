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
 * - **Application Layer (App)**: High-level business logic (Measurement Engine, Energy Logger, Protection Manager,
 *   System Controller, Communication Manager, Display Manager).
 * - **Hardware Abstraction Layer (HAL)**: Drivers for sensors, actuators, HMI, and communication modules.
 * - **Microcontroller Abstraction Layer (MCAL)**: Low-level drivers for the ATmega32 peripherals.
 * - **Common**: Shared macros, configuration, and data structures.
 *
 * @section usage_sec Usage
 * The system uses the **System Controller** as the single entry point: it initializes all sub-modules (ME, PM, DM, Comm, EnergyLogger, SystemData)
 * and schedules the periodic update via Timer0. The main loop keeps the CPU running; all measurement, protection, display,
 * logging, and communication are driven by App_SystemController_Update().
 *
 * @section contact_sec Contact
 * For support, contact the development team at Gestell Company.
 */

#include "Common/Config.h"
#include "Common/Macros.h"
#include "Mcal/GIE/GIE_Interface.h"
#include "App/System_Controller/System_Controller_Interface.h"
#include <util/delay.h>

int main(void)
{
    /* 1. Global Interrupt Enable */
    mGIE_Enable();

    /* 2. Initialize System Controller (initializes ME, PM, DM, Comm, EnergyLogger, SystemData and starts Timer0 scheduler) */
#if System_Controller_Module == Enable
    App_SystemController_Init();
#endif

    /* 3. Main loop: Timer0 drives App_SystemController_Update periodically; loop keeps CPU running */
    while (1)
    {
        _delay_ms(100);
    }
}
