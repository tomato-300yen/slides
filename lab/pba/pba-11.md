---
marp: true
theme: custom
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

# What you will learn

1. Internals of `libdft`
   a. data structure of `libdft`
   b. how `libdft` works

1. How to build a DTA tools with `libdft`
   a. a tool that prevents remoto control-hijacking attacks
   b. a tool that automatically detects information leaks

---

# Overview

1. about `libdft`
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
<code>libdft</code> is based on Pin (between 2.11 and 2.13)

64-bit version of <code>libdft</code> : https://github.com/AngoraFuzzer/libdft64
</note>

---

# Internals of libdft - Overview

- Shadow Memory
  - how to store taint info
- Virtual CPU
  - how to propagete taint info
- The libdft API and I/O interface
  - how to instrument

---

# Internals of libdft - Shadow Memory

### Bitmap (1 colour)

- Supports only 1 taint colour.
- Slightly faster and less memory than STAB.

---

# Internals of libdft - Shadow Memory

### STAB : Segment Translation Table (8 colour)

- Supports 8 taint colour.
- Contains one entry for every memory page.
- Each entry contains an _addend_ value,
  - which is a 32-bit offset from a virtual memory address.
- Obtained address ( VMA + offset ) contains the correcponding shadow byte.

---

# Internals of libdft - Virtual CPU

- _Virtual CPU_ keeps track of the taint status of CPU register.
  - This is stored in memory as a special data structure.
- Virtual CPU is a kind of shadow memory.
  - for each of 32-bit general-purpose CPU registers.

<note>
32-bit general purpose register of x86:
<code>edi</code>,
<code>esi</code>,
<code>ebp</code>,
<code>esp</code>,
<code>ebx</code>,
<code>edx</code>,
<code>ecx</code>,
and <code>eax</code>
</note>

---

# Internals of libdft - libdft API and I/O interface

- `libdft` provides a taint tracking API.
- Two import tools for building DTA tools is those that
  - manipulate tagmap (Tagmap API)
  - add callbacks and instrument code

---

# Internals of libdft - libdft API and I/O interface

- `libdft` provides a taint tracking API.
- Two import tools for building DTA tools is those that
  - <red>manipulate tagmap (Tagmap API)</red>
  - add callbacks and instrument code

### Tagmap API

- `tagmap_setb()` : sets status of tagmap
- `tagmap_getb()` : gets status of tagmap

---

# Internals of libdft - libdft API and I/O interface

- `libdft` provides a taint tracking API.
- Two import tools for building DTA tools is those that
  - manipulate tagmap (Tagmap API)
  - <red>add callbacks and instrument code</red>

### API for adding callbacks and instrumentation code

- `syscall_set_pre()` and `syscall_set_post()` : register callbacks for syscall events.
- `syscall_desc[]` : store syscall pre- and post-handlers.
  - use _syscall number_ to index this array.
- `ins_set_pre` and `ins_set_post` : register callbacks for instructions.

---

# Taint Policy

- `libdft` taint policy defines the following classes of instructions.
- Each of these classess define how to propagete and merge taint.

1. ALU : arithmetic and logic instruction such as `add`, `sbb`, `and`, `xor`, `div`, and `imul`
2. XFER : instructions that copy a value such as `mov`. Simply copies the taint info.
3. CLR : (=clear), instructions that reset taint info of output opperands
4. SPECIAL : instructions that require special rules
5. FPU, MMX, SSE : instructions `libdft` doesn't supports. Doesn't propagete taint info, causing undertainting.

---

# Overview

1. introducing `libdft`
2. <red>Using DTA to Detect Remote Control-Hijacking</red>
3. Circumeventing DTA with implicit Flows
4. A DTA-Based Data Exfiltration Detector

---

# Remote Control-Hijacking Attack

- Goal is to detect attacks where data received from network is used to control the argument of `execve` call (-> arbitary code execution).
- taint source : the network receive functions, `recv` and `recvfrom`.
- tiant sink : `execve`

---

# ref) `exec` family

<style type="text/css">
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-0pky"></th>
    <th class="tg-0pky">引数の渡し方(l or v)</th>
    <th class="tg-0pky">環境変数(e)</th>
    <th class="tg-0pky">パス検索(p)</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-0pky">exec<u>l</u></td>
    <td class="tg-0pky">list</td>
    <td class="tg-0pky">引き継ぐ</td>
    <td class="tg-0pky">しない</td>
  </tr>
  <tr>
    <td class="tg-0pky">exec<u>v</u></td>
    <td class="tg-0pky">vector</td>
    <td class="tg-0pky">引き継ぐ</td>
    <td class="tg-0pky">しない</td>
  </tr>
  <tr>
    <td class="tg-0lax">exec<u>le</u></td>
    <td class="tg-0lax">list</td>
    <td class="tg-0lax">渡す</td>
    <td class="tg-0lax">しない</td>
  </tr>
  <tr>
    <td class="tg-0lax">exec<u>ve</u></td>
    <td class="tg-0lax">vector</td>
    <td class="tg-0lax">渡す</td>
    <td class="tg-0lax">しない</td>
  </tr>
  <tr>
    <td class="tg-0lax">exec<u>lp</u></td>
    <td class="tg-0lax">list</td>
    <td class="tg-0lax">引き継ぐ</td>
    <td class="tg-0lax">する</td>
  </tr>
  <tr>
    <td class="tg-0lax">exec<u>vp</u></td>
    <td class="tg-0lax">vector</td>
    <td class="tg-0lax">引き継ぐ</td>
    <td class="tg-0lax">する</td>
  </tr>
</tbody>
</table>

---

# header files

```c
// dta-execve.cpp
#include "pin.H"           // libdft uses Pin

#include "branch_pred.h"   // hints for branch prediction
#include "libdft_api.h"    // lidft base API
#include "syscall_desc.h"
#include "tagmap.h"
...
```

- all `libdft` tools are just Pin tools ilnked with the `libdft` library.

---

# functions

```c
extern syscall_desc_t syscall_desc[SYSCALL_MAX];               // to store syscall hooks

void alert(uintptr_t addr, const char *source, uint8_t tag);   // print alert and exit
void check_string_taint(const char *str, const char *source);  // check whether tainted
static post_socketcall_hook(syscall_ctx_t *ctx);               // taint sources
static pre_execve_hook(syscall_ctx_t *ctx);                    // taint sinks
```

- Details of these functions will be explained later.

---

# main function

```c
int main(int argc, char **argv) {
  PIN_InitSymbols();

  if (unlikely(PIN_Init(argc, argv))) {
    return 1;
  }

  if (unlikely(libdft_init() != 0)) {
    libdft_die();
    return 1;
  }

  syscall_set_post(&syscall_desc[__NR_socketcall], post_socketcall_hook);
  syscall_set_pre(&syscall_desc[__NR_execve], pre_execve_hook);

  PIN_StartProgram();

  return 0;
}
```

---

# main function

```c
int main(int argc, char **argv) {
  PIN_InitSymbols();

  if (unlikely(PIN_Init(argc, argv))) {
    return 1;
  }

  if (unlikely(libdft_init() != 0)) {
    libdft_die();
    return 1;
  }
...
```

- `unlikely(hoge)` tells compiler that hoge is unlikely to be ture (= likely to be false).

---

# main function

```c
...
  syscall_set_post(&syscall_desc[__NR_socketcall], post_socketcall_hook);
  syscall_set_pre(&syscall_desc[__NR_execve], pre_execve_hook);

  PIN_StartProgram();

  return 0;
}  // end of main
```
