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

/* Test injection source when PM_USE_TEST_CURRENT is enabled. */
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

static float PM_ReadCurrent(void)
{
#if PM_USE_TEST_CURRENT
     return PM_Test_Current;
#else
     return ME_GetCurrentRMS();
#endif
}

static float PM_SanitizeCurrentLimit(uint16_t configuredLimit)
{
     if ((configuredLimit >= PM_MIN_OVERCURRENT_LIMIT) &&
         (configuredLimit <= PM_MAX_OVERCURRENT_LIMIT))
     {
          return (float)configuredLimit;
     }
     return (float)Irms_Threshold;
}

static float PM_SanitizeVoltageLimit(uint16_t configuredLimit)
{
     if ((configuredLimit >= PM_MIN_OVERVOLTAGE_LIMIT) &&
         (configuredLimit <= PM_MAX_OVERVOLTAGE_LIMIT))
     {
          return (float)configuredLimit;
     }
     return (float)Vrms_Threshold;
}

static void PM_SnapshotFault(float voltage, float current, float power)
{
     g_SystemData.Voltage_RMS = (uint16_t)voltage;
     g_SystemData.Current_RMS = (uint16_t)current;
     g_SystemData.Power = (uint16_t)power;
}

static float PM_ResolvePowerLimit(float currentLimit, float voltageLimit)
{
#if (PM_POWER_LIMIT_MODE == PM_POWER_LIMIT_MODE_PRODUCT)
     float derivedPowerLimit = currentLimit * voltageLimit;
     if (derivedPowerLimit > 0.0f)
     {
          return derivedPowerLimit;
     }
#endif
     return (float)P_Threshold;
}

/* Fix-007 Helper to avoid repeat */
static void PM_Trip_Action(void)
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

    /* Startup relay policy is configurable from PM_STARTUP_CONNECT_LOADS. */
    for (uint8_t Relay_id = hRELAY_0; Relay_id <= hRELAY_3; Relay_id++)
    {
        hRelay_Init(Relay_id);
#if PM_STARTUP_CONNECT_LOADS
        hRelay_On(Relay_id);
#else
        hRelay_Off(Relay_id);
#endif
    }

     
     /* Configure External Interrupt for RESET button */
     mEXTI_Init(EXT1_Macro, EXT_RISING_EDGE);
     mDIO_SetDirectionForPin(Reset_BTN_Group, Reset_BTN_Pin, Input);
     mDIO_WritePin(Reset_BTN_Group, Reset_BTN_Pin, High); /* Enable Pull-up */
     mEXTI_setCallback(EXT1_Macro, PM_Reset);

    /* Fix-007 Force Enable Global Interrupts */
    /* To ensure the Reset button response */
     mGIE_Enable();     
     
     mADC_StartGroup();      /* Start ADC conversions */
     hRGB_SetState(RGB_GREEN); /* Default to Safe State */
     DM_ShowProtectionState(Safe);
     Buzzer_Off();
     Protection_State = Safe;
     PM_ResetRequested = 0u;
     Fix_Check = Fixed;
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
     static uint8_t overCurrentCounter = 0u;
     static uint8_t safeStableCounter = 0u;
#if PM_ENABLE_POWER_TRIP
     static uint8_t overPowerCounter = 0u;
#endif

     float RMS_voltage_Read = ME_GetVoltageRMS();
     float Power_Read = ME_GetActivePower();
     float RMS_Current_Read = PM_ReadCurrent();
     float currentLimit = PM_SanitizeCurrentLimit(g_SystemData.OvercurrentLimit);
     float voltageLimit = PM_SanitizeVoltageLimit(g_SystemData.OvervoltageLimit);
     float powerLimit = PM_ResolvePowerLimit(currentLimit, voltageLimit);
     float powerClearLimit = powerLimit - PM_POWER_HYSTERESIS_W;
     float currentClearLimit = currentLimit - PM_CURRENT_HYSTERESIS;

     if (currentClearLimit < 0.0f)
     {
          currentClearLimit = 0.0f;
     }

     if (powerClearLimit < 0.0f)
     {
          powerClearLimit = 0.0f;
     }
     
     /* 1.Trip for Voltage (Immediate) */
     if (RMS_voltage_Read > voltageLimit)
     {
          PM_Trip_Action();
          PM_SnapshotFault(RMS_voltage_Read, RMS_Current_Read, Power_Read);
          PM_ResetRequested = 0u;
          safeStableCounter = 0u;
#if PM_ENABLE_POWER_TRIP
          overPowerCounter = 0u;
#endif
          return;
     }

     /* 2. Short Circuit Protection (Immediate Trip) */
     if (RMS_Current_Read >= (currentLimit * PM_SHORT_CIRCUIT_MULTIPLIER))
     {
          PM_Trip_Action();
          PM_SnapshotFault(RMS_voltage_Read, RMS_Current_Read, Power_Read);
          overCurrentCounter = PM_TRIP_DELAY_TICKS; /* Max out counter */
          PM_ResetRequested = 0u;
          safeStableCounter = 0u;
#if PM_ENABLE_POWER_TRIP
          overPowerCounter = 0u;
#endif
          return;
     }

     /* 3. Overload Protection (Debounced) */
     if (RMS_Current_Read > currentLimit)
     {
          if (overCurrentCounter < PM_TRIP_DELAY_TICKS)
          {
               overCurrentCounter++;
          }

          if (overCurrentCounter >= PM_TRIP_DELAY_TICKS)
          {
               PM_Trip_Action();
               PM_SnapshotFault(RMS_voltage_Read, RMS_Current_Read, Power_Read);
          }
          PM_ResetRequested = 0u;
          safeStableCounter = 0u;
#if PM_ENABLE_POWER_TRIP
          overPowerCounter = 0u;
#endif
          return;
     }

#if PM_ENABLE_POWER_TRIP
     /* 4. Overpower Protection (Debounced) */
     if (Power_Read > powerLimit)
     {
          if (overPowerCounter < PM_POWER_TRIP_DELAY_TICKS)
          {
               overPowerCounter++;
          }

          if (overPowerCounter >= PM_POWER_TRIP_DELAY_TICKS)
          {
               PM_Trip_Action();
               PM_SnapshotFault(RMS_voltage_Read, RMS_Current_Read, Power_Read);
          }
          PM_ResetRequested = 0u;
          safeStableCounter = 0u;
          return;
     }

     if (Power_Read <= powerClearLimit)
     {
          if (overPowerCounter > 0u)
          {
               overPowerCounter--;
          }
     }
#endif

     if (RMS_Current_Read <= currentClearLimit)
     {
          if (overCurrentCounter > 0)
          {
               overCurrentCounter--;
          }

          /* Track stable-safe time before allowing reset */
          if (safeStableCounter < PM_RESET_DELAY_TICKS)
          {
               safeStableCounter++;
               PM_ResetRequested = 0u; /* Ignore early reset presses until stable window is done. */
               return;
          }

          /* Apply reset only if requested and system is stable-safe */
          if (PM_ResetRequested && (safeStableCounter >= PM_RESET_DELAY_TICKS))
          {
               PM_ResetRequested = 0u;
               if (Protection_State == Danger)
               {
                    Protection_State = Safe;
                    for (uint8_t Relay_id = hRELAY_0; Relay_id <= hRELAY_3; Relay_id++)
                    {
                         hRelay_Off(Relay_id);
                    }
                    Buzzer_Off();
                    hRGB_SetState(RGB_GREEN);
                    DM_ShowProtectionState(Safe);
                    Fix_Check = Fixed; /* Flag that reset was attempted/successful */
                    overCurrentCounter = 0u;
                    safeStableCounter = 0u;
#if PM_ENABLE_POWER_TRIP
                    overPowerCounter = 0u;
#endif
               }
          }
          return;
     }

     /* Current is inside the hysteresis band: keep waiting and block reset. */
     PM_ResetRequested = 0u;
     safeStableCounter = 0u;
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
     if (Protection_State == Danger)
     {
          PM_ResetRequested = 1u;
     }
}

#endif /* ProtectionManager == Enable */
