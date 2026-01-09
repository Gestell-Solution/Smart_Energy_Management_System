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
            CurrentState = Wait_data_With_command;
            break;

        case Wait_data_With_command:
            LocalFrameBuffer[Rx_Index] = value;
            Rx_Index++;
            /* Check if we received full frame (Length byte specifies payload length? Logic check needed) */
            /* If FrameLen is payload len, then Total = 3 + FrameLen?
               Original code check: if (Rx_Index >= FrameLen)
               This implies FrameLen INCLUDES the header or is the total count?
               Let's assume the original logic is correct for the custom protocol.
            */
            if (Rx_Index >= FrameLen)
            {
                /* Frame Complete */
                SystemController.Event = EVENT_COMM_RECEIVED_CMD;
                SystemController.CmdID = LocalFrameBuffer[2];
                App_CommManager_ProcessCommand(&LocalFrameBuffer[1]); /* Pass starting from Length? or Command? */
                
                /* Reset State */
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
void App_CommManager_ProcessCommand(uint8_t *frame)
{
    /* frame[1] corresponds to Command ID if frame points to Length? 
       Wait, passed &LocalFrameBuffer[1].
       LocalFrameBuffer: [0]=Header, [1]=Len, [2]=Cmd
       So passed pointer 'frame' starts at [1] (Len).
       frame[0] = Len
       frame[1] = Cmd
       Original code: switch(frame[1]) -> matches Cmd.
    */
    switch (frame[1])
    {
    case GET_RMS_DATA:
        App_CommManager_SendFrame((uint8_t*)App_SystemController_GetState().Data, GET_RMS_DATA, RMS_Message_length);
        break;
        
    case Get_Logged_DATA:
        App_EnergyLogger_ReadLog(stringtoNumber(frame), &Status.RamData);
        App_CommManager_SendFrame((uint8_t*)"Done", SystemController.CmdID, 4);
        break;

    case Update_EEPROM:
        App_EnergyLogger_Update(&Status.RamData);
        App_CommManager_SendFrame((uint8_t*)UpdatedEEPROM_Message, SystemController.CmdID, UpdatedEEPROM_Message_length);
        break;
        
    case CuttOFF:
        App_CommManager_SendFrame((uint8_t*)CuttoFF_Message, SystemController.CmdID, Cutoff_message_length);
        break;
        
    case Calibrate_Sensors:
        /* Handled by Calibration Manager (Placeholder) */
        break;
        
    case SetOverLoad_Current_Limit:
        g_SystemData.OvercurrentLimit = stringtoNumber(frame);
        break;
        
    case SetOverLoad_Voltage_Limit:
        g_SystemData.OvervoltageLimit = stringtoNumber(frame); 
        break;
        
    case SHUTDOWN_Device:
        SystemController.Event = EVENT_Power_Down;
        App_SystemController_HandleEvent(SystemController);
        /* MCUCR_Reg usage was bare in original, moved to Event Handle */
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
