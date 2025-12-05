/**
 *@file Buzzer_Config.h
 *@brief Configuration file for the Buzzer driver.
 *@details
 * Defines the buzzer pin connections and timing parameters for different buzzer modes
 * such as single beep, periodic beep, and alert beep.

 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @copyright Copyright (c) 2025 , Gestell Company
 */
#ifndef BUZZER_CONFIG_H_
#define BUZZER_CONFIG_H_
#include"../../Mcal/DIO/DIO_Interface.h"
/**
 * @defgroup
 * @brief Configurable Pin for buzzer
 */
#define BUZZER_PORT     GroupD
#define BUZZER_PIN      PIN4
/**
 * @defgroup
 * @brief beep durations in milliseconds
 */
#define BUZZER_BEEP_DURATION       500
#define BUZZER_PERIODIC_DURATION   1500
#define BUZZER_ALERT_DURATION        50
#endif /* BUZZER_CONFIG_H_ */
