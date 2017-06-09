
let matrixInt x y =
    let t = Sys.time() in
        let lx = Array.length x
            and ly = Array.length y in
            let vy = if ly = 0 then 0 else Array.length y.(0) in
                let z = Array.make_matrix lx vy 0 in
                    (for i = 0 to lx-1 do
                        for j = 0 to vy-1 do
                            for k = 0 to ly-1 do
                                z.(i).(j) <- z.(i).(j) + x.(i).(k) * y.(k).(j)
                            done
                        done
                    done;
                    Printf.printf "%f sec\n" (Sys.time() -. t) ); z;;

matrixInt (Array.make_matrix 1 1 1) (Array.make_matrix 1 1 3);;
matrixInt (Array.make_matrix 10 10 1) (Array.make_matrix 10 10 3);;
matrixInt (Array.make_matrix 100 100 1) (Array.make_matrix 100 100 3);;
matrixInt (Array.make_matrix 200 200 1) (Array.make_matrix 200 200 3);;
matrixInt (Array.make_matrix 300 300 1) (Array.make_matrix 300 300 3);;
matrixInt (Array.make_matrix 400 400 1) (Array.make_matrix 400 400 3);;
matrixInt (Array.make_matrix 500 500 1) (Array.make_matrix 500 500 3);;
matrixInt (Array.make_matrix 600 600 1) (Array.make_matrix 600 600 3);;
matrixInt (Array.make_matrix 700 700 1) (Array.make_matrix 700 700 3);;
matrixInt (Array.make_matrix 800 800 1) (Array.make_matrix 800 800 3);;
matrixInt (Array.make_matrix 900 900 1) (Array.make_matrix 900 900 3);;
matrixInt (Array.make_matrix 1000 1000 1) (Array.make_matrix 1000 1000 3);;
