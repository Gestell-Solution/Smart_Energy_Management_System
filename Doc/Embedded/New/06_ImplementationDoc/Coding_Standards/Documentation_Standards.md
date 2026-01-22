# Documentation Standards

---

## Doxygen Comments

**Function Header Format**:

| Tag       | Purpose                    | Example                              |
| --------- | -------------------------- | ------------------------------------ |
| `@brief`  | Short function description | "Calculate RMS value from samples"   |
| `@param`  | Parameter description      | "samples - Pointer to sample buffer" |
| `@param`  | Parameter description      | "count - Number of samples"          |
| `@return` | Return value description   | "RMS value (float)"                  |

**Example**: Function `calculateRMS` that takes samples pointer and count, returns float RMS value

---

**File Header Format**:

| Tag          | Purpose            | Example                              |
| ------------ | ------------------ | ------------------------------------ |
| `@file`      | Source filename    | "module.c"                           |
| `@brief`     | Module description | "Module brief description"           |
| `@author`    | Developer name     | "Author Name"                        |
| `@date`      | Creation date      | "YYYY-MM-DD"                         |
| `@version`   | Version number     | "1.0"                                |
| `@copyright` | Copyright notice   | "Copyright (c) 2025 Gestell Company" |

---

## Inline Comments

- Explain **why**, not **what**
- Keep concise (single line preferred)
- Update when code changes

---

**Document Version**: 1.0  
**Last Updated**: January 2026
