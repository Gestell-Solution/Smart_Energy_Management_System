# 📏 Coding Standards

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-Embedded_C-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Standard-brightgreen)

**Coding Standards**

**Smart Energy Management System - C Programming Guidelines**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Key Rules](#-key-rules)
- [Quality Checklist](#-code-quality-checklist)
- [Related Guides](#-related-documentation)

---

## 🔗 Related Documentation

| Document                                                     | Description   | Status       |
| ------------------------------------------------------------ | ------------- | ------------ |
| **[Naming_Conventions.md](Naming_Conventions.md)**           | Naming Rules  | ✅ Available |
| **[Safety_Critical_Coding.md](Safety_Critical_Coding.md)**   | Safety Rules  | ✅ Available |
| **[Documentation_Standards.md](Documentation_Standards.md)** | Doc Standards | ✅ Available |

---

## 🔑 Key Rules

**Function Length**: Maximum 50 lines  
**Nesting Depth**: Maximum 4 levels  
**Magic Numbers**: Forbidden - use `#define` constants  
**Comments**: Doxygen style for all public functions  
**Naming**: Consistent naming conventions (see [Naming_Conventions.md](Naming_Conventions.md))

---

## Code Quality Checklist

- [ ] All functions documented with Doxygen comments
- [ ] No compiler warnings (`-Wall -Wextra`)
- [ ] No magic numbers (replace with named constants)
- [ ] Function length ≤ 50 lines
- [ ] Maximum nesting depth ≤ 4
- [ ] Consistent naming conventions followed
- [ ] Memory leaks checked (static allocation preferred)
- [ ] Tested on target hardware

---

## See Also

- [Module_Development_Guide.md](Module_Development_Guide.md)
- [Documentation_Standards.md](Documentation_Standards.md)
- [Code_Review_Checklist.md](Code_Review_Checklist.md)

---

## 📞 Support & Contact

**Project Information**:

- **Project Name**: Smart Energy Management System
- **Development Company**: Gestell - Professional Embedded Solutions

**Technical Support**: Hisham4Ahmed@gmail.com

---

## 📄 Document Control

| Attribute            | Value                    |
| -------------------- | ------------------------ |
| **Document Type**    | Coding Standards         |
| **Document Status**  | Active                   |
| **Document Version** | 1.0                      |
| **Last Updated**     | January 2026             |
| **Prepared By**      | Gestell Engineering Team |

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
