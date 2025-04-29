//CS 445/545 Prog 5 for Kaden Beaver
/***********************************************************************************
*
*	Kaden Beaver	12.02.2021	Program 5
*
*	EXTRA CREDIT:	*Added WASD as controls to move the view box

					*added a boid swarm to add some life to the background of the fish tank

					*added rocks and plants to the fish tank

					*added a box blur to the background

					*press space to switch between line cliping to polygon cliping

					*the fishes outlines show up ontop of rocks and plants so that you can
					 see fish that are behind rock and plants

					*fish rotate to face direction of travel for all 8 direcitons, using
					 2d Homogenous  matrixs


	LAG:			There is a lot going on in this one, some of it like the box blur and
					boids should really be done on the gpu not cpu but that reqires writeing
					in anouther file

					if this is to laggy on the lab computer lower the number of boids or the
					radius of the box blur

	DESIGN:			There are three main parts to the program.  drawing fuctions, call back
					functions, and utility functions.
					There are drawing functions for the fish(line and polygon) the plants and
					rocks, the view box, the timer and score text, the boids, and the backgroung

					the way the scene is rendered starts by seting the active framebuffer to be
					a framebuffer object. Then the background is rendared to that texture layering
					rocks plants and boids to add depth.  That framebuffer texture is converted to 
					a pixel array that then has a box blur and color corection apllied to it before
					being converted back to a texture.  the active frame buffer is set back to
					the defualt and the background texture is drawn to fill the screen.  The rest 
					of the scene include the main two fish and some more rocks and plants is then 
					rendererd

					The scene is drawn in the glutDisplayFunc call back function displayFunction

					The glutKeyboardFunc call back funtion is keyboardFunction.  the inputs that
					the function keeps track of include:
					"WASD" and "UHJN" to move the view box
					"f" to feed the fish
					" " to change between polygon and line cliping
					"p" to pause the game

					The glutTimerFunc call back function is timerFunction.  there are 7 timer ids
					the first is used to call glutPostDisplay for 64 frames a second
					the second is used to update the fish pos and check for eating food
					the third is for rolling to see if fish change direction
					the forth is for lowing the fish life timer
					the fifth is for removing food
					the six is for updateing the boids pos and velocity
					and the seventh is for animating the plants

					There are many utility functions, some just set up or alicate inicial veribles
					like boidInit, shaderInit, blurTextureInit.  One is for getting the out codes
					of a points. One is used to calculate the boids new pos and vel
					blurFrame uses a boxblur to imitate a gausian blur for the frame
					polyToLine converta a polygonArray struct to LineSegments so they can be line cliped
					retatePolygon uses 2d homogeneus matrixs to rotate a polygon
					clipLine uses Cohen-Sutherland Line Clipper covered in class
					to clip lines to the view box
					clipPolygons uses Sutherland-Hodgman Polygon Clipper from page 449 in textbook
					to clip polygons to view box

					Gobal variable hold the values of moving objects like the two main fish, the boids
					the viewbox, the food, and the plants.  They are also used to keep track the the
					fishes life timers, and the players points.  There are also flags for if the game
					is over or paused



************************************************************************************/


//##################################################	INCLUDES	#######################################################################
#include <iostream>
#include <stdlib.h>
#include <time.h>>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup.h"
#include "pch.h"


struct LineSegment		//has a starting point and ending point 2d
{
	float x1, y1, x2, y2;
};

struct PolygonArray		//2d polygon
{
	float* X;
	float* Y;
	int numberOfPoints;
};


struct boid				//simple boid with velocity and posistion
{
	float x, y, velX, velY;
};


//##################################################	FUNCTIONS	#######################################################################
//Drawing funcs
void drawFish1();
void drawPolyFish1();
void drawFish2();
void drawPolyFish2();
void drawFood();
void drawPlant(float x, float y, float z, float size, float darkness, float theta, float amp, float freq, int segments);
void drawViewBox();
void drawTimer();
void drawScore();
void drawBoids();
void drawRock(float x, float y, float z, float size, float drarkness);
void drawTexture();
void writeBitmapString(void *font, char *string);


//Call back funcs
void displayFunction();									//THIS IS THE GLUTDISPLAYFUNC CALL BACK FUNCTION
void timerFunction(int id);								//THIS IS THE GLUTTIMERFUNC CALL BACK FUNCTION
void keyboardFunction(unsigned char c, int x, int y);	//THIS IS THE GLUTKEYBOARDFUNC CALL BACK FUNCTION

//Untilites funcs
void initShader();
char outCode(float x, float y);
LineSegment* polyToLines(PolygonArray p);
LineSegment clipLine(LineSegment line);
PolygonArray clipPolygon(PolygonArray p);
PolygonArray rotatePolygon(PolygonArray p, float xPos, float yPos, float theta);
void initBoids();
void updateBoids();
bool initBlurTexture();
void blurFrame();


//##################################################	GLOBAL VARIBLES AND DEFINES		###################################################

//The Canvas specifications
#define canvas_Width 800
#define canvas_Height 600
char canvas_Name[] = "Very Hungy Fish";

//Textures
unsigned int framebuffer;
unsigned int textureColorbuffer;
unsigned int bluredTexture;
uint8_t* textureBuffer = new uint8_t[canvas_Width * canvas_Height * 3];


//timer ids
#define TIMER_ID_FRAME_UPDATE 0
#define TIMER_ID_MOVE_FISH 1
#define TIMER_ID_CHANGE_FISHDIRECTION 2
#define TIMER_ID_LOWER_LIFE 3
#define TIMER_ID_REMOVE_FOOD 4
#define TIMER_ID_UPDATE_BOIDS 5
#define TIMER_ID_UPDATE_PLANT_THETA 6



//Boids
#define NUMBER_OF_BOIDS 300			//parameters for the boids, if the programs is laggy lower this number
#define BOID_MAX_SPEED 5
#define BOID_SIGHT_RANGE 20			//how far a boid can see
#define BOID_DISTANCE_MOD 0.13		//how much the distance between boids has an effect
#define BOID_SEPARATION 0.5			//how important seperations is
#define BOID_ALIGNMENT 0.7			//how important that boid travel the same dirctions
#define BOID_COHESION 0.8			//how important that boid try to move to the centar of their group
#define BOID_CURENT_HEADING 0.7		//how much can they accerate
#define BOID_WALL_FORCE 10
boid boidSchool[NUMBER_OF_BOIDS];	//the array of boid structs


bool fillFishPoly = true;
//Fish 1
bool fish1Alive = true;
int fish1LifeRemain = 8;
float fish1X = 400;
float fish1Y = 300;
float fish1Theta = 0;

//fish 2
bool fish2Alive = true;
int fish2LifeRemain = 8;
float fish2X = 350;
float fish2Y = 275;
float fish2Theta = M_PI;

//food
float foodX = -99;
float foodY = 300;


//View Box
float vbLeftX = 200;
float vbRightX = 600;
float vbTopY = 450;
float vbBottomY = 150;


//Score
int score = 0;
bool gameOver = false;
bool gamePaused = false;

//Blur
int blurRadius = 5;					//how blury to make the background, also lower this if laggy
int brightnessAgustment = 30;		//bluring darkents the background so needed to add some light back
int greenAgustment = -70;			//bluring the plants made the background to green

//plants
float plant1Phase = 0.0;			//used the animate the wave in the plants



//##################################################	MAIN	############################################################################
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);								//Set up glut
	my_setup(canvas_Width, canvas_Height, canvas_Name);	//The function for given header

	srand(time(NULL));									//set rnad seed
	
	glutDisplayFunc(displayFunction);					//register call back fuctnion for DisplayFunc
	glutKeyboardFunc(keyboardFunction);					//register call back fucntion for KeyboardFunc
	
	initShader();										//set up the lights
	initBoids();										//set up the boids
	initBlurTexture();									//alocate the framebuffer and texture objects
	
	glutTimerFunc(62.5, timerFunction, TIMER_ID_FRAME_UPDATE);		//start first calls of the timerfuncitons
	glutTimerFunc(50, timerFunction, TIMER_ID_MOVE_FISH);
	glutTimerFunc(2000, timerFunction, TIMER_ID_CHANGE_FISHDIRECTION);
	glutTimerFunc(1000, timerFunction, TIMER_ID_LOWER_LIFE);
	glutTimerFunc(100, timerFunction, TIMER_ID_UPDATE_BOIDS);
	glutTimerFunc(50, timerFunction, TIMER_ID_UPDATE_PLANT_THETA);


	glutMainLoop();										//Start the main loop

	return 0;
}


//##################################################	DRAWING FUNCTIONS	################################################################



void drawFish1()
{
	if (!fish1Alive) return;

	float matAmbAndDif1[] = { 0.0, 1.0, 1.0, 1.0 };//cyan light blue color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	glLoadIdentity();

	PolygonArray headGon;								//head polygon
	headGon.numberOfPoints = 4;
	headGon.X = new float[8];
	headGon.Y = new float[8];

	headGon.X[0] = fish1X + 17.2;
	headGon.Y[0] = fish1Y;
	headGon.X[1] = fish1X;
	headGon.Y[1] = fish1Y + 12.5;
	headGon.X[2] = fish1X - 20.0;
	headGon.Y[2] = fish1Y;
	headGon.X[3] = fish1X;
	headGon.Y[3] = fish1Y - 12.5;

	PolygonArray tailGon;								//tail polygon
	tailGon.numberOfPoints = 3;
	tailGon.X = new float[6];
	tailGon.Y = new float[6];

	tailGon.X[0] = fish1X - 20.0;
	tailGon.Y[0] = fish1Y;
	tailGon.X[1] = fish1X - (20.0 + 12.8);
	tailGon.Y[1] = fish1Y + 8.0;
	tailGon.X[2] = fish1X - (20.0 + 12.8);
	tailGon.Y[2] = fish1Y - 8.0;

														//rotate polygons
	headGon = rotatePolygon(headGon, fish1X, fish1Y, fish1Theta);
	tailGon = rotatePolygon(tailGon, fish1X, fish1Y, fish1Theta);


	LineSegment* lineHead = polyToLines(headGon);		//convert polygons to line arrays
	LineSegment* lineTail = polyToLines(tailGon);
	


	for (int i = 0; i < headGon.numberOfPoints; i++)	//draw head
	{
		lineHead[i] = clipLine(lineHead[i]);			//clip each line
		if (lineHead[i].x1 != -1)
		{
			glBegin(GL_LINES);							//draw clipped line
			glVertex3f(lineHead[i].x1, lineHead[i].y1, -100.0);
			glVertex3f(lineHead[i].x2, lineHead[i].y2, -100.0);
			glEnd();
		}
	}

	for (int i = 0; i < tailGon.numberOfPoints; i++)	//draw tail
	{
		lineTail[i] = clipLine(lineTail[i]);			//clip line
		if (lineTail[i].x1 != -1)
		{
			glBegin(GL_LINES);							//draw line
			glVertex3f(lineTail[i].x1, lineTail[i].y1, -100.0);
			glVertex3f(lineTail[i].x2, lineTail[i].y2, -100.0);
			glEnd();
		}
	}

	delete lineHead;									//avoid memory leak
	delete lineTail;
	delete headGon.X;
	delete headGon.Y;
	delete tailGon.X;
	delete tailGon.Y;

}

void drawPolyFish1()
{
	if (fish1Alive != true) return;			//only draw fish if they are alive

	float matAmbAndDif1[] = { 0.0, 1.0, 1.0, 1.0 };//cyan light blue color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	glLoadIdentity();

	PolygonArray headGon;						//head polygon
	headGon.numberOfPoints = 4;
	headGon.X = new float[8];
	headGon.Y = new float[8];

	headGon.X[0] = fish1X + 17.2;
	headGon.Y[0] = fish1Y;
	headGon.X[1] = fish1X;
	headGon.Y[1] = fish1Y + 12.5;
	headGon.X[2] = fish1X - 20.0;
	headGon.Y[2] = fish1Y;
	headGon.X[3] = fish1X;
	headGon.Y[3] = fish1Y - 12.5;

	PolygonArray tailGon;						//tail polygon
	tailGon.numberOfPoints = 3;
	tailGon.X = new float[6];
	tailGon.Y = new float[6];

	tailGon.X[0] = fish1X - 20.0;
	tailGon.Y[0] = fish1Y;
	tailGon.X[1] = fish1X - (20.0 + 12.8);
	tailGon.Y[1] = fish1Y + 8.0;
	tailGon.X[2] = fish1X - (20.0 + 12.8);
	tailGon.Y[2] = fish1Y - 8.0;

												//rotate polygons
	headGon = rotatePolygon(headGon, fish1X, fish1Y, fish1Theta);
	tailGon = rotatePolygon(tailGon, fish1X, fish1Y, fish1Theta);

	headGon = clipPolygon(headGon);				//clip polygons
	tailGon = clipPolygon(tailGon);

	glBegin(GL_POLYGON);						//draw head
	for (int i = 0; i < headGon.numberOfPoints; i++)
	{
		glVertex3f(headGon.X[i], headGon.Y[i], -100.0);
	}
	glEnd();
	glBegin(GL_POLYGON);						//draw tail
	for (int i = 0; i < tailGon.numberOfPoints; i++)
	{
		glVertex3f(tailGon.X[i], tailGon.Y[i], -100.0);
	}
	glEnd();


	
	delete headGon.X;							//avoid memory leak
	delete headGon.Y;
	delete tailGon.X;
	delete tailGon.Y;
}

void drawFish2()
{
	if (!fish2Alive) return;

	float matAmbAndDif1[] = { 0.2, 0.8, 0.2, 1.0 };//cyan light blue color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	glLoadIdentity();

	PolygonArray headGon;						//head polygon

	headGon.numberOfPoints = 4;
	headGon.X = new float[8];
	headGon.Y = new float[8];

	headGon.X[0] = fish2X + 15.0;
	headGon.Y[0] = fish2Y;
	headGon.X[1] = fish2X;
	headGon.Y[1] = fish2Y + 15.0;
	headGon.X[2] = fish2X - 15.0;
	headGon.Y[2] = fish2Y;
	headGon.X[3] = fish2X;
	headGon.Y[3] = fish2Y - 15.0;

	PolygonArray tailGon;						//tail polygon

	tailGon.numberOfPoints = 3;
	tailGon.X = new float[6];
	tailGon.Y = new float[6];

	tailGon.X[0] = fish2X - 15.0;
	tailGon.Y[0] = fish2Y;
	tailGon.X[1] = fish2X - (15.0 + 10.0);
	tailGon.Y[1] = fish2Y + 8.0;
	tailGon.X[2] = fish2X - (15.0 + 10.0);
	tailGon.Y[2] = fish2Y - 8.0;
												//use 3x3 matrix to rotate polygons
	headGon = rotatePolygon(headGon, fish2X, fish2Y, fish2Theta);
	tailGon = rotatePolygon(tailGon, fish2X, fish2Y, fish2Theta);
												//convert polygons to linesegment arrays
	LineSegment* lineHead = polyToLines(headGon);
	LineSegment* lineTail = polyToLines(tailGon);

												//the number of lines will equale the number of points
	for (int i = 0; i < headGon.numberOfPoints; i++)
	{
		lineHead[i] = clipLine(lineHead[i]);	//clip each line
		if (lineHead[i].x1 != -1)
		{
			glBegin(GL_LINES);					//draw the line
			glVertex3f(lineHead[i].x1, lineHead[i].y1, -100.0);
			glVertex3f(lineHead[i].x2, lineHead[i].y2, -100.0);
			glEnd();
		}
	}

	for (int i = 0; i < tailGon.numberOfPoints; i++)	//draw the tail
	{
		lineTail[i] = clipLine(lineTail[i]);	//clip each line
		if (lineTail[i].x1 != -1)
		{
			glBegin(GL_LINES);					//draw the cliped line
			glVertex3f(lineTail[i].x1, lineTail[i].y1, -100.0);
			glVertex3f(lineTail[i].x2, lineTail[i].y2, -100.0);
			glEnd();
		}
	}



	delete lineHead;							//avoid memory leak
	delete lineTail;
	delete headGon.X;
	delete headGon.Y;
	delete tailGon.X;
	delete tailGon.Y;
}

void drawPolyFish2()
{
	if (!fish2Alive) return;

	float matAmbAndDif1[] = { 0.2, 0.8, 0.2, 1.0 };//light green color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


	glLoadIdentity();

	PolygonArray headGon;						//the head  polygon
	
	headGon.numberOfPoints = 4;
	headGon.X = new float[8];
	headGon.Y = new float[8];

	headGon.X[0] = fish2X + 15.0;
	headGon.Y[0] = fish2Y;
	headGon.X[1] = fish2X;
	headGon.Y[1] = fish2Y + 15.0;
	headGon.X[2] = fish2X - 15.0;
	headGon.Y[2] = fish2Y;
	headGon.X[3] = fish2X;
	headGon.Y[3] = fish2Y - 15.0;

	PolygonArray tailGon;						//the tail poygon
	
	tailGon.numberOfPoints = 3;
	tailGon.X = new float[6];
	tailGon.Y = new float[6];

	tailGon.X[0] = fish2X - 15.0;
	tailGon.Y[0] = fish2Y;
	tailGon.X[1] = fish2X - (15.0 + 10.0);
	tailGon.Y[1] = fish2Y + 8.0;
	tailGon.X[2] = fish2X - (15.0 + 10.0);
	tailGon.Y[2] = fish2Y - 8.0;
												//use the 3x3 matix to rotate polygons
	headGon = rotatePolygon(headGon, fish2X, fish2Y, fish2Theta);
	tailGon = rotatePolygon(tailGon, fish2X, fish2Y, fish2Theta);

	headGon = clipPolygon(headGon);				//the the polygon clipping
	tailGon = clipPolygon(tailGon);

	glBegin(GL_POLYGON);						//draw the head
	for (int i = 0; i < headGon.numberOfPoints; i++)
	{
		glVertex3f(headGon.X[i], headGon.Y[i], -100.0);
	}
	glEnd();
	glBegin(GL_POLYGON);						//draw the tail
	for (int i = 0; i < tailGon.numberOfPoints; i++)
	{
		glVertex3f(tailGon.X[i], tailGon.Y[i], -100.0);
	}
	glEnd();
	delete headGon.X;							//avoid memory leak
	delete headGon.Y;
	delete tailGon.X;
	delete tailGon.Y;
}

void drawFood()
{
	float matAmbAndDif1[] = { 1.0, 1.0, 0.2, 1.0 };//yellow color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	glLoadIdentity();
	



	if (foodX > 0)									//draw yellow square food piece
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(foodX + 2.5, foodY + 2.5, -100.0);
		glVertex3f(foodX - 2.5, foodY + 2.5, -100.0);
		glVertex3f(foodX + 2.5, foodY - 2.5, -100.0);
		glVertex3f(foodX - 2.5, foodY + 2.5, -100.0);
		glVertex3f(foodX - 2.5, foodY - 2.5, -100.0);
		glVertex3f(foodX + 2.5, foodY - 2.5, -100.0);
		glEnd();
	}
}

void drawPlant(float x, float y, float z, float size, float darkness, float theta, float amp, float freq, int segments)
{
																	//darkness controls shade of green
	float matAmbAndDif1[] = { 0.2, darkness, 0.2, 1.0 };//green color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


	glLoadIdentity();
	glTranslatef(-amp*sin(freq*y + theta), 0.0, 0.0);				//so the base stay in place
																	
																	//the x pos is controled by a sine function
																	//graduly changeing theta cause the points to
																	//phase shift making the plants wave
	for (int i = 0; i < segments; i++)
	{
		glBegin(GL_POLYGON);										//draw a quad
		glVertex3f(x + amp*sin(freq*y + theta), y, z);
		glVertex3f(x + size + amp*sin(freq*y + theta), y, z);
		glVertex3f(x + size + amp*sin(freq*(y+ size) + theta), y + size, z);
		glVertex3f(x + amp * sin(freq*(y + size) + theta), y + size, z);
		glEnd();
		y += size;
	}
}

void drawViewBox()
{
	float matAmbAndDif1[] = { 1.0, 1.0, 1.0, 1.0 };//white color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	glLoadIdentity();

	glBegin(GL_LINE_LOOP);							//draw box based off gobal pos
	glVertex3f(vbLeftX, vbTopY, -100.0);
	glVertex3f(vbRightX, vbTopY, -100.0);
	glVertex3f(vbRightX, vbBottomY, -100.0);
	glVertex3f(vbLeftX, vbBottomY, -100.0);
	glEnd();

}

void drawTimer()
{
	char timer1TXT[] = "FISH ONE LIFE: 00";							//string to write
	snprintf(timer1TXT, 17, "FISH ONE LIFE: %d", fish1LifeRemain);	//create the string with the current time
	char timer2TXT[] = "FISH TWO LIFE: 00";							//same for fish 2
	snprintf(timer2TXT, 17, "FISH TWO LIFE: %d", fish2LifeRemain);

	float matAmbAndDif1[] = { 1.0, 1.0, 1.0, 1.0 };//white color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


	glLoadIdentity();
	glRasterPos2f(670, 580);										//posicion to top right of screen
	writeBitmapString(GLUT_BITMAP_8_BY_13, timer1TXT);				//write time
	glRasterPos2f(670, 560);										//posicion to top right of screen
	writeBitmapString(GLUT_BITMAP_8_BY_13, timer2TXT);				//write time

}

void drawScore()
{
	char scoreTXT[] = "SCORE: 000000";									//string to write
	snprintf(scoreTXT, 13, "SCORE: %6d", score);						//create the string with the current score
	float matAmbAndDif1[] = { 1.0, 1.0, 1.0, 1.0 };//white color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
																		//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


	glLoadIdentity();
	glRasterPos2f(670, 10);												//posicion to top middle of screen
	writeBitmapString(GLUT_BITMAP_8_BY_13, scoreTXT);					//write score
}

//draw all the boids
void drawBoids()
{
	float matAmbAndDif1[] = { 0.0, 0.5, 0.7, 1.0 };//cyan light blue color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	glLoadIdentity();
	glPointSize(10);
	glBegin(GL_POINTS);
	for (int i = 0; i < NUMBER_OF_BOIDS; i++)
	{												//draw a size 10 point for each boid
		glVertex3f(boidSchool[i].x, boidSchool[i].y, -170.0);
	}
	glEnd();
	glPointSize(1);
}

//draw a simple rock
void drawRock(float x, float y, float z, float size, float darkness)
{
												//darkness choses the shade or grey
	float matAmbAndDif1[] = { darkness, darkness, darkness, 1.0 };//grey color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	PolygonArray rockGon;						//set up polygon
	rockGon.numberOfPoints = 6;
	rockGon.X = new float[6];
	rockGon.Y = new float[6];

	rockGon.X[0] = x;
	rockGon.Y[0] = y;
	rockGon.X[1] = x + 30;
	rockGon.Y[1] = y;
	rockGon.X[2] = x + 27;;
	rockGon.Y[2] = y + 10;
	rockGon.X[3] = x + 20;
	rockGon.Y[3] = y + 17;
	rockGon.X[4] = x + 10;
	rockGon.Y[4] = y + 9;
	rockGon.X[5] = x + 5;
	rockGon.Y[5] = y + 7;

	glLoadIdentity();							//position and scale it
	glTranslatef(x, y, 0);
	glScalef(size, size, 1.0);
	glTranslatef(-x, -y, 0);
	

	glBegin(GL_POLYGON);						//draw it
	for (int i = 0; i < rockGon.numberOfPoints; i++)
	{
		glVertex3f(rockGon.X[i], rockGon.Y[i], z);
	}
	glEnd();

	delete rockGon.X;							//avoid memory leak
	delete rockGon.Y;
}

//draw a polygon to fill the screen with the blured background texture
void drawTexture()
{
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, bluredTexture);
	glEnable(GL_TEXTURE_2D);										//set up to draw with textures
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(0.0, -20.0, -175.0);			//start and y=-20 because the blur function
	glTexCoord2f(1.0, 0.0);	glVertex3f(800.0, -20.0, -175.0);		//does not go all the way to the edges
	glTexCoord2f(1.0, 1.0);	glVertex3f(800.0, 620.0, -175.0);
	glTexCoord2f(0.0, 1.0);	glVertex3f(0.0, 620.0, -175.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);										//diable drawing with textures or every thing breaks
}


//prints the string of bit maped chars, must set raster pos before calling this
void writeBitmapString(void *font, char *string)						//FROM THE GUHA TEXT BOOK
{
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);	//loop through the string and prints the chars
}




//##################################################	CALL BACK FUNCTIONS		#######################################

void displayFunction()									//glutDisplayFunc call back
{
	glMatrixMode(GL_MODELVIEW);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);		//START ON BACKGROUND TEXTURE
	glClearColor(0.0, 0.25, 0.53, 1.0);					//set background to black
	glClear(GL_COLOR_BUFFER_BIT);						//clear buffer
					
	drawRock(10.0, 0.0, -150.0, 10.0, 0.1);				//draw farthest back rocks
	drawRock(500.0, -30.0, -150.0, 10.0, 0.1);

	drawPlant(700.0, 0.0, -10.0, 40.0, 0.3, plant1Phase, 10.0, 0.05, 10);	//draw far back plants
	
	drawPlant(500.0, 0.0, -10.0, 25.0, 0.4, plant1Phase, 8.0, 0.03, 12);


	drawBoids();										//draw boids

	drawPlant(180.0, 0.0, -10.0, 30.0, 0.4, plant1Phase, 10.0, 0.05, 15);	//draw middle plants
	drawPlant(280.0, 0.0, -10.0, 20.0, 0.4, plant1Phase, 10.0, 0.03, 10);

	drawRock(400.0, -10.0, -100.0, 7.0, 0.3);			//draw middle rock

	drawPlant(400.0, 0.0, -10.0, 25.0, 0.45, plant1Phase, 10.0, 0.03, 6);	//draw even closer plants
	drawPlant(450.0, 0.0, -10.0, 10.0, 0.4, plant1Phase, 8.0, 0.03, 7);


	blurFrame();										//convert the frame buffer to texture and blur it

	glBindFramebuffer(GL_FRAMEBUFFER, 0);				//BACKGROUND DONE
	glClearColor(0.0, 0.0, 0.0, 1.0);					//set background to black
	glClear(GL_COLOR_BUFFER_BIT);	//clear buffer

	 

	drawTexture();										//draw background

	
	
	
	
	if(fillFishPoly) drawPolyFish1();					//draw the polygon fish
	if(fillFishPoly) drawPolyFish2();

	drawPlant(600.0, 0.0, -10.0, 25.0, 0.6, plant1Phase, 10.0, 0.05, 10);	//draw forground plants
	drawPlant(560.0, 0.0, -10.0, 15.0, 0.6, plant1Phase, 10.0, 0.03, 10);
	drawPlant(660.0, 0.0, -10.0, 10.0, 0.5, plant1Phase, 8.0, 0.03, 7);


	drawRock(50.0, 0.0, -10.0, 4.0, 0.4);				//draw forground rocks
	drawRock(200.0, -40.0, -10.0, 4.0, 0.43);
	drawRock(400.0, -200.0, -10.0, 15.0, 0.35);

	drawPlant(60.0, 0.0, -10.0, 30.0, 0.6, plant1Phase, 10.0, 0.05, 10);	//draw more forground plants
	drawPlant(120.0, 0.0, -10.0, 20.0, 0.6, plant1Phase, 10.0, 0.03, 10);
	drawPlant(20.0, 0.0, -10.0, 20.0, 0.5, plant1Phase, 8.0, 0.03, 7);

	drawFish1();										//draw lines of fish so thatthe outline show over top everything but
	drawFish2();										//the text
	drawFood();
	drawViewBox();										//draw the view box
	drawTimer();										//write the tiemrs and score
	drawScore();
	
	glutSwapBuffers();									//swap buffers for duel buffer mode
	
}


void timerFunction(int id)											//glutTimerFunc call back
{
	double r = 0.0;
	switch (id)
	{
	case TIMER_ID_FRAME_UPDATE:										//update the screen every 50ms
		glutPostRedisplay();										//or about 20 frames a second
		glutTimerFunc(62.5, timerFunction, TIMER_ID_FRAME_UPDATE);
		break;

	case TIMER_ID_MOVE_FISH:										//upadte the fish pos based on their veloicty
		if (gamePaused) break;										//if the game is pause end this timer

																	//check if fish 1 is near food
		if (fish1X > foodX - 10 && fish1X < foodX + 10 && fish1Y < foodY + 10 && fish1Y > foodY - 10)
		{
			fish1LifeRemain = 8;
			score += 50;
			glutTimerFunc(100, timerFunction, TIMER_ID_REMOVE_FOOD);
		}
																	//check if fish 2 is near food
		if (fish2X > foodX - 10 && fish2X < foodX + 10 && fish2Y < foodY + 10 && fish2Y > foodY - 10)
		{
			fish2LifeRemain = 8;
			score += 50;
			glutTimerFunc(100, timerFunction, TIMER_ID_REMOVE_FOOD);
		}

		fish1X += 3.2*cos(fish1Theta);								//update fish 1 pos
		fish1Y += 3.2*sin(fish1Theta);

		if (fish1X + 30 > 800)										//if fish 1 hit wall add PI to theta to do 180
		{
			fish1X = 770;
			fish1Theta += M_PI;
		}
		else if (fish1X - 30 < 0)
		{
			fish1X = 30;
			fish1Theta -= M_PI;
		}
		else if (fish1Y + 15 > 600)
		{
			fish1Y = 585;
			fish1Theta -= M_PI;
		}
		else if (fish1Y - 15 < 0)
		{
			fish1Y = 15;
			fish1Theta += M_PI;
		}

		fish2X += 3.2*cos(fish2Theta);								//set new fish 2 pos
		fish2Y += 3.2*sin(fish2Theta);

		if (fish2X + 30 > 800)										//do 180 if fish 2 hits wall
		{
			fish2X = 770;
			fish2Theta += M_PI;
		}
		else if (fish2X - 30 < 0)
		{
			fish2X = 30;
			fish2Theta -= M_PI;
		}
		else if (fish2Y + 15 > 600)
		{
			fish2Y = 585;
			fish2Theta -= M_PI;
		}
		else if (fish2Y - 15 < 0)
		{
			fish2Y = 15;
			fish2Theta += M_PI;
		}

		glutTimerFunc(50, timerFunction, TIMER_ID_MOVE_FISH);		//queue up next fish movment timer
		break;
		
	case TIMER_ID_CHANGE_FISHDIRECTION:								//roll chance to chagne fish direction
		if (gamePaused) break;										//if paused stop this timer
		r = ((double) rand() / (RAND_MAX));							//get rand number 0 to 1
		if (r < 0.125)												//12.5% to rotate positive theta
		{
			fish1Theta += M_PI / 4;
		}
		else if (r < 0.25)											//12.5% to rotate negative theta
		{
			fish1Theta -= M_PI / 4;
		}
		r = ((double)rand() / (RAND_MAX));							//repete for fish 2
		if (r < 0.125)
		{
			fish2Theta += M_PI / 4;
		}
		else if (r < 0.25)
		{
			fish2Theta -= M_PI / 4;
		}

		glutTimerFunc(2000, timerFunction, TIMER_ID_CHANGE_FISHDIRECTION);	//queue up next timer
		break;

	case TIMER_ID_LOWER_LIFE:										//lower the life of each fish every 1 second
		if (gamePaused) break;
		fish1LifeRemain -= 1;
		fish2LifeRemain -= 1;
		if (fish1LifeRemain <= 0) fish1Alive = false;				//if the fish is dead set flag
		if (fish2LifeRemain <= 0) fish2Alive = false;
		glutTimerFunc(1000, timerFunction, TIMER_ID_LOWER_LIFE);
		break;

	case TIMER_ID_REMOVE_FOOD:										//set food out side the bound of tank
		foodX = -99;
		break;

	case TIMER_ID_UPDATE_BOIDS:										//update the boids pos and vel every 50 ms
		updateBoids();
		glutTimerFunc(50, timerFunction, TIMER_ID_UPDATE_BOIDS);
		break;

	case TIMER_ID_UPDATE_PLANT_THETA:								//update the phase shift of the plants every 50 ms
		plant1Phase += M_PI / 64;
		glutTimerFunc(50, timerFunction, TIMER_ID_UPDATE_PLANT_THETA);
	}
	
}

void keyboardFunction(unsigned char c, int x, int y)					//glutKeyboardFunc call back
{
	switch (c)
	{
	case 'w':											//w or u move view box up
	case 'u':
		vbTopY += 5;
		vbBottomY += 5;
		if (vbTopY > 600)								//make sure box does not go out of bounds
		{
			vbTopY = 600;
			vbBottomY = 300;
		}
		break;
	case 'a':											//a or h to move box left
	case 'h':
		vbLeftX -= 5;
		vbRightX -= 5;
		if (vbLeftX < 2)
		{
			vbLeftX = 2;
			vbRightX = 402;
		}
		break;
	case 'd':											//d or j to move box right
	case 'j':
		vbRightX += 5;
		vbLeftX += 5;
		if (vbRightX > 800)
		{
			vbRightX = 800;
			vbLeftX = 400;
		}
		break;
	case 's':											//s or n to move box down
	case 'n':
		vbBottomY -= 5;
		vbTopY -= 5;
		if (vbBottomY < 2)
		{
			vbBottomY = 2;
			vbTopY = 302;
		}
		break;

	case 'f':											//f to feed fish
		if (foodX < 0)
		{
			foodX = x;									//convert mouse cords
			foodY = -y+600;								//and set food pos
			glutTimerFunc(1000, timerFunction, TIMER_ID_REMOVE_FOOD);	//queue timer to remove food
		}
		break;

	case ' ':											// space key to swith from polygon clipping to line clipping
		fillFishPoly = !fillFishPoly;
		break;

	case 'p':											//p to pause fish and timer
		gamePaused = !gamePaused;
		if (!gamePaused)								//reset the timerfunctions for the fish
		{
			glutTimerFunc(50, timerFunction, TIMER_ID_MOVE_FISH);
			glutTimerFunc(2000, timerFunction, TIMER_ID_CHANGE_FISHDIRECTION);
			glutTimerFunc(1000, timerFunction, TIMER_ID_LOWER_LIFE);
		}
		break;
	}
}


//##################################################	UTILITY FUNCTIONS	####################################



//sets up the light source
void initShader()
{
	glLoadIdentity();
	float lightAmb[] = { 1.0, 1.0, 1.0, 1.0 };				//white light
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightPos[] = { 400.0, 500.0, 0.0, 1.0 };			//pos at origin
	
	// Light properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHTING);									//enable lighting
	glEnable(GL_LIGHT0);
	
	glShadeModel(GL_SMOOTH);								//set to smooth
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);	//set local viewer

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

//takes in a vertext and returns a 8bit outcode
char outCode(float x, float y)
{
	char retVal = 0;

	if (x < vbLeftX) retVal = retVal | 1;	//is left
	if (x > vbRightX) retVal = retVal | 2;	//is right
	if (y < vbBottomY) retVal = retVal | 4;	//is below
	if (y > vbTopY) retVal = retVal | 8;	//is above

	return retVal;
}

//convert a PolygonArray struct to an array or LineSegments
//the number of lineSegments will equal the number of point
//in p
//I used this so that I could just create the polygon fish
//and use the same rotate function for the lines
//insetad of creating a rotate funciton spcific for linesegments
LineSegment * polyToLines(PolygonArray p)
{
	LineSegment* lines = new LineSegment[p.numberOfPoints];		//array of segments


	for (int i = 0; i < p.numberOfPoints; i++)
	{
		if (i == p.numberOfPoints - 1)							//loop back around to the first point
		{
			lines[i].x1 = p.X[i];
			lines[i].y1 = p.Y[i];
			lines[i].x2 = p.X[0];
			lines[i].y2 = p.Y[0];
		}
		else
		{
			lines[i].x1 = p.X[i];								//segment from point i to point i+1
			lines[i].y1 = p.Y[i];
			lines[i].x2 = p.X[i + 1];
			lines[i].y2 = p.Y[i + 1];
		}
	}

	return lines;												//return pointer to linesegment array
}

//Cohen-Sutherland Line Clipper
//takes in line segment clips it to view box
//returns line segment
LineSegment clipLine(LineSegment line)
{
	LineSegment retVal = line;					//line to return
	bool done = false;
	bool accpt = false;
	bool reject = false;

	do											//until line is cliped
	{
		char out1 = outCode(retVal.x1, retVal.y1);	//get out codes for each point
		char out2 = outCode(retVal.x2, retVal.y2);
		if (out1 & out2)						//if both point are to one side of viewbox trivial reject
		{
			reject = true;
			retVal.x1 = -1;
		}
		if (reject) done = true;				//set done if so
		else
		{
			if (!(out1 | out2)) accpt = true;	//if both points are in box trivial accept
			if (accpt) done = true;				//set done if so
			else
			{
				if (out1)						//if point 1 is outside box
				{
					if (out1 & 8)				//clip to top
					{
						retVal.x1 = retVal.x1 + ((retVal.x2 - retVal.x1) / (retVal.y2 - retVal.y1))*(vbTopY - retVal.y1);
						retVal.y1 = vbTopY;
					}
					else if (out1 & 4)			//clip to bottom
					{
						retVal.x1 = retVal.x1 + ((retVal.x2 - retVal.x1) / (retVal.y2 - retVal.y1))*(vbBottomY - retVal.y1);
						retVal.y1 = vbBottomY;
					}	
					else if (out1 & 2)			//clip to right
					{
						retVal.y1 = retVal.y1 + ((retVal.y2 - retVal.y1) / (retVal.x2 - retVal.x1))*(vbRightX - retVal.x1);
						retVal.x1 = vbRightX;
					}
					else						//clip to left
					{
						retVal.y1 = retVal.y1 + ((retVal.y2 - retVal.y1) / (retVal.x2 - retVal.x1))*(vbLeftX - retVal.x1);
						retVal.x1 = vbLeftX;
					}
				}
				else							//point 2 is outside box
				{
					if (out2 & 8)				//clip to top
					{
						retVal.x2 = retVal.x2 + ((retVal.x1 - retVal.x2) / (retVal.y1 - retVal.y2))*(vbTopY - retVal.y2);
						retVal.y2 = vbTopY;
					}
					else if (out2 & 4)			//clip to bottom
					{
						retVal.x2 = retVal.x2 + ((retVal.x1 - retVal.x2) / (retVal.y1 - retVal.y2))*(vbBottomY - retVal.y2);
						retVal.y2 = vbBottomY;
					}
					else if (out2 & 2)			//clip to right
					{
						retVal.y2 = retVal.y2 + ((retVal.y1 - retVal.y2) / (retVal.x1 - retVal.x2))*(vbRightX - retVal.x2);
						retVal.x2 = vbRightX;
					}
					else						//clip to left
					{
						retVal.y2 = retVal.y2 + ((retVal.y1 - retVal.y2) / (retVal.x1 - retVal.x2))*(vbLeftX - retVal.x2);
						retVal.x2 = vbLeftX;
					}
				}
			}
		}
	}while(!done);								//if trivial accept or reject is found end loop
	


	return retVal;								//return cliped line
}

//Sutherland-Hodgman Polygon Clipper
//textbook page 449
//takes a polygon and clips it to the view box
PolygonArray clipPolygon(PolygonArray p)
{
	PolygonArray buff;								//create a buffer polygon
	buff.numberOfPoints = 0;
	buff.X = new float[p.numberOfPoints * 2];
	buff.Y = new float[p.numberOfPoints * 2];
	

	//clip left bound copy into buff
	for (int i = 0; i < p.numberOfPoints; i++)		//loop through all points
	{
		int v1 = 0;									//the line segment to check
		int v2 = 0;
		if (i == p.numberOfPoints - 1)				//loop back to the begining to close polygon
		{
			v1 = i;
			v2 = 0;
		}
		else
		{
			v1 = i;
			v2 = i + 1;
		}

		//both points inside
		if (p.X[v1] >= vbLeftX && p.X[v2] >= vbLeftX)
		{
			buff.X[buff.numberOfPoints] = p.X[v2];		//add v2
			buff.Y[buff.numberOfPoints++] = p.Y[v2];
		}
		//first inside next outside
		else if (p.X[v1] >= vbLeftX && p.X[v2] < vbLeftX)
		{
			if (p.X[v1] == vbLeftX)						//add nothing
			{

			}
			else										//add W
			{
				float wY = p.Y[v2] + ((p.Y[v1] - p.Y[v2]) / (p.X[v1] - p.X[v2]))*(vbLeftX - p.X[v2]);
				float wX = vbLeftX;
				buff.X[buff.numberOfPoints] = wX;
				buff.Y[buff.numberOfPoints++] = wY;
			}
		}
		//first outside next outside
		else if (p.X[v1] < vbLeftX && p.X[v2] >= vbLeftX)
		{
			if (p.X[v2] == vbLeftX)						//add v2
			{
				buff.X[buff.numberOfPoints] = p.X[v2];
				buff.Y[buff.numberOfPoints++] = p.Y[v2];
			}
			else										//add w and then v2
			{
				float wY = p.Y[v2] + ((p.Y[v1] - p.Y[v2]) / (p.X[v1] - p.X[v2]))*(vbLeftX - p.X[v2]);
				float wX = vbLeftX;
				buff.X[buff.numberOfPoints] = wX;
				buff.Y[buff.numberOfPoints++] = wY;
				buff.X[buff.numberOfPoints] = p.X[v2];
				buff.Y[buff.numberOfPoints++] = p.Y[v2];
			}
		}
	}//finsh clip leftside

	p.numberOfPoints = 0;								//reset p to be empty

	//clip right side copy into p
	for (int i = 0; i < buff.numberOfPoints; i++)
	{
		int v1 = 0;
		int v2 = 0;
		if (i == buff.numberOfPoints - 1)
		{
			v1 = i;
			v2 = 0;
		}
		else
		{
			v1 = i;
			v2 = i + 1;
		}
		//both inside
		if (buff.X[v1] <= vbRightX && buff.X[v2] <= vbRightX)
		{
			p.X[p.numberOfPoints] = buff.X[v2];
			p.Y[p.numberOfPoints++] = buff.Y[v2];
		}
		//first inside next outside
		else if (buff.X[v1] <= vbRightX && buff.X[v2] > vbRightX)
		{
			if (buff.X[v1] == vbRightX)
			{

			}
			else
			{
				float wY = buff.Y[v2] + ((buff.Y[v1] - buff.Y[v2]) / (buff.X[v1] - buff.X[v2]))*(vbRightX - buff.X[v2]);
				float wX = vbRightX;
				p.X[p.numberOfPoints] = wX;
				p.Y[p.numberOfPoints++] = wY;
			}
		}
		//first outside next outside
		else if (buff.X[v1] > vbRightX && buff.X[v2] <= vbRightX)
		{
			if (buff.X[v2] == vbRightX)
			{
				p.X[p.numberOfPoints] = buff.X[v2];
				p.Y[p.numberOfPoints++] = buff.Y[v2];
			}
			else
			{
				float wY = buff.Y[v2] + ((buff.Y[v1] - buff.Y[v2]) / (buff.X[v1] - buff.X[v2]))*(vbRightX - buff.X[v2]);
				float wX = vbRightX;
				p.X[p.numberOfPoints] = wX;
				p.Y[p.numberOfPoints++] = wY;
				p.X[p.numberOfPoints] = buff.X[v2];
				p.Y[p.numberOfPoints++] = buff.Y[v2];
			}
		}
	}

	buff.numberOfPoints = 0;								//reset buff to be empty

	//clip top copy into buff
	for (int i = 0; i < p.numberOfPoints; i++)
	{
		int v1 = 0;
		int v2 = 0;
		if (i == p.numberOfPoints - 1)
		{
			v1 = i;
			v2 = 0;
		}
		else
		{
			v1 = i;
			v2 = i + 1;
		}
		//both inside
		if (p.Y[v1] <= vbTopY && p.Y[v2] <= vbTopY)
		{
			buff.X[buff.numberOfPoints] = p.X[v2];
			buff.Y[buff.numberOfPoints++] = p.Y[v2];
		}
		//first inside next outside
		else if (p.Y[v1] <= vbTopY && p.Y[v2] > vbTopY)
		{
			if (p.Y[v1] == vbTopY)
			{

			}
			else
			{
				float wX = p.X[v1] + ((p.X[v2] - p.X[v1]) / (p.Y[v2] - p.Y[v1]))*(vbTopY - p.Y[v1]);
				float wY = vbTopY;
				buff.X[buff.numberOfPoints] = wX;
				buff.Y[buff.numberOfPoints++] = wY;
			}
		}
		//first outside next outside
		else if (p.Y[v1] > vbTopY && p.Y[v2] <= vbTopY)
		{
			if (p.Y[v2] == vbTopY)
			{
				buff.X[buff.numberOfPoints] = p.X[v2];
				buff.Y[buff.numberOfPoints++] = p.Y[v2];
			}
			else
			{
				float wX = p.X[v1] + ((p.X[v2] - p.X[v1]) / (p.Y[v2] - p.Y[v1]))*(vbTopY - p.Y[v1]);
				float wY = vbTopY;
				buff.X[buff.numberOfPoints] = wX;
				buff.Y[buff.numberOfPoints++] = wY;
				buff.X[buff.numberOfPoints] = p.X[v2];
				buff.Y[buff.numberOfPoints++] = p.Y[v2];
			}
		}
	}

	p.numberOfPoints = 0;							//reset p to be empty

	//clip bottom copy in to p
	for (int i = 0; i < buff.numberOfPoints; i++)
	{
		int v1 = 0;
		int v2 = 0;
		if (i == buff.numberOfPoints - 1)
		{
			v1 = i;
			v2 = 0;
		}
		else
		{
			v1 = i;
			v2 = i + 1;
		}
		//both inside
		if (buff.Y[v1] >= vbBottomY && buff.Y[v2] >= vbBottomY)
		{
			p.X[p.numberOfPoints] = buff.X[v2];
			p.Y[p.numberOfPoints++] = buff.Y[v2];
		}
		//first inside next outside
		else if (buff.Y[v1] >= vbBottomY && buff.Y[v2] < vbBottomY)
		{
			if (buff.Y[v1] == vbBottomY)
			{

			}
			else
			{
				float wX = buff.X[v2] + ((buff.X[v1] - buff.X[v2]) / (buff.Y[v1] - buff.Y[v2]))*(vbBottomY - buff.Y[v2]);
				float wY = vbBottomY;
				p.X[p.numberOfPoints] = wX;
				p.Y[p.numberOfPoints++] = wY;
			}
		}
		//first outside next outside
		else if (buff.Y[v1] < vbBottomY && buff.Y[v2] >= vbBottomY)
		{
			if (buff.Y[v2] == vbBottomY)
			{
				p.X[p.numberOfPoints] = buff.X[v2];
				p.Y[p.numberOfPoints++] = buff.Y[v2];
			}
			else
			{
				float wX = buff.X[v2] + ((buff.X[v1] - buff.X[v2]) / (buff.Y[v1] - buff.Y[v2]))*(vbBottomY - buff.Y[v2]);
				float wY = vbBottomY;
				p.X[p.numberOfPoints] = wX;
				p.Y[p.numberOfPoints++] = wY;
				p.X[p.numberOfPoints] = buff.X[v2];
				p.Y[p.numberOfPoints++] = buff.Y[v2];
			}
		}
	}


	delete buff.X;							//avoid memory leak
	delete buff.Y;

	return p;								//return cliped polygon
}

//takes a PolygonArray struct along with it current pos and rotates it by theta
//does three 3x3 matrix multiplications per vertex then returns new polygon
PolygonArray rotatePolygon(PolygonArray p, float xPos, float yPos, float theta)
{
															//	1	0	-xPos
															//	0	1	-yPos
															//	0	0	1
	float translateMatrix[3][3] = { 1, 0, -xPos, 0, 1, -yPos, 0, 0, 1 };

															//	cos	-sin	0
															//	sin	 cos	0
															//	0	 0		1
	float rotationMatrix[3][3] = { cos(theta), -sin(theta), 0, sin(theta), cos(theta), 0, 0, 0, 1 };

	float tempPos[3];										//the product vector

	for (int i = 0; i < p.numberOfPoints; i++)				//Translation to origin
	{
		for (int j = 0; j < 3; j++)
		{
			tempPos[j] = p.X[i] * translateMatrix[j][0] + p.Y[i] * translateMatrix[j][1] + translateMatrix[j][2];
		}
		p.X[i] = tempPos[0];
		p.Y[i] = tempPos[1];
	}
	

	for (int i = 0; i < p.numberOfPoints; i++)				//roation about origin
	{
		for (int j = 0; j < 3; j++)
		{
			tempPos[j] = p.X[i] * rotationMatrix[j][0] + p.Y[i] * rotationMatrix[j][1] + rotationMatrix[j][2];
		}
		p.X[i] = tempPos[0];
		p.Y[i] = tempPos[1];
	}
	
	translateMatrix[0][2] *= -1;							//change translation matrix to go back to
	translateMatrix[1][2] *= -1;							//desired pos

	for (int i = 0; i < p.numberOfPoints; i++)				//translation from origin to start pos
	{
		for (int j = 0; j < 3; j++)
		{
			tempPos[j] = p.X[i] * translateMatrix[j][0] + p.Y[i] * translateMatrix[j][1] + translateMatrix[j][2];
		}
		p.X[i] = tempPos[0];
		p.Y[i] = tempPos[1];
	}
	



	return p;												//return rotated polygon
}

//set the starting pos of the boids
void initBoids()
{
	for (int i = 0; i < NUMBER_OF_BOIDS; i++)
	{
		float r = rand() % 600;				//set them to a random pos on the screen
		boidSchool[i].x = r;
		r = rand() % 600;
		boidSchool[i].y = r;
		boidSchool[i].velX = r/600;			//and give them a random velocity
		boidSchool[i].velY = r/600;
	}
}

//update the pos of each boid
void updateBoids()
{
										
	for (int i = 0; i < NUMBER_OF_BOIDS; i++)				//loop through each boid
	{
		float sepX = 0;										//seperation vector
		float sepY = 0;

		float cohX = 0;										//cohesion vector
		float cohY = 0;

		float averageX = 0;									//average pos
		float averageY = 0;

		float aliX = 0;										//average direction of travel vector
		float aliY = 0;

		int numberOfNear = 0;								//to get the average pos and direction

		for (int j = 0; j < NUMBER_OF_BOIDS; j++)			//check every other boid
		{													//if that boid is with in sight or original
			if (boidSchool[i].x > boidSchool[j].x - BOID_SIGHT_RANGE && boidSchool[i].x < boidSchool[j].x + BOID_SIGHT_RANGE && boidSchool[i].y > boidSchool[j].y - BOID_SIGHT_RANGE && boidSchool[i].y < boidSchool[j].y + BOID_SIGHT_RANGE)
			{
				averageX += boidSchool[j].x;				//record its pos
				averageY += boidSchool[j].y;
				aliX += boidSchool[j].velX;					//and dirction
				aliY += boidSchool[j].velY;
															//get distance to that boid to determin seperation vector
															//adding 0.01 to make sure there is not divied by 0 error
				float distance = sqrt(pow((boidSchool[i].x - boidSchool[j].x), 2) + pow((boidSchool[i].y - boidSchool[j].y), 2)) +0.01;

															//get seperation vector
				sepX += (boidSchool[i].x - boidSchool[j].x) / (distance*BOID_DISTANCE_MOD);
				sepY += (boidSchool[i].y - boidSchool[j].y) / (distance*BOID_DISTANCE_MOD);
				numberOfNear++;								//keep track of number of boids near
			}
		}

		averageX /= numberOfNear;							//get averages to get pos vector and ailinment vector
		averageY /= numberOfNear;
		aliX /= numberOfNear;
		aliY /= numberOfNear;

		cohX = averageX - boidSchool[i].x;
		cohY = averageY - boidSchool[i].y;

															//update teh boid velocity to match to the weighted sum of
															//previous vel, seperation vector, alinment vector, and cohesion vector
		boidSchool[i].velX = (boidSchool[i].velX *BOID_CURENT_HEADING) + (sepX * BOID_SEPARATION) + (aliX * BOID_ALIGNMENT) + (cohX * BOID_COHESION);
		boidSchool[i].velY = (boidSchool[i].velY *BOID_CURENT_HEADING) + (sepY * BOID_SEPARATION) + (aliY * BOID_ALIGNMENT) + (cohY * BOID_COHESION);

															//if near the walls of tank agust the velocity
		if (boidSchool[i].x > 780) boidSchool[i].velX -= BOID_WALL_FORCE;
		else if (boidSchool[i].x < 20) boidSchool[i].velX += BOID_WALL_FORCE;
		if (boidSchool[i].y > 580) boidSchool[i].velY -= BOID_WALL_FORCE;
		else if (boidSchool[i].y < 20) boidSchool[i].velY += BOID_WALL_FORCE;

															//get unit vector and set boid at maxspeed
		float mag = sqrt(pow(boidSchool[i].velX, 2) + pow(boidSchool[i].velY, 2));
		boidSchool[i].velX = boidSchool[i].velX / mag * BOID_MAX_SPEED;
		boidSchool[i].velY = boidSchool[i].velY / mag * BOID_MAX_SPEED;
		
															//update boid velocity
		boidSchool[i].x += boidSchool[i].velX;
		boidSchool[i].y += boidSchool[i].velY;
	}

}

//create the framebuffer and textute objects
bool initBlurTexture()
{
	bool retVal = true;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// generate texture
	//only useing a rgb texture no depth or stencil buffer
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, canvas_Width, canvas_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &bluredTexture);


	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	
	
	//check if the framebuffer object is ready to use
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		retVal = false;
	}
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return retVal;
}

//get the pixels from the texture and applys a box blur and color corection
void blurFrame()
{
																	//read the pixels into an array
	glReadPixels(0, 0, canvas_Width, canvas_Height, GL_RGB, GL_UNSIGNED_BYTE, textureBuffer);

																	//loop through and blur horizantaly
	for (int i = blurRadius * 3; i < (canvas_Width*canvas_Height * 3) - 3*blurRadius; i++)
	{
		int buff = 0;
		for (int j = -blurRadius; j <= blurRadius; j++)				//get the sum of pixels to the left and right
		{
			buff += textureBuffer[i + j * 3];
		}															//divide by the number of pixels to get average
		textureBuffer[i] = (buff / (2*blurRadius+1)) + brightnessAgustment;	//also add to the brightness
	}

																	//loop through to blur virticaly
	for (int i = blurRadius * canvas_Width * 3; i < (canvas_Width*canvas_Height * 3) - canvas_Width * 3*blurRadius; i++)
	{
		int buff = 0;
		for (int j = -blurRadius; j <= blurRadius; j++)
		{
			buff += textureBuffer[i + j * canvas_Width * 3];
		}
		textureBuffer[i] = buff / (2*blurRadius+1) + brightnessAgustment;
	}

																	//do the green corection
																	//start one 1 which is a green pixel
																	//+=3 to jump from green to green
	for (int i = 1; i < (canvas_Width*canvas_Height * 3) - 3 * blurRadius; i += 3)
	{
		textureBuffer[i] += greenAgustment;
	}

																	//set bluredText to be current
	glBindTexture(GL_TEXTURE_2D, bluredTexture);
																	//convet the pixel array to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, canvas_Width, canvas_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);								//deselect texture
}

