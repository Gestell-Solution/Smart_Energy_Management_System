// AVR peripheral libraries
#include <avr/interrupt.h> // Required for sei()
#include <util/delay.h>    // Required for _delay_ms()

// HAL (Hardware Abstraction Layer) drivers
// #include "Hal/Voltage/Voltage_Interface.h"
#include "Hal/LCD/LCD_Interface.h"
// We also need the Voltage_Config.h to know which pin to register
// #include "Hal/Voltage/Voltage_Config.h" 
#include "Mcal/Timer1/TIMER1_Interface.h"

#include "Hal/ACS712CurntSnsr/hCurrent_Interface.h"
#include "Mcal/ADC/ADC_Interface.h"
#include "Mcal/GIE/GIE_Interface.h"
int main(void)
{

// mGIE_Enable();
        SetBit(SREG_Reg,7); 

mDIO_SetDirectionForGroup(GroupB , 0xFF );

    mTIMER1_Init();
    hLCD_Init();
    hCurrent_Init();
    hLCD_SetCursor(0, 0);
    mADC_StartGroup();
    // hCurrent_Calibrate();
    while (1)
    {
        hLCD_SendCommand(0x01);
        float Voltage_Rms = hCurrent_ReadRMS();
       float Voltage_Instant = hCurrent_ReadInstant();
        hLCD_SetCursor(1, 0);
        hLCD_WriteNumber(Voltage_Rms);
        hLCD_WriteString("A rms");
        hLCD_SetCursor(3,0);
        hLCD_WriteNumber(Voltage_Instant);
        hLCD_WriteString("A inst");
        _delay_ms(150);
    }
     return 0; 
}