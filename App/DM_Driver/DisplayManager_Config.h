/**
 * @file       Display Manager_Config.h
 * @brief      This file contains The Config for Display Manager .    
 * @version    1.0
 * @author     Developer : Mohamed Abdelgaber (mohamedabdelgaber247@gmail.com)
 * @author     Reviewer : Eng -Mohamed Diaa & Emg -Ahmed Twap
 * @date       2025-11-7
 * @copyright  Copyright (c) 2025 , Gestell Company 
 * 
*/



#ifndef DISPLAYMANAGER_CONFIG_H_
#define DISPLAYMANAGER_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

/* LCD geometry */
#define DM_LCD_COLS   16
#define DM_LCD_ROWS   4

/* Refresh interval (ms) used by test loops */
#define DM_REFRESH_MS 500

/* Messages (exact strings used by the display module) */
#define DM_MSG_NORMAL        "STATUS: NORMAL"   /* 14 chars */
#define DM_MSG_OVERLOAD_L1   "OVERLOAD TRIP!"   /* 15 chars */
#define DM_MSG_OVERLOAD_L2   "PRESS RESET"      /* 11 chars */
#define DM_MSG_AUTO          "Mode:AUTO"        /* 9 chars  */
#define DM_MSG_MANUAL        "Mode:MANUAL"      /* 11 chars */

#endif /* DISPLAYMANAGER_CONFIG_H_ */
