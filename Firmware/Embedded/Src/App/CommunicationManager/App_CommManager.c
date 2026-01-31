/**
 * @file       App_CommManager.c
 * @brief      Implementation of the Communication Manager Module.
 * @details    This module handles the reception, decoding, and transmission of data frames 
 *             over the communication interface (UART/Bluetooth).
 *             It implements a custom framing protocol to ensure data integrity and synchronization.
 *             
 *             **Frame Structure:**
 *             `[HEADER] [LENGTH] [COMMAND] [PAYLOAD...]`
 *             - **HEADER**: Start of Frame byte (0xAA).
 *             - **LENGTH**: Total length of the frame payload (Command + Data).
 *             - **COMMAND**: ID of the command or response type.
 *             - **PAYLOAD**: Variable length data bytes.
 *
 * @version    1.0
 * @date       2025-11-13
 * @author     Developer: Mohammed Diaa (mohammeddiaato@gmail.com)
 * @author     Reviewer:  Ahmed Ashraf
 * @copyright  Copyright (c) 2025, Gestell Company
 */

#include "App_CommManager.h"
#include "../../Mcal/Timer0/TIMER0_Interface.h"
#include "../System_Controller/System_Controller_Interface.h"
#include "../MeasurementEngine/MeasurementEngine_Interface.h"
#include "../../Common/SystemDataManager/SystemDataManager.h"
#include "../../Hal/RelayControl/RELAY_Interface.h"
#include "../../Hal/HC05/HC05_Interface.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "../../Mcal/UART/UART_Rx.h"

/*============================================================================
 *                                 Global Variables
 *============================================================================*/

/** @brief Circular buffer for received raw bytes. */
uint8_t Datareceived[Max_Buffer_size];

/** @brief External Read index for the circular buffer. */
extern uint8_t Comm_Front;

/** @brief External Write index for the circular buffer. */
extern uint8_t Comm_Rear;

/** @brief Current count of bytes in the buffer. */
uint8_t CurrentSizeofBuffer = 0;

/** @brief Flag indicating if the receive handler is currently processing a frame. */
uint8_t Ishandling = 0;

/** @brief External System Controller Event Object. */
extern SystemEvent_t SystemController;

/** @brief External System State Object. */
extern SystemState_t Status;

/*============================================================================
 *                                 Helper Functions
 *============================================================================*/

/**
 * @brief      Converts a string payload into a 16-bit integer.
 * @details    Parses bytes from the frame buffer starting at offset 2 (Payload).
 *             Assumes ASCII digits or raw byte reconstruction depending on protocol.
 *             (Note: The implementation below suggests ASCII-like multiplication or simple decimal parsing).
 * @param[in]  Frame  Pointer to the frame buffer (where Frame[0] is Length?).
 * @return     uint16_t Parsed integer value.
 */
uint16_t stringtoNumber(uint8_t* Frame)
{
        uint16_t sum = 0;
        /* Logic assumes Frame[0] accounts for overhead (3 bytes: Header/Len/Cmd?) */
        /* Loop seems to iterate over payload bytes */
        for (int i = 0; i < Frame[0]-3; i++)
        {
            sum = sum * 10 + Frame[i+2];
        }
        
        return sum;
}

/*============================================================================
 *                                 Function Definitions
 *============================================================================*/

/**
 * @brief      Initializes the Communication Manager.
 * @details    Sets up the Bluetooth module, Timer0 for scheduling, and ensures
 *             the communication task is registered to run periodically.
 */
void App_CommManager_Init()
{
    hBT_Init();
    mTIMER0_Init();
    /* hEsp01_init(); // Wi-Fi initialization (Disabled) */
    
    /* Schedule the periodic task */
    mTIMER0_StartDelay(Scheduling_Time, App_CommManager_Task); 
}

/**
 * @brief      Periodic task for fetching data from the hardware buffer.
 * @details    Reads available bytes from the Bluetooth module and pushes them
 *             into the software circular buffer (`Datareceived`).
 *             If data is present, it triggers the `ReceiveHandler`.
 */
void App_CommManager_Task(void)
{
    uint8_t Value;

    /* Fill software buffer from hardware UART */
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

/**
 * @brief      Sends a formatted frame via Bluetooth.
 * @details    Constructs the frame `[HEADER, LEN, COMMAND, DATA...]` and transmits it.
 * @param[in]  data     Pointer to the payload data.
 * @param[in]  Command  Command ID to identify the message type.
 * @param[in]  len      Length of the payload data.
 */
void App_CommManager_SendFrame(uint8_t *data, uint8_t Command, uint16_t len)
{
    uint8_t Frame_Setting[Max_Buffer_size];
    Frame_Setting[0] = FRAME_HEADER;
    Frame_Setting[1] = len;       /* Note: Protocol seems to put Payload Length here */
    Frame_Setting[2] = Command;
    
    if (data != Null && len != 0)
    {
        int i = 0;
        for (; i < len; i++)
        {
            Frame_Setting[i + 3] = data[i];
        }
        uint16_t Total_length = len + 3; /* Header + Len + Cmd + Payload */
        hBT_SendBuffer(Frame_Setting, Total_length);
    }
    else
    {
        return;
    }
}

/**
 * @brief      Parses received data from the circular buffer.
 * @details    Implements a State Machine to detect valid frames:
 *             1. **WaitTheHeader**: Looks for `0xAA`.
 *             2. **WaitLen**: Reads the length byte.
 *             3. **Wait_data_With_command**: Collects the remaining bytes.
 *             On full frame detection, calls `App_CommManager_ProcessCommand`.
 */
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
            /* Reject oversized payload to prevent buffer overflow */
            if (FrameLen > Max_Buffer_size - 3)
            {
                CurrentState = WaitTheHeader;
                Rx_Index = 0;
                break;
            }
            CurrentState = Wait_data_With_command;
            break;

        case Wait_data_With_command:
            LocalFrameBuffer[Rx_Index] = value;
            Rx_Index++;
            /* Full frame: Header(1) + Len(1) + Cmd(1) + Payload(FrameLen bytes) => 3 + FrameLen */
            if (Rx_Index >= 3 + (uint16_t)FrameLen)
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

/**
 * @brief      Executes actions based on the received command ID.
 * @details    Dispatches commands to System Controller, Energy Logger, or modifies settings.
 * @param[in]  frame  Pointer to the frame buffer (typically pointing to Length/Command).
 */
/** @brief Binary payload length for GET_RMS_DATA (V*10, I*100, P*10, E*100; 4 x 16-bit big-endian = 8 bytes). */
#define GET_RMS_DATA_PAYLOAD_LEN  8

/** @brief Binary payload length for GET_DEVICE_INFO (DeviceID, MaxV, MaxI, MaxP). */
#define GET_DEVICE_INFO_PAYLOAD_LEN 7

void App_CommManager_ProcessCommand(uint8_t *frame)
{
    /* frame = &LocalFrameBuffer[1]: frame[0]=Len, frame[1]=Cmd, frame[2..]=payload */
    switch (frame[1])
    {
    case GET_RMS_DATA:
        {
            uint8_t payload[GET_RMS_DATA_PAYLOAD_LEN];
            uint16_t v = (uint16_t)(ME_GetVoltageRMS() * 10.0f);
            uint16_t i = (uint16_t)(ME_GetCurrentRMS() * 100.0f);
            uint16_t p = (uint16_t)(ME_GetActivePower() * 10.0f);
            float e_kwh = ME_GetEnergy() / 3600000.0f;
            uint16_t e = (uint16_t)(e_kwh * 100.0f);
            payload[0] = (uint8_t)(v >> 8);
            payload[1] = (uint8_t)(v & 0xFF);
            payload[2] = (uint8_t)(i >> 8);
            payload[3] = (uint8_t)(i & 0xFF);
            payload[4] = (uint8_t)(p >> 8);
            payload[5] = (uint8_t)(p & 0xFF);
            payload[6] = (uint8_t)(e >> 8);
            payload[7] = (uint8_t)(e & 0xFF);
            App_CommManager_SendFrame(payload, GET_RMS_DATA, GET_RMS_DATA_PAYLOAD_LEN);
        }
        break;

    case GET_DEVICE_INFO:
        {
            uint8_t payload[GET_DEVICE_INFO_PAYLOAD_LEN];
            payload[0] = g_SystemData.DeviceID;
            payload[1] = (uint8_t)(g_SystemData.OvervoltageLimit >> 8);
            payload[2] = (uint8_t)(g_SystemData.OvervoltageLimit & 0xFF);
            payload[3] = (uint8_t)(g_SystemData.OvercurrentLimit >> 8);
            payload[4] = (uint8_t)(g_SystemData.OvercurrentLimit & 0xFF);
            payload[5] = (uint8_t)(g_SystemData.Power >> 8);
            payload[6] = (uint8_t)(g_SystemData.Power & 0xFF);
            App_CommManager_SendFrame(payload, GET_DEVICE_INFO, GET_DEVICE_INFO_PAYLOAD_LEN);
        }
        break;
        
    case Get_Logged_DATA:
        {
            uint16_t logIdx = (frame[2] << 8) | frame[3];
            App_EnergyLogger_ReadLog(logIdx, &Status.RamData);
            App_CommManager_SendFrame((uint8_t*)"Done", SystemController.CmdID, 4);
        }
        break;

    case Store_In__EEPROM:
        ME_ResetEnergy();
        g_SystemData.EnergyCounter = 0;
        SystemData_SaveToEEPROM();
        App_CommManager_SendFrame((uint8_t*)UpdatedEEPROM_Message, Store_In__EEPROM, UpdatedEEPROM_Message_length);
        break;

    case Update_EEPROM:
        App_EnergyLogger_Update(&Status.RamData);
        App_CommManager_SendFrame((uint8_t*)UpdatedEEPROM_Message, SystemController.CmdID, UpdatedEEPROM_Message_length);
        break;
        
    case CuttOFF:
        {
            uint8_t relayIndex = frame[2];
            uint8_t state      = frame[3];
            if (state != 0)
                hRelay_On(relayIndex);
            else
                hRelay_Off(relayIndex);
            App_CommManager_SendFrame((uint8_t*)CuttoFF_Message, SystemController.CmdID, Cutoff_message_length);
        }
        break;
        
    case Calibrate_Sensors:
        break;
        
    case SetOverLoad_Current_Limit:
        g_SystemData.OvercurrentLimit = (uint16_t)((frame[2] << 8) | frame[3]);
        break;
        
    case SetOverLoad_Voltage_Limit:
        g_SystemData.OvervoltageLimit = (uint16_t)((frame[2] << 8) | frame[3]);
        break;
        
    case SHUTDOWN_Device:
        SystemController.Event = EVENT_Power_Down;
        App_SystemController_HandleEvent(SystemController);
        break;

    default:
        break;
    }
}

/**
 * @brief      Helper to peek at the next byte in the queue (Debug/Test).
 * @return     uint8_t Next byte value.
 */
uint8_t Accesslength()
{
    return Datareceived[(FrontOfQueue + 1) % Max_Buffer_size];
}
