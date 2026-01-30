#include <util/delay.h>

/* Drivers */
#include "../../../../Src/App/ProtectionManager/ProtectionManager_Interface.h"
#include "../../../../Src/Hal/LCD/LCD_Interface.h"
#include "../../../../Src/Hal/RelayControl/RELAY_Interface.h"

extern float PM_Test_Current;

/* Definitions for manual Reset button reading */
#define RESET_BTN_PIN PIN3
#define RESET_BTN_GRP GroupD

void PM_Test1_Init(){
    PM_Init();
    
    hLCD_SendCommand(0x01); _delay_ms(2);
    hLCD_SetCursor(1, 0); 
    hLCD_WriteString("System Ready...");
    _delay_ms(1000);

    /* --- Test 1: Noise (Pass) --- */
    hLCD_SendCommand(0x01); _delay_ms(2);
    hLCD_SetCursor(1, 0); hLCD_WriteString("Test 1: Noise");
    PM_Test_Current = 15.0f;
    for(int i=0; i<3; i++) { PM_Update(); _delay_ms(10); }
    hLCD_SetCursor(2, 0);
    if(PM_IsTripped() == 0) hLCD_WriteString("Result: PASS");
    else                    hLCD_WriteString("Result: FAIL");
    _delay_ms(1000);

    /* --- Cool Down --- */
    PM_Test_Current = 2.0f; 
    for(int i=0; i<10; i++) { PM_Update(); _delay_ms(10); }

    /* --- Test 2: Inrush (Pass) --- */
    hLCD_SendCommand(0x01); _delay_ms(2);
    hLCD_SetCursor(1, 0); hLCD_WriteString("Test 2: Inrush");
    PM_Test_Current = 15.0f;
    for(int i=0; i<8; i++) { PM_Update(); _delay_ms(10); }
    PM_Test_Current = 5.0f; PM_Update(); 
    hLCD_SetCursor(2, 0);
    if(PM_IsTripped() == 0) hLCD_WriteString("Result: PASS");
    else                    hLCD_WriteString("Result: FAIL");
    _delay_ms(1000);
    
    /* --- Cool Down --- */
    PM_Test_Current = 2.0f;
    for(int i=0; i<10; i++) { PM_Update(); _delay_ms(10); }

    /* --- Test 3: Fault (Trip) --- */
    hLCD_SendCommand(0x01); _delay_ms(2);
    hLCD_SetCursor(1, 0); hLCD_WriteString("Test 3: Fault");
    PM_Test_Current = 12.0f;
    for(int i=0; i<15; i++) {
        PM_Update();
        _delay_ms(10);
        if(PM_IsTripped() == 1) break;
    }
    hLCD_SetCursor(2, 0);
    if(PM_IsTripped() == 1) hLCD_WriteString("State: TRIPPED");
    else                    hLCD_WriteString("State: FAIL");

    /* ==========================================================
     * FINAL LOOP: Manual Button Check (Backup Plan)
     * ========================================================== */
    while(1)
    {
        uint8_t btn_state = 1;
        
        /* Read the button state (PD3) */
        mDIO_ReadPin(RESET_BTN_GRP, RESET_BTN_PIN, &btn_state);
        
        /* If button is pressed (Ground = 0) */
        if(btn_state == 0)
        {
             _delay_ms(50); /* Debounce */
             
             /* Manually call the Reset function */
             PM_Reset();
             
             /* Wait for button release to avoid repetition */
             while(btn_state == 0) {
                 mDIO_ReadPin(RESET_BTN_GRP, RESET_BTN_PIN, &btn_state);
             }
        }
    }
}