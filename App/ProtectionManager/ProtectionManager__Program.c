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

static uint8_t Protection_State=Safe;

void PM_Init(){
mTIMER1_Init();     //initialize timer1
Buzzer_Init();     //initialize Buzzer
hBtn_Init();       //initialize Btn
hCurrent_Init();  //initialize Currentsnsr
hRGB_Init();     //initiliaize RGB
hVoltage_Init(); 
DM_Init();
mEXTI_Init(EXT1_Macro,EXT_RISING_EDGE);
mDIO_SetDirectionForPin(GroupD , PIN3, Input);
mDIO_WritePin(GroupD, PIN3, High);
mEXTI_setCallback(EXT1_Macro ,PM_Reset);
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
    //  DM_ShowProtectionState(Danger);
     hBT_SendString("\nDanger , Electrical Spike\n");
     hBT_SendString("\n Please Resolve the problem and press the reset button\n");
}
else {
    Protection_State=Safe;
    // DM_ShowProtectionState(Safe);
    hBT_SendString(" Safe , Everything is fine");
}
}

uint8_t PM_IsTripped(){

   return Protection_State;
}
void PM_Reset(){
if (Protection_State==Safe){
hRGB_SetState(RGB_GREEN);
DM_ShowProtectionState(Safe);
 for (uint8_t Relay_id =hRELAY_0 ; Relay_id <=hRELAY_3 ; Relay_id ++)
{
     hRelay_On(Relay_id);
}
}
    
}






 #endif