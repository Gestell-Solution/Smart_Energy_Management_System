# Code Review Checklist

---

## Before Submitting Code

- [ ] All functions have Doxygen comments
- [ ] No compiler warnings (`-Wall -Wextra`)
- [ ] MISRA C compliance checked
- [ ] All magic numbers replaced with named constants
- [ ] Function length < 50 lines
- [ ] Maximum nesting depth < 4 levels
- [ ] Memory leaks checked (use static allocation)
- [ ] Code tested on target hardware
- [ ] Build successful: `make clean && make all`
- [ ] Flash successful: `make flash`
- [ ] Functional testing completed

---

## Code Quality Metrics

**Cyclomatic Complexity**: < 10 per function  
**Code Coverage**: > 80% (unit tests)  
**Documentation Coverage**: 100% (all public APIs)

---

**Document Version**: 1.0  
**Last Updated**: January 2026
