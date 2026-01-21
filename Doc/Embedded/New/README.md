# Smart Energy Management System - Embedded Documentation

## 📋 Overview

Welcome to the comprehensive documentation for the **Smart Energy Management System** Embedded firmware. This documentation covers all aspects of the embedded system running on the **ATmega32 microcontroller**.

**System Purpose**: Real-time energy monitoring and protection system with remote monitoring capabilities via Bluetooth or WiFi.

---

## 🎯 Quick Navigation by Role

### 👨💻 For Developers

**Start Here:**

1. [System Architecture](./03_System_Architecture/HLD/HLD.md) - Understand the overall design
2. [Software Requirements](./02_Requirements/SRS/SRS.md) - Know what needs to be implemented
3. [Low-Level Design](./04_Detailed_Design/LLD/LLD.md) - Detailed module designs
4. [MCAL Layer Documentation](./06_ImplementationDoc/MCAL_Layer/MCAL_Overview.md) - Hardware drivers
5. [HAL Layer Documentation](./06_ImplementationDoc/HAL_Layer/HAL_Overview.md) - Hardware abstraction
6. [Application Layer Documentation](./06_ImplementationDoc/Application_Layer/App_Overview.md) - Application logic

**Critical Fixes & Issues:**

- [Fix Tasks Overview](./06_ImplementationDoc/FixTasks/README.md) - 8 critical fixes documented
- [Issue Solutions](./06_ImplementationDoc/Issue_Solutions.md) - Detailed problem analysis and solutions
- [Code Analysis Report](./06_ImplementationDoc/Code_Analysis_Report.md) - Current code quality assessment

### 🧪 For Testers

**Start Here:**

1. [Test Strategy](./07_Testing_Validation/01_Test_Planning/Test_Strategy.md) - Overall testing approach
2. [Test Cases](./07_Testing_Validation/01_Test_Planning/Test_Cases.md) - Detailed test scenarios
3. [Test Procedures](./07_Testing_Validation/02_Test_Procedures/) - How to execute tests
4. [Known Issues](./06_ImplementationDoc/FixTasks/) - Current bugs and fixes

### 📊 For Project Managers

**Start Here:**

1. [Project Charter](./01_Managment/Project_Managment/Project_Charter.md) - Project overview
2. [Implementation Plan](./06_ImplementationDoc/Embedded_Fix_Implementation_Plan.md) - Development roadmap
3. [Risk Management](./01_Managment/Risk_Management.md) - Project risks
4. [Requirements Traceability](./08_Traceability/Requirements_Traceability_Matrix.md) - Requirements tracking

### 👤 For End Users

**Start Here:**

1. [User Manual](./10_User_Documentation/User_Manual.md) - How to use the system
2. [Error Codes List](./10_User_Documentation/Error_Codes_List.md) - Understanding error messages
3. [Troubleshooting Guide](./11_Maintenance_Support/Troubleshooting_Guide.md) - Common problems and solutions

---

## 📁 Documentation Structure

### 01_Managment

Project management and workflow documentation

- **[Project_Managment/](./01_Managment/Project_Managment/)** - Project charter, risks, stakeholders
- **[Workflow_Managment/](./01_Managment/Workflow_Managment/)** - Git workflow and processes

### 02_Requirements

System requirements and specifications

- **[SRS/](./02_Requirements/SRS/)** - Software Requirements Specification
- **[HRS/](./02_Requirements/HRS/)** - Hardware Requirements Specification
- **[CRS/](./02_Requirements/CRS/)** - Customer Requirements Specification
- **[DashboardRequirementfromEmbedded.md](./02_Requirements/DashboardRequirementfromEmbedded.md)** - Dashboard communication protocol
- **[MobileRequirementfromEmbedded.md](./02_Requirements/MobileRequirementfromEmbedded.md)** - Mobile App communication protocol
- **[PinOut_Reference.md](./02_Requirements/AddationalInformation/PinOut_Reference.md)** - ATmega32 pin assignments

### 03_System_Architecture

High-level system architecture and design

- **[HLD/](./03_System_Architecture/HLD/)** - High-Level Design
- **[System_Block_Diagram/](./03_System_Architecture/System_Block_Diagram/)** - System diagrams
- **[Hardware_Architecture/](./03_System_Architecture/Hardware_Architecture/)** - Hardware components
- **[HW_SW_Partitioning/](./03_System_Architecture/HW_SW_Partitioning/)** - Hardware vs Software responsibilities

**Key Info**: System has two versions:

- **Version 1**: HC-05 Bluetooth communication
- **Version 2**: ESP-01 WiFi communication

### 04_Detailed_Design

Low-level design and implementation details

- **[LLD/](./04_Detailed_Design/LLD/)** - Low-Level Design with algorithms
- **[Memory_Map/](./04_Detailed_Design/Memory_Map/)** - ATmega32 memory usage
- **[Pin_Assignment/](./04_Detailed_Design/Pin_Assignment/)** - Detailed pin configuration
- **[State_Machines/](./04_Detailed_Design/State_Machines/)** - System state diagrams
- **[Interface_Control_Document_ICD/](./04_Detailed_Design/Interface_Control_Document_ICD/)** - Module interfaces

### 05_Hardware_Design

Hardware schematics, BOM, and PCB design

- **[Component_Datasheets/Component_DatasheetsList.md](./05_Hardware_Design/Component_Datasheets/Component_DatasheetsList.md)** ⭐ **Complete component list with datasheet links**
- **[BOM/](./05_Hardware_Design/BOM/)** - Bill of Materials
- **[Schematics/](./05_Hardware_Design/Schematics/)** - Circuit diagrams
- **[PCB_Layout/](./05_Hardware_Design/PCB_Layout/)** - PCB design files

**Key Components**: ATmega32, ACS712 current sensor, voltage divider, LCD 16x2, HC-05/ESP-01, relays

### 06_ImplementationDoc ⭐ **Most Important for Developers**

Detailed implementation documentation

#### Code Analysis & Fixes

- **[FixTasks/](./06_ImplementationDoc/FixTasks/)** - 8 critical fix tasks (UNCHANGED from original)
- **[Code_Analysis_Report.md](./06_ImplementationDoc/Code_Analysis_Report.md)** - Comprehensive code analysis
- **[Issue_Solutions.md](./06_ImplementationDoc/Issue_Solutions.md)** - Detailed problem solutions
- **[Embedded_Fix_Implementation_Plan.md](./06_ImplementationDoc/Embedded_Fix_Implementation_Plan.md)** - Implementation roadmap

#### Software Layers

- **[MCAL_Layer/](./06_ImplementationDoc/MCAL_Layer/)** - Microcontroller Abstraction Layer (ADC, UART, Timers, EEPROM, DIO, etc.)
- **[HAL_Layer/](./06_ImplementationDoc/HAL_Layer/)** - Hardware Abstraction Layer (sensors, LCD, relays, communication)
- **[Application_Layer/](./06_ImplementationDoc/Application_Layer/)** - Application logic (measurement, protection, energy logging, communication)
- **[Common_Layer/](./06_ImplementationDoc/Common_Layer/)** - Shared data structures and utilities

#### Development Guides

- **[BSP_Documentation/](./06_ImplementationDoc/BSP_Documentation/)** - Board Support Package for ATmega32
- **[Build_Instructions/](./06_ImplementationDoc/Build_Instructions/)** - Build system and compilation
- **[Coding_Standards/](./06_ImplementationDoc/Coding_Standards/)** - Coding guidelines and standards
- **[Embedded_System_Config/](./06_ImplementationDoc/Embedded_System_Config/)** - Memory, interrupts, timing (bare-metal - no RTOS)

### 07_Testing_Validation

Test planning, procedures, and reports

- **[01_Test_Planning/](./07_Testing_Validation/01_Test_Planning/)** - Test strategy and test cases
- **[02_Test_Procedures/](./07_Testing_Validation/02_Test_Procedures/)** - Test execution procedures
- **[03_Test_Reports/](./07_Testing_Validation/03_Test_Reports/)** - Test results

### 08_Traceability

Requirements and test traceability matrices

- **[Requirements_Traceability_Matrix.md](./08_Traceability/Requirements_Traceability_Matrix.md)** - Map requirements to design/code
- **[Test_Traceability_Matrix.md](./08_Traceability/Test_Traceability_Matrix.md)** - Map requirements to tests

### 09_Deployment

Deployment, installation, and calibration guides

- **[Installation_Guide.md](./09_Deployment/Installation_Guide.md)** - Field installation procedures
- **[Calibration_Procedure.md](./09_Deployment/Calibration_Procedure.md)** - Sensor calibration steps
- **[Firmware_Release_Procedure.md](./09_Deployment/Firmware_Release_Procedure.md)** - Release process
- **[User_Manual.md](./09_Deployment/User_Manual.md)** - End-user guide

### 10_User_Documentation

End-user documentation

- **[User_Manual.md](./10_User_Documentation/User_Manual.md)** - Complete user guide
- **[Operator_Guide.md](./10_User_Documentation/Operator_Guide.md)** - Quick reference
- **[Error_Codes_List.md](./10_User_Documentation/Error_Codes_List.md)** - Error code reference

### 11_Maintenance_Support

Maintenance and field support documentation

- **[Maintenance_Manual.md](./11_Maintenance_Support/Maintenance_Manual.md)** - Maintenance procedures
- **[Troubleshooting_Guide.md](./11_Maintenance_Support/Troubleshooting_Guide.md)** - Problem diagnosis and solutions
- **[Field_Update_Procedure.md](./11_Maintenance_Support/Field_Update_Procedure.md)** - Firmware update in field
- **[Spare_Parts_List.md](./11_Maintenance_Support/Spare_Parts_List.md)** - Recommended spares

### 12_Release_Configuration

Release management and version control

- **[Release_Notes.md](./12_Release_Configuration/Release_Notes.md)** - Version history
- **[Configuration_Management_Plan.md](./12_Release_Configuration/Configuration_Management_Plan.md)** - Config management
- **[Version_Control_Strategy.md](./12_Release_Configuration/Version_Control_Strategy.md)** - Git strategy

---

## ⚡ Critical Fixes & Known Issues

### 🔴 Critical Priority (Must Fix Immediately)

| Task                                                                                      | Issue                    | File               | Impact                           |
| ----------------------------------------------------------------------------------------- | ------------------------ | ------------------ | -------------------------------- |
| [Task01](./06_ImplementationDoc/FixTasks/Task01_Fix_Timer1_Duplicate_ISR.md)              | Timer1 Duplicate ISR     | TIMER1_Program.c   | Double sampling rate             |
| [Task02](./06_ImplementationDoc/FixTasks/Task02_Fix_Current_Sensor_Variable_Shadowing.md) | Current Sensor Shadowing | hCurrent_Program.c | Wrong current readings           |
| [Task03](./06_ImplementationDoc/FixTasks/Task03_Add_Power_Factor.md)                      | Missing Power Factor     | MeasurementEngine  | 15-40% energy error              |
| [Task04](./06_ImplementationDoc/FixTasks/Task04_Fix_Protection_Manager_Debouncing.md)     | No Protection Debouncing | ProtectionManager  | False protection trips           |
| [Task05](./06_ImplementationDoc/FixTasks/Task05_Communication_Protocol_Dual_Format.md)    | Dual Protocol Format     | CommManager        | Mobile/Dashboard incompatibility |

### ⚠️ High Priority

| Task                                                                          | Issue                     | Impact                |
| ----------------------------------------------------------------------------- | ------------------------- | --------------------- |
| [Task06](./06_ImplementationDoc/FixTasks/Task06_Add_EEPROM_Timeout.md)        | EEPROM No Timeout         | System can freeze     |
| [Task07](./06_ImplementationDoc/FixTasks/Task07_Energy_Persistence_EEPROM.md) | Missing Data Persistence  | Energy data loss      |
| [Task08](./06_ImplementationDoc/FixTasks/Task08_Fix_Voltage_ADC_Divisor.md)   | Voltage ADC Divisor Error | ±0.22V accuracy issue |

---

## 🔧 System Specifications

### Microcontroller

- **MCU**: ATmega32A
- **Architecture**: 8-bit AVR RISC
- **Flash**: 32 KB
- **SRAM**: 2 KB
- **EEPROM**: 1 KB
- **Clock**: 16 MHz external crystal
- **GPIO**: 32 pins (PORTA-PORTD)

### Software Architecture

- **Implementation**: Bare-metal (no RTOS)
- **Layers**: 4 (Application, HAL, MCAL, Common)
- **Programming Language**: C
- **Compiler**: avr-gcc

### Key Features

- Real-time RMS voltage and current measurement
- Power and energy calculation
- Overcurrent and overvoltage protection
- EEPROM data persistence
- Dual communication options (Bluetooth/WiFi)
- LCD user interface
- Relay control for load switching

---

## 📞 Support & Contact

**Project**: Smart Energy Management System  
**Company**: Gestell  
**Lead Engineer**: Eng. Hesham Ahmed  
**Email**: Hisham4Ahmed@gmail.com

**Development Team**:

1. Mohamed Diaa - mohammediaato@gmail.com
2. Ahmed Ashraf - ahmedashraf2022222@gmail.com
3. Mohamed Abdelgaber - mohamedabdelgaber247@gmail.com
4. Basma Khaled - basmak55@gmail.com
5. Ahmed Twap - ahmedtwap2@gmail.com

---

## 📝 Documentation Notes

- **Content Type**: All documentation contains text descriptions and diagrams - **no code snippets** (code is in the firmware repository)
- **Microcontroller**: All references are to **ATmega32** (not ATmega128)
- **System Versions**: Documentation covers both Bluetooth (V1) and WiFi (V2) variants
- **Fix Tasks**: FixTasks folder content is copied unchanged from original analysis
- **Empty Sections**: Some sections (01_Managment, parts of 05_Hardware_Design) are placeholder structures to be filled later

---

## 📄 Document Control

| Attribute         | Value                          |
| ----------------- | ------------------------------ |
| **Project**       | Smart Energy Management System |
| **Component**     | Embedded System Documentation  |
| **Version**       | 1.0                            |
| **Status**        | Active                         |
| **Last Updated**  | January 2026                   |
| **Maintained By** | Gestell Engineering Team       |

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
