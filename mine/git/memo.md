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

などのことができる。

### git-hooks の設定
スクリプトのファイル名
   - hookの名前
   - 使用可能なhook : https://git-scm.com/docs/githooks#_hooks
      - pre-commit, post-commit, post-checkout, ...

どこにスクリプトをおくか？
1. hookの探索対象のディレクトリ
   - default : `$GIT_DIR/hooks/`
      - `git config core.hooksPath`によって変更可能。
      - 相対pathを指定した時は、$GIT_DIRからの相対pathとして扱われる。
   - git はこのディレクトリ内で見つかったスクリプトを実行する
1. `init.templatedir`におく。
   - `git init`した時に、hookスクリプトをレポジトリの`.git`以下にコピー
      - templatedirについて : https://git-scm.com/docs/git-init#template_directory_
   - git内のconfig変数(`init.templateDir`)で変更可能
      - `git config --global init.templatedir '~/.git_templates'`

おすすめは2つ目のtemplateを使う方法。
既存レポジトリでも`git init`を実行することで、新たに追加したtemplateをコピーしてこれる。

### どうやって共有するか

個人用で使う分にはtemplateを用意すれば十分。

共同開発では`pre-commit`というツールを使うのがおすすめ。
yamlファイルで、レポジトリごとにpre-commitを設定できる。

## binary

## repo size
