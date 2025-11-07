/**
 * @file       BTN_Program.c
 * @brief      This file contains The Program for Display Manager .    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Mohamed Diaa & Emg -Ahmed Twap
 * @date       2025-11-7
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
*/

/* --------------------------------------------------------------------------
*                               Include LIB
* --------------------------------------------------------------------------*/

#include "DisplayManager_Interface.h"
#include "DisplayManager_Config.h"
#include "DisplayManager_Private.h"
#include "../../Hal/LCD/LCD_Interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <string.h>

/* Internal (module) state */
static float DM_Voltage   = 0.0f;
static float DM_Current   = 0.0f;
static float DM_Power     = 0.0f;
static float DM_Energy    = 0.0f;

static bool  DM_IsAuto    = true;
static bool  DM_IsTripped = false;

/**
 * @brief Clear all display lines by writing spaces.
 * @note Uses hLCD_SetCursor / hLCD_WriteString from LCD HAL.
 */
static void DM_ClearDisplay(void)
{
    for (uint8_t line = 0u; line < DM_LCD_ROWS; ++line)
    {
        hLCD_SetCursor(line, 0u);
        hLCD_WriteString(DM_CLEAR_LINE_STRING);
        hLCD_SetCursor(line, 0u);
    }
}

/**
 * @brief Initialize the display manager and the LCD HAL.
 */
void DM_Init(void)
{
    hLCD_Init();
    DM_ClearDisplay();
}

/**
 * @brief Update the LCD with current internal values.
 *        Chooses between normal layout and trip layout.
 */
void DM_Update(void)
{
    char numStr[16];
    char pStr[12];
    char eStr[12];

    /* Clear lines before writing to avoid leftover characters */
    DM_ClearDisplay();

    if (DM_IsTripped)
    {
        /* Trip layout: centered two-line message */
        hLCD_SetCursor(DM_LCD_LINE2, 0u);
        hLCD_WriteString(DM_MSG_OVERLOAD_L1);

        hLCD_SetCursor(DM_LCD_LINE3, 0u);
        hLCD_WriteString(DM_MSG_OVERLOAD_L2);

        return;
    }

    /* Normal layout */

    /* Line 1: Voltage and Current example: "V=230.5V I=5.3A" */
    hLCD_SetCursor(DM_LCD_LINE1, 0u);

    /* Voltage */
    dtostrf(DM_Voltage, 5, 1, numStr);    /* width 5, 1 decimal */
    hLCD_WriteString("V=");
    hLCD_WriteString(numStr);
    hLCD_WriteString("V ");

    /* Current */
    dtostrf(DM_Current, 4, 1, numStr);    /* width 4, 1 decimal */
    hLCD_WriteString("I=");
    hLCD_WriteString(numStr);
    hLCD_WriteString("A");

    /* Line 2: Power and Energy (tight format) "P=1220W E=3.4kWh" */
    hLCD_SetCursor(DM_LCD_LINE2, 0u);

    /* Power: integer-like display (no decimals by design) */
    dtostrf(DM_Power, 4, 0, pStr);  /* e.g. "1220" or " 120" */
    /* Energy: small numeric with one decimal */
    dtostrf(DM_Energy, 3, 1, eStr); /* e.g. "3.4" or "12.3" */

    /* Compose exact 16-char line: "P=XXXXW E=Y.YkWh" (fits 16 cols when P/E sizes kept reasonable) */
    hLCD_WriteString("P=");
    hLCD_WriteString(pStr);
    hLCD_WriteString("W E=");
    hLCD_WriteString(eStr);
    hLCD_WriteString("kWh");

    /* Line 3: Mode */
    hLCD_SetCursor(DM_LCD_LINE3, 0u);
    hLCD_WriteString(DM_IsAuto ? DM_MSG_AUTO : DM_MSG_MANUAL);

    /* Line 4: Status */
    hLCD_SetCursor(DM_LCD_LINE4, 0u);
    hLCD_WriteString(DM_MSG_NORMAL);
}

/**
 * @brief Set measurements and immediately refresh the display.
 */
void DM_ShowMeasurements(float V, float I, float P, float E)
{
    DM_Voltage = V;
    DM_Current = I;
    DM_Power   = P;
    DM_Energy  = E;

    DM_Update();
}

/**
 * @brief Set operating mode and refresh display.
 */
void DM_ShowMode(bool isAuto)
{
    DM_IsAuto = isAuto;
    DM_Update();
}

/**
 * @brief Set protection (trip) state and refresh display.
 */
void DM_ShowProtectionState(bool isTripped)
{
    DM_IsTripped = isTripped;
    DM_Update();
}

/**
 * @brief Show a custom message on line 2 (temporary).
 * @param msg Null-terminated C string (should be short).
 */
void DM_ShowMessage(const char *msg)
{
    DM_ClearDisplay();
    hLCD_SetCursor(DM_LCD_LINE2, 0u);
    hLCD_WriteString(msg);
}

