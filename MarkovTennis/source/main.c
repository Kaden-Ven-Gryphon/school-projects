/*
*
*	Kaden Gryphon
*	Program 4 Markoc chain Tennis
*	03.20.2023
*
*
*/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>


struct tennis_responseVariable
{
	int winner;
	int points;
	int stateCounts[21];
};



struct tennis_responseVariable tennis_runTrial(int verbose);


static char stateStrings[21][6] = {"nulll", "0,0", "15,0", "30,0", "40,0", "0,15", "15,15", "30,15", "40,15", "0,30", "15,30", "30,30", "40,30", "0,40", "15,40", "30,40", "Deuce", "Adv A", "Adv B", "A Win", "B Win"};



int main()
{
	printf("Start main\n");
	srand((unsigned) time(NULL));
	
	int stateTotals[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int aWins = 0;
	int bWins = 0;
	double aRatio = 0;
	double bRatio = 0;
	int shortest = INT_MAX;
	int longest = 0;
	
	//open output file / redirect printf
	FILE *fp = NULL;
	
	if((fp=freopen("kadengryphonOutput.txt", "w", stdout)) == NULL)
	{
		printf("Cannot open output file\n");
		return(1);
	}
	
	
	
	//run one million trials
	for(int i = 0; i < 1000000; i++)
	{
		//run the trial
		struct tennis_responseVariable buf = 	tennis_runTrial(0);
		
		//varifiy the trial did not have an error
		if(buf.winner == -1)
		{
			fprintf(stderr, "ERROR trial failed\n");
			return 1;
		}
		
		//collect response vars
		for(int j = 1; j < 21; j++)
		{
			stateTotals[j]+=buf.stateCounts[j];
		}
		if(buf.winner == 1) aWins++;
		else bWins++;
		if(buf.points < shortest) shortest = buf.points;
		if(buf.points > longest) longest = buf.points;
	}
	aRatio = ((double)aWins) / ((double)1000000);
	bRatio = ((double)bWins) / ((double)1000000);
	
	printf("Tennis Game, trials= 1000000\n");
	printf("Player A wins, count= %7d proportion= %4.2f\n", aWins, aRatio);
	printf("Player B wins, count= %7d proportion= %4.2f\n", bWins, bRatio);
	printf("shortest game= %4d\n", shortest);
	printf("longest  game= %4d\n", longest);
	
	printf("\nTotal Counts for each state\n\n");
	printf("   state | %7s | proportion\n", "count");
	printf("         | %7s |\n", " ");
	for(int i = 1; i < 21; i++)
	{
		printf("%2d %5s | %7d | %4.2f\n", i, stateStrings[i], stateTotals[i], ((double)stateTotals[i]) / 1000000);
	}
	


	
	if(fp!=NULL) fclose(fp);
	return 0;
}




//outputs retVal.winner -1 if the trail runs into an error
struct tennis_responseVariable tennis_runTrial(int verbose)
{
	struct tennis_responseVariable retVal = {0, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	
	static double p = 0.55;
	
	int state = 1;
	
	while(retVal.winner == 0)
	{
		double roll = ((double)rand()) / ((double) RAND_MAX);
		retVal.stateCounts[state]++;
		switch(state)
		{
		case 1:
			//(0,0)
			if(roll <= p) state+=1;
			else state+=4;
			break;
		case 2:
			//(15,0)
			if(roll <= p) state+=1;
			else state+=4;
			break;
		case 3:
			//(30,0)
			if(roll <= p) state+=1;
			else state+=4;
			break;
		case 4:
			//(40,0)
			if(roll <= p) state=19;
			else state+=4;
			break;
		case 5:
			//(15,0)
			if(roll <= p) state+=1;
			else state+=4;
			break;
		case 6:
			//(15,15)
			if(roll <= p) state+=1;
			else state+=4;
			break;
		case 7:
			//(30,15)
			if(roll <= p) state+=1;
			else state+=4;
			break;
		case 8:
			//(40,15)
			if(roll <= p) state=19;
			else state+=4;
			break;
		case 9:
			//(0,30)
			if(roll <= p) state+=1;
			else state+=4;
			break;
		case 10:
			//(15,30)
			if(roll <= p) state+=1;
			else state+=4;
			break;
		case 11:
			//(30,30)
			if(roll <= p) state+=1;
			else state+=4;
			break;
		case 12:
			//(40,30)
			if(roll <= p) state=19;
			else state+=4;
			break;
		case 13:
			//(40,0)
			if(roll <= p) state+=1;
			else state=20;
			break;
		case 14:
			//(40,15)
			if(roll <= p) state+=1;
			else state=20;
			break;
		case 15:
			//(40,30)
			if(roll <= p) state+=1;
			else state=20;
			break;
		case 16:
			//Deuce
			if(roll <= p) state+=1;
			else state+=2;
			break;
		case 17:
			//Adv A
			if(roll <= p) state=19;
			else state=16;
			break;
		case 18:
			//Adv B
			if(roll <= p) state=16;
			else state=20;
			break;
		case 19:
			//A wins
			retVal.winner = 1;
			break;
		case 20:
			//B wins
			retVal.winner = 2;
			break;
		default:
			break;
		}
		if(retVal.winner == 0) retVal.points++;
	}
	
	
	//CS 582 Varification requirment
	//was state 1 entered only once
	if(retVal.stateCounts[1] != 1)
	{
		fprintf(stderr, "ERROR invalid state 1 count, entered state one %d times\n", retVal.stateCounts[1]);
		retVal.winner = -1;
		return retVal;
	}
	//was states 2-15 entered only 1 or 0 times
	for(int i = 2; i < 16; i++)
	{
		if(retVal.stateCounts[i] != 1 && retVal.stateCounts[i] != 0)
		{
			fprintf(stderr, "ERROR invalid state %d count, entered state %d times\n", i, retVal.stateCounts[i]);
			retVal.winner = -1;
			return retVal;
		}
	}
	//was state 19 or 20 but not both entered 1 time
	if(!(retVal.stateCounts[19] == 1 && retVal.stateCounts[20] == 0) && !(retVal.stateCounts[19] == 0 && retVal.stateCounts[20] == 1))
	{
		fprintf(stderr, "ERROR invalid state 19 or 20 count, state 19: %d | state 20: %d\n", retVal.stateCounts[19], retVal.stateCounts[20]);
		retVal.winner = -1;
		return retVal;
	}
	//was the score at least 4;
	if(retVal.points < 4)
	{
		fprintf(stderr, "ERROR there was less than 4 state changes\n");
		retVal.winner = -1;
		return retVal;
	}
	
	return retVal;
}


