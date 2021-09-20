# kleespectre

## Attack Example


```c
if (x < array1_size) {  // VB
  y = array1[x];  // RS
  temp |= array2[y * 64];  // LS
}
```

Example of so called BCB (Bounds Check Bypass) Attack.

- VB : Vulnerable Branch
- RS : Read Secret
- LS : Leak Secret

先行研究:
- branch predictorは攻撃者から操作できる。→ mis-predictionをするように仕向けられる。
  - may be mis-trained by the victim process or outside the victim process

## 改善点
- 攻撃者は実行直後にメモリを観察できるという仮定をおいている。実行途中に関しては見ていない。

## overview

従来のsymbolic executionは branch mis-prediction 由来のリークを検出できない。

## 疑問

mis-predictionしたときに、現実のcpuではどうなっているのか。
もうこの問題解決されていたりして。

## 6.1
- Kocherと呼ばれるリトマステスト群でテストした。
  - real world で BCB vulnerability は見つかってないからKocherを使う。
  - Kocher は秘密データへのアクセスのあとにメモリーアクセスを行っていない。
    - この状況だとすべて検出できる。
  - ので、ひみつデータを呼んだ後にメモリーアクセスを行ってキャッシュをいじって、その後に秘密データが確認できるかどうかを確認する。
