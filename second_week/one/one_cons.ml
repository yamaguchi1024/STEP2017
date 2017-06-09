let rec reverse1 = fun x y ->
        match x with
            |[] -> y
                |a::b -> reverse1 b (a::y) ;;

let reverse = fun x -> reverse1 x [] ;;

module type SEMIRING =
sig
  type t
  val add : t -> t -> t
  val mul : t -> t -> t
  val unit : t
  val zero : t
end

module IntSemiring =
struct
  type t = int
  let add x y =  x + y
  let mul x y = x * y
  let unit = 1
  let zero = 0
end

module type VECTOR =
  functor (T: SEMIRING) ->
  sig
    type t
    val add : t -> t -> t
    val mul : t -> t -> T.t
  end

module Vector = 
  functor (T: SEMIRING) ->
  struct
    type t = T.t list
    let rec add1 x y q =
        match x with 
        | [] -> q
        | a::b -> 
                match y with
                |[] -> q
                | c::d -> add1 b d ((T.add a c)::q)
    let add x y = reverse (add1 x y [])
    let rec mul1 x y q =
        match x with 
        | [] -> q
        | a::b -> 
                match y with
                |[] -> q
                | c::d -> mul1 b d (T.add q (T.mul a c))
    let mul x y = mul1 x y T.zero
end

module VectorInt = Vector(IntSemiring)

module type MATRIX = 
  functor (T: SEMIRING) ->
  sig
    type t
    val add : t -> t -> t
    val mul : t -> t -> t
    val trans : t -> t -> t
  end

module Matrix = 
  functor (T: SEMIRING) ->
  struct
    type t = T.t list list

    exception Comatta

    let rec bind x y = 
      match x with
      | a::b ->
         (match a with
         |c::d -> reverse (bind b (c::y))
         |[]  -> raise Comatta)
      | _ -> y
    let rec sakujo x y =
      match x with
      | a::b -> 
          (match a with
            |c::d -> (sakujo b (d::y))
            |[] -> raise Comatta)
      | _ -> y
    let rec trans1 x y =
      match x with
       |a::b when a=[] -> y
       |a::b -> trans1 (sakujo x []) ((bind x [])::y)
       | _ -> y
    let trans x = reverse (trans1 x [])
    let rec add2 x y q =
      match x with 
      | [] -> q
      | a::b -> 
          (match y with
           |[] -> q
           | c::d -> add2 b d ((T.add a c)::q))
    let rec add1 x y z =
      match x with
      |a::b ->
          (match y with
           |c::d -> add1 b d ((reverse (add2 a c []))::z)
           | _ -> raise Comatta)
      | _ -> z
    let add x y = reverse (add1 x y [])
    let rec mul2 x y q =
        match x with 
        | [] -> q
        | a::b -> 
                (match y with
                |[] -> q
                | c::d -> mul2 b d (T.add q (T.mul a c)))
    let rec mul1 x y z =
        match y with
                |c::d -> mul1 x d ((mul2 x c T.zero)::z)
                | _ -> z
    let rec mul0 x y z t =
        match x with 
        | a::b ->
                mul0 b y ((mul1 a (trans y) [])::z) t
        | _ -> Printf.printf "%f sec\n" (Sys.time() -. t); reverse z
    let mul x y = let t = Sys.time() in mul0 x y [] t
end


module MatrixInt = Matrix(IntSemiring)

let rec make_vector x a l = 
    match x with 
    |0 -> l 
    |_ -> make_vector (x-1) a (a::l)

let rec make_matrix1 x y a l =
    match y with
    |0 -> l
    |_ -> make_matrix1 x (y-1) a ((make_vector x a [])::l)

let make_matrix x y a =
    make_matrix1 x y a [];;

MatrixInt.mul (make_matrix 1 1 1) (make_matrix 1 1 3);;
MatrixInt.mul (make_matrix 10 10 1) (make_matrix 10 10 3);;
MatrixInt.mul (make_matrix 100 100 1) (make_matrix 100 100 3);;
MatrixInt.mul (make_matrix 200 200 1) (make_matrix 200 200 3);;
MatrixInt.mul (make_matrix 300 300 1) (make_matrix 300 300 3);;
MatrixInt.mul (make_matrix 400 400 1) (make_matrix 400 400 3);;
MatrixInt.mul (make_matrix 500 500 1) (make_matrix 500 500 3);;
MatrixInt.mul (make_matrix 600 600 1) (make_matrix 600 600 3);;
MatrixInt.mul (make_matrix 700 700 1) (make_matrix 700 700 3);;
MatrixInt.mul (make_matrix 800 800 1) (make_matrix 800 800 3);;
MatrixInt.mul (make_matrix 900 900 1) (make_matrix 900 900 3);;
MatrixInt.mul (make_matrix 1000 1000 1) (make_matrix 1000 1000 3);;
