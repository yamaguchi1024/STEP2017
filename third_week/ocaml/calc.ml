
(* lexer *)
let rec reverse1 = fun x y ->
    match x with
            |[] -> y
                |a::b -> reverse1 b (a::y) ;;

let reverse = fun x -> reverse1 x [] ;;

type token = INVALID | PLUS | MINUS | MUL | DIV | LPAR | RPAR | FLOAT | INT
type expr = EPLUS | EMINUS | EMUL | EDIV | ELPAR | ERPAR | EFLOAT of string| EINT of string | EOF

let input_buffer: string ref = ref ""
let pos_start = ref 0
let pos_current = ref 0
let last_token = ref INVALID
let last_pos = ref 0
let report_error pos =
    print_string ("invalid token at position "^(string_of_int pos)^"\n")

let get_lexeme() =
    let len = !last_pos - !pos_start in String.sub !input_buffer !pos_start len

    let res = ref []

    (* トークンを表示 *)
    let output_token token =
        match token with
 | PLUS -> res := EPLUS::(!res)
 | MINUS -> res := EMINUS::(!res)
 | MUL -> res := EMUL::(!res)
 | DIV -> res := EDIV::(!res)
 | LPAR -> res := ELPAR::(!res)
 | RPAR -> res := ERPAR::(!res)
 | FLOAT -> res := EFLOAT(get_lexeme())::(!res)
 | INT -> res := EINT(get_lexeme())::(!res)
 | INVALID -> assert false

 (* 一文字読みこむ *)
    let readc() =
        let c = !input_buffer.[!pos_current] in (pos_current:= !pos_current+1; c)
        (* マッチしたトークンとその場所を保存 *)
let save token = (last_token := token; last_pos := !pos_current)

(* メイン：文字列inputを受け取り、トークン列に分解して表示 *)
let rec lexer (input: string) =
    res := [];
   input_buffer := (input^"\000"); (* inputに文字列の最後を表す"\000"を追加 *)
   pos_start := 0; pos_current := 0; last_token := INVALID;
   q0(); reverse (!res);

and q0 () = (* 初期状態 *)
    match readc() with
   ' ' -> (pos_start := !pos_start+1;
           q0())
 | '+' -> (save PLUS; next())
 | '-' -> (save MINUS; next())
 | '*' -> (save MUL; next())
 | '/' -> (save DIV; next())
 | '(' -> (save LPAR; next())
 | ')' -> (save RPAR; next())
 | '0' -> (save INT; q_zero())
 | c -> if '1'<=c && c<='9' then (save INT; q_num())
 else if c='\000' then () (* 文字列の最後なら終了 *)
 else report_error(!pos_current)

and q_zero() = 
    let c= readc() in
    if '.' = c then (save FLOAT; q_float()) 
    else next()

and q_float() = 
    let c= readc() in
    if '0'<=c && c<='9' then (save FLOAT; q_float()) 
    else next()

and q_num() = 
    let c= readc() in
    if '0'<=c && c<='9' then (save INT; q_num()) 
    else if '.' = c then (save FLOAT; q_float()) 
    else next()

and next() = 
    if !last_token=INVALID then (* エラー *)
        report_error(!pos_current)
    else (* トークンを表示して次のトークンの処理へ *)
        (output_token !last_token; pos_start := !last_pos;
        pos_current := !pos_start; last_token := INVALID;
        q0())

        (* LL1 parser *)

(*
S -> E$
E -> T G
G -> epsilon | +E | -E
T -> F H
H -> epsilon | * T | / T
F -> INT | FLOAT | (E) 
*)


exception ERROR
type nt = string
type ptree = PTt of expr | PTnt of nt * ptree list

type symbol = NT of string | T of expr 

(* LL1 parsing table
  　　　　　+    -      INT      (   FLOAT      )      *      /      $   
  　　S                S->E$  S->E$  S->E$
      E                E->TG  E->TG  E->TG  
      G  G->+E  G->-E                          G->ε                G->ε  
      T                T->FH  T->FH  T->FH
      H  H->ε  H->ε                            H->ε   H->*T  H->/T H->ε 
      F                F->INT F->(E) F->FLOAT
*)
let tab : (nt * (expr -> symbol list)) list = 

    [("S", fun t -> 
        match t with
           (EINT _ |ELPAR|EFLOAT _) -> [NT "E"; T EOF]
            | _ -> raise ERROR);
   ("E", fun t ->
       match t with
           (EINT _ |ELPAR|EFLOAT _) -> [NT "T"; NT "G"]
            | _ -> raise ERROR);
   ("G", fun t ->
       match t with
            EPLUS -> [T EPLUS ; NT "E"]
            | EMINUS -> [T EMINUS ; NT "E"]
            | (ERPAR|EOF) -> []
            | _ -> raise ERROR);
   ("T", fun t->
       match t with
           (EINT _|ELPAR|EFLOAT _) -> [NT "F"; NT "H"]
            | _ -> raise ERROR);
   ("H", fun t ->
       match t with
            EMUL -> [T EMUL ; NT "T"]
            | EDIV -> [T EDIV ; NT "T"]
            | (EPLUS|EMINUS|ERPAR|EOF) -> []
            | _ -> raise ERROR);
   ("F", fun t->
       match t with
            EINT(s) -> [T t]
            | EFLOAT(s) -> [T t]
            | ELPAR -> [T ELPAR; NT "E"; T ERPAR]
            | _ -> raise ERROR)
 ]

let input_buffer = ref [| |];;
let current_pos = ref 0;;
let read_token() = 
    let token = (!input_buffer).(!current_pos) in
    (current_pos := !current_pos+1; token)
let unread_token() =
    current_pos := !current_pos-1
let lookahead() =
    (!input_buffer).(!current_pos)

let lookup_LL1table nt t =
    (List.assoc nt tab) t

let rec parsing token_list =
    input_buffer := Array.of_list (token_list@[EOF]);
   current_pos := 0;
     parse (NT "S")

     (* LL(1) parser *)
 and parse x =    
     match x with
     T t -> 
        if read_token()=t then PTt(t) else raise ERROR
    | NT nt ->
         let body = lookup_LL1table nt (lookahead()) in
         let ptlist = List.map parse body in
         PTnt(nt, ptlist)


         (* translator from parse trees to abstract syntax trees 
  (that represent logical structures of expressions)
*)
type ast = EConstInt of int 
         | EConstFloat of float
         | EMul of ast * ast
         | EDiv of ast * ast
         | EAdd of ast * ast
         | ESub of ast * ast

let rec pt2ast pt =
    match pt with
    PTt(EINT(x)) -> EConstInt (int_of_string x)
 | PTt(EFLOAT(x)) -> EConstFloat (float_of_string x)
 | PTnt("S", [pt';_]) -> pt2ast pt'
  | PTnt("E", [pt1;pt2]) ->
          let ast1 = pt2ast pt1 in
          g_or_h2ast pt2 ast1 
  | PTnt("T", [pt1;pt2]) ->
          let ast1 = pt2ast pt1 in
          g_or_h2ast pt2 ast1 
  | PTnt("F", [pt1]) ->
          pt2ast pt1
  | PTnt("F", [PTt ELPAR;pt1;PTt ERPAR]) ->
          pt2ast pt1
  | _ -> assert false

and g_or_h2ast pt ast =
    match pt with
    PTnt("G", [PTt EPLUS; PTnt("E", [pt1; pt2])])->
        let ast1 = pt2ast pt1 in
        let ast0 = EAdd(ast, ast1) in
        g_or_h2ast pt2 ast0
  | PTnt("G", [PTt EMINUS; PTnt("E", [pt1; pt2])])->
          let ast1 = pt2ast pt1 in
          let ast0 = ESub(ast, ast1) in
          g_or_h2ast pt2 ast0
  | PTnt("G", []) -> ast
  | PTnt("H", [PTt EMUL; PTnt("T", [pt1; pt2])])->
          let ast1 = pt2ast pt1 in
          let ast0 = EMul(ast, ast1) in
          g_or_h2ast pt2 ast0
  | PTnt("H", [PTt EDIV; PTnt("T", [pt1; pt2])])->
          let ast1 = pt2ast pt1 in
          let ast0 = EDiv(ast, ast1) in
          g_or_h2ast pt2 ast0
  | PTnt("H", []) -> ast
  | _ -> assert false

type value = VInt of int | VFloat of float

exception EvalErr

let rec eval_expr e =
    match e with
  | EConstInt i ->
          VInt i
  | EConstFloat f ->
          VFloat f
  | EAdd (e1,e2) ->
          let v1 = eval_expr e1 in
          let v2 = eval_expr e2 in
          (match v1, v2 with
     | VInt i1, VInt i2 -> VInt (i1 + i2)
     | VFloat i1, VFloat i2 -> VFloat (i1 +. i2)
     | _ -> raise EvalErr)
          | ESub (e1,e2) ->
                  let v1 = eval_expr e1 in
                  let v2 = eval_expr e2 in
                  (match v1, v2 with
     | VInt i1, VInt i2 -> VInt (i1 - i2)
     | VFloat i1, VFloat i2 -> VFloat (i1 -. i2)
     | _ -> raise EvalErr)
                  | EMul (e1,e2) ->
                          let v1 = eval_expr e1 in
                          let v2 = eval_expr e2 in
                          (match v1, v2 with
     | VInt i1, VInt i2 -> VInt (i1 * i2)
     | VFloat i1, VFloat i2 -> VFloat (i1 *. i2)
     | _ -> raise EvalErr)
                          | EDiv (e1,e2) ->
                                  let v1 = eval_expr e1 in
                                  let v2 = eval_expr e2 in
                                  (match v1, v2 with
     | VInt i1, VInt i2 -> VInt (i1 / i2)
     | VFloat i1, VFloat i2 -> VFloat (i1 /. i2)
     | _ -> raise EvalErr)

let print_value v =
    match v with
    | VInt i -> print_int i; print_string "\n"
    | VFloat f -> print_float f; print_string "\n"

let rec main () = 
    print_string "> "; 
    flush stdout;
    print_value(eval_expr(pt2ast(parsing(lexer(read_line())))));
    main ();;

let _ = main () 
