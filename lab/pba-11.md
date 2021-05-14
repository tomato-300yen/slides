---
marp: true
theme: custom
# header: "header"
# footer: "by **＠tomato-300yen**"
---

# Practical Binary Analysis #11

## Seminar @ Gondow Lab.

<!--
class: title
-->

---

<!--
class: slides
_footer: ''
paginate: true
-->

# What you will learn in this section

1. How to build your own DTA tools with `libdft`
   - with tools that
     - prevents remoto control-hijacking attacks
     - automatically detects information leaks

---

# Overview

1. introducing `libdft`
2. Using DTA to Detect Remote Control-Hijacking
3. Circumeventing DTA with implicit Flows
4. A DTA-Based Data Exfiltration Detector

---

# What is `libdft`

- Open source binary-level taint tracking library
- Byte-granularity taint-tracking system built on Intel Pin
- Supports only 32-bit x86
  - althogh you can use it on a 64-bit platform
- Relies on legacy versions of Pin
- Supports only for "Regular" x86 instructions,
  - not for extended instruction sets like MMX or SSE.

<note>
libdft is based on Pin (between 2.11 and 2.13)

64-bit version of libdft : https://github.com/AngoraFuzzer/libdft64
</note>

---

# Internals of libdft - Overview

---

# Internals of libdft - Shadow Memory

### Bitmap (1 colour)

- Supports only 1 taint colour.
- slightly faster and less memory

### STAB (8 colour)

- STAB : Segment Translation Table
- Supports 8 taint colour
-

# Internals of libdft - Virtual CPU

---

# Internals of libdft - Tracker

---
