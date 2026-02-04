/**
 * @file       DisplayManager_Program.c
 * @brief      This file contains The Program for Display Manager .    
 * @version    2.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Mohamed Diaa & Emg -Ahmed Twap
 * @date       2025-11-24
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
*/
#include "DisplayManager_Interface.h"
#include "DisplayManager_Config.h"
#include "DisplayManager_Private.h"
#include "../../Hal/LCD/LCD_Interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

/* Internal (module) state */
static float DM_Voltage   = 0.0f;
static float DM_Current   = 0.0f;
static float DM_Power     = 0.0f;
static float DM_Energy    = 0.0f;


static uint8_t DM_IsAuto    = 1u; /* 1 = AUTO, 0 = MANUAL */
static uint8_t DM_IsTripped = 0u; /* 1 = TRIPPED, 0 = NORMAL */

/* --------------------------
    Clear a single LCD line
   -------------------------- */
static void DM_ClearLine(uint8_t line)
{
    /* line is 1-based */
    hLCD_SetCursor(line, 0u);
    hLCD_WriteString(DM_CLEAR_LINE_STRING); /* DM_CLEAR_LINE_STRING must be DM_LCD_COLS spaces */
    hLCD_SetCursor(line, 0u);
}

/* --------------------------
   Clear whole display
   -------------------------- */
static void DM_ClearDisplay(void)
{
    for (uint8_t line = 1u; line <= DM_LCD_ROWS; ++line)
    {
        DM_ClearLine(line);
    }
}

/* --------------------------
   Initialize
   -------------------------- */
void DM_Init(void)
{
    hLCD_Init();
    DM_ClearDisplay();
}

/* --------------------------
   Update display
   -------------------------- */
void DM_Update(void)
{
    char numStr[16];
    char pStr[12];
    char eStr[12];
    char lineBuf[DM_LCD_COLS + 1];

    /* If tripped: show trip layout */
    if (DM_IsTripped)
    {
        DM_ClearLine(DM_LCD_LINE1);
        DM_ClearLine(DM_LCD_LINE2);
        DM_ClearLine(DM_LCD_LINE3);
        DM_ClearLine(DM_LCD_LINE4);

        hLCD_SetCursor(DM_LCD_LINE2, 0u);
        hLCD_WriteString("!!! TRIPPED !!!");

        hLCD_SetCursor(DM_LCD_LINE3, 0u);
        hLCD_WriteString("Press Reset Btn");
        return;
    }

    /* ---------- Line1: Voltage & Current ---------- */
    DM_ClearLine(DM_LCD_LINE1);
    hLCD_SetCursor(DM_LCD_LINE1, 0u);

    dtostrf(DM_Voltage, 5, 1, numStr);    
    hLCD_WriteString("V=");
    hLCD_WriteString(numStr);
    hLCD_WriteString("V ");

    dtostrf(DM_Current, 4, 1, numStr);    
    hLCD_WriteString("I=");
    hLCD_WriteString(numStr);
    hLCD_WriteString("A");

    /* ---------- Line2: Power & Energy ---------- */
    DM_ClearLine(DM_LCD_LINE2);
    hLCD_SetCursor(DM_LCD_LINE2, 0u);

    dtostrf(DM_Power, 4, 0, pStr);  
    dtostrf(DM_Energy, 3, 1, eStr); 

    snprintf(lineBuf, sizeof(lineBuf), "P=%sW E=%skWh", pStr, eStr);
    lineBuf[DM_LCD_COLS] = '\0';
    hLCD_WriteString(lineBuf);

    /* ---------- Line3: Mode ---------- */
    DM_ClearLine(DM_LCD_LINE3);
    hLCD_SetCursor(DM_LCD_LINE3, 0u);
    hLCD_WriteString(DM_IsAuto ? DM_MSG_AUTO : DM_MSG_MANUAL);

    /* ---------- Line4: Status ---------- */
    DM_ClearLine(DM_LCD_LINE4);
    hLCD_SetCursor(DM_LCD_LINE4, 0u);
    hLCD_WriteString(DM_MSG_NORMAL);
}

/* --------------------------
   Public API
   -------------------------- */
void DM_ShowMeasurements(float V, float I, float P, float E)
{
    DM_Voltage = V;
    DM_Current = I;
    DM_Power   = P;
    DM_Energy  = E;

    DM_Update();
}

void DM_ShowMode(uint8_t isAuto)
{
    DM_IsAuto = isAuto ? 1u : 0u;
    DM_Update();
}

void DM_ShowProtectionState(uint8_t isTripped)
{
    DM_IsTripped = isTripped ? 1u : 0u;
    DM_Update();
}

void DM_ShowMessage(const char *msg)
{
    if (msg == NULL) return;

    DM_ClearLine(DM_LCD_LINE1);
    DM_ClearLine(DM_LCD_LINE2);
    DM_ClearLine(DM_LCD_LINE3);
    DM_ClearLine(DM_LCD_LINE4);

    hLCD_SetCursor(DM_LCD_LINE2, 0u);
    hLCD_WriteString(msg);
}
