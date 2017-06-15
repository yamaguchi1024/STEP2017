type value = VInt of int | VFloat of float
type ast = EConstInt of int 
         | EConstFloat of float
         | EMul of ast * ast
         | EDiv of ast * ast
         | EAdd of ast * ast
         | ESub of ast * ast

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
