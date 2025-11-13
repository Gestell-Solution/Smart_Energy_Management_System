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



#include <stdint.h>
/**
 * 
 */
void App_CommManager_Init(void);
/**
 * 
 */
void App_CommManager_Task(void);                
/**
 * 
 */
void App_CommManager_SendFrame(uint8_t *data, uint16_t len);
/**
 * 
 */
void App_CommManager_ReceiveHandler(uint8_t *data, uint16_t len); 
/**
 * 
 */
void App_CommManager_ProcessCommand(uint8_t *frame);

#endif