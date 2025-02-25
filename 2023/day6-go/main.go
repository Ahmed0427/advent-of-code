package main

import (
	"fmt"
	"strings"
	"strconv"
	"os"
)

func parseTimeDist(data string) ([]int, []int) {
	var times []int
	var dists []int

	twoLines := strings.Split(data, "\n")

	for _, token := range strings.Split(twoLines[0], " ")[1:] {
		if strings.TrimSpace(token) != "" {
			value, _ := strconv.ParseInt(token, 10, 32)
			times = append(times, int(value))
		}
	}	

	for _, token := range strings.Split(twoLines[1], " ")[1:] {
		if strings.TrimSpace(token) != "" {
			value, _ := strconv.ParseInt(token, 10, 32)
			dists = append(dists, int(value))
		}
	}	

	return times, dists
} 

func calcPart1(times []int, dists []int) int64 {
	var res int64 = 1
	for i, time := range times {
		dist, waysCnt := dists[i], 0
		for i := 0; i <= time; i++ {
			if i * (time - i) > dist {
				waysCnt++
			}
		}
		res *= int64(waysCnt)
	}
	return res
}

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage: go run main.go <filepath>")
		return
	}

	data, err := os.ReadFile(os.Args[1])	
	if err != nil {
		os.Exit(1)
	}

	times, dists := parseTimeDist(string(data))

	ans1 := calcPart1(times, dists)

	fmt.Println(ans1)
}
