/**
 * @file    App_CommManager.c
 * @brief   This File is for Implementing the  main APIs  for the App layer Communication Manager.
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

#include  "App_CommManager.h"

