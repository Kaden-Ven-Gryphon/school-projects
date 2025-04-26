/*
*
*	Kaden Gryphon
*	Program 3 Wolfs and Sheep
*	03.06.2023
*
*
*/

//My image generation code only works of unix machines as it uses Fork-Exicute, uncomment line bellow if testing on a windows machine.
//#define WINDOWS_F


//#define ANIMATION_F

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "binpypng.h"
#include "binimg.h"
#include "fonts.h"

struct wolfsheep_response_s
{
	int wolfMin;
	int wolfMax;
	double wolfMean;
	int sheepMin;
	int sheepMax;
	double sheepMean;
	int grassMin;
	int grassMax;
	double grassMean;
};


struct wolfsheep_response_s wolfsheep_runTrial(int verbose);



int main()
{
	printf("Start main\n");
	srand((unsigned) time(NULL));
	
	struct wolfsheep_response_s totals = {0,0,0,0,0,0,0,0,0};
	struct wolfsheep_response_s buf = {0,0,0,0,0,0,0,0,0};
	
	
	//open output file / redirect printf
	FILE *fp;
	if((fp=freopen("kadengryphonOutput.txt", "w", stdout)) == NULL)
	{
		printf("Cannot open output file\n");
		return(1);
	}
	
	//run once creating all 2000 img for animation
	//#ifdef ANIMATION_F
	//wolfsheep_runTrial(4);
	//#endif
	
	
	
	totals = wolfsheep_runTrial(3);
	printf("%7s || %7s | %7s | %7s | %7s ||", "Trials", "Wolf", "Min", "Max", "Mean");
	printf(" %7s | %7s | %7s | %7s ||", "Sheep", "Min", "Max", "Mean");
	printf(" %7s | %7s | %7s | %7s ||\n", "Grass", "Min", "Max", "Mean");
	printf("%7d || %7s | %7d | %7d | %7.2f ||", 1, " ", totals.wolfMin, totals.wolfMax, totals.wolfMean);
	printf(" %7s | %7d | %7d | %7.2f ||", " ", totals.sheepMin, totals.sheepMax, totals.sheepMean);
	printf(" %7s | %7d | %7d | %7.2f ||\n", " ", totals.grassMin, totals.grassMax, totals.grassMean);
	
	
	for(int i = 2; i <= 30; i++)
	{
		buf = wolfsheep_runTrial(0);
		if(totals.wolfMin > buf.wolfMin) totals.wolfMin = buf.wolfMin;
		if(totals.wolfMax < buf.wolfMax) totals.wolfMax = buf.wolfMax;
		totals.wolfMean += buf.wolfMean;
		if(totals.sheepMin > buf.sheepMin) totals.sheepMin = buf.sheepMin;
		if(totals.sheepMax < buf.sheepMax) totals.sheepMax = buf.sheepMax;
		totals.sheepMean += buf.sheepMean;
		if(totals.grassMin > buf.grassMin) totals.grassMin = buf.grassMin;
		if(totals.grassMax < buf.grassMax) totals.grassMax = buf.grassMax;
		totals.grassMean += buf.grassMean;
		printf("%7d || %7s | %7d | %7d | %7.2f ||", i, " ", buf.wolfMin, buf.wolfMax, buf.wolfMean);
		printf(" %7s | %7d | %7d | %7.2f ||", " ", buf.sheepMin, buf.sheepMax, buf.sheepMean);
		printf(" %7s | %7d | %7d | %7.2f ||\n", " ", buf.grassMin, buf.grassMax, buf.grassMean);
	}
	
	totals.wolfMean /= 30;
	totals.sheepMean /= 30;
	totals.grassMean /= 30;
	
	
	
	printf("\n\n");
	printf("Wolf min min \t\t%7d\n", totals.wolfMin);
	printf("Wolf max max \t\t%7d\n", totals.wolfMax);
	printf("Wolf mean mean \t\t%7.2f\n", totals.wolfMean);
	printf("Sheep min min \t\t%7d\n", totals.sheepMin);
	printf("Sheep max max \t\t%7d\n", totals.sheepMax);
	printf("Sheep mean mean \t%7.2f\n", totals.sheepMean);
	printf("Grass min min \t\t%7d\n", totals.grassMin);
	printf("Grass max max \t\t%7d\n", totals.grassMax);
	printf("Grass mean mean \t%7.2f\n", totals.grassMean);
	
	if(fp!=NULL) fclose(fp);
	return 0;
}



struct wolfsheep_response_s wolfsheep_runTrial(int verbose)
{
	//Execution parameters
	static int endTime = 2000;
	static int cellsX = 50;
	static int cellsY = 50;
	
	static int startWolves = 50;
	static int startSheep = 100;
	static double grassProb = 0.5;
	static int startWolfEnergy = 40;
	static int startSheepEnergy = 8;
	static int moveCost = 1;
	static int wolfEatGain = 20;
	static int sheepEatGain = 4;
	static double wolfReproProb = 0.05;
	static double sheepReproProb = 0.04;
	static int grassRegrowTime = 30;
	
	
	//it took a little bit to figure out how the direction and turnning work from
	//the R code, but I think I got it.  The agent has a 0.2 chance to turn CCW
	//a 0.6 chance to continue straight, and a 0.2 chance to turn CW
	static int turnCCW[] = {0, 8, 1, 2, 3, 4, 5, 6, 7};
	static int turnCW[] = {0, 2, 3, 4, 5, 6, 7, 8, 1};
	static int moveDX[] = {0, 0, 1, 1, 1, 0, -1, -1, -1};
	static int moveDY[] = {0, -1, -1, 0, 1, 1, 1, 0, -1};
	
	
	
	
	//the return val for response var
	struct wolfsheep_response_s retVal;
	retVal.wolfMin = startWolves;
	retVal.wolfMax = startWolves;
	retVal.wolfMean = 0;
	retVal.sheepMin = startSheep;
	retVal.sheepMax = startSheep;
	retVal.sheepMean = 0;
	retVal.grassMin = cellsX*cellsY;
	retVal.grassMax = 0;
	retVal.grassMean = 0;
	
	//hard cap for number of sheep or wolf, cause I do not want to make lists
	//or dynamic arrays
	static int agentMax = 1024;
	
	int time = 0;
	
	
	//wolf val
	int numWolf = 0;
	int wolfX[agentMax];
	int wolfY[agentMax];
	int wolfD[agentMax];
	int wolfE[agentMax];
	
	//sheep val
	int numSheep = 0;
	int sheepX[agentMax];
	int sheepY[agentMax];
	int sheepD[agentMax];
	int sheepE[agentMax];
	
	//grass val
	int numGrass = 0;
	int grass[cellsX+1][cellsY];
	
	
	char textImgState[cellsX][cellsY];
	
	//init the grass
	for(int i = 0; i < cellsX; i++)
	{
		for(int j = 0; j < cellsY; j++)
		{
			//roll chance of tile being grass
			double chance = ((double) rand()) / ((double) RAND_MAX);
			if(chance < grassProb)
			{
				grass[i][j] = 0;
				textImgState[i][j] = '=';
				numGrass++;	
			}
			//if it is dirt set regrow time to random [1,regrowTime]
			else
			{
				grass[i][j] = (rand() % grassRegrowTime) + 1;
				textImgState[i][j] = '-';
			}
		}
	}
	retVal.grassMin = numGrass;
	retVal.grassMax = numGrass;
	
	//init the wolves
	while(numWolf < startWolves)
	{
		int ranX = rand() % cellsX;
		int ranY = rand() % cellsY;
		

		
		if(textImgState[ranX][ranY] != 'W' && textImgState[ranX][ranY] != 'S')
		{
			//printf("place wolf at %d, %d\n", ranX, ranY);
			textImgState[ranX][ranY] = 'W';
			wolfX[numWolf] = ranX;
			wolfY[numWolf] = ranY;
			wolfD[numWolf] = (rand() % 8) + 1;
			wolfE[numWolf] = (rand() % startWolfEnergy) + 1;
			numWolf++;
		}
	}
	
	//init the sheep
	while(numSheep < startSheep)
	{
		int ranX = rand() % cellsX;
		int ranY = rand() % cellsY;
		
		if(textImgState[ranX][ranY] != 'W' && textImgState[ranX][ranY] != 'S')
		{
			textImgState[ranX][ranY] = 'S';
			sheepX[numSheep] = ranX;
			sheepY[numSheep] = ranY;
			sheepD[numSheep] = (rand() % 8) + 1;
			sheepE[numSheep] = (rand() % startSheepEnergy) + 1;
			numSheep++;
		}
	}

	
	
	
	//open a file to write the current num of agents to export to excl
	/*
	FILE *outfp;
	if(verbose == 4)
	{
		outfp = fopen("populationExcel.csv", "w");
		if(outfp == NULL)
		{
			printf("ERROR can not open output file\n");
			retVal.wolfMin = -1;
			return retVal;
		}
	}
	*/
	
	while(1)
	{
		//draw the state if verbose
		
		//this is to output images only works on UNIX machines
		//only draws the top left 50,50 cells, which is the whole map for
		//this hw, but I dont feel like making this part dynamic
		#ifndef WINDOWS_F
		if(((verbose == 3 && time <= 10) || verbose == 4) && (cellsX >= 50 || cellsY >= 50))
		{
			char imgFileName[32];
			if(verbose == 3)
			{
				//create the img file name
				sprintf(imgFileName, "frameT%04d.bin", time);
			}
			else
			{
				//create the img file name
				
				sprintf(imgFileName, "frames/frameT%04d.bin", time);
			}
			
			struct binimg_image_s stateImg;
			binimg_initImage(1080, 1080, &stateImg);
			struct pixel_s black = {0,0,0,255};
			struct pixel_s white = {255,255,255,255};
			struct pixel_s brown = {153,76,0,255};
			struct pixel_s green = {76,150,0,255};
			struct pixel_s red = {255,0,0,255};
			
			//draw base
			binimg_drawRectangle(60,60,960,960,1,black, &stateImg);
			
			//draw grass and dirt
			for(int i = 0; i < 50; i++)
			{
				for(int j = 0; j < 50; j++)
				{
					//if it is grass
					if(grass[i][j] == 0)
					{
						binimg_drawRectangle( (i*19)+65, (j*19)+65, 17, 17, 1, green, &stateImg);
					}
					//else it is dirt
					else
					{
						binimg_drawRectangle( (i*19)+65, (j*19)+65, 17, 17, 1, brown, &stateImg);
					}
				}
			}
			
			//draw wolves
			for(int i = 0; i < numWolf; i++)
			{
				binimg_drawCircle(wolfX[i]*19+65+8,wolfY[i]*19+65+8,6,1,black, &stateImg);
				if(wolfE[i] == 1) binimg_drawCircle(wolfX[i]*19+65+8,wolfY[i]*19+65+8,2,1,red, &stateImg);
			}
			
			//draw sheep
			for(int i = 0; i < numSheep; i++)
			{
				binimg_drawCircle(sheepX[i]*19+65+8,sheepY[i]*19+65+8,6,1,white, &stateImg);
				if(sheepE[i] == 1) binimg_drawCircle(sheepX[i]*19+65+8,sheepY[i]*19+65+8,2,1,red, &stateImg);
			}
			
			
			//draw title text
			char titleTxt[] = "WOLF SHEEP PREDATION\0";
			binimg_drawBitmapText(60, 20, black, titleTxt, pixLetters, 3, &stateImg);
			
			//draw info text
			char infoTxt[128];
			sprintf(infoTxt, "CLOCK= %4d WOLVES= %4d SHEEP= %4d GRASS= %4d DIRT= %4d", time, numWolf, numSheep, numGrass, (cellsX*cellsY)-numGrass);
			binimg_drawBitmapText(60, 1040, black, infoTxt, pixLetters, 2, &stateImg);
			
			binpypng_writebin(imgFileName, &stateImg);
			binpypng_convert(imgFileName);
		}
		#endif
		
		//create the text img output
		if(verbose == 3 && time <= 10)
		{
			//place grass
			for(int j = 0; j < cellsX; j++)
			{
				for(int i = 0; i < cellsY; i++)
				{
					if(grass[i][j] == 0)
					{
						textImgState[i][j] = '=';
					}
					else
					{
						textImgState[i][j] = '-';
					}
				}
				textImgState[cellsX][j] = '\0';
			}
			//place sheep
			for(int i = 0; i < numSheep; i++)
			{
				textImgState[sheepX[i]][sheepY[i]] = 'S';
			}
			//place wolves
			for(int i = 0; i < numWolf; i++)
			{
				textImgState[wolfX[i]][wolfY[i]] = 'W';
			}
			
			//write to screen
			printf("Time %5d\n", time);
			for(int i = 0; i < cellsY; i++)
			{
				for(int j = 0; j < cellsX; j++)
				{
					printf("%c", textImgState[j][i]);
				}
				printf("\n");
			}
			printf("\n\n---------------------------------------------------------------------\n\n");
		}
		
		
		//write population to output file
		/*
		if(verbose == 4)
		{
			fprintf(outfp, "%d,%d,%d\n", numWolf, numSheep, numGrass);
		}
		*/
		
		
		retVal.wolfMean += numWolf;
		retVal.sheepMean += numSheep;
		retVal.grassMean += numGrass;
		if(retVal.wolfMax < numWolf) retVal.wolfMax = numWolf;
		if(retVal.wolfMin > numWolf) retVal.wolfMin = numWolf;
		if(retVal.sheepMax < numSheep) retVal.sheepMax = numSheep;
		if(retVal.sheepMin > numSheep) retVal.sheepMin = numSheep;
		if(retVal.grassMax < numGrass) retVal.grassMax = numGrass;
		if(retVal.grassMin > numGrass) retVal.grassMin = numGrass;
		
		
		
		//update the time and check if its the end
		time++;
		if(time>endTime) break;
		
		
		
		//update agents
		
		
		//grass regrows
		for(int i = 0; i < cellsX; i++)
		{
			for(int j = 0; j < cellsY; j++)
			{
				if(grass[i][j] > 0)
				{
					grass[i][j]--;
					if(grass[i][j] == 0) numGrass++;
				}
			}
		}
		
		
		//move wolves
		for(int i = 0; i < numWolf; i++)
		{
			//roll chance to change direction
			double changeChance = ((double) rand()) /((double) RAND_MAX);
			if(changeChance <= 0.2) wolfD[i] = turnCW[wolfD[i]];
			else if(changeChance <= 0.4) wolfD[i] = turnCCW[wolfD[i]];
			wolfX[i]+=moveDX[wolfD[i]];
			wolfY[i]+=moveDY[wolfD[i]];
			if(wolfX[i] < 0) wolfX[i] = cellsX-1;
			if(wolfX[i] >= cellsX) wolfX[i] = 0;
			if(wolfY[i] < 0) wolfY[i] = cellsY-1;
			if(wolfY[i] >= cellsY) wolfY[i] = 0;
			wolfE[i] -= moveCost;
		}
		
		
		//move sheep
		for(int i = 0; i < numSheep; i++)
		{
			//roll chance to change direction
			double changeChance = ((double) rand()) /((double) RAND_MAX);
			if(changeChance <= 0.2) sheepD[i] = turnCW[sheepD[i]];
			else if(changeChance <= 0.4) sheepD[i] = turnCCW[sheepD[i]];
			sheepX[i]+=moveDX[sheepD[i]];
			sheepY[i]+=moveDY[sheepD[i]];
			if(sheepX[i] < 0) sheepX[i] = cellsX-1;
			if(sheepX[i] >= cellsX) sheepX[i] = 0;
			if(sheepY[i] < 0) sheepY[i] = cellsY-1;
			if(sheepY[i] >= cellsY) sheepY[i] = 0;
			sheepE[i] -= moveCost;
		}
		
		
		
		//Wolves eat sheep and sheep eat grass
		for(int i = 0; i < cellsX; i++)
		{
			for(int j = 0; j < cellsY; j++)
			{
				//for each tile count the number of sheep and wolfs on it
				int countWolf = 0;
				int countSheep = 0;
				for(int w = 0; w < numWolf; w++)
				{
					if(wolfX[w] == i && wolfY[w] == j) countWolf++;
				}
				for(int s = 0; s < numSheep; s++)
				{
					if(sheepX[s] == i && sheepY[s] == j) countSheep++;
				}
				
				//while there is at least one sheep and one wolf left
				while(countWolf > 0 && countSheep > 0)
				{
					//randomly pick a wolf and sheep from that tile
					int pickWolf = (rand() % countWolf) + 1;
					int pickSheep = (rand() % countSheep) + 1;
					
					
					//find that wolf and give energy
					for(int w = 0; w < numWolf; w++)
					{
						//if it is the pickWolf 'th wolf give it the energy from eating sheep
						if(wolfX[w] == i && wolfY[w] == j) pickWolf--;
						if(pickWolf == 0) { wolfE[w] += wolfEatGain; break; }
					}
					
					//find that sheep and remove it
					for(int s = 0; s < numSheep; s++)
					{
						//find the picked sheep
						if(sheepX[s] == i && sheepY[s] == j) pickSheep--;
						if(pickSheep == 0)
						{
							//remove it from array
							for(int k = s; k < numSheep - 1; k++)
							{
								sheepX[k] = sheepX[k+1];
								sheepY[k] = sheepY[k+1];
								sheepE[k] = sheepE[k+1];
								sheepD[k] = sheepD[k+1];
							}
							numSheep--;
							countSheep--;
							break;
						}
					}
				}//while there are sheep and wolves left
				
				
				//if there are sheep left in the cell pick one to eat the grass
				if(countSheep > 0 && grass[i][j] == 0)
				{
					//randomly pick one of the sheep in the tile
					int pickSheep = (rand() % countSheep) + 1;
					
					//find that sheep and feed it
					for(int s = 0; s < numSheep; s++)
					{
						//find the picked sheep
						if(sheepX[s] == i && sheepY[s] == j) pickSheep--;
						if(pickSheep == 0) { sheepE[s] += sheepEatGain; break; }
					}
					
					//set grass regrow time
					grass[i][j] = grassRegrowTime;
					numGrass--;
				}
			}
		}//checking each tile for wolfs and sheep
		
		
		//do any agents reproduce
		for(int w = 0; w < numWolf; w++)
		{
			double chance = ((double) rand()) / ((double) RAND_MAX);
			if(chance <= wolfReproProb && numWolf < agentMax)
			{
				wolfX[numWolf] = wolfX[w];
				wolfY[numWolf] = wolfY[w];
				wolfD[numWolf] = (rand() % 8) + 1;
				wolfE[numWolf] = wolfE[w] / 2;
				wolfE[w] /= 2;
				numWolf++;
			}
		}
		
		for(int s = 0; s < numSheep; s++)
		{
			double chance = ((double) rand()) / ((double) RAND_MAX);
			if(chance <= sheepReproProb && numWolf < agentMax)
			{
				sheepX[numSheep] = sheepX[s];
				sheepY[numSheep] = sheepY[s];
				sheepD[numSheep] = (rand() % 8) + 1;
				sheepE[numSheep] = sheepE[s] / 2;
				sheepE[s] /= 2;
				numSheep++;
			}
		}
		
		//do any agents starve
		for(int w = 0; w < numWolf; w++)
		{
			if(wolfE[w] <= 0)
			{
				for(int i = w; i < numWolf-1; i++)
				{
					wolfX[i] = wolfX[i+1];
					wolfY[i] = wolfY[i+1];
					wolfE[i] = wolfE[i+1];
					wolfD[i] = wolfD[i+1];
				}
				numWolf--;
			}
		}
		for(int s = 0; s < numSheep; s++)
		{
			if(sheepE[s] <= 0)
			{
				for(int i = s; i < numSheep-1; i++)
				{
					sheepX[i] = sheepX[i+1];
					sheepY[i] = sheepY[i+1];
					sheepE[i] = sheepE[i+1];
					sheepD[i] = sheepD[i+1];
				}
				numSheep--;
			}
		}
	
	}//until time runs out
	
	//cal means
	retVal.wolfMean /= endTime;
	retVal.sheepMean /= endTime;
	retVal.grassMean /= endTime;
	
	//close file
	//if(outfp != NULL) fclose(outfp);
	
	return retVal;
}
