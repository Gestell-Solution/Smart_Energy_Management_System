/**
 * @file    App_CommManager.h
 * @brief   This File is for declaring main APIs And needed Macros for the App layer Communication Manager.
 * @details - This Module is about Decoding incoming commands and route them to the appropriate module.
 *          - Provide a unified interface for communication with both Bluetooth (local) and Wi-Fi (remote) and Applying the wanted framing
 *            Protocol and lastly maintain the synchronization between UART and Network Layer.
 * @version 1.0
 * @author Developer : Mohammed Diaa (mohammeddiaato@gmail.com)
 * @author Reviewer :  Ahmed Ashraf @Ahmed-Ashraf750
 * @date 2025-11-13
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#ifndef _APP_COMM_MANAGER_H
#define _APP_COMM_MANAGER_H

/**
 * @addtogroup App
 * @{
 * @addtogroup Communication_Manager
 * @{
 */

#include <stdint.h>

typedef enum
{
        WaitTheHeader,
        WaitLen,
        Wait_data_With_command,
} Recive_states;

#define Scheduling_Time 5
#define FRAME_HEADER 0xAA
#define Done_Action 1
#define Max_Buffer_size 64
#define YES 1
#define NO 0
/**
 * @defgroup Queue_circular_Buffer
 * @brief    This is for defining circular array to handle the data of the recieved Action
 * @{
 */
uint8_t Comm_Front;
uint8_t Comm_Rear;
/**
 * @def   RearOfQueue
 * @brief Queue
 */
#define RearOfQueue (Comm_Rear % Max_Buffer_size)
/**
 * @def   FrontOfQueue
 * @brief Queue
 */
#define FrontOfQueue (Comm_Front % Max_Buffer_size)
/**
 * @def   Update_RearOfQueue
 * @brief Queue
 */
#define Update_RearOfQueue Comm_Rear = ((Comm_Rear + 1) % Max_Buffer_size)
/**
 * @def   Update_FrontOfQueue
 * @brief Queue
 */
#define Update_FrontOfQueue Comm_Front = ((Comm_Front + 1) % Max_Buffer_size)
/**
 * @}
 */

/**
 * @defgroup    Commands ID
 * @brief       This group is concerned to define the Commands sent by the communication Manager to other places in the APP
 * @{
 */

enum
{
        Calibrate_Sensors               = 0x02,
        Read_From_EEPROM                = 0x03,
        Store_In__EEPROM                = 0x04,
        GET_RMS_DATA                    = 0x05,
        Get_Logged_DATA                 = 0x06,
        Notification_To_User            = 0x07,
        Update_EEPROM                   = 0x08,
        CuttOFF                         = 0x09,
        SetOverLoad_Current_Limit       = 0x0A,
        SetOverLoad_Voltage_Limit       = 0x0B,
        SHUTDOWN_Device                 = 0x0C,
        ShowModeState                   = 0x0D,
        SendToDashboard                 = 0x15
        

};

/**
 * @}
 */

/**
 * @fn void App_CommManager_Init(void)
 * @brief Initializes the Application Communication Manager.
 * @details
 * Configures and initializes internal resources used by the communication
 * manager module (e.g., framing state, internal buffers, and protocol state
 * machines). This function does not enable or start lower-layer peripherals
 * (UART/Wi-Fi); those must be initialized separately.
 * @return void
 * @note Call this function once during system startup before calling any
 *       other `App_CommManager` APIs.
 */
void App_CommManager_Init(void);

/**
 * @fn void App_CommManager_Task(void)
 * @brief Periodic task for the Communication Manager.
 * @details
 * Performs periodic processing such as handling transmit queues, processing
 * received frames, timeouts, and performing housekeeping required by the
 * communication manager. This function is intended to be called regularly
 * from the main loop or a scheduler.
 * @return void
 * @warning Ensure `App_CommManager_Init()` has been called prior to invoking
 *          this function.
 */
void App_CommManager_Task(void);

/**
 * @fn void App_CommManager_SendFrame(uint8_t *data, uint16_t len)
 * @brief Sends a framed data packet through the communication manager.
 * @details
 * Accepts a raw data buffer, applies the selected framing protocol and
 * enqueues the framed packet for transmission over the active transport(s)
 * (UART, Bluetooth, Wi‑Fi, etc.). This function returns immediately after
 * queuing; actual transmission may be handled asynchronously by the
 * lower-layer driver.
 * @param data Pointer to the payload buffer to send.
 * @param len  Number of bytes in `data` to send.
 * @return void
 * @note The `data` pointer must remain valid until this function returns.
 * @warning Ensure the transport layer is initialized and not in error state
 *          before calling this function.
 */
void App_CommManager_SendFrame(uint8_t *data, uint8_t Command, uint16_t len);

/**
 * @fn void App_CommManager_ReceiveHandler(uint8_t *data, uint16_t len)
 * @brief Handler called by lower layers when raw framed data is received.
 * @details
 * This function is intended to be invoked by the transport driver (e.g.,
 * UART ISR or network receive callback) when a complete framed packet is
 * available. The communication manager will decode/validate the frame and
 * forward it to `App_CommManager_ProcessCommand` for command processing.
 * @param data Pointer to the received framed data buffer.
 * @param len  Length in bytes of the received buffer.
 * @return void
 * @note The buffer referenced by `data` may be owned by the caller; if the
 *       communication manager needs to retain it asynchronously it must copy
 *       the content before returning.
 */
void App_CommManager_ReceiveHandler();

/**
 * @fn void App_CommManager_ProcessCommand(uint8_t *frame)
 * @brief Processes a decoded command frame and routes it to the appropriate module.
 * @details
 * The function decodes the payload of a validated frame, extracts command ID
 * and parameters, and dispatches the command to the target application module
 * (e.g., configuration manager, sensor manager, or OTA handler). The exact
 * frame format is defined by the framing protocol used by the project.
 * @param frame Pointer to the decoded command frame. The frame format is
 *              implementation-specific (command ID + parameters).
 * @return void
 * @warning The caller must ensure `frame` points to a valid, decoded frame.
 * @note Consider extending the signature with a `len` parameter if variable
 *       length frames are expected to be processed in the future.
 */
void App_CommManager_ProcessCommand(uint8_t *frame);

uint8_t Accesslength();


/**
 * @}
 * @}
 */

#endif