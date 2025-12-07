/**
 * @file Calibration_Manager_Program.c
 * @brief Contains the Implementation of Calibrations Manager Module.
 * @version 1.0
 * @author Developer : Ahmed Ashraf (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Mohamed Diaa (mohammediaato@gmail.com)
 * @date 2025-12-04
 * @copyright Copyright (c) 2025, Gestell Company
 */

#include "../../Common/Config.h"
#include "Calibration_Manager_Interface.h"
extern float calibrationFactor;
uint8_t Compare_Strings(const uint8_t *str1, const uint8_t *str2)
{
    while (*str1 == *str2)
    {
        if (*str1 == NullChar)
        {
            return Matches; // Both strings ended together
        }
        else
        {
            str1++;
            str2++;
        }
    }

    return NotMatches; // Mismatch found
}

void App_Calibration_Init()
{
    void hVoltage_Init();
    hCurrent_Init();
    hLCD_Init();
    hBT_Init();
    mTIMER1_Init();
    mADC_StartGroup();
}
void App_Calibration_Apply(void)
{
    while (1)
    {

        hCurrent_Calibrate();
        hVoltage_Calibrate(Voltage_Ref); // Example reference voltage for calibration
        if (calibrationFactor > 0.9f && calibrationFactor < 1.1f)
        {
            mEEPROM_WriteBlock(Voltage_Calib_Add, (uint8_t *)&calibrationFactor, sizeof(calibrationFactor));
            hLCD_WriteString("Calibration Success");
            break;
        }
        hLCD_WriteNumber(hVoltage_ReadRMS());
    }
}

void App_Calibration_Remote(uint8_t *cmdData)
{
    if (Compare_Strings(cmdData, "CAL") == Matches)
    {
        App_Calibration_Apply();
    }
}
