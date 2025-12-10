#include "ADC_Test.h"
#include <util/delay.h>
void ADC_Synchronous_Test()
{
    mGIE_Enable();
    uint16_t adc_value = 0;
    hLCD_Init();
    mADC_Init();

    while(1)
    {
        adc_value = mADC_Read(0); // Read ADC value from channel 0
        hLCD_WriteString("ADC Value: ");
        hLCD_WriteNumber(adc_value);
        hLCD_SetCursor(2,0);
        _delay_ms(1000);
        hLCD_SendCommand(0x01); // Clear display
    }
}