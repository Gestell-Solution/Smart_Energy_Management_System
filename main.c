#include "Hal/LCD/LCD_Interface.h"
#include <util/delay.h>
int main(void)
{


hLCD_Init();

while (1)
{
   
hLCD_SetCursor(1, 0);
hLCD_WriteString("Line 1");
    _delay_ms(1000);

hLCD_SetCursor(2, 0);
hLCD_WriteString("Line 2");
    _delay_ms(1000);

hLCD_SetCursor(3, 0);
hLCD_WriteString("Line 3");
    _delay_ms(1000);

hLCD_SetCursor(4, 0);
hLCD_WriteString("Line 4");
    _delay_ms(1000);

    }
}