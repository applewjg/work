package main

import (
	"crypto/md5"
	"encoding/binary"
	"fmt"
	"regexp"
	"strings"
	"unicode/utf8"
)

func calcu(text string) (res uint64) {
	md5Ctx := md5.New()
	str := strings.ToLower(text)
	re, _ := regexp.Compile(`[\pZ|\pC|\pS|\s]`)
	str = re.ReplaceAllString(str, "")
	reg := regexp.MustCompile(`[\PP]+`)
	strs := reg.FindAllString(str, -1)

	if len(strs) == 0 {
		return 0
	}

	maxLen := utf8.RuneCountInString(strs[0])
	result := strs[0]
	for i := 1; i < len(strs); i++ {
		l := utf8.RuneCountInString(strs[i])
		if maxLen < l {
			maxLen = l
			result = strs[i]
		}
	}

	md5Ctx.Write([]byte(result))
	cipherStr := md5Ctx.Sum(nil)
	res = binary.BigEndian.Uint64(cipherStr[:8])
	return
}

func main() {
	text := string("是主要 的计\t算数据 fa结构， ABDCfdffdsafdsafasfdD    , 使用中 有并。发的　问题fdas!fda    ，可能需要加锁")
	res := calcu(text)
	fmt.Println(res)
}
