
## 第三回 電卓を作る課題

### ほぼチートなrubyの動作例。多分一番短い。
```
> 1 + 1
2
> 3 / 8
0
> 9/3
3
> 3.3 + 1
4.3
> (2 - 1) * 3
3
```

### 完全にやりすぎなocamlの実装。
* トークンに分割し、LL(1)で構文解析をして構文木を作りそこから抽象構文木を作り、それを評価するということをしている。
* 学校の授業でちょうどコンパイラを作るというのをやっていて、そこではocamllexやocamlyaccを使っているので全部手作りというのは初めてて、非常に勉強になった。
* 整数型と浮動小数点型同士の演算はできない。これは私の趣味に寄るところが大きい。暗黙の型変換は良くない。

```
> 3 + 1
4
> 3+3/5*9-1
2
> 3.3-2.1*2.222
-1.3662
> (2-1)
1
> (2-3)*9
-9
```

* lexer 動作例
```
# lexer(read_line());;
3+2-1*2.2+2.2222
- : expr list =
[EINT "3"; EPLUS; EINT "2"; EMINUS; EINT "1"; EMUL; EFLOAT "2.2"; EPLUS;
 EFLOAT "2.2222"]
# lexer(read_line());;
7.77777-1.2222*222
- : expr list = [EFLOAT "7.77777"; EMINUS; EFLOAT "1.2222"; EMUL; EINT "222"]
```

* parser 動作例。構文木
```
# parsing(lexer(read_line()));;
7.77777-1.2222*222.55
- : ptree =
PTnt ("S",
 [PTnt ("E",
   [PTnt ("T", [PTnt ("F", [PTt (EFLOAT "7.77777")]); PTnt ("H", [])]);
    PTnt ("G",
     [PTt EMINUS;
      PTnt ("E",
       [PTnt ("T",
         [PTnt ("F", [PTt (EFLOAT "1.2222")]);
          PTnt ("H",
           [PTt EMUL;
            PTnt ("T", [PTnt ("F", [PTt (EFLOAT "222.55")]); PTnt ("H", [])])])]);
        PTnt ("G", [])])])]);
  PTt EOF])
```

* 抽象構文木
```
# pt2ast(parsing(lexer(read_line())));;
7.77777-1.2222*222.55
- : ast =
ESub (EConstFloat 7.77777, EMul (EConstFloat 1.2222, EConstFloat 222.55))
```

* eval 動作例
```
# eval_expr(pt2ast(parsing(lexer(read_line()))));;
7.77777-(1.2222*222.55)
- : value = VFloat (-264.22284)
```
