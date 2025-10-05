/**
 * @file EEPROM_Program.c
 * @brief This file contains the implementation of the EEPROM module.
 * @version 1.0
 * @author Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author Reviewer  : Basma Khaled  (basmak55@gmail.com)
 * @date 2025-10-5
 * @copyright Copyright (c) 2025 , Gestell Company
 */

#include "../../Common/Config.h"

#if EEP_Module == Enable
#include "EEPROM_Interface.h"
void mEEPROM_WriteByte(uint16_t Address, uint8_t Data)
{ 
    if (Address > AVR_EEPROM_MAXAddress)
    {
        // Handle invalid address error
        return;
    }
    
    /**
     * psuedo code 
     * - Wait for completion of previous write
     * - Set up address and data registers
     */

    while (GetBit(EECR_Reg, EEWE_Bit) == 1)
    {
        // Wait until the previous write operation is complete
    }

    EEAR_Reg = Address; // Set the EEPROM address
    EEDR_Reg = Data;    // Set the data to be written
    SetBit(EECR_Reg, EEMWE_Bit); // Set the Master Write Enable bit
    SetBit(EECR_Reg, EEWE_Bit);  // Start the write by setting the Write Enable bit
  
}
uint8_t mEEPROM_ReadByte(uint16_t Address)
{
 /**
  * psuedo code
  * - Wait for completion of previous write
  * - Set up address register
  * - Start EEPROM read by writing EERE_Bit in EECR
  * - Return data from data register
  */
    if (Address > AVR_EEPROM_MAXAddress)
    {
        // Handle invalid address error
        return 0; // Return 0 or some error code
    }

    while (GetBit(EECR_Reg, EEWE_Bit) == 1)
    {
        // Wait until the previous write operation is complete
    }

    EEAR_Reg = Address; // Set the EEPROM address

    SetBit(EECR_Reg, EERE_Bit); // Start the read by setting the Read Enable bit

    return EEDR_Reg; // Return the data from the data register
  
}
void mEEPROM_WriteBlock(uint16_t Address, uint8_t* data, uint16_t length)
{
    if (Address + length - 1 > AVR_EEPROM_MAXAddress)
    {
        // Handle invalid address error
        return;
    }
    /**
     * psuedo code
     * - Loop through the data block and write each byte using mEEPROM_WriteByte
     */
    for (uint16_t index = 0; index < length; index++)
    {
        mEEPROM_WriteByte(Address + index, data[index]);
    }

}
void mEEPROM_ReadBlock(uint16_t Address, uint8_t* data, uint16_t length)
{
    if (Address + length - 1 > AVR_EEPROM_MAXAddress)
    {
        // Handle invalid address error
        return;
    }

    /**
     * psuedo code
     * - Loop through the data block and read each byte using mEEPROM_ReadByte
     */
    for (uint16_t index = 0; index < length; index++)
    {
        data[index] = mEEPROM_ReadByte(Address + index);
    }

}



#endif