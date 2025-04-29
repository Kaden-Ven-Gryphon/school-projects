//###########################################################################################
// Kaden Beaver
// 10.05.2021
// CS 424-01
//
// main.go
// Programing Assignment 1
// Takes user input for a file name an opens that file
// grabs all of thel lines from the file and store them in an array
// go through each line and pass it to the newPlayer function which converts the string
// to an instance of PlayerEntry and calculates the stats
// use the sort interface to sort based on OPS
// use printf for print fortated table of the data to the consol
//
//###########################################################################################



package main

import (												//imports
	"bufio"
	"fmt"
	"os"
	"log"
	"strings"
	"strconv"
	"sort"
)
														//the player entry data type
type PlayerEntry struct {
	FirstName string									//these are from the file
	LastName string
	PlateAppearances int
	AtBats int
	Singles int
	Doubles int
	Triples int
	Homeruns int
	Walks int
	HitByPitch int
	
	Average float64										//these are calcuated
	Slugging float64
	OnBase float64
	Ops float64
}

func newPlayer(line string) *PlayerEntry {				//converts a string to a PlayerEntry
	var buf int											//buf to convert stirngs to ints
		
		words := strings.Fields(line)					//create an array (slice?) of each of the elements of the line
		entry := PlayerEntry{FirstName: words[0]}		//create an entry and set the first name
		entry.LastName = words[1]						//set the last name
		buf, _ = strconv.Atoi(words[2])					//convert string to int for PlateAppearances
		entry.PlateAppearances = buf					//set plate Apperances
		buf, _ = strconv.Atoi(words[3])					//REPEATE for each element
		entry.AtBats = buf
		buf, _ = strconv.Atoi(words[4])
		entry.Singles = buf
		buf, _ = strconv.Atoi(words[5])
		entry.Doubles = buf
		buf, _ = strconv.Atoi(words[6])
		entry.Triples = buf
		buf, _ = strconv.Atoi(words[7])
		entry.Homeruns = buf
		buf, _ = strconv.Atoi(words[8])
		entry.Walks = buf
		buf, _ = strconv.Atoi(words[9])
		entry.HitByPitch = buf
														//calcuate the four statistics
		entry.Average = (float64(entry.Singles) + float64(entry.Doubles) + float64(entry.Triples) + float64(entry.Homeruns))/float64(entry.AtBats)
		entry.Slugging = (float64(entry.Singles) + 2*float64(entry.Doubles) + 3*float64(entry.Triples) + 4*float64(entry.Homeruns))/float64(entry.AtBats)
		entry.OnBase =  (float64(entry.Singles) + float64(entry.Doubles) + float64(entry.Triples) + float64(entry.Homeruns) + float64(entry.Walks) + float64(entry.HitByPitch))/float64(entry.PlateAppearances)
		entry.Ops = entry.OnBase + entry.Slugging
		
	
	return &entry
}


type ByOPS []PlayerEntry								//set a type for the sort interface to work off

func (a ByOPS) Len() int			{ return len(a) }					//these three are the interface for sort
func (a ByOPS) Less(i, j int) bool	{ return a[i].Ops > a[j].Ops }		//this is swaped to sort in revers order
func (a ByOPS) Swap(i, j int)		{ a[i], a[j] = a[j], a[i] }



func main() {											//MAIN
	
	var fileName string									//the file name
	var txtLines []string								//where the file contents get stored
	var players []PlayerEntry							//where the players get stored
	
	fmt.Print("Enter the file name:> ")					//prompt the user for file name
	fmt.Scanln(&fileName)								//get string
	
	file, err := os.Open(fileName+".txt")				//open the file
	if err != nil {										//check for error
		log.Fatal(err)
	}
	
	defer file.Close()									//que up closeing the file
	
	scaner := bufio.NewScanner(file)					//set up a scaner
	for scaner.Scan() {									//until the end of file add lines from txt to array
		txtLines = append(txtLines, scaner.Text())
	}
	err = scaner.Err()									//if error not EOF occors
	if err != nil {
		log.Fatal(err)
	}
	
	for _, line := range txtLines {						//for each line
		
		entry := newPlayer(line)						//convert line to entry
		players = append(players, *entry)				//add entry to array
	}
	
	sort.Sort(ByOPS(players))							//sort by OPS, casting players to a ByOPS
	
	
	
	fmt.Printf("\n\nBASEBALL TEAM REPORT --- %d PLAYERS FOUNT IN FILE\n\n", len(players))				//print summury
	fmt.Printf("%20s : %12s %12s %12s %8s\n","PLAYER NAME", "AVERAGE", "SLUGGING", "ONBASE%", "OPS")	//print headers
	fmt.Printf("----------------------------------------------------------------------------------\n")	//print break line
	
	for _, player := range players {																					//for each player, print their stats
		fmt.Printf("%20s : %12.3f %12.3f %12.3f %8.3f\n", player.LastName + ", " + player.FirstName, player.Average, player.Slugging, player.OnBase, player.Ops)
	}
	
}//END OF PROGRAM