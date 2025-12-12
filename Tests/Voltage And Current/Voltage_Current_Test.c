#include "Voltage_Current_Test.h"
#include <util/delay.h>
void Voltage_Current_Test(){

    mGIE_Enable();
    hVoltage_Init();
    hCurrent_Init();
    mTIMER1_Init();
    mADC_StartGroup();
    hLCD_Init();
     while (1)
    {
        uint16_t Voltage_rms = 0;
        uint16_t Current_rms = 0;
        Voltage_rms = hVoltage_ReadRMS();
        Current_rms = hCurrent_ReadRMS();
        hLCD_WriteString("V:");
        hLCD_WriteNumber(Voltage_rms);
        hLCD_SetCursor(3, 0);
        hLCD_WriteString(" I:");
        hLCD_WriteNumber(Current_rms);
                _delay_ms(1000);

        hLCD_SendCommand(0x01); // Clear display
       
    }
}