# SpecTaint[NDSS'21]

# 用語

- SpecX : Speculative Execution(投機実行)
- Spectre : Spectre 脆弱性

# 概要

- 解きたい問題
  <!-- - Spectre を塞ぎたい -->
  - Spectre の検出(特に Spectre V1, V1.1)
    - 検出箇所にシリアライズ命令を置く
- 既存手法の問題点
  - SPECTECTOR : 精度は良いがスケールしない。
    - 記号実行ベース
  - SpecFuzz : スケールするが recall/precision ともにそこそこ
    - SpecX のロジックを静的に組み込み Fuzzing
  - oo7 : スケールするが、FP が多い
    - 静的テイント解析、SpecX を考慮しない
- 提案手法
  - (Fuzzing +) 動的テイント解析 + SpecX のシミュレート
  - 既存の動的テイント解析で SpecX のシミュレートできるようにした
- 結果
  - Scalability を悪化させることなく精度の向上
    - Max : 11M, 17h

# Spectre V1

```
i = input[0];         // <1>
if (i < size) {       // <2>
  secret = foo[i];    // <3>
  baz = bar[secret];  // <4>
}
```

# SpecX のシミュレート

概略:

1. 分岐命令に到達したらチェックポイントを取る
   1. CPU のレジスタ状態
   1. テイント解析の状態(shadow memory)
1. 本来の飛び先とは逆側を実行
1. メモリを変更する命令を計装
   - 書き込み先
   - 書き込み前の値
1. SpecX の終了条件が満たされたらロールバック
   1. チェックポイントを復元
   1. メモリの変更を戻す
1. 本来の飛び先を実行

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

- シリアライズ命令に遭遇したら
- 所定の命令数を実行したら
  - ReorderBuffer のサイズを真似て 250

## SpecX 中の例外

- SpecFuzz では例外が発生した段階で、SpecX を中止
- ほとんどの CPU のモデルでは(?)、例外が発生しても SpecX を中止しないとされている
- → 例外が発生しても SpecX を中止しない

# Spectre の検出

- G スライドを参照する
- Spectre のコード
- 検出方法

# 実験と評価

- accuracy (recall/precision)
- scalability

## データセット

- Kocher らによる 15 個の Spectre サンプル
- SpecFuzz の評価で用いられたデータセット
- 人工的に脆弱性を注入したデータセット

## 実験環境

## 実験

# 関連研究

- スプシを参照のこと

# 結論

- 動的テイント解析において SpecX をシミュレートできるようにした
