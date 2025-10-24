/**
 * @file       GIE_Interface.h
 * @brief      
 * @details    
 * @version    0.1
 * @author     Hesham Ahmed (Hisham4Ahmed@gmail.com)
 * @date       2025-10-13
 * @copyright  Copyright (c) 2025 , Gestell Company 
 *   
 */  

#ifndef _GIE_INTERFACE_H_
#define _GIE_INTERFACE_H_

#include <stdint.h>
#include "../Atmega32RegistersAddress.h"
#include "../../Common/Macros.h"

#define I_Bit  7

void mGIE_Enable(void);
void mGIE_Disable(void);
#endif /*_GIE_INTERFACE_H_*/