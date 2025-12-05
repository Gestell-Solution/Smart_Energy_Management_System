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
#include "../../Hal/HC05/HC05_Interface.h"

uint8_t Datareceived[50];
uint8_t Index_recieving=0;
static uint8_t Send_ = 0;
void App_CommManager_Init()
{
    hBT_Init();
    // hEsp01_init();
    // mTIMER0_StartDelay(Scheduling_Time, App_CommManager_Task); // scheduling the task of the APP ecery 50ms it checks the items
    // UART_Rx_SetCallback(DataUpdater);
}

void App_CommManager_Task(void)
{

    uint8_t temp_buffer[Max_Buffer_size];
    uint8_t Value;
    uint16_t length = 0;

    while ((hBT_ReadByte(&Value) == Done_Action) && (length < Max_Buffer_size))
    {
        temp_buffer[length] = Value;
        length++;
    }

    if (length > 0)
    {
        App_CommManager_ReceiveHandler(temp_buffer);
    }
}

void App_CommManager_SendFrame(uint8_t *data, uint8_t Command, uint16_t len)
{
    uint8_t Frame_Setting[50];
    Frame_Setting[0] = FRAME_HEADER;
    Frame_Setting[1] = len;
    Frame_Setting[2] = Command;
    if (data != Null&&len!=0)
    {
        int i = 0;
        for (; i < len; i++)
        {
            Frame_Setting[i + 3] = data[i];

        }
        uint16_t Total_length = len + 3;
        hBT_SendBuffer(Frame_Setting,Total_length);
    }
    else
    {
        return;
    }
}
void App_CommManager_ReceiveHandler(uint8_t *data)
{

    static  uint8_t RxFrameBuffer[50];
    static  uint8_t CurrentState = WaitTheHeader;
    static  uint8_t Rx_Index;

    if (data != Null ||data[1] != 0)
    {
        for (uint16_t i = 0; i <data[1]; i++)
        {

            uint8_t value = data[i];

            switch (CurrentState)
            {

            case WaitTheHeader:
                if (value == FRAME_HEADER) // frame header 0xAA
                {
                    Rx_Index = 0;
                    RxFrameBuffer[Rx_Index] = value;
                    CurrentState = WaitLen;
                }
                break;

            case WaitLen:
                RxFrameBuffer[Rx_Index++] = value;

                if (value > 50 )
                {
                    CurrentState = WaitTheHeader; // error data is not there
                }
                else
                {
                    CurrentState = Wait_data_With_command;
                }
                break;

            case Wait_data_With_command:
                RxFrameBuffer[Rx_Index++] = value;

                if (Rx_Index >=data[1] || Rx_Index >= 50)
                {

                    App_CommManager_ProcessCommand(&RxFrameBuffer[2]); // to avoid the header and the length

                    CurrentState = WaitTheHeader;
                    Rx_Index = 0;
                }
                break;
            }
        }
    }
    else
    {
        return;
    }

}

void App_CommManager_ProcessCommand(uint8_t *frame)
{
    

}

