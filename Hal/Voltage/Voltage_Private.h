/**
 * @file Voltage_Interface.h
 * @brief This file contains the Private Definitions for the Voltage module.
 * @version 1.0
 * @date 2024-10-24
 * @author Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author Reviewer : Mohamed Diaa   (mohammeddiaato@gmail.com)
 * @copyright Copyright (c) 2025 ,Gestell Company
 */




#ifndef Voltage_Private_H
#define Voltage_Private_H

typedef struct 
{
    
    float sumOfSquares;
    float Voltage_RMS_Value;
    float Voltage_Previous_RMS;
    uint16_t sampleCount;

}Voltage_RMS_Data;


#endif 