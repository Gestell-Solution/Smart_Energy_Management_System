/**
 * @file ADC_Private.h
 * @brief This file contains the private definitions for the ADC (Analog-to-Digital Converter) module.
 * @version 1.0
 * @author  Developer : Ahmed Ashraf  (ahmedashraf2022222@gmail.com)
 * @author  Reviewer  :
 * @date 2025-10-11
 * @copyright Copyright (c) 2025 , Gestell Company
 */

 #ifndef ADC_PRIVATE_H
 #define ADC_PRIVATE_H
#include <stdint.h>
/**
 * @struct ADC_ConfigType
 * @brief This structure holds the configuration parameters for the ADC module.
 * @details This structure is used to initialize the ADC with specific settings such as prescaler, voltage reference, and channel selection.
 * @note  This structure is defined in the private header file and should not be modified directly by users.
 */
typedef struct 
{
    uint8_t prescaler;        // ADC prescaler setting                        
    uint8_t voltageRef;      // Voltage reference selection                   
    uint8_t autoTrigger;     // Auto trigger enable/disable                   
    uint8_t interrupt;       // Interrupt enable/disable                      
    uint8_t triggerSource;   // Auto trigger source selection                 
    uint8_t enable;          // ADC enable/disable                            
  
}ADC_ConfigType;

/**
 * @defgroup ADC Prescaler Values
 * @brief This section defines the prescaler values for the ADC module.
 * @details The prescaler values determine the division factor for the ADC clock.
 * @{
 */
#define ADC_Prescaler_DIV2       1   /**< Prescaler Values for ADC */  
#define ADC_Prescaler_DIV4       2    /**< Prescaler Values for ADC */  
#define ADC_Prescaler_DIV8       3    /**< Prescaler Values for ADC */  
#define ADC_Prescaler_DIV16      4    /**< Prescaler Values for ADC */  
#define ADC_Prescaler_DIV32      5    /**< Prescaler Values for ADC */  
#define ADC_Prescaler_DIV64      6    /**< Prescaler Values for ADC */  
#define ADC_Prescaler_DIV128     7    /**< Prescaler Values for ADC */  
/** @} */ // End of Prescaler Values

/**
 * @defgroup ADC Channel Selection
 * @brief This section defines the channel selection options for the ADC module.
 * @{
 */
#define ADC0_Channel        0         /**< ADC Channels */  
#define ADC1_Channel        1         /**< ADC Channels */  
#define ADC2_Channel        2         /**< ADC Channels , not used now but expected to be used in future */ 
#define ADC3_Channel        3         /**< ADC Channels , not used now but expected to be used in future */ 
#define ADC4_Channel        4         /**< ADC Channels , not used now but expected to be used in future */ 
#define ADC5_Channel        5         /**< ADC Channels , not used now but expected to be used in future */ 
#define ADC6_Channel        6         /**< ADC Channels , not used now but expected to be used in future */ 
#define ADC7_Channel        7         /**< ADC Channels , not used now but expected to be used in future */ 
/** @} */ //End of ADC Channel Selection
/**
 * @defgroup ADC Voltage Options
 * @brief This section defines the voltage reference options for the ADC module.
 * @def  ADC_AREF
 * @brief This option selects the external AREF pin as the voltage reference.
 * @def  ADC_AVCC
 * @brief This option selects AVCC with an external capacitor at the AREF pin as the voltage reference.
 * @def  ADC_INTERNAL_2_56V
 * @brief This option selects the internal 2.56V voltage reference with an external capacitor at the AREF pin.
 * @{
 */
#define ADC_AREF               0    /**< Connect VCC with both AREF & AVCC Pins */  
#define ADC_AVCC               1    /**< VCC With AVCC & Capacitor with AREF */  
#define ADC_INTERNAL_2_56V     3   /**<  AVCC WITH VCC &  */  
/** @} */ // End of Voltage Options

/**
 * @defgroup ADMUX_Reg Bits Names
 * @brief This section defines the bit names for the ADMUX register.
 * @details The ADMUX register is used to configure the ADC settings such as channel selection and voltage reference.
 * @{
 */
#define MUX0_bit  0        /**< These Bits for selecting ADC Channel */  
#define MUX1_bit  1        /**< These Bits for selecting ADC Channel */
#define MUX2_bit  2        /**< These Bits for selecting ADC Channel */
#define MUX3_bit  3        /**< These Bits for selecting ADC Channel */
#define MUX4_bit  4        /**< These Bits for selecting ADC Channel */
#define ADLAR_bit 5        /* must be zero*/                            
#define REFS0_bit 6        /**< Bits for Voltage Reference Selection */ 
#define REFS1_bit 7        /**< Bits for Voltage Reference Selection */ 
/** @} */ // End of ADMUX_Reg Bits Names

/**
 * @defgroup ADCSRA_Reg Bits Names
 * @brief This section defines the bit names for the ADCSRA register.
 * @{
 */
#define ADPS0_bit 0       /**< Bits for ADC Prescaler Selection */                
#define ADPS1_bit 1       /**< Bits for ADC Prescaler Selection */                
#define ADPS2_bit 2       /**< Bits for ADC Prescaler Selection */                
#define ADIE_bit  3       /**< ADC Interrupt Enable Bit */                        
#define ADIF_bit  4       /**< ADC Interrupt Flag Bit */                          
#define ADATE_bit 5       /**< ADC Auto Trigger Enable Bit */                    
#define ADSC_bit  6       /**< ADC Start Conversion Bit */                       
#define ADEN_bit  7       /**< ADC Enable Bit */        
/** @} */ // End of ADCSRA_Reg Bits Names                         

/**
 * @defgroup SFIOR_Reg Bits Names
 * @brief This section defines the bit names for the SFIOR register.
 * @note  bit 4 is reserved & Bits from 0:3 are for PUD, PSR2, PSR10 preferable not to touch them
 * @{
 */

#define ADTS0_bit 5          /**< Bits for Auto Trigger Selection */   
#define ADTS1_bit 6          /**< Bits for Auto Trigger Selection */   
#define ADTS2_bit 7          /**< Bits for Auto Trigger Selection */        
/** @} */ // End of SFIOR_Reg Bits Names

/**
 * @defgroup ADC Auto Trigger Source Options
 * @brief This section defines the auto trigger source options for the ADC module.
 * @{
 */
#define ADC_FREE_RUNNING_MODE       0    /**< Auto Trigger Source Options */   
#define ADC_ANALOG_COMPARATOR       1    /**< Auto Trigger Source Options */   
#define ADC_EXTERNAL_INTERRUPT0     2    /**< Auto Trigger Source Options */   
#define ADC_TIMER0_COMPARE_MATCH    3    /**< Auto Trigger Source Options */   
#define ADC_TIMER0_OVERFLOW         4    /**< Auto Trigger Source Options */   
#define ADC_TIMER1_COMPARE_MATCH_B  5    /**< Auto Trigger Source Options */   
#define ADC_TIMER1_OVERFLOW         6    /**< Auto Trigger Source Options */   
#define ADC_TIMER1_CAPTURE_EVENT    7    /**< Auto Trigger Source Options */   
/** @} */ // End of Auto Trigger Source Options

/**
 * @defgroup ADC Auto Trigger Enable/Disable
 * @brief This section defines the enable/disable options for the ADC auto trigger feature.
 * @details Enabling auto trigger allows the ADC to start conversions automatically based on the selected trigger source.
 * @note  This feature is useful for applications that require periodic or event-driven ADC conversions without CPU intervention.
 * @{
 */
#define ADC_AUTO_TRIGGER_Enable    1    
#define ADC_AUTO_TRIGGER_Disable   0 
/** @} */ // End of Auto Trigger Options   
/**
 * @defgroup ADC Interrupt Enable/Disable
 * @brief This section defines the enable/disable options for the ADC interrupt feature.
 * @{
 */
#define ADC_INTERRUPT_Enable       1   
#define ADC_INTERRUPT_Disable      0   
/** @} */ // End of Interrupt Options
/**
 * @defgroup ADC Enable/Disable
 * @brief This section defines the enable/disable options for the ADC module. by setting or clearing the ADEN bit in the ADCSRA register.
 * @{
 */
#define ADC_ENABLE                 1  
#define ADC_DISABLE                0  
/** @} */ // End of ADC Enable/Disable
/**
 * @defgroup ADC Operation Modes
 * @brief This section defines the operation modes for the ADC module.
 * @def   Synchronous_Mode
 * @brief This mode is used for blocking (synchronous) ADC reads where the CPU waits for the conversion to complete.
 * @def   Asynchronous_Mode
 * @brief This mode is used for non-blocking (asynchronous) ADC reads using interrupts and auto-triggering. round-robin sampling of multiple channels.
 * @{
 */
#define Synchronous_Mode           1  
#define Asynchronous_Mode          0   
/** @} */ // End of ADC Operation Modes

/**
 * @defgroup ADC Channel Mask
 * @brief This section defines masks for selecting ADC channels.
 * @{
 */
#define ADC_Channel_UpperNibble_Mask   0xF0   
#define ADC_Channel_LowerNibble_Mask   0x07  
/** @} */ // End of ADC Channel Mask
/**
 * @def ADC_Max_used_Channel
 * @brief This macro represents the adc channles used in our project (starting from 0)
 */
#define ADC_Max_used_Channel        2// we are using only channel 0 and 1 in the application 

/**
 * @def Flagdown
 * @brief This macro is used to represent the flag down state
 * @def Flagup
 * @brief This macro is used to represent the flag up state
 */
#define Flagdown 0
#define Flagup   1

 #endif // End Of ADC_PRIVATE_H