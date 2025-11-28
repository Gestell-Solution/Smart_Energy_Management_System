/**
 * @file       DisplayManager_Interface.h
 * @brief      This file contains The Interface for Display Manager .    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Mohamed Diaa & Emg -Ahmed Twap
 * @date       2025-11-7
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
*/


#ifndef DISPLAYMANAGER_INTERFACE_H_
#define DISPLAYMANAGER_INTERFACE_H_

#include <stdint.h>


/**
 * @brief Initialize the Display Manager and underlying LCD.
 * @remarks Must be called once before any other Display Manager API.
 */
void DM_Init(void);

/**
 * @brief Refresh the display with the latest internal values.
 * @remarks Non-blocking routine intended to be called periodically.
 */
void DM_Update(void);

/**
 * @brief Update measurement values used by the display.
 * @param V Voltage (Volts).
 * @param I Current (Amperes).
 * @param P Power (Watts).
 * @param E Energy (kWh).
 */
void DM_ShowMeasurements(float V, float I, float P, float E);

/**
 * @brief Set the display mode.
 * @param isAuto true = AUTO mode, false = MANUAL mode.
 */
void DM_ShowMode(uint8_t isAuto);

/**
 * @brief Set protection/trip state.
 * @param isTripped true = protection active (trip), false = normal.
 */
void DM_ShowProtectionState(uint8_t isTripped);

/**
 * @brief Show a short custom message (temporary).
 * @param msg Null-terminated C string to display.
 */
void DM_ShowMessage(const char *msg);

#endif /* DISPLAYMANAGER_INTERFACE_H_ */
