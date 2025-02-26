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

func calcPart2(times []int, dists []int) int64 {
	var timeStr string
	var distStr string
	for i := range times {
		t := strconv.Itoa(times[i])
		d := strconv.Itoa(dists[i])
		timeStr += t
		distStr += d 
	}

	time, _ := strconv.ParseInt(timeStr, 10, 64)
	dist, _ := strconv.ParseInt(distStr, 10, 64)
	
	var res int64
	for i := int64(0); i <= time; i++ {
		if i * (time - i) > dist {
			res++
		}
	}
	return res
}

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage: go run main.go <filepath>")
		os.Exit(1)
	}

	data, err := os.ReadFile(os.Args[1])	
	if err != nil {
		os.Exit(1)
	}

	times, dists := parseTimeDist(string(data))

	ans1 := calcPart1(times, dists)
	ans2 := calcPart2(times, dists)

	fmt.Println(ans1)
	fmt.Println(ans2)
}
