/**
 * @file       PowerManager_Config.h
 * @brief      Power Manager Configuration Parameters
 * @version    1.0
 * @date       2026-02-04
 * @author     Gestell Team
 * @copyright  Copyright (c) 2026, Gestell Company
 */

#ifndef _POWER_MANAGER_CONFIG_H_
#define _POWER_MANAGER_CONFIG_H_

/**
 * @brief Power-Down Detection Method
 * @details Options:
 *          - 0: ADC-based monitoring (check VCC via internal reference)
 *          - 1: External comparator on dedicated pin
 *          - 2: Disabled (for systems without power monitoring)
 */
#define POWERDOWN_DETECTION_METHOD  2  // Disabled by default (requires hardware setup)

/**
 * @brief ADC Channel for Power Monitoring (if METHOD = 0)
 * @details ATmega32: Channel for voltage divider from VCC
 */
#define POWERDOWN_ADC_CHANNEL  7

/**
 * @brief Power-Down Voltage Threshold (ADC counts)
 * @details For 5V system with 10-bit ADC:
 *          Threshold = (Vthreshold / Vref) * 1024
 *          Example: 4.5V threshold with 5V ref = (4.5/5)*1024 = 921
 */
#define POWERDOWN_VOLTAGE_THRESHOLD  921

/**
 * @brief External Interrupt Pin for Power-Down Signal (if METHOD = 1)
 */
#define POWERDOWN_EXTI_PIN  EXTI2

#endif /* _POWER_MANAGER_CONFIG_H_ */
