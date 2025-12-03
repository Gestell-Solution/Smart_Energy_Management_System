/**
 * @file       DisplayManager_Private.h
 * @brief      This file contains The Private for Display Manager .    
 * @version    2.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Mohamed Diaa & Emg -Ahmed Twap
 * @date       2025-11-24
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
*/



#ifndef DISPLAYMANAGER_PRIVATE_H_
#define DISPLAYMANAGER_PRIVATE_H_

#include "DisplayManager_Config.h"
#include "../../Hal/LCD/LCD_Interface.h"
#include <stdbool.h>
#include <stdint.h>

/* Line indices (1-based) */
#define DM_LCD_LINE1   1
#define DM_LCD_LINE2   2
#define DM_LCD_LINE3   3
#define DM_LCD_LINE4   4

/* Clear-line string exactly DM_LCD_COLS characters (16) */
#define DM_CLEAR_LINE_STRING "                "  /* 16 spaces */

#endif /* DISPLAYMANAGER_PRIVATE_H_ */
