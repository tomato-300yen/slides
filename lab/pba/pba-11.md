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

# Examples of `exec` family

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

```c
#include <unistd.h>
int main(void){
  char *argv[] = {"ls", "-l", NULL};
  char *envp[] = {NULL};
  execve("/bin/ls", argv, envp);
}
```

---

# header files

```c
// dta-execve.cpp
...
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
static post_socketcall_hook(syscall_ctx_t *ctx);               // taint source
static pre_execve_hook(syscall_ctx_t *ctx);                    // taint sink
```

- `syscall_desc` : Index this array with `syscall` number of `syscall` you're installing
  - such as `__NR_socketcall` or `__NR_execve`.
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
  PIN_InitSymbols();  // in case symbols are available

  if (unlikely(PIN_Init(argc, argv))) {
    return 1;
  }

  if (unlikely(libdft_init() != 0)) {  // init data structures such as tagmap
    libdft_die();  // deallocat any resources libdft may have allocated
    return 1;
  }
...
```

- `unlikely(hoge)` tells compiler that hoge is unlikely to be ture (= likely to be false).
  - Better branch prediction and less cycles.

---

# main function

```c
...
  // socketcall events as the taint sources
  syscall_set_post(&syscall_desc[__NR_socketcall], post_socketcall_hook);
  // taint sink
  syscall_set_pre(&syscall_desc[__NR_execve], pre_execve_hook);

  PIN_StartProgram();  // never returns

  return 0;            // never reached
}  // end of main
```

- `socketcall` events include `recv` and `recvfrom` events

<note>
On some architectures—for example, x86-64 and ARM—there is no socketcall() system call; instead socket(2), accept(2), bind(2), and so on really are implemented as separate system calls.
(from <code>man socketcall</code>)
</note>

---

# Details of func - `alert`

```c
void alert(uintptr_t addr, const char *source, uint8_t tag) {
  fprintf(stderr,
          "\n(dta-execve) !!!!!!! ADDRESS 0x%x IS TAINTED (%s, tag=0x%02x), "
          "ABORTING !!!!!!!\n",
          addr, source, tag);
  exit(1);
}
```

- This `alert` function
  1. prints an alert message with the details about the tainted address.
  2. `exit` from the application

---

# Details of func - `check_string_taint`

```c
void check_string_taint(const char *str, const char *source) {
  uint8_t tag;                                   // to store "colour"
  uintptr_t start = (uintptr_t)str;              // start of string
  uintptr_t end = (uintptr_t)str + strlen(str);  // end of string

  fprintf(stderr, "(dta-execve) checking taint on bytes 0x%x -- 0x%x (%s)... ",
          start, end, source);

  for (uintptr_t addr = start; addr <= end; addr++) {
    tag = tagmap_getb(addr);                     // get the "colour" of addr
    if (tag != 0)
      alert(addr, source, tag);                  // alert if tag is tainted
  }

  fprintf(stderr, "OK\n");
}  // end of check_string_taint
```

---

# Details of func - `post_socketcall_hook`

```c
static void post_socketcall_hook(syscall_ctx_t *ctx) {
  int fd;
  void *buf;
  size_t len;

  int call = (int)ctx->arg[SYSCALL_ARG0];
  unsigned long *args = (unsigned long *)ctx->arg[SYSCALL_ARG1];

  switch (call) {
  case SYS_RECV:
  case SYS_RECVFROM:
    // ...
    // omitted
    // ...
    break;

  default:
    break;
  }
}
```

---

# Details of func - `post_socketcall_hook`

```c
static void post_socketcall_hook(syscall_ctx_t *ctx) {
  int fd;      // these are used in the omitted part
  void *buf;
  size_t len;

  int call = (int)ctx->arg[SYSCALL_ARG0];  // syscall number
  unsigned long *args = (unsigned long *)ctx->arg[SYSCALL_ARG1];
...
```

- `ctx` : contains
  - the arguments that were passed to the syscall
  - the return value of syscall.

---

# Details of func - `post_socketcall_hook`

```c
...
  switch (call) {
  case SYS_RECV:
  case SYS_RECVFROM:
    // ...
    // omitted
    // ...
    break;

  default:
    break;
  }
}  // end of post_socketcall_hook
```

- Ignores any cases other than `SYS_RECV` or `SYS_RECVFROM`.
  - Thus, catching all the `socketcall` does work.

---

# Details of func - `post_socketcall_hook`( omitted part)

```c
... // start of omitted part
    if (unlikely(ctx->ret <= 0)) {
      return;
    }

    fd = (int)args[0];
    buf = (void *)args[1];
    len = (size_t)ctx->ret;

    fprintf(stderr, "(dta-execve) recv: %zu bytes from fd %u\n", len, fd);

    for (size_t i = 0; i < len; i++) {
      if (isprint(((char *)buf)[i]))
        fprintf(stderr, "%c", ((char *)buf)[i]);
      else
        fprintf(stderr, "\\x%02x", ((char *)buf)[i]);
    }
    fprintf(stderr, "\n");

    fprintf(stderr, "(dta-execve) tainting bytes %p -- 0x%x with tag 0x%x\n",
            buf, (uintptr_t)buf + len, 0x01);

    tagmap_setn((uintptr_t)buf, len, 0x01);
... // end of omitted part
```

---

# Details of func - `post_socketcall_hook`( omitted part)

```c
... // start of omitted part
    if (unlikely(ctx->ret <= 0)) {  // if syscall didn't receive any bytes
      return;
    }

    fd = (int)args[0];              // socket fd
    buf = (void *)args[1];          // buffer address
    len = (size_t)ctx->ret;         // # of received bytes

    fprintf(stderr, "(dta-execve) recv: %zu bytes from fd %u\n", len, fd);
...
```

---

# Details of func - `post_socketcall_hook`( omitted part)

```c
...
    for (size_t i = 0; i < len; i++) {  // print each char
      if (isprint(((char *)buf)[i]))
        fprintf(stderr, "%c", ((char *)buf)[i]);
      else
        fprintf(stderr, "\\x%02x", ((char *)buf)[i]);
    }
    fprintf(stderr, "\n");

    fprintf(stderr, "(dta-execve) tainting bytes %p -- 0x%x with tag 0x%x\n",
            buf, (uintptr_t)buf + len, 0x01);

    tagmap_setn((uintptr_t)buf, len, 0x01);
... // end of omited part
```

<div class=twocols>
<p>
  <ul>
    <li>
    <code>isprint(hoge)</code> returns
    </li>
    <ul>
      <li>
      none-0 if hoge can be printed
      </li>
      <li>
      0 if hoge can't be printed
      </li>
    </ul>
  </ul>
  </p>
  <p class=break>
    <ul>
      <li>
        <code>buf</code> : first address that will be tainted
      </li>
      <li>
        <code>len</code> : # of bytes to taint
      </li>
      <li>
        <code>0x01</code> : taint colour 
      </li>
    </ul>
  </p.break>
</div.twocols>

---

# Details of func - `post_socketcall_hook` (repeated)

```c
static void post_socketcall_hook(syscall_ctx_t *ctx) {
  int fd;
  void *buf;
  size_t len;

  int call = (int)ctx->arg[SYSCALL_ARG0];
  unsigned long *args = (unsigned long *)ctx->arg[SYSCALL_ARG1];

  switch (call) {
  case SYS_RECV:
  case SYS_RECVFROM:
    // ...
    // omitted, just explained
    // ...
    break;

  default:
    break;
  }
}
```

---

# Details of func - `pre_execve_hook`

```c
static void pre_execve_hook(syscall_ctx_t *ctx) {
  const char *filename = (const char *)ctx->arg[SYSCALL_ARG0];
  char *const *args = (char *const *)ctx->arg[SYSCALL_ARG1];
  char *const *envp = (char *const *)ctx->arg[SYSCALL_ARG2];

  fprintf(stderr, "(dta-execve) execve: %s (@%p)\n", filename, filename);

  check_string_taint(filename, "execve command");
  while (args && *args) {
    fprintf(stderr, "(dta-execve) arg: %s (@%p)\n", *args, *args);
    check_string_taint(*args, "execve argument");
    args++;
  }
  while (envp && *envp) {
    fprintf(stderr, "(dta-execve) env: %s (@%p)\n", *envp, *envp);
    check_string_taint(*envp, "execve environment parameter");
    envp++;
  }
}
```

---

# Details of func - `pre_execve_hook`

```c
static void pre_execve_hook(syscall_ctx_t *ctx) {
  const char *filename = (const char *)ctx->arg[SYSCALL_ARG0];
  char *const *args = (char *const *)ctx->arg[SYSCALL_ARG1];
  char *const *envp = (char *const *)ctx->arg[SYSCALL_ARG2];

  fprintf(stderr, "(dta-execve) execve: %s (@%p)\n", filename, filename);
  check_string_taint(filename, "execve command");
...
```

- taint sink

- `ctx` : contains

  - the arguments that were passed to the syscall
  - the return value of syscall.

- Check whether `filename` is tainted or not.

---

# Details of func - `pre_execve_hook`

```c
...
  while (args && *args) {
    fprintf(stderr, "(dta-execve) arg: %s (@%p)\n", *args, *args);
    check_string_taint(*args, "execve argument");
    args++;
  }
  while (envp && *envp) {
    fprintf(stderr, "(dta-execve) env: %s (@%p)\n", *envp, *envp);
    check_string_taint(*envp, "execve environment parameter");
    envp++;
  }
}
```

- Check whether `args` and `envp` are taited or not.

---

# Test of Control-Flow Hijacking - `main`

```c
int main(int argc, char *argv[]) {
  char buf[4096];
  struct sockaddr_storage addr;

  int sockfd = open_socket("localhost", "9999");

  socklen_t addrlen = sizeof(addr);
  recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addrlen);

  int child_fd = exec_cmd(buf);
  FILE *fp = fdopen(child_fd, "r");

  while (fgets(buf, sizeof(buf), fp)) {
    sendto(sockfd, buf, strlen(buf) + 1, 0, (struct sockaddr *)&addr, addrlen);
  }

  return 0;
}
```

- Some error-handling codes are omitted.

---

# Test of Control-Flow Hijacking - `main`

```c
int main(int argc, char *argv[]) {
  char buf[4096];
  struct sockaddr_storage addr;

  int sockfd = open_socket("localhost", "9999");

  socklen_t addrlen = sizeof(addr);
  recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addrlen);
...
```

1. Opens a socket.
2. Receives a meassage from the socket.

---

# Test of Control-Flow Hijacking - `main`

```c
...
  int child_fd = exec_cmd(buf);
  FILE *fp = fdopen(child_fd, "r");

  while (fgets(buf, sizeof(buf), fp)) {
    sendto(sockfd, buf, strlen(buf) + 1, 0, (struct sockaddr *)&addr, addrlen);
  }

  return 0;
}
```

3. Executes a command.
4. Writes the output of the command output to network socket.

- `exec_cmd` is vulnerable function.
  - The args of `execve` can be influenced by an attacker.

---

# Test of Control-Flow Hijacking - `cmd`

```c
static struct __attribute__((packed)) {
  char prefix[32];
  char datefmt[32];
  char cmd[64];
} cmd = {"date: ", "\%Y-\%m-\%d \%H:\%M:\%S",
         "/home/binary/code/chapter11/date"};
```

- `cmd` contains a `prefix` for the command output from the meassage.

---

# Test of Control-Flow Hijacking - `exec_cmd`

```c
int exec_cmd(char *buf) {
  int pid; int p[2]; char *argv[3];

  for (i = 0; i < strlen(buf); i++) {  // [1]
    if (buf[i] == '\n') {
      cmd.prefix[i] = '\0';
      break;
    }
    cmd.prefix[i] = buf[i];  // ** Buffer overflow **
  }

  argv[0] = cmd.cmd;
  argv[1] = cmd.datefmt;
  argv[2] = NULL;

  pipe(p)  // omitted error-handling
...
```

- [1] lacks propper bound check, allowing attackers to overwrite the `cmd` field.

---

# Test of Control-Flow Hijacking - `exec_cmd`

```c
...
  switch (pid = fork()) {
  ...  // case -1:
  case 0: /* Child */
    printf("(execve-test/child) execv: %s %s\n", argv[0], argv[1]);
    fflush(stdout);

    close(1);
    dup(p[1]);
    close(p[0]);

    printf("%s", cmd.prefix);
    fflush(stdout);
    execv(argv[0], argv);
    ...  // error-handling
  default: /* Parent */
    close(p[1]);
    return p[0];
  }
  return -1;
}
```

---

# Test of Control-Flow Hijacking - `main` (repeated)

```c
int main(int argc, char *argv[]) {
  char buf[4096];
  struct sockaddr_storage addr;

  int sockfd = open_socket("localhost", "9999");

  socklen_t addrlen = sizeof(addr);
  recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addrlen);

  int child_fd = exec_cmd(buf);
  FILE *fp = fdopen(child_fd, "r");

  while (fgets(buf, sizeof(buf), fp)) {
    sendto(sockfd, buf, strlen(buf) + 1, 0, (struct sockaddr *)&addr, addrlen);
  }

  return 0;
}
```

---

# Test of Control-Flow Hijacking - test of overflow

#### No Buffer-Overflow

```sh
$ ./execve-test-overflow &
[1] 1913
$ uc -u 127.0.0.1 9999
prefix!!!!:
(execve-test/child) execv: /home/binary/code/chapter11/date %Y-%m-%d %H:%M:%S
prefix!!!!: 2021-05-19 06:48:45
^C[1]+ Done                      ./execve-test-overflow
```

1. Start server. (localhost(=127.0.0.1), port 9999)
2. Send prefix to localhost:9999 using `nc`.

<note>
<code>-u</code> option : Use UDP instead of TCP.
</note>

---

# Test of Control-Flow Hijacking - test of overflow

#### Buffer-Overflow

```sh
$ ./execve-test-overflow &
[1] 2061
$ uc -u 127.0.0.1 9999
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb/home/binary/code/chapter11/echo
(execve-test/child) execv: /home/binary/code/chapter11/echo bb...bb/home/binary/.../echo
aa...aabb...bb/home/binary/code/chapter11/echo bb...bb/home/binary/code/chapter11/echo
^C[1]+ Done                      ./execve-test-overflow
```

<note>
<code>-u</code> option : Use UDP instead of TCP.
</note>

```c
static struct __attribute__((packed)) {
  char prefix[32];   // aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
  char datefmt[32];  // bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
  char cmd[64];      // /home/binary/code/chapter11/echo <- This command is executed.
} cmd;
```

---

# Test of Control-Flow Hijacking - Detect Hijacking

#### Using DTA to Detect Hijacking Attempt

```sh
$ cd /home/binary/lidft/pin-2.13-61206-gcc.4.4.7-linux/
$ ./pin.sh -follow_execv -t /home/binary/code/chapter11/dta-execve.so \
        -- /home/binary/code/chapter11/execve-test-overflow &
[1] 2994
$ nc -u 127.0.0.1 9999
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb/home/binary/code/chapter11/echo
(dta-execve) recv: 97 bytes from fd 4
aa...aabb...bb/home/binary/code/chapter11/echo\x0a
(dta-execve) taintin bytes 0xfff9499c -- 0xfff949fd with tag 0x1  # taints all the bytes received
(execve-test/child) execv: /home/binary/code/chapter11/echo bb...bb/home/binary/.../echo
(dta-execve) execve: /home/binary/code/chapter11/echo (@0x804b100)
(dta-execve) checking taint on bytes 0x804b100 -- 0x804b120 (execve command)...
(dta-execve) !!!!!!! ADDRESS 0x804b100 IS TAINTED (execve command, tag=0x01), ABORTING !!!!!!!
```

1. Check whether argments of `execve` are taitned.
2. `dta-execve` notices that the command is tainted with taint colour `0x01`.
3. Raises an alert and then stops the child process.
