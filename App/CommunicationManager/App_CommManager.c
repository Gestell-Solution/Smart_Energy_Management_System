/**
 * @file    App_CommManager.c
 * @brief     This File is for Implementing the  main APIs  for the App layer Communication Manager.
 * @details - This Module is about Decoding incoming commands and route them to the appropriate module.
 *          - Provide a unified interface for communication with both Bluetooth (local) and Wi-Fi (remote) and Applying the wanted framing
 *            Protocol and lastly maintain the synchronization between UART and Network Layer.
 * @version 1.0
 * @author Developer : Mohammed Diaa (mohammeddiaato@gmail.com)
 * @author Reviewer :  Ahmed Ashraf @Ahmed-Ashraf750
 * @date 2025-11-13
 * @copyright Copyright (c) 2025 , Gestell Company
 */

// #if   _Comm_Manger_ ==Enable

#include "App_CommManager.h"
#include "../../Mcal/Timer0/TIMER0_Interface.h"
#include "../System_Controller/System_Controller_Interface.h"
#include "../../Hal/HC05/HC05_Interface.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "../../Mcal/UART/UART_Rx.h"

uint8_t Datareceived[Max_Buffer_size];
extern uint8_t Comm_Front ;
extern uint8_t Comm_Rear ;
uint8_t CurrentSizeofBuffer = 0;
uint8_t Ishandling = 0;
extern SystemEvent_t SystemController;
extern SystemState_t Status;

uint16_t stringtoNumber(uint8_t* Frame)
{
        uint16_t sum;
        for (int i = 0; i < Frame[0]-3; i++)
        {
            sum=sum*10 +Frame[i+2];
        }
        
        return sum ;
}
void App_CommManager_Init()
{
    hBT_Init();
    mTIMER0_Init();
    // hEsp01_init();
    mTIMER0_StartDelay(Scheduling_Time, App_CommManager_Task); // scheduling the task of the APP ecery 50ms it checks the items
}

void App_CommManager_Task(void)
{

    uint8_t Value;

    while ((hBT_ReadByte(&Value) == Done_Action) && (CurrentSizeofBuffer < Max_Buffer_size))
    {

        Datareceived[RearOfQueue] = Value;
        Update_RearOfQueue;
        CurrentSizeofBuffer++;
    }

    if (CurrentSizeofBuffer > 0)
    {
        App_CommManager_ReceiveHandler();
    }
}

void App_CommManager_SendFrame(uint8_t *data, uint8_t Command, uint16_t len)
{
    uint8_t Frame_Setting[Max_Buffer_size];
    Frame_Setting[0] = FRAME_HEADER;
    Frame_Setting[1] = len;
    Frame_Setting[2] = Command;
    if (data != Null && len != 0)
    {
        int i = 0;
        for (; i < len; i++)
        {
            Frame_Setting[i + 3] = data[i];
        }
        uint16_t Total_length = len + 3;
        hBT_SendBuffer(Frame_Setting, Total_length);
    }
    else
    {
        return;
    }
}
void App_CommManager_ReceiveHandler()
{
    if (Ishandling || CurrentSizeofBuffer >= Max_Buffer_size)
    {
        return;
    }

    Ishandling = 1;

    static uint8_t LocalFrameBuffer[Max_Buffer_size];
    static uint8_t Rx_Index = 0;
    static uint8_t CurrentState = WaitTheHeader;
    static uint8_t FrameLen = 0;

    while (CurrentSizeofBuffer > 0)
    {
        uint8_t value = Datareceived[Comm_Front];
        Update_FrontOfQueue;
        CurrentSizeofBuffer--;
        switch (CurrentState)
        {
        case WaitTheHeader:
            if (value == FRAME_HEADER)
            {
                Rx_Index = 0;
                LocalFrameBuffer[Rx_Index++] = value;
                CurrentState = WaitLen;
            }

            break;
        case WaitLen:
            FrameLen = value;
            LocalFrameBuffer[Rx_Index++] = value;
            CurrentState = Wait_data_With_command;
            break;

        case Wait_data_With_command:
            LocalFrameBuffer[Rx_Index] = value;
            Rx_Index++;
            if (Rx_Index >= FrameLen)
            {
                SystemController.Event = EVENT_COMM_RECEIVED_CMD;
                SystemController.CmdID = LocalFrameBuffer[2];
                App_CommManager_ProcessCommand(&LocalFrameBuffer[1]);
                CurrentState = WaitTheHeader;
                Rx_Index = 0;
                Ishandling = 0;
                return;
            }
            break;
        }
    }

    Ishandling = 0;
}

void App_CommManager_ProcessCommand(uint8_t *frame)
{
    switch (frame[1])
    {

    case GET_RMS_DATA:

        App_CommManager_SendFrame(App_SystemController_GetState().Data, GET_RMS_DATA, RMS_Message_length);

        break;
    case Get_Logged_DATA:
        App_EnergyLogger_ReadLog(stringtoNumber(frame),&Status.RamData);
        App_CommManager_SendFrame("Done", SystemController.CmdID,4);

        break;
    // case Notification_To_User:
        
    //     App_CommManager_SendFrame(, SystemController.CmdID);

    //     break;
    case Update_EEPROM:
        
        App_EnergyLogger_Update(&Status.RamData);
        App_CommManager_SendFrame(UpdatedEEPROM_Message, SystemController.CmdID,UpdatedEEPROM_Message_length);

        break;
    case CuttOFF:

        App_CommManager_SendFrame(CuttoFF_Message, SystemController.CmdID, Cutoff_message_length);
        break;
    case Calibrate_Sensors:
        // calibration Manager Action
        break;
    case SetOverLoad_Current_Limit:
        
        g_SystemData.OvercurrentLimit=stringtoNumber(frame);
        break;
    case SetOverLoad_Voltage_Limit:
        g_SystemData.OvervoltageLimit=stringtoNumber(frame); 
        break;
    case SHUTDOWN_Device:
        SystemController.Event=EVENT_Power_Down;
        App_SystemController_HandleEvent(SystemController);
        MCUCR_Reg;
        break;

    default:
        break;
    }
}

uint8_t Accesslength()
{
    return Datareceived[(FrontOfQueue + 1) % Max_Buffer_size];
}
