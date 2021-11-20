# SpecTaint[NDSS'21]

# 用語

- SpecX : Speculative Execution(投機実行)
- Spectre : Spectre 脆弱性

# 概要

- 解きたい問題
  - Spectre を塞ぎたい
  - Spectre を検出(特に Spectre V1, V1.1)し、
    - 検出箇所に fence 命令を置く
- 既存手法の問題点
  - SPECTECTOR : 精度は良いがスケールしない。
    - 記号実行ベース
  - SpecFuzz : スケールするが recall/precision ともにそこそこ
    - SpecX のロジックを静的に組み込み Fuzzing
- 提案手法
  - 動的テイント解析 + SpecX のシミュレート + (Fuzzing)
  - 既存の動的テイント解析で SpecX のシミュレートできるようにした
- 結果
  - 精度を保ちつつ Scale する

# Spectre V1

# SpecX のシミュレート

サンプルコード:

```c
int main(void) {
  int a, b, c, i;
  a = 100;
  b = 200;
  c = 0;
  scanf("%d", &i);

  if (i > 0) {
      a = 200;
      b = 300;
  }
  c = a + b;
  return 0;
}
```

- `i`には-1 を与えるとする。

## 1. CPU のレジスタ状態とテイントの状態を保存

- 分岐命令に到達したら以下を保存する
  - CPU のレジスタ状態
  - テイントの状態(shadow memory)
- SpecX のシミュレートが終わったら復元する。

```c
int main(void) {
  int a, b, c, i;
  a = 100;
  b = 200;
  c = 0;
  scanf("%d", &i);  // i = -1

  if (i > 0) {  // <-
      a = 200;
      b = 300;
  }
  c = a + b;
  return 0;
}
```

- a: 100
- b: 200
- c: 0

## 2. 逆側の飛び先を実行

- 分岐条件が True だったら False 側
- 分岐条件が False だったら True 側

```c
int main(void) {
  int a, b, c, i;
  a = 100;
  b = 200;
  c = 0;
  scanf("%d", &i);  // i = -1

  if (i > 0) {
      a = 200;  // <-
      b = 300;
  }
  c = a + b;
  return 0;
}
```
- a: 100
- b: 200
- c: 0


## 3. メモリを変更する命令を計装

以下の値を保存しておく:

- 書き込み先のアドレス
- 書き込むサイズ
- 書き込み前の値

```c
int main(void) {
  int a, b, c, i;
  a = 100;
  b = 200;
  c = 0;
  scanf("%d", &i);  // i = -1

  if (i > 0) {
      a = 200;  // <-
      b = 300;  // <-
  }
  c = a + b;  // <-
  return 0;
}
```

- a: 200
- b: 300
- c: 500

## 4. SpecX のシミュレートが終わったらロールバック

1. CPU のレジスタ状態、テイントの状態を復元
1. メモリ操作のロールバック
   1. 保存しておいた古い値を当該のアドレスに逆順に書き込む

```c
int main(void) {
  int a, b, c, i;
  a = 100;
  b = 200;
  c = 0;
  scanf("%d", &i);  // i = -1

  if (i > 0) { // <-
      a = 200;
      b = 300;
  }
  c = a + b;
  return 0;
}
```

- a: 100
- b: 200
- c: 0

## 5. 本来の飛び先を実行

```c
int main(void) {
  int a, b, c, i;
  a = 100;
  b = 200;
  c = 0;
  scanf("%d", &i);  // i = -1

  if (i > 0) {
      a = 200;
      b = 300;
  }
  c = a + b;  // <-
  return 0;
}
```

- a: 100
- b: 200
- c: 300

## SpecX の終了条件

- `fence`命令に遭遇したら
- 所定の命令数を実行したら
  - ReorderBufferのサイズを真似て250

# Spectre の検出

# 実験と評価

# データセット

# 関連研究

# 結論

- 動的テイント解析において SpecX をシミュレートできるようにした
