/**
 * @file ProtectionManager_Private.h
 * @brief This File Contains the Protection Manager Private Parameters
 * @version 1.0
 * @date 11-11-2025
 * @author Developer : Ahmed Ashraf (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Mohamed Diaa (mohammeddiaato@gmail.com)
 * @copyright Copyright (c) 2025 , Gestell Company
 */
#ifndef _PM_PRIVATE_H_
#define _PM_PRIVATE_H_


/**
 * @def EXT1_Macro
 * @brief EXTI Line used for the Reset Button
 */
#define EXT1_Macro 1

/**
 * @def Fixed
 * @brief State when the problem is fixed
 */
#define Fixed 0
/**
 * @def Not_Fixed
 * @brief State when the problem is not fixed
 * @details Fixed , Not_Fixed Are used to check if the problem is fixed or not in order to display the correct message
 */
#define Not_Fixed 1
#endif