#include "Hal/LCD/LCD_Interface.h"
#include <util/delay.h>
int main(void)
{


hLCD_Init();
char* TEST_String1="Hey";
char* TEST_String2="I know";
char* TEST_String3="it is";
char* TEST_String4="Working";
while (1)
{
   
hLCD_SetCursor(1, 2);
hLCD_WriteNumber(550);
    _delay_ms(1000);

hLCD_SetCursor(2, 1);
hLCD_WriteString(TEST_String2);
    _delay_ms(1000);

hLCD_SetCursor(3, 0);
hLCD_WriteString(TEST_String3);
    _delay_ms(1000);

hLCD_SetCursor(4, 6);
hLCD_WriteString(TEST_String4);
    _delay_ms(1000);

    }
}