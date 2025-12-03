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

static uint8_t Protection_State = Safe;
static uint8_t Fix_Check = 0;
void PM_Init()
{
     mEXTI_Enable(EXT1_Macro);
     mTIMER1_Init();  // initialize timer1
     Buzzer_Init();   // initialize Buzzer
     hCurrent_Init(); // initialize Currentsnsr
     hVoltage_Init();
     hRGB_Init(); // initiliaize RGB
     DM_Init();
     mEXTI_Init(EXT1_Macro, EXT_RISING_EDGE);
     mDIO_SetDirectionForPin(GroupD, PIN3, Input);
     mDIO_WritePin(GroupD, PIN3, High);
     mEXTI_setCallback(EXT1_Macro, PM_Reset);
     mADC_StartGroup();
     hRGB_SetState(RGB_GREEN);
}

void PM_Update()
{
     if (hVoltage_ReadRMS() > Vrms_Threshold || hCurrent_ReadRMS() > Irms_Threshold)
     {
          Protection_State = Danger;
          Buzzer_On();

          for (uint8_t Relay_id = hRELAY_0; Relay_id <= hRELAY_3; Relay_id++)
          {
               hRelay_Off(Relay_id);
          }
          hRGB_SetState(RGB_RED);
          DM_ShowProtectionState(Danger);
          Fix_Check = 1;
     }
     else if (Fix_Check == Fixed)
     {
          Protection_State = Safe;
          DM_ShowProtectionState(Safe);
     }
     else if (Fix_Check == Not_Fixed)
     {
          hLCD_SendCommand(0x01);
          hLCD_WriteString("Fixed,");
          hLCD_SetCursor(2, 0);
          hLCD_WriteString("Press Reset");
     }
}

uint8_t PM_IsTripped()
{

     return Protection_State;
}
void PM_Reset()
{
     if (Protection_State == Safe)
     {
          for (uint8_t Relay_id = hRELAY_0; Relay_id <= hRELAY_3; Relay_id++)
          {
               hRelay_On(Relay_id);
          }
          Buzzer_Off();
          hRGB_SetState(RGB_GREEN);
          DM_ShowProtectionState(Safe);
     }
     Fix_Check = Fixed;
}

#endif