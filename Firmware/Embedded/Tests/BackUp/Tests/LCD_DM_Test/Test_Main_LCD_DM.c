#include "../../../../Src/Common/Config.h"

#if LcdAndDisplayManager_TestModule == Enable

#include <util/delay.h>
#include <stdlib.h>  
#include "Test_Main_LCD_DM.h"


int LCD_DM_Test(void)
{
    /* Initialize Display Manager */
    DM_Init();

    /* Initial values */
    float V = 220.5;
    float I = 1.23;
    float P = 1220.0;
    float E = 3.4;

    /* =======================================================
     * Case 1 — Display initial values (Normal Operation)
     * ======================================================= */
    DM_ShowMeasurements(V, I, P, E);
    DM_ShowMode(1);             // AUTO
    DM_ShowProtectionState(0);  // NORMAL
    _delay_ms(3000);

    /* =======================================================
     * Case 2 — Change mode to MANUAL
     * ======================================================= */
    DM_ShowMode(0);             // MANUAL
    _delay_ms(3000);

    /* =======================================================
     * Case 3 — Change values during operation
     * ======================================================= */
    V = 220.5;
    I = 1.23;
    P = 1400.0;
    E = 3.7;

    DM_ShowMeasurements(V, I, P, E);
    _delay_ms(3000);

    /* =======================================================
     * Case 4 — Temporary message
     * ======================================================= */
    DM_ShowMessage("DATA SAVED!");
    _delay_ms(2000);

    /* =======================================================
     * Case 5 — Enter TRIP MODE
     *  Note: The DisplayManager program itself displays the TRIP
     *  message automatically, so do not call DM_ShowMessage here.
     * ======================================================= */
    DM_ShowProtectionState(1); // TRIPPED--> (OVERLOAD / TRIP) will appear from DM_Update
    _delay_ms(3000);

    /* =======================================================
     * Case 6 — Message after reset
     * ======================================================= */
    DM_ShowProtectionState(0); // Return to normal state
    DM_ShowMessage("RESET DONE");
    _delay_ms(2000);

    /* =======================================================
     *Case 7 — Return to normal operation
     * ======================================================= */
    DM_ShowMode(1);             // AUTO
    DM_ShowMeasurements(V, I, P, E);
    _delay_ms(3000);

    /* =======================================================
     * LOOP — Continuous update
     * ======================================================= */
    while (1)
    {
        /* Increase values smoothly */
        V += 0.2;
        I += 0.1;
        P += 15;
        E += 0.02;

        /* Wrap-around logic */
        if (V > 260.0)  V = 220.0;
        if (I > 20.0)   I = 0.0;
        if (P > 3000.0) P = 500.0;
        if (E > 99.9)   E = 0.0;

        /* Update values */
        DM_ShowMeasurements(V, I, P, E);

        /* DM_Update() already gets called inside DM_ShowMeasurements */
        _delay_ms(500);
    }

    return 0;
}

#endif