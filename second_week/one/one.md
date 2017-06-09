
## 考察
ocamlで行列積を計算しました。題意(L1,L2,L3の違いを表にして欲しかった？)とは違う気がしますが、ocamlのconsとappendの差がすごい！と感動する課題になりました。
実行方法は
```
$ ocaml
# #use "one_module.ml";;
```
という感じです。

チャートです。
https://live.amcharts.com/2EzYz/

one_append.mlというファイルはappendを使っていて、最初これでやったのですが見れば分かるように死ぬほど時間がかかり、ocamlの標準モジュールのArrayを使ったのがone_module.mlです。
何がこんなに違うのかというと、foo@barでappendはfooのcarを一つ一つbarにconsするみたいなことをしているのでfooのリネアだけ時間がかかります。
これを多用していたから時間がかかったのだと思い、consを使って書き直したのがone_cons.mlです。
moduleとは倍くらい違いますがappendに比べると圧倒的速さだということがわかりました。

## 実装が正しいことのverification
* one_append.ml
```
# MatrixInt.mul [[1;2;3];[4;5;6];[7;8;9]] [[1;2;1];[2;1;2];[3;2;3]];;
0.000025 sec
- : IntSemiring.t list list = [[14; 10; 14]; [32; 25; 32]; [50; 40; 50]]
```

* one_cons.ml
```
# MatrixInt.mul [[1;2;3];[4;5;6];[7;8;9]] [[1;2;1];[2;1;2];[3;2;3]];;
0.000020 sec
- : IntSemiring.t list list = [[14; 10; 14]; [32; 25; 32]; [50; 40; 50]]
```

* one_module.ml
```
# matrixInt [|[|1;2;3|];[|4;5;6|];[|7;8;9|]|] [|[|1;2;1|];[|2;1;2|];[|3;2;3|]|];;
0.000011 sec
- : int array array = [|[|14; 10; 14|]; [|32; 25; 32|]; [|50; 40; 50|]|]
```
