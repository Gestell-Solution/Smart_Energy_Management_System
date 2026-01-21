# Coding Standards

**Project**: Smart Energy Management System  
**Standard**: MISRA-C subset + Embedded C best practices

---

## Key Rules

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

**Document Version**: 1.0  
**Last Updated**: January 2026
