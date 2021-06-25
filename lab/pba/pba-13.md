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

---

# Path Coverage - `find_new_input`(1/3)

- Branch Constraints
  - (flag, src, dst, AST)
    - flag : whether this constraint was used (!= taken)
    - src/dst : source/destination address
    - AST : AST encoding branch constraint

![bg right:29% contain](./fig/path-constraints.svg)

---

# Path Coverage - `find_new_input`(2/3)

```cpp
static void
find_new_input(triton::API &api, Section *sec, uint64_t branch_addr) {
  ...
  for(auto &pc: path_constraints) {  // for all path constraints
    if(!pc.isMultipleBranches()) continue;  // ex. call
    for(auto &branch_constraint: pc.getBranchConstraints()) {
      ...
      if(src_addr != branch_addr) {  // if src is not the one we wanna flip
        if(flag) {  // `flag` is True if `constraint` was used.
          constraint_list = ast.land(constraint_list, constraint);
        }
      } else {
        ...
}}}}
```

- Add constraint which is used,
  - if the branch we're looking is not the target branch.

---

# Path Coverage - `find_new_input`(3/3)

```cpp
static void
find_new_input(triton::API &api, Section *sec, uint64_t branch_addr) {
  ...
  for(auto &pc: path_constraints) {  // for all path constraints
    for(auto &branch_constraint: pc.getBranchConstraints()) {
      ...
      else {
        if(!flag) {  // `flag` is False if `constraint` was not used.
          constraint_list = ast.land(constraint_list, constraint);
          for(auto &kv: api.getModel(constraint_list)) {  // invoke Z3
            printf("SymVar %u (%s) = 0x%jx\n",
              kv.first,
              api.getSymbolicVariableFromId(kv.first)->getComment().c_str(),
              (uint64_t)kv.second.getValue());
}}}}}}
```

- Add constraint which is not used,
  - if the branch we're looking is the target branch.

---

# Path Coverage - test program

- See https://hackmd.io/@C5FCqN8cSSO75WvPfrj9aw/SJurj-Q2O#branchc
- `x` and `y` are the user inputs.
  - We shall symbolize these variables to get concrete values from `Z3`.
- We want to analyze `if (y == 10)...` branch.

### Info from disassembly(, which we won't see here)

- `x` and `y` is contained in `rdi` and `rsi`, respectively.
- Start of `branch` function is at `0x4005b6`
- Target branch is at `0x4005ce`

---

# Path Coverage - config file

We can symbolize registers as below.

```
%rdi=$
%rdi=0
%rsi=$
%rsi=0
```

- We also provide concrete value(`x`=0, `y`=0)
  - from which `code_coverage.cc` generate another concrete value to cover another path.

---

# Path Coverage - Generating New Input(1/2)

As we can see the code below, arguments to pass to `code_coverage` is...

1. `binary` : path to binary file, in this case, `branch`
2. `sym-config` : path to config file, in this case, `branch.map`
3. `entry` : entry point, in this case, `0x4005b6`
4. `branch-addr` : target branch address, in this case, `0x4005ce`

```cpp
  if(argc < 5) {
    printf("Usage: %s <binary> <sym-config> <entry> <branch-addr>\n", argv[0]);
    return 1;
  }
```

---

# Path Coverage - Generating New Input(2/2)

Results

```
$ ./code_coverage branch branch.map 0x4005b6 0x4005ce
evaluationg branch 0x4005ce:
    0x4005ce -> 0x4005dc (taken)
    0x4005ce -> 0x4005d0 (not taken)
    computing new input for 0x4005ce -> 0x4005d0
      SymVar 0 (rdi) = 0x0
      SymVar 1 (rsi) = 0xa
```

Initial inputs

```
$ ./branch 0 0
x < 5 && y != 10
```

New inputs

```
$ ./branch 0 0xa
x < 5 && y == 10  // new path!!
```
