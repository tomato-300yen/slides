# git advanced

## hooks

参考文献
- official document
  - https://git-scm.com/docs/githooks
- 自分のhooksを管理する方法
  - https://qiita.com/quattro_4/items/59fdf8b9aa9ef48ecbdf

### git hooksとは

git操作のある時点で特定の処理を行う機能。
よく使うのはpre-commit。

例えば、pre-commitというファイルの中にhookスクリプトを空いたとすると、
- commmit実行直前に`pre-commit`が実行される。
- スクリプトの終了ステータスが
  - 0 : そのままcommitを実行
  - 0以外 : commitを中止

pre-commitを共有することで
- pep8違反コード片はそもそもcommitさせない
- commit前にautopep8を実行
- 巨大ファイルのcommitを防ぐ
- awsの認証情報のcommitを防ぐ

などのことができる。

### git-hooks の設定
スクリプトのファイル名
   - hookの名前
   - 使用可能なhook : https://git-scm.com/docs/githooks#_hooks
      - pre-commit, post-commit, post-checkout, ...

どこにスクリプトをおくか？
1. hookの探索対象のディレクトリにおく
   - default : `$GIT_DIR/hooks/` (ex. repo-name/.git/hooks/)
      - `git config core.hooksPath`によって変更可能。
      - 相対pathを指定した時は、$GIT_DIRからの相対pathとして扱われる。
      - 絶対pathを指定した時は、全てのrepoにおいて共通のhookを使うことになる。
   - git はこのディレクトリ内で見つかったスクリプトを実行する
1. `init.templatedir`におく。
   - `git init`した時に、hookスクリプトをレポジトリの`.git`以下にコピー
      - templatedirについて : https://git-scm.com/docs/git-init#template_directory_
   - git内のconfig変数(`init.templateDir`)で変更可能
      - `git config --global init.templatedir '~/.git_templates'`

おすすめは2つ目のtemplateを使う方法。
既存レポジトリでも`git init`を実行することで、新たに追加したtemplateをコピーできる。

### どうやって共有するか

個人用で使う分にはtemplateを用意すれば十分。

共同開発では`pre-commit`というツールを使うのがおすすめ。
yamlファイルで、レポジトリごとにpre-commitを設定できる。

github actionsなどが使えない環境(ex. backlogのgit)では重宝する。

## binary
あまり面白くないので後回し。

- diff生成の設定: https://git-scm.com/docs/gitattributes#_generating_diff_text

## diff

- 参考文献
  - 公式doc
    - https://git-scm.com/docs/gitattributes#_generating_diff_text
  - qiita
    - https://qiita.com/yuya_presto/items/ef199e08021dea777715

### 前提知識

gitの(globalな)設定は`~/.gitconfig`で行うことができる。
最低限の場合こんな感じ

```
[user]
  name = Yusuke Uchida
  email = hogehoge@gmail.com
```

直接`~/.gitconfig`を編集するか、以下のコマンドを実行することで設定可能。

```
git config --global user.name Yusuke Uchida
git config --global user.email hogehoge@gmail.com
```

gitのファイルごとの設定は`~/.gitattributes`で行うことができる。
例えば

```
*.py diff=python
```

この意味は後述。

### 色をつける

```
[color]
  ui = auto
```
### word単位でdiffを表示する

デフォルトでは行単位でdiffが表示される。場合によっては単語単位で表示したほうが見やすい。

```
git diff --color-words
```

### 各種言語の文法を考慮し見やすくする

`~/.gitattributes`でdiffの生成ルールを指定できる。
diff生成ルールは1から指定可能だが、いくつかのファイル用にデフォルトでルールが用意されている。

例えば`~/.gitattributes`に以下を追加することで

```
*.py diff=python
```

pythonファイルのdiffがかなり見やすくなる。

**ここに例を追加**

### そもそもdiff属性とは

gitconfigで定義し、
```
[diff "tex"]
  wordRegex = "\\\\[a-zA-Z]+|[{}]|\\\\.|[^\\{}[:space:]]+"
```

gitattributesで指定できる。
```
*.tex diff=tex
```

pathに対して、diff(に限らずその他)属性は以下の状態を取ることができる。

- Set
  - textとして扱う。
- Unset
  - binaryとして扱う。
    - つまり`Binary files differ`をdiffの生成ルールとして使う。
- Unspecified(default)
  - textっぽいファイルはtextとして扱う。それ以外はbinaryとして扱う。
- String
  - "diff driver"(後述)を指定する。"foo"が指定された場合、configの"diff.foo"で定義されたルールを使う。

#### diff driver

diff driverの定義方法は省略。知りたい方は[ドキュメント](https://git-scm.com/docs/gitattributes#_defining_an_external_diff_driver)を参照。

git本体によっていくつかのdiff driverが用意されている。(以下に列挙)

- ada
- bash
- bibtex
- cpp
- csharp
- css
- dts
- elixir
- fortran
- fountain
- golang
- html
- java
- markdown
- matlab
- objc
- pascal
- perl
- php
- python
- ruby
- rust
- scheme
- tex

ここに列挙されているファイルタイプに関してはデフォルトのdiff driverを使えば良い。

## repo size

参考文献
- bitbucket
  - https://support.atlassian.com/bitbucket-cloud/docs/maintain-a-git-repository/
- git-objects
  - https://git-scm.com/book/en/v2/Git-Internals-Git-Objects
- git-gc
  - https://git-scm.com/docs/git-gc
- git-filter-branch
  - https://git-scm.com/docs/git-filter-branch

### Caution

歴史改変コマンドを使用するため以下の注意が必要。
- 事前にbackupを取っておく。
  - `--mirror`オプション付きでclone
    - `git clone --mirror https://github.com/foo/bar.git`
- それぞれの操作には細心の注意を払うこと。
- remoteへのpushは慎重に。
  - `repo-dir/.git/config`でremoteのurlを消しておくのも1つの手

### 実験として使うレポジトリ

- SSD_OCR
  - (重みをgitで管理していた過去があるっぽかったので)
  - [参照](https://ai-orw4874.slack.com/archives/C01U15XN0EM/p1621785455057500)

### git objects

詳細はドキュメントに譲る([git-objects](https://git-scm.com/book/en/v2/Git-Internals-Git-Objects))。

git-objectsには**変更履歴が格納**されており、ここを参照することで**任意のタイミング**でのソースコードの状態を復元できる。

= どんなファイルでも一度コミットされたものは**すべてが復元可能な形で保持**されている。

= (巨大な/大量の)ファイルをcommitした場合
- git-objectsのサイズが膨らむ
  - **ファイルを削除するcommitだけではgit-objectsのサイズは減らない**

git-objectsが巨大であることの弊害
- cloneが遅くなる
  - `SSD_OCR`: 3m22s


### 状況の確認と前準備

サイズの確認
```
~/Code/nagase/SSD_OCR master
❯ du -sh .
630M    .

~/Code/nagase/SSD_OCR master
❯ du -sh .git/objects
531M    .git/objects
```
