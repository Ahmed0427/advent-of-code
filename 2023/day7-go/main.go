package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"strconv"
	"sort"
)

const cards = "AKQJT98765432"

type HandBid struct {
	Hand string
	Bid int
}

type ByHand []HandBid

func reverse(slice []int) {
	n := len(slice)
	for i := 0; i < n / 2; i++ {
		slice[i], slice[n - 1 - i] = slice[n - 1 - i], slice[i]
	}
}

func getHandType(s string) int {
	charFreq := make(map[rune]int)
	for _, char := range s {
		charFreq[char]++
	}

	var cnts []int	

	for _, freq := range charFreq {
		cnts = append(cnts, freq)
	}

	sort.Ints(cnts)
	reverse(cnts)


	handType := 6 

	if cnts[0] == 5 {
		handType = 0
	} else if cnts[0] == 4 {
		handType = 1
	} else if cnts[0] == 3 {
		if cnts[1] == 2 {
			handType = 2
		} else {
			handType = 3
		}
	} else if cnts[0] == 2 {
		if cnts[1] == 2 {
			handType = 4 
		} else {
			handType = 5 
		}
	}
	
	return handType
}

func (h ByHand) Len() int {
	return len(h)
}

func (h ByHand) Swap(i, j int) {
	h[i], h[j] = h[j], h[i]
} 

func (h ByHand) Less(i, j int) bool {
	ti := getHandType(h[i].Hand)
	tj := getHandType(h[j].Hand)

	if ti == tj {
		for x := 0; x < 5; x++ {
			if h[i].Hand[x] != h[j].Hand[x] {
				xi := strings.IndexRune(cards, rune(h[i].Hand[x]))
				xj := strings.IndexRune(cards, rune(h[j].Hand[x]))
				return xi > xj
			}
		}
	}
	return ti > tj
}

func main() {
	if len(os.Args) != 2 {
		log.Fatal("ERROR: uncorrect usage of the program")
	}

	data, err := os.ReadFile(os.Args[1])
	if err != nil {
		log.Fatal("ERROR: reading file")
	}
	
	var handBidList []HandBid
	for _, line := range strings.Split(string(data), "\n") {
		tokens := strings.Split(line, " ")
		if len(tokens) == 2 {
			hand, strBid := tokens[0], tokens[1]
			bid, err := strconv.Atoi(strBid)
			if err != nil {
				log.Fatal("ERROR: parsing bid")
			}
			handBidList = append(handBidList, HandBid{hand, bid})
		}
	}

	sort.Sort(ByHand(handBidList))

	ans := 0
	for i, handBid := range(handBidList) {
		ans += (i + 1) * handBid.Bid	
	}

	fmt.Println(ans)
}
