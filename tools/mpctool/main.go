package main

import (
	"flag"
	"fmt"
	"os"
	"os/signal"
	"strings"
	"time"
)

var (
	urlFlag = flag.String("url", "", "Url to the redis server (default = '')")
	whoFlag = flag.String("flag", "", "Flag to show who compute (alice or bob)")
)

func main() {
	// Parse and ensure all needed inputs are specified.
	flag.Parse()
	if *urlFlag == "" {
		fmt.Printf("No url (--url) specified \n")
		os.Exit(-1)
	}
	if *whoFlag == "" {
		fmt.Println("No who (--flag) specified \n")
		os.Exit(-1)
	}

	var (
		redisUrl = *urlFlag
		who = *whoFlag
	)
	if !strings.EqualFold(strings.ToLower(who), strings.ToLower("alice")) &&
		!strings.EqualFold(strings.ToLower(who), strings.ToLower("bob")) {
		fmt.Println("Invalid flag value, must be (bob or alice)")
		os.Exit(-1)
	}

	redis, err := NewRedis(redisUrl)
	if err != nil {
		fmt.Println("Invalid url for redis server. - ", redisUrl)
		os.Exit(-1)
	}

	go mainLoop(redis, who)

	sigCh := make(chan os.Signal)
	signal.Notify(sigCh)
	quit := <- sigCh
	fmt.Println("收到退出信号：", quit)
}

func mainLoop(redis *Redis, who string) {

	fmt.Println("start main loop...")
	defer redis.Con.Close()
	// 创建loop循环读取队列中的数据，并进行触发，定时读取
	read := time.NewTicker(time.Second * 2)
	defer read.Stop()

	for {
		select {
		case <-read.C:
			// 读取redis数据进行处理
			var redisKey string
			if strings.EqualFold(strings.ToLower(who), strings.ToLower("alice")) {
				redisKey = MPC_TASK_KEY_ALICE
			}
			if strings.EqualFold(strings.ToLower(who), strings.ToLower("bob")) {
				redisKey = MPC_TASK_KEY_BOB
			}
			fmt.Println("用于获取队列的key为：", redisKey)
			err := Notify(redis.GetOne(redisKey))
			if err != nil {
				fmt.Println("接口调用VM或数据获取失败 : ", err.Error())
				//os.Exit(-1)
			}
			//res := string(redis.RPop(redisKey))
			//fmt.Println("从redis收到数据为：", res)
		}
	}
}
