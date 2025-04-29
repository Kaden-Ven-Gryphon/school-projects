//CS 445/545 Prog 2 for Kaden Beaver
/***********************************************************************************
*
*	Kaden Beaver	09.28.2021	Program 2
*
*	EXTRA CREDIT:	'c' Key changes color to oragne and back
					added clock timer in top right corner
					ant rotates in the dirction of travel (Also changed ant to eat at front shpere not middle)
					user and reset game after win or lose
					
	DESIGN:			The program has four functions to draw objects three callback functions
					and one utility function.  
					The draw functions draw: the ant, the sugur cube, the clock, and a string
					the draw functions use global varible, or arguments to place and rotate objects
					The callback functions are:
								glutDisplayFunc = displayFunction
								glutTimerFunc	= timerFunction
								glutKeyboardFunc= keyboardFunction
					each call back function uses swtich statment to determine what should be done
					for example the keyboardFunction has a switch statment for which key was pressed
					the utitly function is for figuring out if the ant mouth is colliding with sugur cube

					The program starts be registuring a timer for the clock and
					registering keyboard and diplayfunctions

					when a key is pressed the keyboard is disabled (to prevent input buffering) and
					the handler updates the global verables and ques a redisplay event

					the timer controls the two step movment and re enables the keyboard
					the timer also updates the clock pos and check for win or lose cons

					the displayFunction draws one of three sences, the ant and sugur cube,
					the win screen, or the lose screen

					When one of the ending screens is reached the user can press and key to restart

************************************************************************************/


//##################################################	INCLUDES	#######################################################################
//#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup.h"
#include "pch.h"


//##################################################	FUNCTIONS	#######################################################################
//Drawing funcs
void drawAnt(int x, int y, int z, int theta);			//This draws an ant at the given pos and rotation
void drawFood(int x, int y, int z);						//This draws a YUM sugur cube at given pos
void drawClock(int theta);								//This draws a clock in the top right conor with the hand at theta degrees
void writeBitmapString(void *font, char *string);		//This writes out a string in bitmap charactes,  THIS IS FROM THE GUHA TEXTBOOK PG 66

//Call back funcs
void displayFunction();									//THIS IS THE GLUTDISPLAYFUNC CALL BACK FUNCTION
void timerFunction(int id);								//THIS IS THE GLUTTIMERFUNC CALL BACK FUNCTION
void keyboardFunction(unsigned char c, int x, int y);	//THIS IS THE GLUTKEYBOARDFUNC CALL BACK FUNCTION

//Untilites funcs
bool isMouthOnFood(int x, int y);						//This detects if the the mouth of the ant is within a sugur cube at given pos


//##################################################	GLOBAL VARIBLES AND DEFINES		###################################################

//The Canvas specifications
#define canvas_Width 640
#define canvas_Height 640
char canvas_Name[] = "DO YOU WANT ANTS?  CAUSE THIS IS HOW YOU GET ANTS!";

//The Sugur cube pos
#define SUGUR_CUBE_X -200
#define SUGUR_CUBE_Y 0

//The Ant Pos and status
int antPosX = 150;
int antPosY = 0;
int antTheta = 0;
bool antIsOrange = false;

//The Clock pos
int clockTheta = 0;

//The clock hand pos
int screenID = 0;


//##################################################	MAIN	############################################################################
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);								//Set up glut
	my_setup(canvas_Width, canvas_Height, canvas_Name);	//The function for given header

	glutDisplayFunc(displayFunction);					//register call back fuctnion for DisplayFunc
	glutKeyboardFunc(keyboardFunction);					//register call back fucntion for KeyboardFunc
	glutTimerFunc(250, timerFunction, 3001);			//Start Timer for moving clock hand (timer IDs listed with timer function)
	glutMainLoop();										//Start the main loop

	return 0;
}


//##################################################	DRAWING FUNCTIONS	################################################################

//draw a sugur cube at the given pos
void drawFood(int x, int y, int z)
{
	char yumText[] = "YUM";								//string for write function
	glColor3f(0.9, 0.9, 0.6);							//set pen color to cream yellow
	glLoadIdentity();									//reset transform matrix
	glTranslatef(x, y, z);								//set pos to given pos
	glutWireCube(40);									//draw cube of size 40
	glRasterPos3f(-12.0, -4.0, 20.0);					//set raster pos for leters
	writeBitmapString(GLUT_BITMAP_8_BY_13, yumText);	//write YUM starting at raster pos
	
}

//draw a clock in the right top coner with a hand pointing at theta degrees
void drawClock(int theta)
{
	//varible for drawing the clock
	int radius = 25;									
	double t = 0;		//the theta for drawing a circle
	int numVert = 20;	//the number of sides for the 'circle'
	int xpos = 290;
	int ypos = 290;

	glLoadIdentity();									//reset the transform matix
	glTranslatef(xpos, ypos, -200);						//move to top right coner
	glColor3f(0.0, 0.0, 0.0);							//set pen color to black
	glBegin(GL_LINE_LOOP);								//begin line loop
		for (int i = 0; i < numVert; i++)				//THIS CODE IS FROM THE GUHA TEXT BOOK PG 36
		{
			glVertex3f(radius*cos(t), radius * sin(t), 0);	//draw vertex based on t and radius
			t += 2 * M_PI / numVert;					//increment t by fraction of cirle based on number of vetex
		}
	glEnd();											//End Line loop
	glRotatef(theta, 0.0, 0.0, 1.0);					//set rotation based on the hands theta
	glBegin(GL_LINES);									//draw the clock hand
		glVertex3i(0, 0, 0);
		glVertex3i(0, radius, 0);
	glEnd();
}


//draw ant at pos and rotation given
//color based on golbal variable antIsOrange
void drawAnt(int x, int y, int z, int theta)			
{
	if (antIsOrange)									//if flag is set
	{
		glColor3f(0.7, 0.4, 0.0);						//set pen color to orange
	}
	else glColor3f(0.0, 0.0, 0.0);						//else set the pen color to black
	
	glLoadIdentity();									//reset the transform matix
	glTranslatef(x, y, z);								//move to ant center
	glRotatef(theta, 0.0, 0.0, 1.0);					//set ant rotation
	glutWireSphere(25, 20, 20);							//draw center sphere
	glTranslatef(60, 0, 0);								//move to ant head
	glutWireSphere(25, 20, 20);							//draw ant right sphere
	glTranslatef(-120, 0, 0);							//move to ant tail
	glutWireSphere(25, 20, 20);							//draw ant left sphere
	glTranslatef(60, 0, 0);								//move back to center of ant
	glBegin(GL_LINES);									//begin drawing legs
		glVertex3i(25, 0, 0);							//center to right segment
		glVertex3i(35, 0, 0);
		glVertex3i(-25, 0, 0);							//center to left segment
		glVertex3i(-35, 0, 0);
		glVertex3i(0, 25, 0);							//middle top leg
		glVertex3i(0, 55, 0);
		glVertex3i(0, -25, 0);							//middle bottom leg
		glVertex3i(0, -55, 0);
		glVertex3i(60, 25, 0);							//right top leg
		glVertex3i(85, 50, 0);
		glVertex3i(60, -25, 0);							//right bottom leg
		glVertex3i(85, -50, 0);
		glVertex3i(-60, 25, 0);							//left top leg
		glVertex3i(-85, 50, 0);
		glVertex3i(-60, -25, 0);						//left bottom leg
		glVertex3i(-85, -50, 0);
	glEnd();											//End of legs
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
	char youLoseStr[] = "You Lose!";					//set strings for end screens
	char youWinStr[] = "You Win!";
	char restartStr[] = "Press any button to restart.";

	//ID 0: Ant and sugur cube
	//ID 1: the lose screen
	//ID 2: the win screen
	switch (screenID)									//draw sceene based on screenID
	{
	case 0:												//ID 0: draw and and sugur
		glClearColor(0.3, 0.7, 0.3, 1.0);				//set background to green
		glClear(GL_COLOR_BUFFER_BIT);
		drawFood(SUGUR_CUBE_X, SUGUR_CUBE_Y, -200);		//draw the food
		drawAnt(antPosX, antPosY, -200, antTheta);		//draw the ant
		drawClock(clockTheta);							//draw the clock
		glFlush();										//ask OS to update canvas
		break;

	case 1:												//ID 1: lose screen
		glClearColor(0.2, 0.2, 0.2, 1.0);				//set background to grey
		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(0.0, 0.0, 0.0);						//set pen color to black
		glLoadIdentity();								//clear transfrom matix
		//These strings are not perfect center,  I am
		//sure there is a way to calculat the number
		//of pixels of a string before hand so you can
		//calculate where to start to have centered
		//text but I did not look for it
		glRasterPos3i(-70, 0, -200);					//set start pos for string
		writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, youLoseStr);	//draw string
		glRasterPos3i(-135, -20, -200);					//set start pos for string
		writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, restartStr);	//draw string
		glFlush();										//ask OS to draw Canvas
		break;

	case 2:												//ID 2: win screen
		glClearColor(0.2, 0.8, 0.2, 1.0);				//set background to green
		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(0.0, 0.0, 0.0);						//set pen color to black
		glLoadIdentity();								//reset the transform matix
		glRasterPos3i(-70, 0, -200);					//set start pos of string
		writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, youWinStr);	//draw string
		glRasterPos3i(-135, -20, -200);					//set start pos of string
		writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, restartStr);	//draw string
		glFlush();										//ask OS to draw Canvas
		break;
	}
	
}

					
void timerFunction(int id)								//glutTimerFunc call back
{
	//ID h:			the h key was pressed
	//ID 1000+h:	second move for h
	//ID 2000+h:	enable keyboard after move
	//SIMULAR FOR ID j, u, n
	//ID 3001:		update the clock hand and check for win con
	//ID 3002:		enable key board to reset game

	switch (id)
	{
	case 'h':											//h key was pressed
		antPosX -= 5;									//first move
		antTheta = 0;									//set direction
		glutPostRedisplay();							//que a display event
		glutTimerFunc(60, timerFunction, 1000 + 'h');	//que timer for second move in 60 ms
		break;
	case (1000+'h'):									//second move of h
		antPosX -= 5;									//second move
		glutPostRedisplay();							//que display event
		glutTimerFunc(60, timerFunction, 2000 + 'h');	//que timer for reenable keyboard
		break;
	case (2000+'h'):									//reenable keyboard for h
		glutKeyboardFunc(keyboardFunction);				//enable
		break;


	case 'j':											//Same as above for j, n, u
		antPosX += 5;
		antTheta = 180;
		glutPostRedisplay();
		glutTimerFunc(60, timerFunction, 1000 + 'j');
		break;
	case (1000+'j'):
		antPosX += 5;
		glutPostRedisplay();
		glutTimerFunc(60, timerFunction, 2000 + 'j');
		break;
	case (2000+'j'):
		glutKeyboardFunc(keyboardFunction);
		break;


	case 'u':
		antPosY += 5;
		antTheta = 270;
		glutPostRedisplay();
		glutTimerFunc(60, timerFunction, 1000 + 'u');
		break;
	case (1000 + 'u'):
		antPosY += 5;
		glutPostRedisplay();
		glutTimerFunc(60, timerFunction, 2000 + 'u');
		break;
	case (2000 + 'u'):
		glutKeyboardFunc(keyboardFunction);
		break;


	case 'n':
		antPosY -= 5;
		antTheta = 90;
		glutPostRedisplay();
		glutTimerFunc(60, timerFunction, 1000 + 'n');
		break;
	case (1000 + 'n'):
		antPosY -= 5;
		glutPostRedisplay();
		glutTimerFunc(60, timerFunction, 2000 + 'n');
		break;
	case (2000 + 'n'):
		glutKeyboardFunc(keyboardFunction);
		break;

	case 3001:											//ID 3001: update hand and check win con

		
		if (isMouthOnFood(SUGUR_CUBE_X, SUGUR_CUBE_Y))	//if ant mouth on food
		{
			glutKeyboardFunc(NULL);						//diable keyboard
			screenID = 2;								//set screen to win
			glutPostRedisplay();						//que rediplay event
			glutTimerFunc(2000, timerFunction, 3002);	//que timer to enable keyboard
			break;
		}												//else update clock hand


		if (clockTheta <= -360)							//if hand has rotated 360 degrees
		{
			glutKeyboardFunc(NULL);						//disable the keyboard
			screenID = 1;								//set screen to lose
			glutPostRedisplay();						//que display event
			glutTimerFunc(2000, timerFunction, 3002);	//que timer to enable keyboard
			break;
		}												//else move hand
		clockTheta -= 3;								//change theta by 3 degrees
		glutTimerFunc(250, timerFunction, 3001);		//que timer to move hand again in quarter of sec
		glutPostRedisplay();							//que display event
		break;
	case 3002:											//reenable the keyboard to reset the game
		glutKeyboardFunc(keyboardFunction);
		break;
	}
}

void keyboardFunction(unsigned char c, int x, int y)	//glutKeyboardFunc call back
{
	if (screenID != 0)									//if the screen is not the ant screen
	{
		antPosX = 150;									//reset the variable to resset the game
		antPosY = 0;
		antTheta = 0;
		antIsOrange = false;

		clockTheta = 0;

		screenID = 0;

		glutTimerFunc(250, timerFunction, 3001);		//que up the start of the clock hand

	}
	else												//else it is the ant screen
	{
		switch (c)										//switch base on key pressed
		{
		case 'h':										//h key
			glutTimerFunc(60, timerFunction, 'h');		//que timer with id h
			glutKeyboardFunc(NULL);						//disable keyboard so there is no input buffering
			break;
		case 'j':										//same for key j, u, n
			glutTimerFunc(60, timerFunction, 'j');
			glutKeyboardFunc(NULL);
			break;
		case 'u':
			glutTimerFunc(60, timerFunction, 'u');
			glutKeyboardFunc(NULL);
			break;
		case 'n':
			glutTimerFunc(60, timerFunction, 'n');
			glutKeyboardFunc(NULL);
			break;


		case 'c':										//key c
			if (antIsOrange)							//swap the flag for ant color
			{
				antIsOrange = false;
			}
			else antIsOrange = true;
			glutPostRedisplay();						//que display event
			break;
		}
	}	
}


//##################################################	UTILITY FUNCTIONS	####################################

bool isMouthOnFood(int x, int y)						//check if ant mouth is in cube at given pos
{
	int mouthPosX = 0;									//mouth pos
	int mouthPosY = 0;

	switch (antTheta)									//set the mouth pos based on the ants rotaiton
	{
	case 0:												//left
		mouthPosX = antPosX - 80;
		mouthPosY = antPosY;
		break;
	case 90:											//down
		mouthPosY = antPosY - 80;
		mouthPosX = antPosX;
		break;
	case 180:											//right
		mouthPosX = antPosX + 80;
		mouthPosY = antPosY;
		break;
	case 270:											//up
		mouthPosY = antPosY + 80;
		mouthPosX = antPosX;
	}
														//check if mouth is within cube, must be atleast 5 units into cube
	if (mouthPosX < x + 15 && mouthPosX > x - 15 && mouthPosY < y + 15 && mouthPosY > y - 15) return true;
	else return false;
}
