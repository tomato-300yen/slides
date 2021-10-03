# Meltdown

> Meltdown breaks all security guarantee provided by address space isolation as well as paravirtualized environments.

対策としてpage-tableを分離することが考えられる。
- KAISER
  - (Kernel Address Isolation to have Side-channel Effectively Removed)
- KPTI(こっちで呼ばれることのほうが多い)
  - (Kernel Page-Table Isolation)
- もともとはKASLRという、メモリランダマイズが突破されたための対策の実装が存在していた。


## A Toy Example

```
raise_exception();
// the line below is never reached
access(probe_array[data * 4096]);
```

- Due to OoO execution, CPU might have already executed the following instruction.
  - there is no data-dependency between these instruction
- This OoO execution have microarchitectural side effect.
- Flush+Reload という攻撃手法が使える。
