/**
 * @file Calibration_Manager_Private.h
 * @brief Contains the Private Definitions and Declarations for the Calibration Manager module.
 * @version 1.0
 * @author Developer : Ahmed Ashraf (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Mohamed Diaa (mohammediaato@gmail.com)
 * @date 2025-12-04
 * @copyright Copyright (c) 2025, Gestell Company
 */

#ifndef _CALIBRATION_MANAGER_PRIVATE_H_
#define _CALIBRATION_MANAGER_PRIVATE_H_
#include <stdint.h>
#include "../../Common/Macros.h"
/**
 * @def Matches
 * @brief Macro indicating that two strings match.
 */
#define Matches        0
/**
 * @def NotMatches
 * @brief Macro indicating that two strings do not match.
 */
#define NotMatches     1

/**
 * @fn Compare_Strings
 * @brief Compares Two Strings.
 * @param str1 Pointer to the First String.
 * @param str2 Pointer to the Second String.
 * @return Returns Matches if Strings are Identical, Otherwise Returns NotMatches.
 */
uint8_t Compare_Strings(const uint8_t* str1, const uint8_t* str2) ;



#endif /* _CALIBRATION_MANAGER_PRIVATE_H_ */