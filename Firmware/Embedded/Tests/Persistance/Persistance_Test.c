/**
 * @file Persistance_Test.c
 * @brief tests for the Persistence module.
 * @version 1.0.0
 * @author <Basma khaled> <basmak55@gmail.com>
 * @date 2026-1-29
 * @copyright Copyright (c) 2025 , Gestell Company
 */
#include "../../Src/App/Persistance/Persistance_Interface.h"
#include "../../Src/Mcal/EEPROM/EEPROM_Interface.h"
#include "../../Src/Hal/LCD/LCD_Interface.h"
/**
* @fn Test_PowerCycle
* @brief Test persistence across a simulated power cycle.
* @details 
*This test verifies that the energy value stored in EEPROM
* is correctly recovered after a simulated system reboot
* @return void
*/
void Test_PowerCycle(void)
{
    hLCD_Init(); 
    hLCD_SendCommand(0x01);
    hLCD_SetCursor(1, 0);
    hLCD_WriteString("Power Cycle");
    /**  Accumulate 0.5 kWh*/ 
    Persistence_Save(0.5f);
    

    /**  Force a Save */ 
    for (uint16_t i = 0; i < SAVE_INTERVAL_TICKS; i++){
        Persistence_Update();
    }


    /** Simulate Power Off */  
    
    Persistence_Reset();

    /** Simulate Power On  */ 
    float recovered_energy = Persistence_Load();
    

    if (recovered_energy  == 0.5f){
        hLCD_SendCommand(0x01);
        hLCD_SetCursor(1, 0);
        hLCD_WriteString("Test Passed");
    }
    else{
        hLCD_SendCommand(0x01);
        hLCD_SetCursor(1, 0);
        hLCD_WriteString("Test Failed");
    }
      
}
/**
* @fn Test_WearLeveling
* @brief Test EEPROM wear-leveling behavior.
* @details This test ensures that EEPROM write operations are limited
* @return void
*/
void Test_WearLeveling(void)
{
    hLCD_Init(); 

    hLCD_SendCommand(0x01);
    hLCD_SetCursor(1, 0);
    hLCD_WriteString("Wear Leveling");
    EEPROM_WriteCount = 0;

    /**Simulate 24 hours */  
    for (uint16_t t = 0; t < 24*60*60; t++)
    {
        Persistence_Update();
    }

    int expected_max_writes = 6*24; 
    if (EEPROM_WriteCount <= expected_max_writes){
        hLCD_SendCommand(0x01);
        hLCD_SetCursor(1, 0);
        hLCD_WriteString("Test Passed");
    }
    else{
        hLCD_SendCommand(0x01);
        hLCD_SetCursor(1, 0);
        hLCD_WriteString("Test Failed");
    }
}
