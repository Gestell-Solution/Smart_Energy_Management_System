/**
 * @file       ProtectionManager_Program.c
 * @brief      Implementation of the Protection Manager Module.
 * @details    This module is responsible for monitoring system parameters (Voltage, Current, Power)
 *             and executing protective actions (tripping relays, activating buzzer/LED) when
 *             safe limits are exceeded. It implements a latching protection mechanism that requires
 *             a manual reset or specific condition to clear.
 * @version    2.0
 * @date       2025-11-11
 * @author     Developer: Ahmed Ashraf (ahmedashraf2022222@gmail.com)
 * @author     Developer(Bug Fixing) : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer:  Mohamed Diaa (mohammeddiaato@gmail.com)
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#include "../../Common/Config.h"
#include"../../Mcal/Atmega32RegistersAddress.h"

#if ProtectionManager == Enable
#include "ProtectionManager_Interface.h"
#include "../../Mcal/GIE/GIE_Interface.h"

/*============================================================================
 *                                 Global Variables
 *============================================================================*/

/**
 * @brief Global system data structure (External).
 * @details Used to update the latest fault values (Voltage, Current, Power) when a trip occurs.
 */
extern SystemData_t g_SystemData;

/* Fix-007 Test Injection Current Variable for Main.c compatibility */
float PM_Test_Current = 0.0f; 


/*============================================================================
 *                                 Private Variables
 *============================================================================*/

/**
 * @brief Current protection state of the system.
 * @details 
 * - `Safe` (0): Normal operation.
 * - `Danger` (1): A fault condition is active.
 */
 uint8_t Protection_State = Safe;

/**
 * @brief Fix check status flag.
 * @details Used to manage the state transition from Fault -> Fixed -> Safe.
 * - `0`: Initial state / Normal.
 * - `1`: Fault active (Trip).
 * - `Fixed`: User has acknowledged or condition is cleared (ready to reset).
 * - `Not_Fixed`: Fault persists or reset not yet requested.
 */
static uint8_t Fix_Check = 0;
static volatile uint8_t PM_ResetRequested = 0;
/*============================================================================
 *                                Helper Function 
 *============================================================================*/

/* Fix-007 Helper to avoid repeat */
static void PM_Trip_Action
(void)
{
     if (Protection_State == Safe)
     {
          Protection_State = Danger;
          /* Trip all relays */
          for (uint8_t Relay_id = hRELAY_0; Relay_id <= hRELAY_3; Relay_id++)
          {
               hRelay_Off(Relay_id);
          }
          /* Visual and Audio Alarm */
          hRGB_SetState(RGB_RED);
          DM_ShowProtectionState(Danger);
          Buzzer_On();
          
          Fix_Check = Not_Fixed;
     }
}
/*============================================================================
 *                                 Function Definitions
 *============================================================================*/

/**
 * @brief      Initializes the Protection Manager.
 * @details    Sets up necessary peripherals:
 *             - External Interrupts (EXTI) for reset button.
 *             - Timer1 (if used for timing protection).
 *             - Output actuators: Buzzer, Relays, RGB LED.
 *             - Input sensors: Voltage, Current.
 *             - Display Manager (for showing status).
 *             Configures the system to start in the Safe (Green) state.
 * @return     void
 */
void PM_Init()
{
     mEXTI_Enable(EXT1_Macro);
     Buzzer_Init();          /* Initialize Alarm Buzzer */
     hCurrent_Init();        /* Initialize Current Sensor */
     hVoltage_Init();        /* Initialize Voltage Sensor */
     hRGB_Init();            /* Initialize Status LED */
     DM_Init();              /* Initialize Display */

    /* Fix-007 Turn ON Relays at startup (Start Safe) */
    for (uint8_t Relay_id = hRELAY_0; Relay_id <= hRELAY_3; Relay_id++)
    {
        hRelay_Init(Relay_id);
        hRelay_Off(Relay_id);
    }

     
     /* Configure External Interrupt for RESET button */
     mEXTI_Init(EXT1_Macro, EXT_RISING_EDGE);
     mDIO_SetDirectionForPin(GroupD, PIN3, Input);
     mDIO_WritePin(GroupD, PIN3, High); /* Enable Pull-up */
     mEXTI_setCallback(EXT1_Macro, PM_Reset);

    /* Fix-007 Force Enable Global Interrupts */
    /* To ensure the Reset button response */
     mGIE_Enable();     
     
     mADC_StartGroup();      /* Start ADC conversions */
     hRGB_SetState(RGB_GREEN); /* Default to Safe State */
}

/**
 * @brief      Periodically checks system parameters against safety thresholds.
 * @details    This function should be called in the main loop.
 *             1. Uses latest measurement readings (ME_Update must be called earlier in the tick).
 *             2. Compares readings with `Vrms_Threshold`, `Irms_Threshold`, and `P_Threshold`.
 *             3. If limits exceeded:
 *                - Sets state to `Danger`.
 *                - Opens all Relays (CUTOFF).
 *                - Turns RGB LED to RED.
 *                - Activates Buzzer.
 *                - Logs fault values to `g_SystemData`.
 *             4. Handles UI updates for Safe/Danger states.
 * @return     void
 */
void PM_Update()
{
     static uint8_t overCurrentCounter = 0;
     static uint8_t safeStableCounter = 0;

     /* Fix-007 Use Test Variable instead of Sensor for Logic Testing */
     float RMS_voltage_Read = ME_GetVoltageRMS();
     float Power_Read = ME_GetActivePower();
     float RMS_Current_Read =ME_GetCurrentRMS();// We Can Replace ME_GetCurrentRMS(); by Varaible PM_Test for injecting 
     //different values of current.
     
     /* 1.Trip for Voltage or Power */
     if (RMS_voltage_Read > Vrms_Threshold || Power_Read > P_Threshold)
     {
          PM_Trip_Action();
          safeStableCounter = 0;
          return;
     }

     /* 2. Short Circuit Protection (Immediate Trip) */
     if (RMS_Current_Read >= (Irms_Threshold * PM_SHORT_CIRCUIT_MULTIPLIER))
     {
          PM_Trip_Action();
          overCurrentCounter = PM_TRIP_DELAY_TICKS; /* Max out counter */
          safeStableCounter = 0;
          return;
     }

     /* 3. Overload Protection (Debounced) */
     if (RMS_Current_Read > Irms_Threshold)
     {
          if (overCurrentCounter < PM_TRIP_DELAY_TICKS)
          {
               overCurrentCounter++;
          }

          if (overCurrentCounter >= PM_TRIP_DELAY_TICKS)
          {
               PM_Trip_Action();
               
               /* Snapshot fault values */
               g_SystemData.Current_RMS = RMS_Current_Read;
          }
          safeStableCounter = 0;
     }
     else
     {
          /* HYSTERESIS */
          if (overCurrentCounter > 0)
          {
               overCurrentCounter--;
          }

          /* Track stable-safe time before allowing reset */
          if (safeStableCounter < PM_RESET_STABLE_TICKS)
          {
               safeStableCounter++;
          }

          /* Apply reset only if requested and system is stable-safe */
          if (PM_ResetRequested && (safeStableCounter >= PM_RESET_STABLE_TICKS))
          {
               PM_ResetRequested = 0;
               if (Protection_State == Danger)
               {
                    Protection_State = Safe;
                    for (uint8_t Relay_id = hRELAY_0; Relay_id <= hRELAY_3; Relay_id++)
                    {
                         hRelay_On(Relay_id);
                    }
                    Buzzer_Off();
                    hRGB_SetState(RGB_GREEN);
                    DM_ShowProtectionState(Safe);
                    Fix_Check = Fixed; /* Flag that reset was attempted/successful */
               }
          }
     }
}

/**
 * @brief      Gets the current protection status.
 * @return     uint8_t status
 *             - `Safe` (0)
 *             - `Danger` (1)
 */
uint8_t PM_IsTripped()
{
     return Protection_State;
}

/**
 * @brief      Callback function to reset the protection state.
 * @details    Called via External Interrupt (Button Press).
 *             If the system considers the fault condition "Fixed" (state logic),
 *             this restores normal operation:
 *             - Closes Relays (Power ON).
 *             - Turns off Buzzer.
 *             - Sets RGB LED to GREEN.
 * @return     void
 */
void PM_Reset()
{
     /* EXTI callback: only mark request. Actual reset is handled in PM_Update. */
     PM_ResetRequested = 1;
}

#endif /* ProtectionManager == Enable */
