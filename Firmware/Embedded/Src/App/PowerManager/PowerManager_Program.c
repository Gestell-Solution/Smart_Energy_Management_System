/**
 * @file       PowerManager_Program.c
 * @brief      Power-Down Detection and Emergency Save Implementation
 * @details    This module provides power monitoring and emergency EEPROM save
 *             to prevent data loss during unexpected power failures.
 *             
 *             Features:
 *             - Voltage monitoring via ADC or external comparator
 *             - Emergency interrupt-driven EEPROM save
 *             - Configurable detection methods
 * 
 * @version    1.0
 * @date       2026-02-04
 * @author     Gestell Team
 * @copyright  Copyright (c) 2026, Gestell Company
 */

#include "PowerManager_Interface.h"
#include "../../Common/SystemDataManager/SystemDataManager.h"
#include "../../Mcal/ADC/ADC_Interface.h"
#include "../../Mcal/EXTI/EXTI_Interface.h"
#include "../../Mcal/GIE/GIE_Interface.h"

/*============================================================================
 *                                 Private Variables
 *============================================================================*/

static uint8_t PowerStableFlag = 1;  // 1 = Stable, 0 = Power-Down detected

/*============================================================================
 *                                 Function Definitions
 *============================================================================*/

/**
 * @brief Initialize Power-Down Detection
 */
void PowerManager_Init(void)
{
#if POWERDOWN_DETECTION_METHOD == 0
    /* ADC-based Power Monitoring */
    mADC_Init();
    // Configure ADC channel for VCC monitoring
    // Note: Requires hardware voltage divider from VCC to ADC pin
    
#elif POWERDOWN_DETECTION_METHOD == 1
    /* External Comparator/Supervisor IC */
    mEXTI_Init(POWERDOWN_EXTI_PIN, EXT_FALLING_EDGE);
    mEXTI_setCallback(POWERDOWN_EXTI_PIN, PowerManager_EmergencySave);
    mEXTI_Enable(POWERDOWN_EXTI_PIN);
    mGIE_Enable();
    
#elif POWERDOWN_DETECTION_METHOD == 2
    /* Power monitoring disabled */
    /* User must manually save or rely on periodic save only */
    PowerStableFlag = 1;
    
#endif

    PowerStableFlag = 1;
}

/**
 * @brief Emergency Save on Power-Down
 * @details This is called from ISR or polling when power drop detected
 *          Must be FAST - save only critical data
 */
void PowerManager_EmergencySave(void)
{
    PowerStableFlag = 0;
    
    /* CRITICAL: Save system data immediately */
    SystemData_SaveToEEPROM();
    
    /* Optional: Turn off peripherals to reduce power consumption */
    // hRelay_Off(hRELAY_0);
    // hRelay_Off(hRELAY_1);
    // hRelay_Off(hRELAY_2);
    // hRelay_Off(hRELAY_3);
    
    /* Wait for EEPROM write to complete before power loss */
    // Typically EEPROM write takes ~3.4ms on ATmega32
    // Ensure capacitor bank can sustain power for this duration
}

/**
 * @brief Check Power Stability
 */
uint8_t PowerManager_IsPowerStable(void)
{
#if POWERDOWN_DETECTION_METHOD == 0
    /* Poll ADC for voltage level */
    uint16_t vcc_reading = mADC_ReadChannel(POWERDOWN_ADC_CHANNEL);
    
    if (vcc_reading < POWERDOWN_VOLTAGE_THRESHOLD)
    {
        /* Power drop detected */
        PowerManager_EmergencySave();
        return 0;
    }
#endif

    return PowerStableFlag;
}
