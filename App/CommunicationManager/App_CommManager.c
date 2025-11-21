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
#include "../../Mcal/UART/UART_Init.h"
#include "../../Mcal/UART/UART_RX.h"
#include "../../Mcal/UART/UART_Tx.h"

static uint8_t Send_ = 0;
void App_CommManager_Init()
{
    hBT_Init();
    // hEsp01_init();
    mTIMER0_StartDelay(Scheduling_Time, App_CommManager_Task); // scheduling the task of the APP ecery 50ms it checks the items
}

void App_CommManager_Task(void)
{

    uint8_t temp_buffer[Max_Buffer_size]; 
    uint8_t Value;
    uint16_t length = 0;

    
    while ( (hBT_Read(&Value) == Done_Action) && (length < Max_Buffer_size) )
    {
        temp_buffer[length] = Value;
        length++;
    }

   
    if (length > 0)
    {
        App_CommManager_ReceiveHandler(temp_buffer, length);
    }
}

void App_CommManager_SendFrame(uint8_t *data, uint8_t Command, uint16_t len)
{
    uint8_t Frame_Setting[50];
    Frame_Setting[0] = FRAME_HEADER;
    Frame_Setting[1] = len;
    Frame_Setting[2] = Command;

    if (data != Null)
    {
        int i = 0;
        for (; i < len; i++)
        {
            Frame_Setting[i + 3] = data[i];
        }
        uint16_t Total_length=len+3;

        hBT_SendBuffer(Frame_Setting, Total_length);
    }
    else
    {
        return;
    }
}
void App_CommManager_ReceiveHandler(uint8_t *data, uint16_t len)
{
    
    uint8_t RxFrameBuffer[50]; 
    
    if (data != Null||len!=0)
    {
        uint8_t CurrentState=WaitTheHeader;  
        uint8_t Rx_Index;      
    for (uint16_t i = 0; i < len; i++)
    {

        uint8_t byte = data[i]; 

        switch (CurrentState)
        {
       
            case WaitTheHeader:
                if (byte == FRAME_HEADER) // frame header 0xAA
                {
                    Rx_Index = 0;
                    RxFrameBuffer[Rx_Index++] = byte; 
                    CurrentState = WaitLen;   
                }
                break;


            case WaitLen:
                RxFrameBuffer[Rx_Index++] = byte;     
                                 

               
                if (byte > 50 || byte < 3) 
                {
                    CurrentState = WaitTheHeader; //error data is not there
                } 
                else 
                {
                    CurrentState = Wait_data_With_command;   
                }
                break;

   
            case Wait_data_With_command:
                RxFrameBuffer[Rx_Index++] = byte;   

               
                if (Rx_Index >= len||Rx_Index>=50)
                {
             
                    App_CommManager_ProcessCommand(&RxFrameBuffer[2]); // to avoid the header and the length

                    
                    CurrentState =WaitTheHeader ;
                    Rx_Index = 0;
                }
                break;
        } 
    } 
    }
    else
    {
        return ;
    }
}

void App_CommManager_ProcessCommand(uint8_t *frame)
{

    uint8_t cmd_id = frame[0];
    uint8_t *data = &frame[1]; 

    uint8_t tx_buffer[50]; 
    float temp_val;

    switch (cmd_id)
    {
        case Calibrate_Current_Sensor:
            
            break;

        case Calibrate_Voltage_Sensor:
            break;

        case Write_EEPROM:
            App_EnergyLogger_StoreToEEPROM();

            break;
        case Read_EEPROM:
                App_EnergyLogger_ReadLog( data, 15 );
             break;


        case GET_RMS_DATA:

    
            uint16_t volt =(uint16_t) ME_GetVoltageRMS();
            uint16_t curr =(uint16_t) ME_GetVoltageRMS();

            
            // 2. Pack the data into a temp buffer
            // We split 16-bit integers into 2 bytes (High, Low)


            tx_buffer[0] = ShiftToLowerbyte(volt); 
            tx_buffer[1] = TakeUpperByte(volt) ;
            tx_buffer[2] = ShiftToLowerbyte(curr);
            tx_buffer[3] = TakeUpperByte(curr);

            // 3. Send the Reply Frame back to the App
            // We reuse the SAME ID (CMD_GET_RMS_DATA) so the App knows what this data is
            App_CommManager_SendFrame( tx_buffer,GET_RMS_DATA, 4);
            break;
            

        default: // case of unknown ID
            
            break;
    }
}


void App_SendAction()
{
    Send_ = 1;
}