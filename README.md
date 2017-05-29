
* アルゴリズム
QuはまとめてQにしている。Q単体は出てきているの見たこと無いため。
最初にmakedicで辞書のキーと値を赤黒木dicに入れている。
入力を受け取ったら辞書と同様にQuをQにして、makeset関数で「c3は入力された文字の中で最も点が低い文字の集合」、「c2は入力された文字の中で二番目に点が低い文字とc3との和集合」という風に集合を作っている。
このようにした理由としては、一番点が低い文字の集合からcombinationで何文字か選び、その文字を除いた入力のsubstringを作ることによって点数が高い方から効率的に見ていくことができるからである。
`search, search_set, for_each_combination`で入力から何文字除くかという整数iについてc3,c2,c1から除く文字のcombinationを作り、入力からその集合を引いたものが辞書にあれば返すということをしている。
