/**
 * @file Buzzer_Interface.h
 * @brief Public interface for the buzzer driver.
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @copyright Copyright (c) 2025 , Gestell Company
 */
#ifndef BUZZER_INTERFACE_H_
#define BUZZER_INTERFACE_H_
#include"../Buzzer/Buzzer_Config.h"
#include"../Buzzer/Buzzer_Private.h"
#include"../../Common/Macros.h"
#include <stdint.h>

/**
 * @enum Buzzer_Mode_t
 * @details Define buzzer mode
 */
typedef enum 
{   BUZZER_OFF,
    BUZZER_ON,
    BUZZER_BEEP_ONCE,
    BUZZER_BEEP_PERIODIC,
    BUZZER_ALERT
} Buzzer_Mode_t;
/**
 * @struct Buzzer_State_t
 * @brief Holds the current operational state of the buzzer.
 * @details This structure stores both the current buzzer mode (ON, OFF, ALERT, etc.)
 * and whether the buzzer output is physically active (pin HIGH or LOW).
 */
typedef struct {
    Buzzer_Mode_t mode;   /**< Current operating mode of the buzzer (see @ref Buzzer_Mode_t). */
    uint8_t is_on;        /**< Indicates whether the buzzer output is currently ON (1) or OFF (0). */
} Buzzer_State_t;

/**
 * @brief  Initialize buzzer GPIO pin.
 */
void Buzzer_Init(void); 
/**
 * @brief  Turn buzzer ON.
 */
void Buzzer_On(void); 
/**
 * @brief  Turn buzzer OFF.
 */
void Buzzer_Off(void); 
/**
 * @brief  Generate a single beep .
 * @param duration Duration of the beep in milliseconds.
 */
void Buzzer_Beep(uint16_t duration);

/**
 * @brief  Set buzzer behavior based on system state.
 * @param mode Desired mode from Buzzer_Mode_t.
 *  - BUZZER_OFF: Turn off buzzer.
 *  - BUZZER_ON: Keep buzzer continuously ON.
 *  - BUZZER_BEEP_ONCE: Short beep for a defined duration.
 *  - BUZZER_BEEP_PERIODIC: Beeps periodically with a defined interval.
 *  - BUZZER_ALERT: Fast repeated beeps (alert mode).
 * @details This function automatically uses Timer0 to control timing for beep and periodic modes.
 */
void Buzzer_SetMode(Buzzer_Mode_t mode); 

/**
 * @brief Callback function used for repeating the buzzer in alert mode. 
 * @details This function is triggered automatically by the timer when the previous alert beep ends.
 */
void Buzzer_Alert_Callback(void);
/**
 * @brief Callback function used for repeating the buzzer in periodic mode.
 * @details function is triggered automatically by the timer when the previous periodic beep ends.

 */
void Buzzer_Periodic_Callback(void) ;


#endif /* BUZZER_INTERFACE_H_ */
