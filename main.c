#include "Hal/LCD/LCD_Interface.h"
#include <util/delay.h>
int main(void)
{


hLCD_Init();

while (1)
{
   
hLCD_SetCursor(1, 0);
hLCD_WriteString("Line 1");

hLCD_SetCursor(2, 0);
hLCD_WriteString("Line 2");

hLCD_SetCursor(3, 0);
hLCD_WriteString("Line 3");

hLCD_SetCursor(4, 0);
hLCD_WriteString("Line 4");

    }
}