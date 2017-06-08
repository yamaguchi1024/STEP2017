package main

import (
	"fmt"
	"math/rand"
    "strings"
)

func main() {
	var s = "JSZBHRWOQLHXLKGI"
	for i := 0; i < 1000000000; i++ {
	    var flag = true
		rand.Seed(int64(i))
        var t = s
		for j := 0; j < 16; j++ {
            var cc = string(byte('A' + rand.Intn(26)))
			if strings.Contains( t, cc ) == false {
				flag = false
			} else {
                strings.Replace(t, cc, "", -1)
            }
		}
		if flag {
			fmt.Printf("%d",i)
			fmt.Println()
		}
	}
}
