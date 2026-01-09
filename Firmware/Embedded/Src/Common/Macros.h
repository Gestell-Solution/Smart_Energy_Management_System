/**
 * @file       Macros.h
 * @brief      This file contains commonly used macros for the project.
 * @details    This header file defines several macros that are used throughout the project to enhance code readability and maintainability.
 *             It includes macros for array size calculation, safe deletion of pointers, and other utility macros
 * @version    0.1
 * @author     Hesham Ahmed (Hisham4Ahmed@gmail.com)
 * @date       2025-10-01
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
 */

#ifndef _MACROS_H_
#define _MACROS_H_

/**
 * @addtogroup Common
 * @{
 */

/**
 * @defgroup  Common Functions Macros
 * @brief     This section contains commonly used macros for various operations.
 * @details   The macros defined in this section are intended to simplify common tasks and improve code readability.
 * @{
 */ 

/**
 * @def   SetBit 
 * @brief This Macro is Responsible for 
 */
#define SetBit(Reg,Bit)                ((Reg)|=(1<<(Bit)))

/**
 * @def   ClearBit
 * @brief This Macro is Responsible for 
 */
#define ClearBit(Reg,Bit)              ((Reg)&=~(1<<(Bit)))

/**
 * @def   ToggleBit
 * @brief This Macro is Responsible for 
 */
#define ToggleBit(Reg,Bit)            ((Reg)^=(1<<(Bit)))

/**
 * @def   GetBit
 * @brief This Macro is Responsible for 
 */
#define GetBit(Reg,Bit)                (((Reg)>>(Bit))&0x01)

/**
 * @def    ClearFlag
 * @brief  This Macro is Responsible for 
 */
#define ClearFlag(Reg,FlagBit)         SetBit(Reg,FlagBit)

/**
 * @def    ReadFlag
 * @brief  This Macro is Responsible for 
 */
#define  ReadFlag(Reg,FlagBit)         GetBit(Reg,FlagBit)  

/** @} */
/*****************************************************************************************************************************************************/
/**
 * @defgroup  Common Object  Macros 
 * @brief     This section contains commonly used macros for object manipulation.
 * @details   The macros defined in this section are intended to simplify object management tasks and improve code readability.
 * @{
 */

/**
 * @def     Null 
 * @brief   This Macro is Responsible for
 */
#define Null        (void*)0

/**
 * @def     Null 
 * @brief   This Macro is Responsible for
 */
#define NullChar    '\0'


/**
 * @def     Input
 * @brief   This Macro is Responsible for
 * @def     Output
 * @brief   This Macro is Responsible for
 */
#define Input    0 
#define Output   1


/**
 * @def     Low
 * @brief   This Macro is Responsible for
 * @def     High
 * @brief   This Macro is Responsible for
 */
#define Low      0 
#define High     1

/**
 * @def     FlagDown
 * @brief   This Macro is Responsible for
 * @def     FlagUp
 * @brief   This Macro is Responsible for
 */
#define FlagDown   0 
#define FlagUp     1

/**
 * @def     Enabled
 * @brief   This Macro is Responsible for
 * @def     Disabled
 * @brief   This Macro is Responsible for
 */
#define Disable    0 
#define Enable     1

/** @}*/

/*****************************************************************************************************************************************************/


/**
 * @}
 */

#endif /* _MACROS_H_ */