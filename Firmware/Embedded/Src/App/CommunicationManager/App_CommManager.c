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

// added
#include "../MeasurementEngine/MeasurementEngine_Interface.h"
#include "../../Common/SystemDataManager/SystemDataManager.h"
#include "../../Hal/RelayControl/RELAY_Interface.h"
#include "../../Hal/RelayControl/RELAY_Config.h"
#include "../../Mcal/GIE/GIE_Interface.h"
/*============================================================================
 *                                 Global Variables
 *============================================================================*/

/** @brief Circular buffer for received raw bytes (module-private). */
static uint8_t Datareceived[Max_Buffer_size];

/** @brief Read index for the circular buffer (module-private). */
static uint8_t Comm_Front = 0;

/** @brief Write index for the circular buffer (module-private). */
static uint8_t Comm_Rear = 0;

/** @brief Current count of bytes in the buffer (module-private). */
static uint8_t CurrentSizeofBuffer = 0;

/** @brief Flag indicating if the receive handler is currently processing a frame (module-private). */
static uint8_t Ishandling = 0;

/** @brief External System Controller Event Object. */
extern SystemEvent_t SystemController;

/** @brief External System State Object. */
extern SystemState_t Status;

/** @brief Global persistent system data (EEPROM-mirrored). */
extern SystemData_t g_SystemData;
/*============================================================================
 *                                 Helper Functions
 *============================================================================*/
/**
 * @brief   Converts the Value from little-endian to big-endian fromat
 * @details It takes the 2-byte (16-bit)value and put the value of each byte
 *          and put them into reversed places in two places of 1-byte(8-bits) buffer input.
 *
 * @param[in] value : The value we want to transform to be in the form of big-endian.
 * @param[in] buffer: The Buffer we will send to the user with the big endian value.
 * @param[in] index : The index of the value we will be put in the buffer
 *
 * @return  Does not return any value
 */
// void Comm_PackUint16_BigEndian(uint16_t value, uint8_t *buffer, uint8_t index)
// {
//     // High Byte First
//     buffer[index] = (uint8_t)((value >> 8) & 0xFF);
//     // Low Byte Second
//     if ((index + 1) != 10)
//     {
//         buffer[index + 1] = (uint8_t)(value & 0xFF);
//     }
// }
/**
 * @brief   It gets the value of curr,volt, power, energy and Send them to the dashboard
 * @details it gets the Values from the measurement engine, saves them into variables and
 *          put them in the buffer in the form of the Big endian inside a buffer one by one.
 *          Only after that it sends the whole buffer to the user.
 *
 * @return  Does not return any value
 */
void Comm_SendMobileData(void)
{
    float v = ME_GetVoltageRMS();
    float i = ME_GetCurrentRMS();
    float p = ME_GetActivePower();
    float e_j = ME_GetEnergy();
    uint8_t relayMask = SystemData_GetRelayStatesMask();
    /* Mobile expects energy/100; commonly Wh. So send (Wh * 100) as uint32 big-endian. */
    uint32_t e_wh_x100 = (uint32_t)((e_j / (1000.0f * 3600.0f)) * 100.0f);

    uint16_t v16 = (uint16_t)(v * 10.0f);
    uint16_t i16 = (uint16_t)(i * 100.0f);
    uint16_t p16 = (uint16_t)(p * 10.0f);

    uint8_t rmsPayload[11];
    rmsPayload[0] = (uint8_t)(v16 >> 8);
    rmsPayload[1] = (uint8_t)(v16 & 0xFF);
    rmsPayload[2] = (uint8_t)(i16 >> 8);
    rmsPayload[3] = (uint8_t)(i16 & 0xFF);
    rmsPayload[4] = (uint8_t)(p16 >> 8);
    rmsPayload[5] = (uint8_t)(p16 & 0xFF);
    rmsPayload[6] = (uint8_t)(e_wh_x100 >> 24);
    rmsPayload[7] = (uint8_t)(e_wh_x100 >> 16);
    rmsPayload[8] = (uint8_t)(e_wh_x100 >> 8);
    rmsPayload[9] = (uint8_t)(e_wh_x100 & 0xFF);
    rmsPayload[10] = (uint8_t)(relayMask & SYSTEMDATA_RELAY_STATE_MASK);
    App_CommManager_SendFrame(rmsPayload, GET_RMS_DATA, 11);
}
/**
 * @brief   TBD
 * @details TBD
 *
 * @return  Does not return any value
 */
void Comm_SendDeviceInfo(void)
{
    uint8_t devPayload[7];
    devPayload[0] = g_SystemData.DeviceID;
    uint16_t vmax = g_SystemData.OvervoltageLimit;
    uint16_t imax = g_SystemData.OvercurrentLimit;
    uint16_t pmax = (uint16_t)(vmax * imax);
    devPayload[1] = (uint8_t)(vmax >> 8);
    devPayload[2] = (uint8_t)(vmax & 0xFF);
    devPayload[3] = (uint8_t)(imax >> 8);
    devPayload[4] = (uint8_t)(imax & 0xFF);
    devPayload[5] = (uint8_t)(pmax >> 8);
    devPayload[6] = (uint8_t)(pmax & 0xFF);
    App_CommManager_SendFrame(devPayload, GET_DEVICE_INFO, 7);
}
/**
 * @brief   It gets the value of curr,volt, power, energy and Send them to the dashboard
 * @details it gets the Values from the measurement engine and send them sequentially one by one.
 *
 * @return  Does not return any value
 */

void Comm_SendDashboardData(void)
{
    // Get the value from the Measurement Engine
    float v = ME_GetVoltageRMS();
    // Send raw bytes (AVR is Little Endian by default)
    mUART_SendBuffer((uint8_t *)&v, 4);

    v = ME_GetCurrentRMS();

    mUART_SendBuffer((uint8_t *)&v, 4);

    v = ME_GetEnergy();

    mUART_SendBuffer((uint8_t *)&v, 4);

    v = ME_GetActivePower();

    mUART_SendBuffer((uint8_t *)&v, 4);
}
/**
 * @brief      Converts a string payload into a 16-bit integer.
 * @details    Parses bytes from the frame buffer starting at offset 2 (Payload).
 *             Assumes ASCII digits or raw byte reconstruction depending on protocol.
 *             (Note: The implementation below suggests ASCII-like multiplication or simple decimal parsing).
 * @param[in]  Frame  Pointer to the frame buffer (where Frame[0] is Length?).
 * @return     uint16_t Parsed integer value.
 */
uint16_t stringtoNumber(uint8_t *Frame)
{
    /* Frame format (as passed by ProcessCommand): Frame[0]=Len, Frame[1]=Cmd, Frame[2...] = payload bytes
     * Parse ASCII decimal from payload starting at Frame[2]. Return 0 on invalid/empty.
     */
    uint16_t sum = 0;
    uint8_t len = Frame[0];
    if (len <= 1u) return 0u;
    uint8_t payloadLen = (uint8_t)(len - 1u);
    for (uint8_t i = 0; i < payloadLen; ++i)
    {
        uint8_t b = Frame[2 + i];
        if (b >= '0' && b <= '9')
        {
            sum = (uint16_t)(sum * 10u + (uint16_t)(b - '0'));
        }
        else
        {
            break;
        }
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
    /* hEsp01_init(); // Wi-Fi initialization (Disabled) */
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
        /* Protect index updates from concurrent access (e.g., ISR) */
        mGIE_Disable();
        Datareceived[Comm_Rear] = Value;
        Comm_Rear = (uint8_t)((Comm_Rear + 1u) % Max_Buffer_size);
        CurrentSizeofBuffer++;
        mGIE_Enable();
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
    Frame_Setting[1] = (uint8_t)len;
    Frame_Setting[2] = Command;

    if (len > 0 && data != Null)
    {
        uint16_t i = 0;
        for (; i < len && i < Max_Buffer_size - 3; i++)
            Frame_Setting[i + 3] = data[i];
        len = i;
    }
    else
    {
        len = 0;
    }

    hBT_SendBuffer(Frame_Setting, 3 + len);
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
    if (Ishandling || CurrentSizeofBuffer == 0u)
    {
        return;
    }

    Ishandling = 1;

    static uint8_t LocalFrameBuffer[Max_Buffer_size];
    static uint8_t Rx_Index = 0;
    static uint8_t CurrentState = WaitTheHeader;
    static uint8_t FrameLen = 0;

    while (CurrentSizeofBuffer > 0u)
    {
        /* Dequeue byte with protection */
        mGIE_Disable();
        uint8_t value = Datareceived[Comm_Front];
        Comm_Front = (uint8_t)((Comm_Front + 1u) % Max_Buffer_size);
        CurrentSizeofBuffer--;
        mGIE_Enable();

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
            /* Validate FrameLen does not exceed space */
            if (FrameLen > (Max_Buffer_size - 3u))
            {
                CurrentState = WaitTheHeader;
                Rx_Index = 0;
                Ishandling = 0;
                return;
            }
            LocalFrameBuffer[Rx_Index++] = value;
            CurrentState = Wait_data_With_command;
            break;

        case Wait_data_With_command:
            if (Rx_Index < Max_Buffer_size)
            {
                LocalFrameBuffer[Rx_Index++] = value;
            }

            if (Rx_Index >= (uint8_t)(3u + FrameLen))
            {
                /* Complete frame received; pass pointer to Len (index 1) */
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
void App_CommManager_ProcessCommand(uint8_t *NonHeadered_frame)
{
     /* NonHeadered_frame: [0]=Len, [1]=Cmd, [2..]=payload
      * Validate length before accessing payload bytes.
      */
     uint8_t len = NonHeadered_frame[0];
     if (len < 1u) return; /* must contain at least command */
     uint8_t cmd = NonHeadered_frame[1];
     switch (cmd)
    {
    // case SendToDashboard:
    //     //     Comm_SendDashboardData();
    //     //     break;
    case GET_RMS_DATA:
        Comm_SendMobileData();
        break;

    case Get_Logged_DATA:
        // App_EnergyLogger_ReadLog(stringtoNumber(NonHeadered_frame), &Status.RamData);
        // App_CommManager_SendFrame((uint8_t *)"Done", SystemController.CmdID, 4);
        break;
    case GET_DEVICE_INFO:
        Comm_SendDeviceInfo();
        break;
    case Update_EEPROM:
        // App_EnergyLogger_Update(&Status.RamData);
        // App_CommManager_SendFrame((uint8_t *)UpdatedEEPROM_Message, SystemController.CmdID, UpdatedEEPROM_Message_length);
        break;

    case CuttOFF:
        /* Expect payload: [relayIndex, onOff] => total len >= 3 (cmd + 2) */
        if (len >= 3u)
        {
            uint8_t relayId = NonHeadered_frame[2];
            uint8_t onOff = NonHeadered_frame[3] ? 1u : 0u;
            if (onOff)
                hRelay_On(relayId);
            else
                hRelay_Off(relayId);

            /* Track relay state changes in persisted SystemData (mark dirty inside) */
            SystemData_SetRelayState(relayId, onOff);
            /* Persist immediately for critical relay changes to avoid data loss on power failure */
            SystemData_SaveToEEPROM();
        }

        break;

    case Calibrate_Sensors:
        /* Handled by Calibration Manager (Placeholder) */
        break;

    case SetOverLoad_Current_Limit:
        if (len >= 2u)
        {
            g_SystemData.OvercurrentLimit = stringtoNumber(NonHeadered_frame);
            SystemData_MarkDirty();
            /* Persist immediately to ensure setting survives reboot */
            SystemData_SaveToEEPROM();
        }
        break;

    case SetOverLoad_Voltage_Limit:
        if (len >= 2u)
        {
            g_SystemData.OvervoltageLimit = stringtoNumber(NonHeadered_frame);
            SystemData_MarkDirty();
            /* Persist immediately to ensure setting survives reboot */
            SystemData_SaveToEEPROM();
        }
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
    if (CurrentSizeofBuffer == 0u) return 0u;
    return Datareceived[(Comm_Front + 1u) % Max_Buffer_size];
}
