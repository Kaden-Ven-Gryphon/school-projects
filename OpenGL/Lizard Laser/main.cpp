//CS 445/545 Prog  for Kaden Beaver
/***********************************************************************************
*
*	Kaden Beaver	11.16.2021	Program 4
*
*	EXTRA CREDIT:	Added to the lizard model.  it has a colored nose and a googly eye that watches the player.
					Lizard now moves like the catapiller it looks like
					The Laser now interacts with the lizards.
					when hit the lizard shrinks by 50 units and dies if it is less than 20 units
					lizard grows every chanel it drops and speeds up
					there can be mulitple lizards up to 10



	DESIGN:			This program is set up into drawing functions that draw an object to the screen, utility
					functions that a do a non visable task such as reseting flags, or seting up the light
					and callback functions that are registered to the glut events.

					There are three call back functions

					keyboardFunction is the call back function for glutKeyboardFunc.  It looks for one of
					three key presses, j and k for left and right movment and space bar to fire the laser
					the keyboardFunciton also detirmines if the laser hits a lizards and shrink that lizard

					timerFunction is the callback function for glutTimerFunc.  it has three main loops
					the first is TIMER_ID_FRAME_UPDATE this is run every 50 ms to get 20 frames a second
					the second is TIMER_ID_UPDATE_LIZARDS this updates each lizards pos and length, it 
					also checks if the game is lost or if a lizard is dead
					the third timer is TIMER_ID_SPAWN_LIZARD which spawns a lizard every 5 seconds
					The other timer ids are to animate the laser duration, and move the gun left and right

					the last callback function is the displayFunction for glutDisplayFunc
					this one clears the screen, draws all the elements to the screen and switches the buffer

					the drawing functions that the displayFunction calls include:

					drawLizard: which uses a givin xy pos, direction, and length to position and draw the
					lizard it uses the sine wave to give the lizards their curve

					drawHuman: which draws a human pulling a lever to the left or right based on isLeft

					drawGun: which just draws the gun at pos x

					drawLaser: which finds the distace from thr gun to the first lizard that itercpts it
					and draws the line to it

					drawScore: which writes out the score to the top middle of the screen

					drawGameOver: which write GAME OVER and puts a red X over the human

					writeBitmapString: which is the function from the text book to write a c array string

					The two utility functions are initShader to set up the light and lizards, and resetLizard
					which spawns a lizard at the top right of length 99

					There are global variables and defines for the posistions and states of many of the objects
					For the lizards there is a set of parallel arrays to store their xy pos, lengths, and state
					There are flags for if the human is left or right, if the game is over, and if the laser
					is ready to fire.  The is a varible for the x pos of the gun and the score.

					In main the callback functions are registers, the light is created and the lizards set,
					the three main loops are started, and glutMainLoop is started.


************************************************************************************/


//##################################################	INCLUDES	#######################################################################
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup.h"
#include "pch.h"


//##################################################	FUNCTIONS	#######################################################################
//Drawing funcs
void drawLizard(float x, float y, int length, bool leftFace, int maxLength);
void drawHuman(bool isLeft);
void drawGun(float x);
void drawLaser(float alpha);
void drawScore();
void drawGameOver();
void writeBitmapString(void *font, char *string);

//Call back funcs
void displayFunction();									//THIS IS THE GLUTDISPLAYFUNC CALL BACK FUNCTION
void timerFunction(int id);								//THIS IS THE GLUTTIMERFUNC CALL BACK FUNCTION
void keyboardFunction(unsigned char c, int x, int y);	//THIS IS THE GLUTKEYBOARDFUNC CALL BACK FUNCTION

//Untilites funcs
void initShader();
void resetLizard(int index);

//##################################################	GLOBAL VARIBLES AND DEFINES		###################################################

//The Canvas specifications
#define canvas_Width 600
#define canvas_Height 600
char canvas_Name[] = "Laser the Lizard! (That is a Lizard, right?)";

//timer ids
#define TIMER_ID_FRAME_UPDATE 0
#define TIMER_ID_UPDATE_LIZARDS 1
#define TIMER_ID_SPAWN_LIZARD 2
#define TIMER_ID_GUN_RIGHT 5
#define TIMER_ID_GUN_LEFT 6
#define TIMER_ID_LASER_FADE 7

//Lizard
#define MAX_LIZARDS 10
int lizardNums = 0;
int lizardState[MAX_LIZARDS];
float lizardX[MAX_LIZARDS];
float lizardY[MAX_LIZARDS];
bool lizardLookLeft[MAX_LIZARDS];
int lizardLength[MAX_LIZARDS];
int lizardMaxLength[MAX_LIZARDS];
float lizardSpeed[MAX_LIZARDS];

//human
#define HUMAN_X -290.0
#define HUMAN_Y -280.0
bool humanIsLeft = false;

//gun
#define GUN_BOUND 250
#define GUN_MOVE_MS 5
#define GUN_Y -290.0
float gunX = 0.0;

//Laser
float laserAlpha = 1;
bool laserReady = true;

//Score
int score = 0;
bool gameOver = false;



//##################################################	MAIN	############################################################################
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);								//Set up glut
	my_setup(canvas_Width, canvas_Height, canvas_Name);	//The function for given header

	
	glutDisplayFunc(displayFunction);					//register call back fuctnion for DisplayFunc
	glutKeyboardFunc(keyboardFunction);					//register call back fucntion for KeyboardFunc
	
	initShader();										//set up the lights and lizards
	resetLizard(0);										//spawn the first lizard

	glutTimerFunc(50, timerFunction, TIMER_ID_UPDATE_LIZARDS);	//start timer for lizard updates
	glutTimerFunc(50, timerFunction, TIMER_ID_FRAME_UPDATE);	//start timer for 20 frams a second
	glutTimerFunc(5000, timerFunction, TIMER_ID_SPAWN_LIZARD);	//start timer to spawn lizard every 5 seconds

	glutMainLoop();										//Start the main loop

	return 0;
}


//##################################################	DRAWING FUNCTIONS	################################################################


//Draw a lizard using the givin stats
void drawLizard(float x, float y, int length, bool leftFace, int maxLength)
{
	int squares = (length - 24) / 15;								//calculate the number of full squares
	float remainder = (length - 24) % 15;							//find the remining length

	float matAmbAndDif1[] = { 0.2, 0.7, 0.2, 1.0 };//Bright Green
	float matAmbAndDif2[] = { 0.8, 0.8, 0.2, 1.0 };//Yellow
	float matAmbAndDif3[] = { 1.0, 1.0, 1.0, 1.0 };//White
	float matAmbAndDif4[] = { 0.0, 0.0, 0.0, 1.0 };//Black
	float matAmbAndDif5[] = { 0.1, 0.4, 0.1, 1.0 };//DarkGreen
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
																	//set up material to yellow
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


	glLoadIdentity();

	if (leftFace)													//if lizard is facing left
	{
		glTranslatef(x + ((length - 24) / 2), y - 7.5, 0.0);		//move it to pos
		glScalef(-1.0, 1.0, 1.0);									//mirror the lizard over x axis
	}

	else glTranslatef(x-((length-24)/2), y-7.5, 0.0);				//else move it to pos normaly
																	//lizard is translated so that x,y is the center of the lizard


	glBegin(GL_TRIANGLES);											//draw the yellow tail
	glVertex3f(0.0, 0.0, -150.0);
	glVertex3f(0.0, 15.0, -150.0);
	glVertex3f(-12.0, 7.5, -150.0);
	glEnd();
																	//switch color to dark green
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif5);

	glBegin(GL_LINES);												//draw the back legs
	glVertex3f(7.5, 0.0, -150.0);
	glVertex3f(13.5, -6.0, -150.0);
	glVertex3f(7.5, 0.0, -150.0);
	glVertex3f(1.5, -6.0, -150.0);
	glEnd();
																	//switch the color to light green
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);

	
	for (int i = 0; i < squares; i++)								//draw each of the body squares
	{
		glPushMatrix();
		//this was the old method to make the lizards bend
		//this is based of a lineer functions so the lizards ended up
		//looking very pointy as the moved instead of a smooth arch
		//I replaced this with a sine function
		/*
		float slope = ((lizardMaxLength - lizardLength)*0.6) / ((lizardLength - 24) / 2);
		if (i * 15 <= ((lizardLength - 24) / 2))
		{
			glTranslatef(i * 15.0, i * 15 * slope, 0.0);
		}
		else glTranslatef(i * 15, ((i * 15 * -slope) + ((lizardMaxLength - lizardLength))), 0.0);
		*/

		//I wanted a smooth curve so I opened the desmos graphing calculator to mess
		//around.  I tried a sqrt functions but I needed to control the height of the curve
		//so that was not working.  I needed to control the hight and width of the curve
		//which I relized was just the amplitude and frequacy of a sine wave.

		//A is the amplitude of the wave it is based of the lizards current length and max length
		float A = (maxLength - length)*0.4;
		//B is the frequency of the wave, it is the inverse of the number of PI = to the length of the lizard
		float B = 1/((length-24)/M_PI);
		glTranslatef(i*15.0, A*sin(i*15.0*B), 0.0);					//the square x pos is based on which number square it is
																	//and its y pos is based off the sine wave using A and B

		glBegin(GL_TRIANGLES);										//draw square
		glVertex3f(0.0, 0.0, -150.0);
		glVertex3f(15.0, 0.0, -150.0);
		glVertex3f(15.0, 15.0, -150.0);
		glVertex3f(0.0, 0.0, -150.0);
		glVertex3f(15.0, 15.0, -150.0);
		glVertex3f(0.0, 15.0, -150.0);
		glEnd();
		glPopMatrix();
	}																//all full square have been drawn
	glTranslatef(squares * 15, 0.0, 0.0);
	glBegin(GL_TRIANGLES);											//draw the remainder rectangle
	glVertex3f(0.0, 0.0, -150.0);
	glVertex3f(remainder, 0.0, -150.0);
	glVertex3f(remainder, 15.0, -150.0);
	glVertex3f(0.0, 0.0, -150.0);
	glVertex3f(remainder, 15.0, -150.0);
	glVertex3f(0.0, 15.0, -150.0);
	glEnd();
	glTranslatef(remainder, 0.0, 0.0);
																	//set the color to dark green
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif5);

	glBegin(GL_LINES);												//draw the front legs
	glVertex3f(-7.5, 0.0, -150.0);
	glVertex3f(-1.5, -6.0, -150.0);
	glVertex3f(-7.5, 0.0, -150.0);
	glVertex3f(-13.5, -6.0, -150.0);
	glEnd();
																	//set color to yellow
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);
							
	glBegin(GL_TRIANGLES);											//draw head
	glVertex3f(0.0, 0.0, -150.0);
	glVertex3f(12, 7.5, -150.0);
	glVertex3f(0.0, 15.0, -150.0);
	glEnd();

	glTranslatef(0.0, 15.0, 0.0);									//set color to white
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif3);
	glutSolidSphere(6, 20, 20);										//draw eye ball
	//MOVE EYE TO LOOK AT OBJECT
	int direction = 0;
	if (leftFace) direction = -1;
	else direction - 1;
	float targetX = HUMAN_X - (x+((length/2)*direction));			//get vector from eye to human
	float targetY = HUMAN_Y - y;									//and get its length
	float targetLength = sqrt((targetX*targetX + targetY * targetY));
	targetX /= targetLength;										//get unit vector
	targetY /= targetLength;

	if (leftFace) targetX *= -1;									//agust for if the lizard is mirrored

	glTranslatef(targetX * 3, targetY * 3, 0.0);					//move 3 units along the unit vector from eye to human
																	//change color to black
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif4);
	glutSolidSphere(2, 10, 10);										//draw pupil
}

void drawHuman(bool isLeft)
{
	float matAmbAndDif1[] = { 0.8, 0.4, 0.1, 1.0 };//red
	float matAmbAndDif2[] = { 0.2, 0.4, 0.9, 1.0 };//blue
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
																		//set the matial to blue
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


	glLoadIdentity();
	

	glTranslatef(HUMAN_X, HUMAN_Y, -150);								//move to humans pos
	if (isLeft)															//if the human is leaning left draw this
	{
		//Lever
		glBegin(GL_TRIANGLES);				//the base of the lever
		glVertex3f(16.0, 0.0, 0.0);
		glVertex3f(24.0, 0.0, 0.0);
		glVertex3f(24.0, 2.5, 0.0);
		glVertex3f(16.0, 0.0, 0.0);
		glVertex3f(24.0, 2.5, 0.0);
		glVertex3f(16.0, 2.5, 0.0);
		glEnd();
		glBegin(GL_LINES);					//the arm of the lever
		glVertex3f(20.0, 2.5, 0.0);
		glVertex3f(17.5, 12.5, 0.0);
		glEnd();
		glPushMatrix();
		glTranslatef(17.5, 12.5, 0.0);
		glutSolidSphere(2, 5, 5);			//the head of the lever
		glPopMatrix();
																		//change color to red
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);

		glPushMatrix();
		glTranslatef(11.0, -5.0, 0.0);		//this translate and rotate is so I can reuse the code I wrote for
		glRotatef(45.0, 0.0, 0.0, 1.0);		//when The human is leaning right
											//this draws the same torso and head of the lean right but rotated to the left

		glBegin(GL_TRIANGLES);
		//Torso
		glVertex3f(13.4, 16.6, 0.0);
		glVertex3f(17.5, 20.8, 0.0);
		glVertex3f(15.0, 23.2, 0.0);
		glVertex3f(13.4, 16.6, 0.0);
		glVertex3f(15.0, 23.2, 0.0);
		glVertex3f(10.0, 17.5, 0.0);

		//belly
		glVertex3f(13.4, 16.6, 0.0);
		glVertex3f(10.0, 17.5, 0.0);
		glVertex3f(8.4, 14.6, 0.0);
		glVertex3f(8.4, 14.6, 0.0);
		glVertex3f(12.5, 14.2, 0.0);
		glVertex3f(11.1, 16.0, 0.0);

		//hips
		glVertex3f(10.8, 11.4, 0.0);
		glVertex3f(12.5, 14.2, 0.0);
		glVertex3f(8.4, 14.6, 0.0);
		glVertex3f(10.8, 11.4, 0.0);
		glVertex3f(8.4, 14.6, 0.0);
		glVertex3f(8.9, 11.4, 0.0);

		//chin
		glVertex3f(19.8, 26.6, 0.0);
		glVertex3f(17.8, 22.0, 0.0);
		glVertex3f(16.8, 26.6, 0.0);

		//neck
		glVertex3f(15.0, 23.2, 0.0);
		glVertex3f(17.5, 20.8, 0.0);
		glVertex3f(15.5, 24.6, 0.0);

		glEnd();

		//head
		glPushMatrix();
		glTranslatef(17.5, 26.6, 0.0);
		glutSolidSphere(3, 5, 5);
		glPopMatrix();

		glPopMatrix();						//no longer rotated

		glBegin(GL_TRIANGLES);
		//leg1
		glVertex3f(5.8, 5.8, 0.0);			//upper leg
		glVertex3f(9.8, 13.5, 0.0);
		glVertex3f(5.8, 13.0, 0.0);
		glVertex3f(5.8, 5.8, 0.0);			//lower leg
		glVertex3f(2.3, 6.5, 0.0);
		glVertex3f(0.0, 0.0, 0.0);

		//leg2
		glVertex3f(12.5 - 2.3, 14.2, 0.0);	//upper
		glVertex3f(11.0 - 2.3, 9.0, 0.0);
		glVertex3f(17.0 - 2.3, 8.2, 0.0);
		glVertex3f(17.0 - 2.3, 8.2, 0.0);	//lower
		glVertex3f(12.8 - 2.3, 7.0, 0.0);
		glVertex3f(14.0 - 2.3, 0.0, 0.0);

		//arm
		glVertex3f(9.0, 21.0, 0.0);			//upper
		glVertex3f(13.6, 14.0, 0.0);
		glVertex3f(12.0, 21.0, 0.0);
		glVertex3f(13.6, 14.0, 0.0);		//lower
		glVertex3f(17.5, 12.5, 0.0);
		glVertex3f(13.9, 15.5, 0.0);
		glEnd();

		
	}
	else																//the human is leaning right
	{
		//Lever
		glBegin(GL_TRIANGLES);				//the base
		glVertex3f(16.0, 0.0, 0.0);
		glVertex3f(24.0, 0.0, 0.0);
		glVertex3f(24.0, 2.5, 0.0);
		glVertex3f(16.0, 0.0, 0.0);
		glVertex3f(24.0, 2.5, 0.0);
		glVertex3f(16.0, 2.5, 0.0);
		glEnd();
		glBegin(GL_LINES);					//the arm
		glVertex3f(20.0, 2.5, 0.0);
		glVertex3f(22.5, 12.5, 0.0);
		glEnd();
		glPushMatrix();
		glTranslatef(22.5, 12.5, 0.0);
		glutSolidSphere(2, 5, 5);			//the head
		glPopMatrix();
																			//set the color to red
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);

		glBegin(GL_TRIANGLES);
		//Torso
		glVertex3f(13.4, 16.6, 0.0);
		glVertex3f(17.5, 20.8, 0.0);
		glVertex3f(15.0, 23.2, 0.0);
		glVertex3f(13.4, 16.6, 0.0);
		glVertex3f(15.0, 23.2, 0.0);
		glVertex3f(10.0, 17.5, 0.0);

		//belly
		glVertex3f(13.4, 16.6, 0.0);
		glVertex3f(10.0, 17.5, 0.0);
		glVertex3f(8.4, 14.6, 0.0);
		glVertex3f(8.4, 14.6, 0.0);
		glVertex3f(12.5, 14.2, 0.0);
		glVertex3f(11.1, 16.0, 0.0);

		//hips
		glVertex3f(10.8, 11.4, 0.0);
		glVertex3f(12.5, 14.2, 0.0);
		glVertex3f(8.4, 14.6, 0.0);
		glVertex3f(10.8, 11.4, 0.0);
		glVertex3f(8.4, 14.6, 0.0);
		glVertex3f(8.9, 11.4, 0.0);

		//chin
		glVertex3f(19.8, 26.6, 0.0);
		glVertex3f(17.8, 22.0, 0.0);
		glVertex3f(16.8, 26.6, 0.0);

		//neck
		glVertex3f(15.0, 23.2, 0.0);
		glVertex3f(17.5, 20.8, 0.0);
		glVertex3f(15.5, 24.6, 0.0);

		glEnd();

		//head
		glPushMatrix();
		glTranslatef(17.5, 26.6, 0.0);
		glutSolidSphere(3, 5, 5);
		glPopMatrix();

		glBegin(GL_TRIANGLES);
		//leg1
		glVertex3f(5.8, 5.8, 0.0);
		glVertex3f(9.8, 13.5, 0.0);
		glVertex3f(5.8, 13.0, 0.0);
		glVertex3f(5.8, 5.8, 0.0);
		glVertex3f(2.3, 6.5, 0.0);
		glVertex3f(0.0, 0.0, 0.0);

		//leg2
		glVertex3f(12.5, 14.2, 0.0);
		glVertex3f(11.0, 9.0, 0.0);
		glVertex3f(17.0, 8.2, 0.0);
		glVertex3f(17.0, 8.2, 0.0);
		glVertex3f(12.8, 7.0, 0.0);
		glVertex3f(14.0, 0.0, 0.0);

		//arm
		glVertex3f(15.0, 21.0, 0.0);
		glVertex3f(16.6, 12.5, 0.0);
		glVertex3f(17.0, 20.0, 0.0);
		glVertex3f(16.6, 12.5, 0.0);
		glVertex3f(22.5, 12.5, 0.0);
		glVertex3f(17.5, 14.5, 0.0);
		glEnd();
	}
}

void drawGun(float x)
{
	float matAmbAndDif1[] = { 1.0, 1.0, 1.0, 1.0 };//WHITE
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
																		//set matieral
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


	glLoadIdentity();
	glTranslatef(x, GUN_Y, -150);										//set the guns pos
	glutSolidCylinder(2.5, 10, 10, 10);									//draw the cylinder
}

void drawLaser(float alpha)
{
	float distance = 700.0;												//set defualt distance that reachs off screen

	int lizardIndex = -1;												//find the nearest lizard in line with laser
	for (int i = 0; i < MAX_LIZARDS; i++)
	{
		if (lizardState[i] != -1)										//only if lizard is active
		{																//if laser is in line with it
			if (gunX > lizardX[i] - lizardLength[i] * 0.5  && gunX < lizardX[i] + lizardLength[i] * 0.5)
			{
				if (lizardIndex == -1) lizardIndex = i;					//if this is the first lizard found
				else
				{
					if (lizardY[i] < lizardY[lizardIndex])				//other wise if this lizard is nearer than the previous lizard
					{
						lizardIndex = i;
					}
				}
			}
		}
	}
	if (lizardIndex != -1)												//if a lizard was found
	{
		//i is the point on the lizards body that the laser is hitting
		float i = -(lizardX[lizardIndex] - gunX) + ((lizardLength[lizardIndex] - 24) / 2);
		//A if the amplitude of sin for the lizards curve
		float A = (lizardMaxLength[lizardIndex] - lizardLength[lizardIndex])*0.4;
		//B is frequency for the sin wave
		float B = 1 / ((lizardLength[lizardIndex] - 24) / M_PI);
		distance = A * sin(i*B) + lizardY[lizardIndex] - 7.5;			//find the distace from the gun to the lizard
	}
	


	float matAmbAndDif1[] = { 0.8, 0.7, 0.4, alpha };//cream color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
																		//set up the matieral
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


	glLoadIdentity();

	glPointSize(2.0);													//the spcified width of two

	glBegin(GL_LINES);													//draw the laser
	glVertex3f(gunX, GUN_Y, -150);
	glVertex3f(gunX, distance, -150);
	glEnd();

	glPointSize(1.0);													//set pen back to defualt
}

void drawScore()
{
	char scoreTXT[] = "SCORE: 000000";									//string to write
	snprintf(scoreTXT, 13, "SCORE: %d", score);							//create the string with the current score
	float matAmbAndDif1[] = { 1.0, 1.0, 1.0, 1.0 };//white color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
																		//set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


	glLoadIdentity();
	glRasterPos2f(-20.0, 290.0);										//posicion to top middle of screen
	writeBitmapString(GLUT_BITMAP_8_BY_13, scoreTXT);					//write score
}

void drawGameOver()
{
	char gameOverTXT[] = "GAME OVER";									//string to write
	float matAmbAndDif1[] = { 1.0, 0.0, 0.0, 1.0 };//red color
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
																		//set matierial
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


	glLoadIdentity();
	glRasterPos2f(-20.0, -290.0);											//position to middle bottom of screen
	writeBitmapString(GLUT_BITMAP_8_BY_13, gameOverTXT);					//write score

	glBegin(GL_LINES);														//draw the red X over the human
	glVertex3f(-295.0, -295.0, -150.0);
	glVertex3f(-265.0, -265.0, -150.0);
	glVertex3f(-265.0, -295.0, -150.0);
	glVertex3f(-295.0, -265.0, -150.0);
	glEnd();
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
	glClearColor(0.0, 0.0, 0.0, 1.0);					//set background to black
	glClear(GL_COLOR_BUFFER_BIT);						//clear buffer
	for (int i = 0; i < MAX_LIZARDS; i++)				//loop through and draw each lizard
	{
		if (lizardState[i] != -1)						//only draw it if it is active
		{
			drawLizard(lizardX[i], lizardY[i], lizardLength[i], lizardLookLeft[i], lizardMaxLength[i]);
		}
	}
	
	if(!gameOver) drawHuman(humanIsLeft);				//only draw the human if the game is not over
	drawGun(gunX);										//draw the gun
	if(!laserReady) drawLaser(laserAlpha);				//only draw the laser if it is not ready to fire
	drawScore();										//write the score
	if (gameOver) drawGameOver();						//if the game is over draw the red x
	
	glutSwapBuffers();									//swap buffers for duel buffer mode
	glFlush();											//ask OS to draw frame
}


void timerFunction(int id)											//glutTimerFunc call back
{
	
	switch (id)
	{
	case TIMER_ID_FRAME_UPDATE:										//update the screen every 50ms
		glutPostRedisplay();										//or about 20 frames a second
		glutTimerFunc(50, timerFunction, TIMER_ID_FRAME_UPDATE);
		break;
	case TIMER_ID_UPDATE_LIZARDS:									//update the posistions of all the lizards
																	//To get the lizards to grow propuly the tail must stay in place
																	//while the head moves forward
																	//this is achived by moveing the lizard in the x at half
																	//the rate at which it grows
																	//this is reveresd for the lizard shrinking


		for (int i = 0; i < MAX_LIZARDS; i++)						//loop through to update each lizard
		{
			if (lizardMaxLength[i] < 20 && lizardState[i] != -1)	//if the lizard is active and to short to live
			{
				lizardState[i] = -1;								//mark it as in active
				score += 100;										//and give 100 points for killing the lizard
																	//checking for dead lizards used to be its own timer
																	//but if I have to loop through all the lizards to update them
																	//might as well check if their dead while i am at it and save a loop
			}
			switch (lizardState[i])									//update the lizard based on it state
			{
			case -1:												//the lizard is not acttive, skip it
				break;

			case 1:													//the lizard is growing right
				lizardLength[i] += lizardSpeed[i];					//add to the lizards length
				lizardX[i] += lizardSpeed[i] / 2;					//move the lizard to the right at half the rate of growth

				if (lizardX[i] + (lizardLength[i] / 2) > 290)		//if the head of the lizard reachs the edge of the screen
				{
					lizardY[i] -= 75;								//move it to the next channel
					lizardX[i] = 250;								//reset is pos and length
					lizardLength[i] = lizardMaxLength[i];
					lizardMaxLength[i] += 50;						//increase the lizards max length
					lizardLookLeft[i] = true;						//set it to look left
					lizardSpeed[i] += 2;							//increase it speed
					lizardState[i] = 3;								//set it to grow left
				}
				else												//else the lizard has not reached the end of the channel
				{
					if (lizardLength[i] > lizardMaxLength[i])		//if the lizard has reached maximun stretch
					{
						lizardState[i] = 2;							//set it to start shrinking
					}
				}
				break;

			case 2:													//the lizard is shrinking to the right
				lizardLength[i] -= lizardSpeed[i] * 0.7;			//lizards skrink slower than they grow
				lizardX[i] += 0.5*lizardSpeed[i]*0.7;
				if (lizardLength[i] < lizardMaxLength[i] / 2)		//if the lizard is at half its max length
				{
					lizardState[i] = 1;								//start growing again
				}
				break;

			case 3:													//The lizard is gowing to the left
				lizardLength[i] += lizardSpeed[i];					//This works the same as growing to the right except
				lizardX[i] -= lizardSpeed[i] / 2;

				if (lizardX[i] - (lizardLength[i] / 2) < -290)		//when it reachs the left side of the sceen
				{
					lizardY[i] -= 75;
					lizardX[i] = -250;
					lizardLength[i] = lizardMaxLength[i];
					lizardMaxLength[i] += 50;
					lizardLookLeft[i] = false;
					lizardSpeed[i] += 2;
					lizardState[i] = 1;
					if (lizardY[i] < -275) gameOver = true;			//check if the lizard is on the bottom channel, which is gameover
				}
				else
				{
					if (lizardLength[i] > lizardMaxLength[i])
					{
						lizardState[i] = 4;
					}
				}
				break;

			case 4:													//lizard is shrinking to the left works the same as shrinking to the right
				lizardLength[i] -= lizardSpeed[i] * 0.7;
				lizardX[i] -= 0.5*lizardSpeed[i]*0.7;
				if (lizardLength[i] < lizardMaxLength[i] / 2)
				{
					lizardState[i] = 3;
				}
				break;
			}
		}
		if(!gameOver) glutTimerFunc(50, timerFunction, TIMER_ID_UPDATE_LIZARDS);	//as long as the game is not over queue timer to update lizards again
		break;

	case TIMER_ID_SPAWN_LIZARD:										//Spawn a lizard every 5 seconds
		for (int i = 0; i < MAX_LIZARDS; i++)						//loop through lizards
		{
			if (lizardState[i] == -1)								//find first non active lizards
			{
				resetLizard(i);										//set it active
				break;
			}
		}
		glutTimerFunc(5000, timerFunction, TIMER_ID_SPAWN_LIZARD);	//queue timer to spawn next lizard
		break;

	case TIMER_ID_GUN_RIGHT:										//move the gun to the right
		gunX += 4;
		if (gunX > GUN_BOUND) gunX = GUN_BOUND;						//lock the gun within the right bound
		glutKeyboardFunc(keyboardFunction);							//reenable the keyboard
		break;

	case TIMER_ID_GUN_LEFT:											//move the gun to the left
		gunX -= 4;
		if (gunX < -GUN_BOUND) gunX = -GUN_BOUND;					//lock the gun to the left bound
		glutKeyboardFunc(keyboardFunction);							//reenable the keyboard
		break;

	case TIMER_ID_LASER_FADE:										//update the laser animation
		laserAlpha -= 0.04;											//decrease the alpha of the laser
		if (laserAlpha <= 0.0)										//if the alpha is below 0
		{
			laserAlpha = 1;											//reset it for the next laser
			laserReady = true;										//set laser ready to true
		}
		else glutTimerFunc(5, timerFunction, TIMER_ID_LASER_FADE);	//otherwise queue up next alpha decrease
															
																	//I dont think that the alpha is actuall making
																	//a visual diffrance as I do not have the alpha
																	//depthbuffer thing enabled.  I belive that is what
																	//you are going to get into next class but I would
																	//rather just turn this in early so I can focus on
																	//other homework this weekend
		break;
	}
	
}

void keyboardFunction(unsigned char c, int x, int y)					//glutKeyboardFunc call back
{
	switch (c)
	{
	case 'j':															//j key
		humanIsLeft = true;												//make the human lean left
		glutTimerFunc(GUN_MOVE_MS, timerFunction, TIMER_ID_GUN_LEFT);	//queue timer to move gun left
		glutKeyboardFunc(NULL);
		break;
	case 'k':															//k key
		humanIsLeft = false;											//make human lean right
		glutTimerFunc(GUN_MOVE_MS, timerFunction, TIMER_ID_GUN_RIGHT);	//queue timer fo more gun right
		glutKeyboardFunc(NULL);
		break;
	case ' ':															//space key pressed to fire laser
		if (laserReady)													//only fire if the laser is ready
		{
			laserReady = false;											//set the laser not ready
			glutTimerFunc(5, timerFunction, TIMER_ID_LASER_FADE);		//queue up the timer to control the laser animation
			int lizardIndex = -1;										//search for colosest lizard in line with laser
			for (int i = 0; i < MAX_LIZARDS; i++)
			{
				if (lizardState[i] != -1)								//only check the lizard if the lizard is active
				{														//if the lizard is in line with the laser
					if (gunX > lizardX[i] - lizardLength[i] * 0.5  && gunX < lizardX[i] + lizardLength[i] * 0.5)
					{
						if (lizardIndex == -1) lizardIndex = i;			//if not lizard has been found yet set this lizard as hit
						else
						{
							if (lizardY[i] < lizardY[lizardIndex])		//else if this new lizard is closer to the laser hit it
							{
								lizardIndex = i;
							}
						}
					}
				}
			}
			if (lizardIndex != -1)										//if a lizard was hit
			{
				lizardMaxLength[lizardIndex] -= 50;						//shrink that lizards max length
				score += 50;											//add 50 to score for a hit
			}
		}
		break;
	}
}


//##################################################	UTILITY FUNCTIONS	####################################



//sets up the light source and init lizards
void initShader()
{
	glLoadIdentity();
	float lightAmb[] = { 1.0, 1.0, 1.0, 1.0 };			//white light
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightPos[] = { 0.0, 0.0, 0.0, 1.0 };			//pos at origin
	
	// Light properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHTING);								//enable lighting
	glEnable(GL_LIGHT0);
	
	glShadeModel(GL_SMOOTH);							//set to smooth
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);//set local viewer

	for (int i = 0; i < MAX_LIZARDS; i++)				//set all lizards to -1 (not active)
	{
		lizardState[i] = -1;
	}
}

//This spawns a lizard and sets its starting stats
void resetLizard(int index)
{
	if (index < MAX_LIZARDS)
	{
		lizardX[index] = -250;					//start pos
		lizardY[index] = 300 - (75 / 2);
		lizardLookLeft[index] = false;			//start faceing right
		lizardLength[index] = 99;
		lizardMaxLength[index] = 99;
		lizardSpeed[index] = 5;
		lizardState[index] = 1;					//start growing to the right
	}
}
