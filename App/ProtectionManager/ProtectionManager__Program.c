/**
 * @file ProtectionManager_Program.c
 * @brief This File Contains the implementation of the protection Manager Driver
 * @version 1.0
 * @date 11-11-2025
 * @author Developer : Ahmed Ashraf (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Mohamed Diaa (mohammeddiaato@gmail.com)
 * @copyright Copyright (c) 2025 , Gestell Company
 */

 #include "../../Common/Config.h"

 #if ProtectionManager == Enable
#include "ProtectionManager_Interface.h"

static uint8_t Protection_State=255;

void PM_Init(){
Buzzer_Init();
hBtn_Init();
hCurrent_Init();
hRGB_Init();
hVoltage_Init();
}

void PM_Update(){
if (hVoltage_ReadRMS()>Vrms_Threshold || hCurrent_ReadRMS()>Irms_Threshold)
{
    Protection_State =Danger;
   Buzzer_On();

  for (uint8_t Relay_id =hRELAY_0 ; Relay_id <=hRELAY_3 ; Relay_id ++)
{
     hRelay_Off(Relay_id);
}
     hRGB_SetState(RGB_RED);
     DM_ShowProtectionState(Danger);
}
else {
    Protection_State=Safe;
    DM_ShowProtectionState(Safe);
}
}

uint8_t PM_IsTripped(){

    if (Protection_State==Danger){
        return Danger ;
    }
    else {
        return Safe;
    
    }
}
void PM_Reset(){

    
}






 #endif