# kleespectre

## アプローチの分類

- HWから攻める
  - cacheに関する提案が多い
- SWから攻める
  - 投機実行を遅らせる系の手法はoverheadが大きい
  - lfenceを置く手法
- モデル検査
- Side-channel攻撃の解析(w/ cache modeling)
- Side-channel攻撃の検出(w/ cache modeling)
  - abstract interpretation の手法はここに分類される

## 比較の観点

- 記号実行で投機的実行を扱えるか
- Cache のモデル化
- Scalableかどうか
- spectre攻撃を検出できるか
- 対象

