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
  - 投機的実行の過程で変更されたキャッシュの状態はパフォーマンスの観点からロールバックされない。(コレがまずい)

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

### 具体的なコードといくつかの仮定

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

- then節はdead codeだが、投機的実行によって実行され得る。(VB)
  - 実行されてキャッシュに読み出されたとしても、キャッシュのロールバックは行われない。

仮定
- 攻撃対象と攻撃者は同じマシン上にいることを仮定。(ref. [31])
- プロセッサーの分岐予測器は外部から(間違った側に)訓練(mis-train)できる。(ref. [9])
  - すべての分岐は潜在的にmis-trainedされる可能性がある。
  - つまり、すべての分岐をVBとして扱う。
- 攻撃者は access-based cache side-channel attack か trace-based cache side-channel attack を行うと仮定。(ref [38])
- 攻撃可能かどうかの判定方法
  - プログラムの実行の最後で攻撃者が potentially secret なデータを観測できる場合に攻撃可能と判定する。

## 問題設定

考察対象のコード片
```c
uint32_t SIZE = 16;
uint8_t array1[16];
array2[256*64], array3[16];

uint8_t foo(uint32_t x) {
  uint8_t temp = 0;
  if (x < SIZE) {          // b1
    temp = array1[x];       // A
    temp |= array2[temp];  // A
    if (x <= 8) {          // b2
      temp |= array2[8];   // B
    }
  }
  temp |= array3[8];       // C
  return temp;             // C
}
```

![symbolic_path](img/symbolic_path.png)

- 通常の実行を行った場合、out-of-bound access はおきない。
- (b)は従来の記号実行を行った時の execution tree
- (c)はこの論文でやりたいこと(以下で解説)

### 問題となる状況

- x が x >= SIZE
- b1 がmis-trained されている

以上のような場合、投機実行によって

- array1[x] が参照される。(out-of-bound reference)
- そのデータをもとにして、array2[temp]が参照される。
  - array1[x] が秘密データを参照していた場合、ここで得られる値も秘密データに依存していることになる。
  - ここで読んだ値がcacheに残っている場合、攻撃者は間接的にarray1[x]の値を入手してしまう。

従来の記号実行ではこのデータリークは検出できない。
これを検出するためには、

- 分岐予測による投機実行
- cache の挙動

を扱える必要がある。

## 提案手法の概要

- 投機的実行を考慮した記号実行
- キャッシュのモデル化

### 投機的実行を考慮した記号実行

1. p_T1 : x < SIZE, b1 is correctly predicted.
1. p_F1 : x >= SIZE, b1 is correctly predicted.
1. sp_T1 : x >= SIZE, b1 is mis-predicted. (秘密情報をリークする可能性があるのはこのパターン)
1. sp_F1 : x < SIZE, b1 is mis-predicted.

3つ目のパターンを検出したいというモチベーションがある。

が、愚直にやると検査対象の数が指数的に爆発するのでうまく工夫する必要がある。
KLEEspetreでは2つの工夫をおこなう。

1. Speculative Execution Window(SEW)によって、探索する命令数に上限を設ける。(この数に達したら当該分岐の探索は終了)
1. 秘密データを流出し得ない命令はメモリアクセスであっても無視する。
  - 何を秘密データとするか(詳しくは後述するが、大きく分けて2つの戦略がある)
    1. 範囲外参照先の値
    2. 外から明示的に秘密データを指定
  - 上の例で言えば、sp_T1の場合でコード片Aを実行している場合のみを考慮する。

### キャッシュのモデル化

やりたいこと：読み込んだ秘密情報がプログラム終了時点でキャッシュに残っているかを確かめたい。

KLEEspetre は cache side-channel attack の可能性のあるメモリーアクセスシークエンス(set of memory access sequences)のセットを計算する。

それぞれのシークエンスは以下のように構成されている。

- 少なくとも１つの、投機実行path上のメモリアクセス
  - secret data に関連するアクセスのみ(計算量を抑えるため)
  - 投機実行におけるデータリークにのみ興味があるため
- 複数の通常実行でのメモリアクセス

メモリアクセス単体は以下のように表せる。

- (A, x > SIZE, &array2[array1[x]])
  - &array2[array1[x]] を
  - x > SIZE という記号制約(symbolic constrain)で
  - Aのコード片から

シークエンスは以下のように表せる。

- ((A, x > SIZE, &array2[array1[x]]), (C, x > SIZE, &array3[8]))
  - この例はsp_T1, p_F1の順番で実行する例
  - sp_T1 はmis-predict

このようなシークエンスを記号実行を通して集める。

そして、以下の条件を考えることにより、プログラム実行終了時にキャッシュに秘密データが残っているかどうかを計算できる。(残っている場合にTrue)

- (x > SIZE) and (set(array2[arra1[x]]) != set(&array3[8])) or (tag(&array2[array1[x]]) = tag(&array3[8]))
  - orの前半：投機実行で秘密データをキャッシュに読み出して、かつ、かつ後続のメモリアクセスによって上書きされない。
  - orの後半：投機実行では読み出されなかったものの、&array3[8]の読み出しと一緒にキャッシュに読み出された場合。

## 提案手法の詳細(アルゴリズムなど)

記号実行のアルゴリズムは省略。キャッシュのモデル化のうちキーとなる考え方の部分を紹介する。

### cache conflictについて

そもそもcacheってなんだったっけ？

![cache_intro](img/cache_intro.svg)

![cache_definition](img/cache_definition.png)
![cache_conflict](img/cache_conflict.png)

## 実装
pass

## 実験

### cacheモデルの役割

False positive を減らす。

## 関連研究

### Spectre攻撃に対するハードウェアからのアプローチ

- cache ではなく Speculative Buffer(SB) を使ってデータをロードし、ロールバックする場合はSBをスカッシュする手法(InvisiSpec, [44])
- cache で"Undo"を行う手法(CleanupSpec, InvisiSpecの改良版, [39])
- cacheを撹乱してsensitibe dataのleakを防ぐ手法(CEASER [37], ScatterCache[42])

上記手法はハードウェアに大きく依存している。
KLEEspetreは、分岐予測をもとに投機的実行を行う任意のプロセッサの上で動く任意のアプリケーションを対象にできる。

### Spectre攻撃に対するソフトウェアからのアプローチ

- コンパイル時点でVBを発見し、データリークの可能性のあるpointerを無効化するコードを挿入する手法(RSを消す手法)(Speculative Load Hardening(SLH), [10])
  - 実行時オーバーヘッドが無視できない程度に増加する(36%程度)
- potentially vulnerable な命令の実行を遅らせることで、投機的実行を回避する手法([33])
  - 上記の手法と同様、実行時オーバーヘッドがひどい
    - "potentially" vulnerable な命令すべてに対して遅延を導入するため

KLEEspetreと併用することによって、実行時のオーバーヘッドを削減することができる。

- Microsoft Visual C/C++ コンパイラ[15]
  - オプションで"lfence"と呼ばれるコードを挿入することで防御をしているが、2/15しか防げていない(Spectre litmus test)。
- 静的解析によって投機的実行をモデル化する手法(草分け的論文?)。(007, TSE'20)
  - conservative static analysis 由来のfalse positiveが顕著
  - KLEEspetreのほうが優れている（らしい）

### Speculative Semantics and Formal Models

モデル検査

- Trace property-dependent observational determinism(TPOD) というセキュリティに関する性質を用いる手法([13])
  - この論文では実装はないらしい
  - arahori-sanリスト5
    - モデル検査による解析
- 投機的評価をモデル化するmicroarchitecturalなモデル([16])
- model for constant-time programming([11])
  - constant-time programming??
  - arahori-sanリスト2
- out-of-order実行をモデル化するためのインフラ提供(InSpectre, [6])
  - microarchitectural featuresのsemanticsを考察
- 投機的実行に対してfuzz testを実行することによりSpectre脆弱性を検出する(SpecFuzz, [34])
  - cacheのモデル化は行っていない。
- speculative noninterference?を用いた記号実行(SPECTECTOR, [23])

上記の手法は、speculative semantics のモデル化し、プログラム解析における脆弱性を定義しているものの、cache side-channel攻撃のモデル化とチェックは行っていない。
KLEEspetreはcacheのモデル化を行うことにより達成している。

### Side-Channel Analysis without Cache Modeling

- キャッシュの明示的なモデル化を行わないので、false positiveが多い。
- 投機的実行をモデル化しないので、Spectre攻撃を検出できない。

### Side-Channel Attack Identification via Cache Modeling

## 結論

- KLEEspetre という新しいテストツールの提案
  - 投機的実行やキャッシュ(microarchitectural features)を扱うことができる記号実行のフレームワーク(?)
  - hardware でプログラムを動かす際のmicroarchitectural features由来の脆弱性を検出できるようになる
  - 実際にBCB vulnerability を見つけるとこができる。
    - cacheモデルによって、false positive が少ない。
- microarchitetural features 由来の脆弱性に関するテストの範囲を広げた
