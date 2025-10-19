/**
 * @file       RGB_Program.h
 * @brief      This file contains RGB LED program functions.
 * @version    1.0
 * @author     Developer : Ahmed twap  (ahmedtwap2@gmail.com)
 * @author     Reviewer : bassma khaled
 * @date       2025-10-19
 * @copyright  Copyright (c) 2025 , Gestell Company
 *
 */

#include "../../Common/Config.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "RGB_Interface.h"

#if RGB_Driver == Enable
void RGB_Init(void)
{

    mDIO_SetDirectionForPin(RGB_RED_GROUP,RGB_RED_PIN,Output);
    mDIO_SetDirectionForPin(RGB_GREEN_GROUP,RGB_GREEN_PIN,Output);
}

void RGB_SetColor(uint8_t red, uint8_t green)
{
    mDIO_WritePin(RGB_RED_GROUP,RGB_RED_PIN,red);
    mDIO_WritePin(RGB_GREEN_GROUP,RGB_GREEN_PIN,green);
}

void RGB_SetState(RGB_State_t state)
{
    switch(state)
    {
        case RGB_OFF:
            RGB_SetColor(0,0);
            break;
        case RGB_RED:
            RGB_SetColor(1,0);
            break;
        case RGB_GREEN:
            RGB_SetColor(0,1);
            break;
        case RGB_ERROR:
            RGB_SetColor(1,0);
            break;
        case RGB_NORMAL:
            RGB_SetColor(0,1);
            break;
        default:
            // Handle invalid state if necessary
            break;
    }
}

#endif /* _RGB_PROGRAM_H_ */