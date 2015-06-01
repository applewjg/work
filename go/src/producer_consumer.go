package main

import "fmt"
import "time"
import "math/rand"

func producer(c chan int64) {
	r := rand.New(rand.NewSource(time.Now().UnixNano()))
	for i := 1; i < 5; i++ {
		time.Sleep(time.Second * time.Duration(r.Intn(3)))
		c <- time.Now().Unix()
	}
}

func main() {
	c := make(chan int64, 10)
	go producer(c)
	go func(ch chan int64) {
		time.Sleep(time.Second * 20)
		fmt.Println("put put")
		c <- time.Now().Unix()
	}(c)
	for {
		select {
		case res := <-c:
			fmt.Println(res)
		case <-time.After(2 * time.Second):
			fmt.Println("timer of 2 second arrived!")
		}
	}
}
