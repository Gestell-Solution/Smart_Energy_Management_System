#include "../../../../Src/Mcal/ADC/ADC_Interface.h"
#include "../../../../Src/Hal/LCD/LCD_Interface.h"
#include "../../../../Src/Mcal/GIE/GIE_Interface.h"
/**
 * @warning Make Sure to configure ADC_Config.h file to work in Synchronous Mode before running this test.
 * @details Expected to Read ADC Value From Channel 0 and 1 and swap the printing on LCD every second.
 * 
 */
void ADC_Synchronous_Test();

/**
 * 
 * @note Asynchronous Mode Test needs CallBack so it will be handled with Hal Modules Tests
 */