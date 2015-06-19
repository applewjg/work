package main

import (
	"crypto/md5"
	"encoding/hex"
	"fmt"
)

func main() {
	md5Ctx := md5.New()
	md5Ctx.Write([]byte("test md5 encrypto"))
	cipherStr := md5Ctx.Sum(nil)
	fmt.Print(cipherStr)
	fmt.Print("\n")
	fmt.Println(hex.EncodeToString(cipherStr))
}
