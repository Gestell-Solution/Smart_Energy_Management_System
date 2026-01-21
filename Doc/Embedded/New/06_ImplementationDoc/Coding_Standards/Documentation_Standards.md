# Documentation Standards

---

## Doxygen Comments

**Function Header**:
```c
/**
 * @brief Calculate RMS value from samples
 * @param samples Pointer to sample buffer
 * @param count Number of samples
 * @return RMS value (float)
 */
float calculateRMS(uint16_t* samples, uint8_t count);
```

**File Header**:
```c
/**
 * @file       module.c
 * @brief      Module brief description
 * @author     Author Name
 * @date       YYYY-MM-DD
 * @version    1.0
 * @copyright  Copyright (c) 2025 Gestell Company
 */
```

---

## Inline Comments

- Explain **why**, not **what**
- Keep concise (single line preferred)
- Update when code changes

---

**Document Version**: 1.0  
**Last Updated**: January 2026
