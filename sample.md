---
marp: true
theme: custom
# header: "**Qiita** __Marp samples__"
# footer: "by **＠tomato-300yen**"
---
# Title of your presentation
## subtitle of your presentation
<!--
class: title
-->
---
<!--
class: slides
_footer: ''
paginate: true
-->
# Tools for graph
1. [Kroki.io](https://kroki.io/)
   - PlantUML, Graphviz, Mermaidなどお馴染みの記法から、少しニッチなものまで、20以上の記法に対応する、オープンソースのレンダリングエンジン、Web APIサービス。
2. [Draw.io](https://marketplace.visualstudio.com/items?itemName=hediet.vscode-drawio)
   - お馴染みのSVGエディタ。VSCodeのプラグインで、オフラインエディタをVSCode画面内で扱える。またそこで保存したSVGを、そのままMarpスライド内で活用できる。
3. [Vega](https://marketplace.visualstudio.com/items?itemName=RandomFractalsInc.vscode-vega-viewer)
   - データセットと、その描画方法をJSONで指定すると、綺麗なグラフを返してくれるVega。Draw.io同様、VSCodeプラグインがあり、生成したSVGを、Marpスライド内で活用できる。


---
# Slide1
## Lead Sentence


> cite

```python
import pandas as np
```

`inlinecode` is useful.

math $y = a x$

<red>
RED STRING
</red>

<note>
Note
<cite>Cite in note</cite>
<p></p>
</note>

---
# Slide2

* What's the answer? Ans-><hide>This is the ANSWER</hide>

1. test
2. test
* test1
* test2

<table>
    <tr>
        <th></th>
        <th>Practical Binary Analysis</th>
        <th>Introduction to Static Analysis</th>
    </tr>
    <tr>
        <th>面白さ</th>
        <td>最高におもしろい</td>
        <td>ごく普通</td>
    </tr>
    <tr>
        <th>内容</th>
        <td>Binary解析の基礎</td>
        <td>静的解析の基礎</td>
    </tr>
</table>

---
# Slide3
<div class=twocols>
    <p>
    Left side
    </p>
    <p class=break>
    Right side
    <ol>
    <li>a</li>
    <li>really?</li>
    <li>a</li>
    </ol>
    </p>
</div>
