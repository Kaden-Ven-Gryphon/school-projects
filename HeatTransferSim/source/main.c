/****************************************************
*
*	Kaden Gryphon
*	Program 5 Heat Transfer
*	04.05.2023
*	CS 582-01
*
****************************************************/

//The 3 videos are of the 2d implementation
//2d1 is some random spots start at max temp, and the bottom left heatsink gives heat
//2d2 at spot is locked at max temp to heat the plate
//2d3 a random spot is set to max temp every time step to slowly raise the average tempeture (radiation heating a plate?)


//I did the first option for 582 students creating a image plot of the first 10 timesteps + the inital condition


//The image generator only works on UNIX so uncomment the line bellow if testing on windows
//#define WINDOWS_F


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "binimg.c"
#include "binpypng.c"
#include "fonts.h"



int heatTransfer1DTrial(int scenario, int verbose);
int heatTransfer2DTrial();


int main()
{
	FILE *fp;
	if((fp=freopen("kadengryphonOutput.txt", "w", stdout)) == NULL)
	{
		printf("Cannot open output file\n");
		return(1);
	}	
	srand(time(NULL));
	heatTransfer1DTrial(1,1);
	printf("\n");
	heatTransfer1DTrial(4,1);
	//heatTransfer2DTrial();
	fclose(fp);
	return 0;
}





int heatTransfer1DTrial(int scenario, int verbose)
{
	double *metal = NULL;
	double *metalNew = NULL;
	double **history = NULL;
	int n = 0;
	int histFrames = 0;
	double deltaT = 0;
	double deltaX = 0;
	double eta = 0;
	double time = 0;
	int numTimeSteps = 100;


	//set up the inital conditions for the scenario
	switch(scenario)
	{
	case 1:
		n = 9;
		histFrames = 11;
		metal = (double*)malloc((sizeof(double)*(n+2)));
		metalNew = (double*)malloc((sizeof(double)*(n+2)));
		if(metal == NULL) { fprintf(stderr, "ERROR malloc failed to allicate metal array\n"); return -1; }
		if(metalNew == NULL) { fprintf(stderr, "ERROR malloc failed to allicate metal array\n"); return -1; }
		history = (double**)malloc(sizeof(double*)*histFrames);
		if(history == NULL) { fprintf(stderr, "ERROR malloc failed to allicate history array\n"); return -1; }
		for(int i = 0; i < histFrames; i++)
		{
			history[i] = (double*)malloc((sizeof(double)*(n+2)));
			if(history[i] == NULL) { fprintf(stderr, "ERROR malloc failed to allicate history array\n"); return -1; }
		}
		deltaT = 0.25;
		deltaX = 1;
		eta = 0.25;
		for(int i = 0; i < n+2; i++) { metal[i] = 0; }
		metal[5] = 1;
		break;
		
	case 4:
		n = 9;
		histFrames = 11;
		metal = (double*)malloc((sizeof(double)*(n+2)));
		metalNew = (double*)malloc((sizeof(double)*(n+2)));
		if(metal == NULL) { fprintf(stderr, "ERROR malloc failed to allicate metal array\n"); return -1; }
		if(metalNew == NULL) { fprintf(stderr, "ERROR malloc failed to allicate metal array\n"); return -1; }
		history = (double**)malloc(sizeof(double*)*histFrames);
		if(history == NULL) { fprintf(stderr, "ERROR malloc failed to allicate history array\n"); return -1; }
		for(int i = 0; i < histFrames; i++)
		{
			history[i] = (double*)malloc((sizeof(double)*(n+2)));
			if(history[i] == NULL) { fprintf(stderr, "ERROR malloc failed to allicate history array\n"); return -1; }
		}
		deltaT = 0.25;
		deltaX = 1;
		eta = 0.25;
		for(int i = 0; i < n+2; i++) { metal[i] = 0; }
		metal[6] = 1;
		metal[8] = 1;
		break;
	
	default:
		fprintf(stderr, "ERROR unknown scenario\n");
		return -1;
		break;
	}
	
	
	
	for(int j = 1; j <= n; j++)
	{
		history[0][j] = metal[j];
	}
	
	
	//print scenario conditions
	printf("Conductive 1D, scenario= %1d n= %3d deltaT= %5.2f deltaX= %5.2f eta= %5.2f\n", scenario, n, deltaT, deltaX, eta);
	printf("time step= %4d t= %7.2f metal= ", 0, (double)0);
	for(int k = 1; k <= n; k++)
	{
		printf("%7.4f ", metal[k]);
	}
	printf("\n");
	
	//loop for the time steps
	for(int i = 0; i < numTimeSteps; i++)
	{
		//set time step's vars
		time+=deltaT;
		
		
		//loop through the bar
		for(int j = 1; j <= n; j++)
		{
			//formula for 3 point stencil
			metalNew[j] = metal[j] + eta*(metal[j+1] - 2*metal[j] + metal[j-1]);
		}
		for(int j = 1; j <= n; j++)
		{
			metal[j] = metalNew[j];
		}
		
		//print the state to the file
		printf("time step= %4d t= %7.2f metal= ", i+1, time);
		for(int k = 1; k <= n; k++)
		{
			printf("%7.4f ", metal[k]);
		}
		printf("\n");
	
	
		//save a copy for the image figure
		if(i<histFrames-1)
		{
			for(int j = 1; j <= n; j++)
			{
				history[i+1][j] = metalNew[j];
			}
		}
	}
	
	
	//image generation
	#ifndef WINDOWS_F
	
	char imgFileName[128];
	sprintf(imgFileName, "scenerio%d.bin", scenario);
	char bufText[256];
	sprintf(bufText, "SCENERIO %d FIRST %d TIME STEPS", scenario, histFrames);
	
	//set colors
	struct binimg_image_s outImg;
	binimg_initImage(900, 650, &outImg);
	struct pixel_s black = {0,0,0,255};
	struct pixel_s white = {255,255,255,255};
	struct pixel_s yellow = {255,255,51,255};
	struct pixel_s orange = {255,128,0,255};
	struct pixel_s red = {150,0,0,255};
	struct pixel_s darkgray = {20,20,20,255};
	
	//build gradiant
	struct gradiant_s heatColorGrad;
	binimg_initGradiant(7, &heatColorGrad);
	heatColorGrad.colors[0] = darkgray;
	heatColorGrad.pos[0] = 0;
	heatColorGrad.colors[1] = darkgray;
	heatColorGrad.pos[1] = 0.02;
	heatColorGrad.colors[2] = red;
	heatColorGrad.pos[2] = 0.1;
	heatColorGrad.colors[3] = orange;
	heatColorGrad.pos[3] = 0.2;
	heatColorGrad.colors[4] = yellow;
	heatColorGrad.pos[4] = 0.4;
	heatColorGrad.colors[5] = white;
	heatColorGrad.pos[5] = 0.8;
	heatColorGrad.colors[6] = white;
	heatColorGrad.pos[6] = 1;
	
	binimg_drawBitmapText(50,50, black, bufText, pixLetters, 2, &outImg);
	
	//for each of the time steps
	for(int i = 0; i < histFrames; i++)
	{
		sprintf(bufText,"T= %3.2f", deltaT*i);
		binimg_drawBitmapText(50,100+(i*50), black, bufText, pixLetters, 1, &outImg);
		binimg_drawRectangle(100,85+(i*50), 720, 40, 1, black, &outImg);
		
		
		
		for(int j = 0; j < n; j++)
		{
			//get the color from the gradiant and draw the segment
			struct pixel_s bufColor = binimg_colorFromGradiant(heatColorGrad, history[i][j+1]);
			binimg_drawRectangle(102+(80*j),87+(i*50), 76, 36, 1, bufColor, &outImg);
			sprintf(bufText, "%5.3f",history[i][j+1]);
			int lumi = (bufColor.r + bufColor.g + bufColor.b) / 3;
			//chose the text color based on how bright the segment color is
			if(lumi < 100)
			{
				binimg_drawBitmapText(123+(80*j),101+(i*50), white, bufText, pixLetters, 1, &outImg);
			}
			else
			{
				binimg_drawBitmapText(123+(80*j),101+(i*50), black, bufText, pixLetters, 1, &outImg);
			}
		}
	}
	
	binpypng_writebin(imgFileName, &outImg);
	binpypng_convert(imgFileName);
	
	
	#endif

	return 0;
}



int heatTransfer2DTrial()
{
	static int x = 102;
	static int y = 102;
	double metal[x][y];
	double metalNew[x][y];
	double deltaT = 0.0025;
	//double deltaX = 0.1;
	double eta = 0.025;
	double time = 0;
	int numTimeSteps = 10001;
	
	for(int i = 0; i < 102; i++)
	{
		for(int j = 0; j < 102; j++)
		{
			metal[i][j] = 0;
		}
	}
	
	/*
	for(int i = 0; i < 102; i++)
	{
		metal[i][0] = 1;
		metal[i][101] = 1;
		metal[0][i] = 1;
		metal[101][i] = 1;
	}
	*/
	
	for(int k = 0; k < numTimeSteps; k++)
	{
		int randx = (rand() %100) +1;
		int randy = (rand() %100) +1;
		metal[randx][randy] = 1;
	
		#ifndef WINDOWS_F
		char imgFileName[128];
		sprintf(imgFileName, "frames/frameT%04d.bin", k);
		char bufText[256];
		sprintf(bufText, "TIME= %5.2f", time);
		
		//set colors
		struct binimg_image_s outImg;
		binimg_initImage(400, 400, &outImg);
		struct pixel_s black = {0,0,0,255};
		struct pixel_s white = {255,255,255,255};
		struct pixel_s yellow = {255,255,51,255};
		struct pixel_s orange = {255,128,0,255};
		struct pixel_s red = {150,0,0,255};
		struct pixel_s darkgray = {20,20,20,255};
		
		//build gradiant
		struct gradiant_s heatColorGrad;
		binimg_initGradiant(7, &heatColorGrad);
		heatColorGrad.colors[0] = darkgray;
		heatColorGrad.pos[0] = 0;
		heatColorGrad.colors[1] = darkgray;
		heatColorGrad.pos[1] = 0.02;
		heatColorGrad.colors[2] = red;
		heatColorGrad.pos[2] = 0.1;
		heatColorGrad.colors[3] = orange;
		heatColorGrad.pos[3] = 0.2;
		heatColorGrad.colors[4] = yellow;
		heatColorGrad.pos[4] = 0.4;
		heatColorGrad.colors[5] = white;
		heatColorGrad.pos[5] = 0.8;
		heatColorGrad.colors[6] = white;
		heatColorGrad.pos[6] = 1;
		
		binimg_drawBitmapText(50,20, black, bufText, pixLetters, 2, &outImg);
		
		for(int i = 1; i < 101; i++)
		{
			for(int j = 1; j < 101; j++)
			{
				//get the color from the gradiant and draw the segment
				struct pixel_s bufColor = binimg_colorFromGradiant(heatColorGrad, metal[i][j]);
				binimg_drawRectangle(47+(3*i),47+(j*3), 3, 3, 1, bufColor, &outImg);
			}
		}
		
		binpypng_writebin(imgFileName, &outImg);
		binpypng_convert(imgFileName);
		
		#endif
				
				
		for(int i = 1; i < 101; i++)
		{
			for(int j = 1; j < 101; j++)
			{
				metalNew[i][j] = metal[i][j] + eta*(metal[i-1][j] + metal[i+1][j] + metal[i][j-1] + metal[i][j+1] - 4*metal[i][j]);
			}
		}
		for(int i = 1; i < 101; i++)
		{
			for(int j = 1; j < 101; j++)
			{
				metal[i][j] = metalNew[i][j];
			}
		}
		time+=deltaT;
	}
	
	
	
	return 0;
}

