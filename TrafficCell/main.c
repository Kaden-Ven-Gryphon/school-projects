#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



struct roadtraffic_response_s
{
	double meanVelocity;
	double flowRate;
};


struct roadtraffic_response_s roadtraffic_runTrial(double density, double dallyChance, int length, int verbose);


int main()
{
	//var and init rand
	struct roadtraffic_response_s stats;
	double meanmeanVel = 0;
	double meanflow = 0;
	srand((unsigned) time(NULL));
	
	FILE *fp;
	if((fp=freopen("kadengryphonOutput.txt", "w", stdout)) == NULL)
	{
		printf("Cannot open output file\n");
		return(1);
	}
	
	//run scenario 1
	printf("Road Traffic, Scenario = 1, Dally = 0.00, Trials = 100\n");
	for(int i = 0; i < 100; i++)
	{
		stats = roadtraffic_runTrial(0.10, 0.0, 100, 0);
		meanmeanVel += stats.meanVelocity;
		meanflow += stats.flowRate;
		printf("trial= %3d, traffic density= 0.10, mean velocity= %4.2f, flow rate= %4.2f\n", i+1, stats.meanVelocity, stats.flowRate);
		
	}
	//calc response vars
	meanmeanVel /= 100;
	meanflow /= 100;
	printf("mean traffic density= %4.2f\nmean mean velocity  = %4.2f\nmean flow rate      = %4.2f\n\n\n", 0.10, meanmeanVel, meanflow);
	
	//reset means
	meanmeanVel = 0;
	meanflow = 0;
	
	//run scenario 2
	printf("Road Traffic, Scenario = 2, Dally = 0.20 Trials = 100\n");
	for(int i = 0; i < 100; i++)
	{
		if(i == 99) stats = roadtraffic_runTrial(0.20, 0.2, 200, 1);
		else stats = roadtraffic_runTrial(0.20, 0.2, 200, 0);
		meanmeanVel += stats.meanVelocity;
		meanflow += stats.flowRate;
		printf("trial= %3d, traffic density= 0.10, mean velocity= %4.2f, flow rate= %4.2f\n", i+1, stats.meanVelocity, stats.flowRate);
		
	}
	//calc respone vars
	meanmeanVel /= 100;
	meanflow /= 100;
	printf("mean traffic density= %4.2f\nmean mean velocity  = %4.2f\nmean flow rate      = %4.2f\n\n\n", 0.20, meanmeanVel, meanflow);
	
	fclose(fp);
}
//##################################################################### END MAIN

struct roadtraffic_response_s roadtraffic_runTrial(double density, double dallyChance, int length, int verbose)
{
	int velocity[200];
	char state[201];
	int numberOfCars = 200*density;
	int time = 0;
	int maxSpeed = 5;
	//the full history of all states
	char statehist[100000];
	statehist[0] = '\0';
	
	struct roadtraffic_response_s retVal;
	retVal.meanVelocity=0;
	retVal.flowRate=0;
	
	if(numberOfCars > 200) numberOfCars = 200;
	
	
	for(int i = 0; i < 200; i++)
	{
		velocity[i] = 0;
		state[i] = '_';
	}
	//turn state into a string so it can be printed
	state[200] = '\0';
	
	//init
	//place the tracked car
	velocity[0] = rand() % (maxSpeed+1);
	state[0] = '#';
	
	//place the remaining cars
	for(int i = 0; i < numberOfCars-1; i++)
	{
		//atempt to randomly place car upto 10 times
		int y = 0;
		while(y < 10)
		{
			int pos = rand() % 200;
			if(state[pos] == '_')
			{
				state[pos] = '=';
				velocity[pos] = rand() % (maxSpeed+1);
				break;
			}
			y++;
		}
		//if all 10 fail to find free space pick a random space and count till an empty one is found
		int pos = rand() % 200;
		while(y==10)
		{
			if(pos < 200)
			{
				if(state[pos] == '_')
				{
					state[pos] = '=';
					velocity[pos] = rand() % 6;
					break;
				}
				else
				{
					pos++;
				}
			}
			else
			{
				pos = 2;
			}
		}
	}//for... placing cars
	
	
	//print start con
	if(verbose)
	{
		printf("clock = %d numberOfCars = %d\n", time, numberOfCars);
		strcat(statehist, state);
		strcat(statehist, "\n");
		printf("Time: %d\n", time);
		printf("%s\n", statehist);
	}
	
	
	//run for length time steps
	for(time = 1; time <= length; time++)
	{
		char newState[200];
		int newVelocity[200];
		for(int k = 0; k < 200; k++)
		{
			newState[k] = '_';
			newVelocity[k] = 0;
		}
		//update cars
		for(int j = 0; j < 200; j++)
		{
			//find the cars
			if(state[j] != '_')
			{
				//get their vel
				int n = velocity[j];
				
				//accelerate up to maxspeed
				n++;
				if(n > maxSpeed) n = maxSpeed;
				
				//find the number of empty spaces ahead, up to maxSpeed;
				int between = maxSpeed;
				for(int k = maxSpeed; k >= 0; k--)
				{
					//use mod 200 to loop around
					if(state[(j+k+1)%200] != '_') between = k;
				}
				
				//slow down if cars ahead
				if(between < n) n = between;
				
				//check for dally
				double dally = (double)rand() / (double)RAND_MAX;
				if(n > 0 && dally <= dallyChance) n--;
				
				
			
				
				//check is velocity is above zero
				if(n == 0)
				{
					//if it is not the tracked car update it state to X
					if(state[j] != '#')
					{
						newState[j] = 'X';
						newVelocity[j] = '0';
					}
					else
					{
						newState[j] = '#';
						newVelocity[j] = '0';

					}
				}
				else
				{
					//count car if entering or passing space 0
					if(j+n >= 200) retVal.flowRate += 1;
					
					//set cars new pos and state
					//check if it is the tracked car
					if(state[j] == '#')
					{
						newState[(j+n)%200] = '#';
					}
					else
					{
						//chose the right sysbol based on new velocity
						if(n == velocity[j]) newState[(j+n)%200] = '=';
						else if (n > velocity[j]) newState[(j+n)%200] = '>';
						else newState[(j+n)%200] = '<';
					}
				}
				//set the new velocity and tally the mean
				newVelocity[(j+n)%200] = n;
				retVal.meanVelocity += n;
				
			}
		}//update cars
		
		for(int k = 0; k < 200; k++)
		{
			state[k] = newState[k];
			velocity[k] = newVelocity[k];
		}
		
		
		//print state
		if(verbose)
		{
			//append the current state to the history of states and print it
			strcat(statehist, state);
			strcat(statehist, "\n");
			printf("Time: %d\n", time);
			printf("%s\n", statehist);
		}
		
		
	}//loop time steps
	
	
	retVal.meanVelocity /= (length*numberOfCars);
	retVal.flowRate /= length;
	
	return retVal;
}
