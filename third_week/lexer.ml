
let rec reverse1 = fun x y ->
        match x with
            |[] -> y
                |a::b -> reverse1 b (a::y) ;;

let reverse = fun x -> reverse1 x [] ;;

type token = INVALID | PLUS | MINUS | MUL | DIV | LPAR | RPAR | FLOAT | INT
type expr = EPLUS | EMINUS | EMUL | EDIV | ELPAR | ERPAR | EFLOAT of string| EINT of string 

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
let rec main (input: string) =
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
