package main

import (
	"fmt"
	"math/rand"
)

func main() {
	var s = "JSZBHRWOQLHXLKGI"
	for i := 0; i < 1000000; i++ {
		var flag = true
		rand.Seed(int64(i))
		for j := 0; j < 16; j++ {
			if byte('A'+rand.Intn(26)) != s[j]  {
				flag = false
			}
		}
		if flag {
			fmt.Printf("%d",i)
			fmt.Println()
		}
	}
}
