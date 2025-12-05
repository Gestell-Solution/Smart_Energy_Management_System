#include "CAL_Test.h"
#include <util/delay.h>
void Calibration_test()
{
    SetBit(SREG_Reg,7);
    hLCD_Init();
    hLCD_WriteString("Starting Calibration Test");
    App_Calibration_Init();
     uint16_t StoredData = 0;
 float Current_RMS = 0.0f;
    while (1)
    {
        Current_RMS = hCurrent_ReadRMS();
        float Voltage_RMS = hVoltage_ReadRMS();
        hLCD_WriteString("Voltage RMS:");
        hLCD_WriteNumber((int32_t)Voltage_RMS);
         hLCD_SetCursor(4,0);
        hLCD_WriteString("Current RMS:");
        hLCD_WriteNumber((int32_t)Current_RMS);
        
        App_Calibration_Apply();
        mEEPROM_ReadBlock(Voltage_Calib_Add, (uint16_t *)&StoredData, 2);
        hLCD_SetCursor(3, 0);
        hLCD_WriteNumber(StoredData);
       
        _delay_ms(30);
        hLCD_SendCommand(0x01);
    }
}