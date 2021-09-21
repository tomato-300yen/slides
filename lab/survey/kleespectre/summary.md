# kleespectre

## 背景
- キャッシュからのデータ流出の可能性
  - 投機的実行に伴う分岐予測が外れた時、実行自体はスカッシュされるが、キャッシュの中身はパフォーマンスの観点からスカッシュしない。
- この脆弱性は現代のすべてのプロセッサーに現れる。 [SP'19]

### 投機実行

- 分岐命令の結果を計算している最中に分岐先の命令を先に実行することで、プロセッサーの性能を向上させる手法。
- 投機的に実行した命令はROB(Reorder Buffer)に格納され、予測が正しかった場合commit、外れていた場合はsquashする。
- ただし、性能の観点から、cacheの状態についてはsquashされない。
  - ここから機密情報が流出する恐れがある。

### 攻撃モデル (BCB)

```c
if (x < array1_size) {  // VB
  y = array1[x];  // RS
  temp |= array2[y * 64];  // LS
}
```

- VB : Vulnerable Branch
- RS : Read Secret
- LS : Leak Secret

## 従来手法
- 記号実行
  - テストカバレッジを向上させる手法


## 従来手法の問題点
- 従来の記号実行
  - 投機実行を考慮した解析ができない (完璧ではない)

## 提案手法
- モデルの作成
  - 投機実行を記号実行で
  - キャッシュへのアクセス
- キャッシュ経由のデータ流出(の可能性)を検出

## 実験結果

## これから

# 発表

## 概要

Spectre攻撃(cache経由)によるデータ流出を記号実行を用いて検出する手法の提案。

投機的実行を記号実行で扱えるようにし、さらにキャッシュの挙動もモデル化する。

## 背景知識

### 投機的実行 (Speculative execution)

- プロセッサのパフォーマンス向上の観点から、分岐命令の計算結果を計算している最中に分岐先を予測して命令を実行する。(投機的実行)
- 分岐予測が間違っていた場合、実行した命令はロールバックされる
  - 投機的実行の過程で変更されたキャッシュの状態はパフォーマンスの観点からロールバックされない。(実はコレがまずい)

### 記号実行

すでに輪講で扱っているので軽く。

プログラムの入力を記号として扱い、pathを探索していく過程で制約を集め、その制約を解くことによりそのpathを通るような入力を生成する手法。

### Bounds Check Bypass (BCB) Attack

Spectre攻撃の一種。

```c
if (x < array1_size) {  // VB
  y = array1[x];  // RS
  temp |= array2[y * 64];  // LS
}
```

- VB : Vulnerable Branch
- RS : Read Secret
- LS : Leak Secret

```c
int a=100, size=16;
char array1[16];
char array2[256*64];
int victim() {
  int y=0, tmep=0;
  if (a < size) {  // VB
    y = array1[a];
    temp |= array2[y];
  }
  return temp;
}
```

- then節はdead codeだが、投機的実行によって実行され得る。
- 実行されてキャッシュに読み出されたとしても、キャッシュのロールバックは行われない。

## 問題設定

```c
uint32_t SIZE = 16;
uint8_t array1[16];
array2[256*64], array3[16];

uint8_t foo(uint32_t x) {
  uint8_t temp = 0;
  if (x < SIZE) {
    temp = array[x];
    temp |= array2[temp];
    if (x <= 8) {
      temp |= array2[8];
    }
  }
  temp |= array3[8];
  return temp;
}
```

- Normal Execution Pathsの画像
- Execution Paths with speculationの画像

## 提案手法

- 投機的実行を考慮した記号実行
- キャッシュのモデル化

### 投機的実行を考慮した記号実行

### キャッシュのモデル化

## 実装

## 実験

## 関連研究

## 結論
