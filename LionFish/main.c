//Kaden Gryphon
//Program 2
//02.22.2023
//CS 582-01

//This takes about 20 to run, change QUICK to 0 to speed it up to about 5 min
//but this will make hill climb give a mean of 50 instead of 60


#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define QUICK 0





int swaps = 1000000;





int lionfish_runTrial(double x, double y, double density, double swimSpeed, double spearSpeed, double diveLength, int method, int verbose);
double findDistance(double x1, double y1, double x2, double y2);


static int timesImproved = 0;


int main()
{
	if(QUICK) swaps = 100000;
	double ttablevalue = 2.045;
	srand((unsigned) time(NULL));
	double meanGreedy = 0;
	double trials[30];
	double sdGreedy = 0;
	double meanHill = 0;
	double sdHill = 0;
	double meanGreedyHill = 0;
	double sdGreedyHill = 0;
	
	
	FILE *fp;
	if((fp=freopen("kadengryphonOutput.txt", "w", stdout)) == NULL)
	{
		printf("Cannot open output file\n");
		return(1);
	}
	
	//run 30 greedy trails
	printf("Greedy Trials\n");
	for(int i = 0; i < 30; i++)
	{
		int buf = lionfish_runTrial(150,150,0.0121,0.5,20,60*40,0,0);
		printf("trial= %2d lionfish culled= %3d\n", i+1, buf);
		meanGreedy += buf;
		trials[i] = buf;
	}
	//calc greedy stats
	meanGreedy /= 30;
	for(int i = 0; i < 30; i++)
	{
		sdGreedy += pow(trials[i] - meanGreedy, 2);
	}
	sdGreedy /= 30;
	sdGreedy = sqrt(sdGreedy);
	
	//run 30 hill climb trails
	printf("Hill Climb Trials\n");
	for(int i = 0; i < 30; i++)
	{
		int buf = lionfish_runTrial(150,150,0.0121,0.5,20,60*40,2,0);
		printf("trial= %2d lionfish culled= %3d\n", i+1, buf);
		meanHill += buf;
		trials[i] = buf;
	}
	//calc hill stats
	meanHill /= 30;
	for(int i = 0; i < 30; i++)
	{
		sdHill += pow(trials[i] - meanHill, 2);
	}
	sdHill /= 30;
	sdHill = sqrt(sdHill);
	
	
	//run 30 greedy + hill climb trails
	printf("Greedy Hill Climb Trials\n");
	for(int i = 0; i < 30; i++)
	{
		int buf = lionfish_runTrial(150,150,0.0121,0.5,20,60*40,1,0);
		printf("trial= %2d lionfish culled= %3d\n", i+1, buf);
		meanGreedyHill += buf;
		trials[i] = buf;
	}
	//calc greedy + hill stats
	meanGreedyHill /= 30;
	for(int i = 0; i < 30; i++)
	{
		sdGreedyHill += pow(trials[i] - meanGreedyHill, 2);
	}
	sdGreedyHill /= 30;
	sdGreedyHill = sqrt(sdGreedyHill);
	
	
	printf("greedy culled mean=\t\t\t\t %8.2f\n", meanGreedy);
	printf("greedy culled sd=  \t\t\t\t %8.2f\n", sdGreedy);
	printf("greedy culled ci lower=\t\t\t %8.2f\n", meanGreedy-(ttablevalue * sdGreedy / sqrt(30)));
	printf("greedy culled ci upper=\t\t\t %8.2f\n\n", meanGreedy+(ttablevalue * sdGreedy / sqrt(30)));
	printf("hill climb culled mean=\t\t\t %8.2f\n", meanHill);
	printf("hill climb culled sd=  \t\t\t %8.2f\n", sdHill);
	printf("hill climb culled ci lower=\t\t\t %8.2f\n", meanHill-(ttablevalue * sdHill / sqrt(30)));
	printf("hill climb culled ci upper=\t\t\t %8.2f\n\n", meanHill+(ttablevalue * sdHill / sqrt(30)));
	printf("greedy hill climb culled mean=\t\t %8.2f\n", meanGreedyHill);
	printf("greedy hill climb culled sd=  \t\t %8.2f\n", sdGreedyHill);
	printf("greedy hill climb culled ci lower=\t\t %8.2f\n", meanGreedyHill-(ttablevalue * sdGreedyHill / sqrt(30)));
	printf("greedy hill climb culled ci upper=\t\t %8.2f\n", meanGreedyHill+(ttablevalue * sdGreedyHill / sqrt(30)));
	printf("Times Greedy hill climb improved greedy= \t %8d\n", timesImproved);
	
	fclose(fp);
	return 0;
}



int lionfish_runTrial(double x, double y, double density, double swimSpeed, double spearSpeed, double diveLength, int method, int verbose)
{
	int totalFish = (x * y) * density;		//area divieded by density = number of fish
	int culledFish = 0;
	double fishX[totalFish];			//parallel arrays for the live and culled fish
	double fishY[totalFish];
	double fishcopyX[totalFish];			//copy of the fish to manipulate
	double fishcopyY[totalFish];
	double diverX = x/2;				//diver starts at the center
	double diverY = y/2;
	double time = 0;
	
	double greedyOrderX[totalFish];		//the greedy order of culling
	double greedyOrderY[totalFish];
	double hillfishX[totalFish];			//the order of hill climb fish
	double hillfishY[totalFish];
	
	int bestbestScore = 0;
	
	if(verbose)
	{
		printf("START TRIAL\n");
		printf("\ttotal fish= %3d\n", totalFish);
	}
	
	
	//place fish
	for(int i = 0; i < totalFish; i++)
	{
		fishX[i] = ((double)rand() / (double)RAND_MAX) * x;
		fishY[i] = ((double)rand() / (double)RAND_MAX) * y;
		fishcopyX[i] = fishX[i];
		fishcopyY[i] = fishY[i];
		if(verbose)
		{
			printf("\tfish %3d: %8.2f, %8.2f\n", i, fishX[i], fishY[i]);
		}
	}
	
	//get greedy order of fish
	for(int i = 0; i < totalFish; i++)
	{
		//find the next nearest fish
		double minD = x*y;
		int minIndex = 0;
		for(int j = 0; j < totalFish; j++)
		{
			if(fishcopyX[j] != -1)
			{
				double d = findDistance(diverX, diverY, fishcopyX[j], fishcopyY[j]);
				if(d < minD)
				{
					minD = d;
					minIndex = j;
				}	
			}
		}
		if(verbose)
		{
			printf("\tgreedy fish %3d: %8.2f, %8.2f\n", i, fishcopyX[minIndex], fishcopyY[minIndex]);
		}
		//add the fish to the greedy order and remove it from the copy
		diverX = fishcopyX[minIndex];
		diverY = fishcopyY[minIndex];
		greedyOrderX[i] = fishcopyX[minIndex];
		greedyOrderY[i] = fishcopyY[minIndex];
		fishcopyX[minIndex] = -1;
	}
	diverX = x/2;
	diverY = y/2;

	//pick method of choosing next fish
	switch(method)
	{
	//greedy method#########################
	default:
	case 0:;
		if(verbose) printf("method greedy\n");
		
		//cull the fish in the greedy order untill time runs out
		
		while(time < diveLength && culledFish < totalFish)
		{
			double d = findDistance(diverX, diverY, greedyOrderX[culledFish], greedyOrderY[culledFish]);
			if(time + spearSpeed + (d/swimSpeed) < diveLength)
			{
				if(verbose) printf("culled fish %d at: %10.2f, %10.2f distance: %10.2f\n", culledFish+1, greedyOrderX[culledFish], greedyOrderY[culledFish], d);
				diverX = greedyOrderX[culledFish];
				diverY = greedyOrderY[culledFish];
				culledFish++;
				time += spearSpeed + (d/swimSpeed);
			}
			else break;
		}
	break;
	
	
	//hill climbing############################
	//I atempted to implment a hill climbing method of selecting fish
	//starting with a random order the program makes random swaps and
	//checks if the swap creates a better score.  swaps that create a 
	//better score are kept, swaps that create a worse score are reversed
	//
	//
	//The first scoreing method I used was based of the total distance
	//needed to travel to cull all fish,  this was not very effective
	//I try weighing the score so that the fish that where culled first
	//where more important but gave only minor improments
	//
	//the current scoreing method is based around just running the sim
	//and counting how many fish could be culled if done in that order
	//
	//This still does out preform the greedy method, I also tryed starting
	//from the greedy order, and hill climb from there, this sometimes did
	//better but only 1 or 2 fish better once every 20-40 trials
	//
	//The final verion of this program runs both hill climb methods, one
	//starting with a random order and one starting with the greedy order
	
	case 1:;

		bestbestScore = 0;
		diverX = x/2;
		diverY = y/2;
		time = 0;
		int greedyScore = 0;
		while(time < diveLength && greedyScore < totalFish)
		{
			double d = findDistance(diverX, diverY, greedyOrderX[greedyScore], greedyOrderY[greedyScore]);
			if(time + spearSpeed + (d/swimSpeed) < diveLength)
			{
				diverX = greedyOrderX[greedyScore];
				diverY = greedyOrderY[greedyScore];
				greedyScore++;
				time += spearSpeed + (d/swimSpeed);
			}
			else break;
		}
		if(verbose) printf("greedy score: %d\n", greedyScore);
		//give 10 attempts
		for(int k = 0; k < 10; k++)
		{
			//make copy
			for(int j = 0; j < totalFish; j++)
			{
				fishcopyX[j] = greedyOrderX[j];
				fishcopyY[j] = greedyOrderY[j];
			}
			double newfishX[totalFish];			//varibles to store the current
			double newfishY[totalFish];			//attemp's best order
			int bestScore = greedyScore;
			
			//try 100k swaps
			for(int i = 0; i < 100000; i++)
			{
				//make copy
				for(int j = 0; j < totalFish; j++)
				{
					newfishX[j] = fishcopyX[j];
					newfishY[j] = fishcopyY[j];
				}
				
				//make swap
				int a = rand() % totalFish;
				int b = rand() % totalFish;
				double tempX = newfishX[a];
				double tempY = newfishY[a];
				newfishX[a] = newfishX[b];
				newfishY[a] = newfishY[b];
				newfishX[b] = tempX;
				newfishY[b] = tempY;
				
				
		
				//calc score
				diverX = x/2;
				diverY = y/2;
				time = 0;
				int newScore = 0;
				while(time < diveLength && newScore < totalFish)
				{
					double d = findDistance(diverX, diverY, newfishX[newScore], newfishY[newScore]);
					if(time + spearSpeed + (d/swimSpeed) < diveLength)
					{
						//if(verbose) printf("culled fish %d at: %10.2f, %10.2f distance: %10.2f\n", culledFish+1, newfishX[newScore], newfishY[newScore], d);
						diverX = newfishX[newScore];
						diverY = newfishY[newScore];
						newScore++;
						time += spearSpeed + (d/swimSpeed);
					}
					else break;
				}
				
				
				//compare scores
				if(newScore >= bestScore)
				{
					bestScore = newScore;
					fishcopyX[a] = newfishX[a];
					fishcopyY[a] = newfishY[a];
					fishcopyX[b] = newfishX[b];
					fishcopyY[b] = newfishY[b];
				}
				
				
			}//swaps
			
			//compare to overall best
			if(bestScore > bestbestScore)			//if this attempts score was better
			{						//than attempts so far record a copy
				bestbestScore = bestScore;
				for(int j = 0; j < totalFish; j++)
				{
					hillfishX[j] = fishcopyX[j];
					hillfishY[j] = fishcopyY[j];
				}
			}
			if(verbose) printf("new best score: %3d\n", bestbestScore);
			
		}//climb attmpts
		
		if(verbose) printf("Best Score:   %3d\n", bestbestScore);
		if(greedyScore < bestbestScore) timesImproved++;
		
		//cull fish in order given by hill climb
		diverX = x/2;
		diverY = y/2;
		time = 0;
		while(time < diveLength && culledFish < totalFish)
		{
			double d = findDistance(diverX, diverY, hillfishX[culledFish], hillfishY[culledFish]);
			if(time + spearSpeed + (d/swimSpeed) < diveLength)
			{
				if(verbose) printf("culled fish %d at: %10.2f, %10.2f distance: %10.2f\n", culledFish+1, hillfishX[culledFish], hillfishY[culledFish], d);
				diverX = hillfishX[culledFish];
				diverY = hillfishY[culledFish];
				culledFish++;
				time += spearSpeed + (d/swimSpeed);
			}
			else break;
		}
		
	break;
	
	case 2:;
		bestbestScore = 0;
		//give 10 attempts
		for(int k = 0; k < 10; k++)
		{
			//make copy
			for(int j = 0; j < totalFish; j++)
			{
				fishcopyX[j] = fishX[j];
				fishcopyY[j] = fishY[j];
			}
			double newfishX[totalFish];			//varibles to store the current
			double newfishY[totalFish];			//attemp's best order
			int bestScore = 0;
			
			//try 100k swaps
			for(int i = 0; i < swaps; i++)
			{
				//make copy
				for(int j = 0; j < totalFish; j++)
				{
					newfishX[j] = fishcopyX[j];
					newfishY[j] = fishcopyY[j];
				}
				
				//make swap
				int a = rand() % totalFish;
				int b = rand() % totalFish;
				double tempX = newfishX[a];
				double tempY = newfishY[a];
				newfishX[a] = newfishX[b];
				newfishY[a] = newfishY[b];
				newfishX[b] = tempX;
				newfishY[b] = tempY;
				
				
		
				//calc score
				diverX = x/2;
				diverY = y/2;
				time = 0;
				int newScore = 0;
				while(time < diveLength && newScore < totalFish)
				{
					double d = findDistance(diverX, diverY, newfishX[newScore], newfishY[newScore]);
					if(time + spearSpeed + (d/swimSpeed) < diveLength)
					{
						//if(verbose) printf("culled fish %d at: %10.2f, %10.2f distance: %10.2f\n", culledFish+1, newfishX[newScore], newfishY[newScore], d);
						diverX = newfishX[newScore];
						diverY = newfishY[newScore];
						newScore++;
						time += spearSpeed + (d/swimSpeed);
					}
					else break;
				}
				
				
				//compare scores
				if(newScore >= bestScore)
				{
					bestScore = newScore;
					fishcopyX[a] = newfishX[a];
					fishcopyY[a] = newfishY[a];
					fishcopyX[b] = newfishX[b];
					fishcopyY[b] = newfishY[b];
				}
				
				
			}//swaps
			
			//compare to overall best
			if(bestScore > bestbestScore)			//if this attempts score was better
			{						//than attempts so far record a copy
				bestbestScore = bestScore;
				for(int j = 0; j < totalFish; j++)
				{
					hillfishX[j] = fishcopyX[j];
					hillfishY[j] = fishcopyY[j];
				}
			}
			if(verbose) printf("new best score: %3d\n", bestbestScore);
			
		}//climb attmpts
		
		if(verbose) printf("Best Score:   %3d\n", bestbestScore);
		
		//cull fish in order given by hill climb
		diverX = x/2;
		diverY = y/2;
		time = 0;
		while(time < diveLength && culledFish < totalFish)
		{
			double d = findDistance(diverX, diverY, hillfishX[culledFish], hillfishY[culledFish]);
			if(time + spearSpeed + (d/swimSpeed) < diveLength)
			{
				if(verbose) printf("culled fish %d at: %10.2f, %10.2f distance: %10.2f\n", culledFish+1, hillfishX[culledFish], hillfishY[culledFish], d);
				diverX = hillfishX[culledFish];
				diverY = hillfishY[culledFish];
				culledFish++;
				time += spearSpeed + (d/swimSpeed);
			}
			else break;
		}
	break;
	}//switch method
	
	
	
	//ERROR / CHEAT DETECTION
	//sanity check to make sure that the greedy order and hill climb order contain one of
	//each of the original fish
	switch(method)
	{
	default:
	case 0:
		for(int i = 0; i < totalFish; i++)
		{
			int found = 0;
			for(int j = 0; j < totalFish; j++)
			{
				if(fishX[i] == greedyOrderX[j] && fishY[i] == greedyOrderY[j])
				{
					found = 1;
					greedyOrderX[j] = -1;
					break;
				}
			}
			if(verbose && !found) printf("Fish could not be found in greedy order\n");
			if(!found) return -1;
		}
		if(verbose) printf("All fish found in greedy order\n");
		break;
	case 1:
	case 2:
		for(int i = 0; i < totalFish; i++)
		{
			int found = 0;
			for(int j = 0; j < totalFish; j++)
			{
				if(fishX[i] == hillfishX[j] && fishY[i] == hillfishY[j])
				{
					found = 1;
					hillfishX[j] = -1;
					break;
				}
			}
			if(verbose && !found) printf("Fish could not be found in hill order\n");
			if(!found) return -1;
		}
		if(verbose) printf("All fish found in hill order\n");
		break;	
	}
	
	
	
	
	return culledFish;
}


double findDistance(double x1, double y1, double x2, double y2)
{
	return sqrt((double)(pow(x1-x2,2) + pow(y1-y2,2)));
}
