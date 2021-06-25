---
marp: true
theme: custom
---

# Practical Binary Analysis #13.4~

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

# Overview

1. Increase Path Coverage
   - `main` function
   - `find_new_input` function
2. Automatically exploit vulnerability

---

# Source Code

- https://hackmd.io/@C5FCqN8cSSO75WvPfrj9aw/SJurj-Q2O

---

# Path Coverage - `main` (1/4)

```c
int main(int argc, char *argv[]) {
  ...
  // optional arguments to pass to parse_sym_config
  std::vector<triton::arch::registers_e> symregs;
  std::vector<uint64_t> symmem;
  ...
  if(argc < 5) {
    printf("Usage: %s <binary> <sym-config> <entry> <branch-addr>\n", argv[0]);
    return 1;
  }
  ...
}
```

- `binary` : path to binary to analyze
- `sym-config` : path to config file
- `entry` : address at which execution(emulation) starts
- `branch-addr` : target branch address

---

# Path Coverage - `main`(2/4)

```c
int main(int argc, char *argv[]) {
  ...
  if(parse_sym_config(argv[2], &regs, &mem, &symregs, &symmem) < 0) return 1;
  ...
}
```

- Difference between this and previous example
  - Create Symbolic Variable
    - Symbolize all memory locatinos and regisnters that contain user input.
    - `parse_sym_config` writes to `symregs`/`symmem` where/which to symbolize.

---

# Path Coverage - `main`(3/4)

```c
int main(int argc, char *argv[]) {
  ...
  for(auto regid: symregs) {  // Symbolize Registers
    triton::arch::Register r = api.getRegister(regid);
    api.convertRegisterToSymbolicVariable(r)->setComment(r.getName());
  }
  ...
  for(auto memaddr: symmem) {  // Symbolize Memory
    api.convertMemoryToSymbolicVariable(
      triton::arch::MemoryAccess(memaddr, 1)
    )->setComment(std::to_string(memaddr));
  }
  ...
}
```

- Symbolize memory locatinos and registers

---

# Path Coverage - `main`(4/4)

```c
int main(int argc, char *argv[]) {
  ...
    if(pc == branch_addr) {  // find new user input to reach not explorered path.
      find_new_input(api, sec, branch_addr);
      break;
    }
  ...
}
```

- Call `find_new_input`, which is explained later.
