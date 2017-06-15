(* LL1 parser *)

(*
S -> E$
E -> T G
G -> epsilon | +E | -E
T -> F H
H -> epsilon | * T | / T
F -> INT | FLOAT | (E) 
*)


type expr = EPLUS | EMINUS | EMUL | EDIV | ELPAR | ERPAR | EFLOAT of string| EINT of string | EOF

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
      H  H->ε  H->ε                                   H->*T  H->/T H->ε 
      F                F->INT F->(E) F->FLOAT F->(E)
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
          | (EPLUS|EMINUS|EOF) -> []
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
exception ERROR;;
let read_token() = 
   let token = (!input_buffer).(!current_pos) in
      (current_pos := !current_pos+1; token)
let unread_token() =
    current_pos := !current_pos-1
let lookahead() =
   (!input_buffer).(!current_pos)

let lookup_LL1table nt t =
  (List.assoc nt tab) t

let rec main token_list =
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
type ast = ASTid of string
         | ASTtimes of ast * ast
         | ASTplus of ast * ast
         | ASTminus of ast * ast

let rec pt2ast pt =
  match pt with
    PTt(EINT(x)) -> ASTid x
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
      let ast0 = ASTplus(ast, ast1) in
        g_or_h2ast pt2 ast0
  | PTnt("G", [PTt EMINUS; PTnt("E", [pt1; pt2])])->
      let ast1 = pt2ast pt1 in
      let ast0 = ASTminus(ast, ast1) in
        g_or_h2ast pt2 ast0
  | PTnt("G", []) -> ast
  | PTnt("H", [PTt EMUL; PTnt("T", [pt1; pt2])])->
      let ast1 = pt2ast pt1 in
      let ast0 = ASTtimes(ast, ast1) in
        g_or_h2ast pt2 ast0
  | PTnt("H", [PTt EDIV; PTnt("T", [pt1; pt2])])->
      let ast1 = pt2ast pt1 in
      let ast0 = ASTtimes(ast, ast1) in
        g_or_h2ast pt2 ast0
  | PTnt("H", []) -> ast
  | _ -> assert false
