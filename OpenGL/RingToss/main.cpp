//CS 445/545 Prog 3 for Kaden Beaver
/***********************************************************************************
*
*	Kaden Beaver	10.19.2021	Program 3
*
*	EXTRA CREDIT:	Ring rotates when you throw it
					Added messages of Encouragement
					Added basic ridged body simulation when the ring is close to the post

	DESIGN:			The Program is borken up into Drawing Functions, Call back Functions, and Utility Functions.
					The Drawing functions draw a specific object in the scene and are all called in 
					the display callback.

					drawPost just draws the to post the ring is supsed to land on useing POST_X for it x pos
					drawUI writes out the dircetions and uses score, and userVelocityTXT to writne
					out the score and the user input.
					drawRing uses the ringPos, ringVel, ringRotation, ringColor, and the RING_.. 
					defines to draw the ring at the right posittion, rotation, color, and size
					writeCheers wrties the words of encouragment.  the value passed to it deturmins
					if it should write noting, or  which of the phrases to use
					writeBitmapString is the function for the textbook that loops 
					through a char[] and writes it in bitmap charaters

					displayFunction is the callback function for glutDisplayFunc.  It calls all of the other 
					draw functions, and determines wrather to draw the ring or draw the GAME OVER text.  It 
					also calls glFlush to ask the OS to display the frame

					timerFunction is the callbackFunction for glutTImerFunc.  Their are two main timers going.
					One calls a redisplay event every ANIMATION_UPDATE_RATE ms, and the other updates physics,
					changeing the pos, vel of the ring and checking for win condisions every ANIMATION_UPDATE_RATE
					ms there is a thrid timer that is called  when the ring needs to be reset

					keyboardFunction is the callback funciton for glutKeyboardFunc, it tracks the mouse pos and
					if the mouse is over the velocity input, it adds the pressed character, if it is a number, to 
					a char[].  it also tracks backspace to remove a char from the char[]

					mouseFunction is the callback function for the glutMouseFunc, it dectecs if the mouse is
					over the GO button when pressed and if so converts the userVelocityTXT to a float and sets 
					the velocity for x and y and sets the global flags

					ringStart simplily resets or incremnts all of the global varibles for the next ring toss,
					it also check for if all three rings have been thrown and sets the gameOver flag

					collisonDetect tries to simulate the interation with the ring and the post if the
					ring does not land perfeclty on the post.  it keeps track of the intersection point between 
					the ring and post to calulate the torque which it uses to figure out the angular acceleration,
					velocity, and rotation.  it is very basic, as it is not checking the surface of the ring, just 
					the point where the post meets it and a point at each end of the ring that can interact with
					the ground.

					The defines are mostly just the perminate posistion of variaous elements or constants like
					gravity and the size of the userVelocityTXT char[].  The other important global variables 
					are the flags.  readyToThrow is set when the go is pressed to keep the button from being
					pressed again untill the ring is reset.  ringInFlight is used to know if collisonDetect needs 
					to be called.  ringLanded is set when the ring touches the ground so that the simulation can
					freeze for the second before the ring get reset.  And gameOver is used to know when end the game


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
void drawPost();
void drawUI();
void drawRing();
void writeCheers(int val);
void writeBitmapString(void *font, char *string);

//Call back funcs
void displayFunction();									//THIS IS THE GLUTDISPLAYFUNC CALL BACK FUNCTION
void timerFunction(int id);								//THIS IS THE GLUTTIMERFUNC CALL BACK FUNCTION
void keyboardFunction(unsigned char c, int x, int y);	//THIS IS THE GLUTKEYBOARDFUNC CALL BACK FUNCTION
void mouseFunction(int button, int state, int x, int y);//THIS IS THE GLUTMOUSEFUNC CALL BACK FUNCTION

//Untilites funcs
void setRingStart();									//reset the global varibles for the next throw
void collisonDetect();									//very basic and limited ridged body sim


//##################################################	GLOBAL VARIBLES AND DEFINES		###################################################

//The Canvas specifications
#define canvas_Width 550
#define canvas_Height 550
char canvas_Name[] = "Yeet The Ring";

//Timer Stats and Physic Constants
#define PHYSICS_UPDATE_RATE 20.0						//ms between updates
#define ANIMATION_UPDATE_RATE 50.0						//''
#define GRAVITY_ACC -32.17								//in ft/s/s
#define GROUND_PLANE_Y -280

//Velocity Input
#define VEL_INPUT_BOX_X -200.0
#define VEL_INPUT_BOX_Y 0.0
#define VEL_INPUT_BOX_WIDTH 75.0
#define VEL_INPUT_BOX_HEIGHT 25.0

#define INPUT_BUFFER_VEL 6
char userVelocityTXT[INPUT_BUFFER_VEL];

//cheers
int cheerState = 0;

//Go button
#define GO_BOX_X -130.0
#define GO_BOX_Y 0.0
#define GO_BOX_DIAMETER 30

//Post
#define POST_X 210
#define POST_Y -235

//Ring Stats
#define RING_INNER_R 7.5								//actualy the minor radius, glut labels it wrong
#define RING_OUTER_R 20									//actualy the magor radius, glut labels it wrong
#define RING_SEGMENTS 12
#define RING_LOOPS 8
bool ringInFlight = false;
bool ringLanded = false;
bool readyToThrow = true;
float ringVelX = 0;
float ringVelY = 0;
float ringPosX = 0;
float ringPosY = 0;
float ringThetaY = 0;
float ringThetaZ = 0;
float ringThetaVelZ = 0;
float ringMass = 100;									//can acutaly be set to anything as the mass gets canceled out
int ringColor = -1;

//Score
int score = 0;
bool gameOver = false;



//##################################################	MAIN	############################################################################
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);								//Set up glut
	my_setup(canvas_Width, canvas_Height, canvas_Name);	//The function for given header

	strcpy_s(userVelocityTXT, INPUT_BUFFER_VEL, "0");	//set the user input string to "0"

	glutDisplayFunc(displayFunction);					//register call back fuctnion for DisplayFunc
	glutKeyboardFunc(keyboardFunction);					//register call back fucntion for KeyboardFunc
	glutMouseFunc(mouseFunction);						//register call back function for MouseFunc
	glutTimerFunc(PHYSICS_UPDATE_RATE, timerFunction, 1);		//start the physic timer
	glutTimerFunc(ANIMATION_UPDATE_RATE, timerFunction, 100);	//start the animation timer
	setRingStart();										//set the first rings start
	glutMainLoop();										//Start the main loop

	return 0;
}


//##################################################	DRAWING FUNCTIONS	################################################################

//Draws a post at teh x pos listed in the defines
void drawPost()
{
	int CUBE_SIZE = 10;
	glColor3f(1.0, 1.0, 1.0);							//set color to white
	glLoadIdentity();									//reset the transformation
	
	glTranslatef(POST_X, -275+(CUBE_SIZE/2), -50);		//set the start point of post
	glRotatef(30.0, 0.0, 1.0, 0.0);						//set the rotaion of post
	glutWireCube(CUBE_SIZE);							//draw first cube
	glTranslatef(0, CUBE_SIZE, 0);						//move up
	glutWireCube(CUBE_SIZE);							//and draw next cube
	glTranslatef(0, CUBE_SIZE, 0);						//''
	glutWireCube(CUBE_SIZE);
	glTranslatef(0, CUBE_SIZE, 0);
	glutWireCube(CUBE_SIZE);
}

//draws all the various UI elements 
//relies on the defines to place them
void drawUI()
{
	char intructionsTXT[] = "Enter Toss Velocity and then GO to Toss";
	char goTXT[] = "GO";
	char scoreTXT[] = "SCORE: 000";

	snprintf(scoreTXT, 11, "SCORE: %3d", score);		//create the string for the score

	
	glColor3f(1.0, 1.0, 1.0);							//set color to white
	glLoadIdentity();									//reset the transforms

	glRasterPos3f(-((float)strlen(intructionsTXT) * 9 / 2), 255, -50);	//center direction text
	writeBitmapString(GLUT_BITMAP_9_BY_15, intructionsTXT);				//wrtie the directions

	glTranslatef(VEL_INPUT_BOX_X, VEL_INPUT_BOX_Y, -50);				//move to pos of input box
	glPushMatrix();
	glScalef(VEL_INPUT_BOX_WIDTH/VEL_INPUT_BOX_HEIGHT, 1.0, 1.0);		//scale to get rectangle
	glutSolidCube(VEL_INPUT_BOX_HEIGHT);								//draw rectangle
	glPopMatrix();

	glColor3f(0.0, 0.0, 0.0);											//change pen toe black
																		//right justify text
	glRasterPos3f(((-(float)strlen(userVelocityTXT) * 9) +(VEL_INPUT_BOX_WIDTH/2)-5), -5.8, -50);
	writeBitmapString(GLUT_BITMAP_9_BY_15, userVelocityTXT);			//write the user input

	glColor3f(1.0, 1.0, 1.0);											//change color to white
	glLoadIdentity();													//reset transforms
	glTranslatef(GO_BOX_X, GO_BOX_Y, -50.0);							//go to pos for go button
	glutSolidCube(GO_BOX_DIAMETER);										//draw box for go button
	glColor3f(0.0, 0.0, 0.0);											//chagne color to black
	glRasterPos3f(-((float)strlen(goTXT) * 9 / 2), -5.8, -50.0);		//center text in box
	writeBitmapString(GLUT_BITMAP_9_BY_15, goTXT);						//write "go"

	glColor3f(1.0, 1.0, 1.0);											//change color to white
	glLoadIdentity();
	glRasterPos3f(95, 240, -50);										//move to pos of score
	writeBitmapString(GLUT_BITMAP_8_BY_13, scoreTXT);					//write score
}

//draw the ring using the global variables
void drawRing()
{
	switch (ringColor)													//chose a color for the ring
	{
	case 0:
		glColor3f(1.0, 1.0, 0.0);										//yellow
		break;
	case 1:
		glColor3f(1.0, 0.4,0.0);										//oragne
		break;
	case 2:
		glColor3f(0.52, 0.67, 0.64);									//blue
		break;
	}
	glLoadIdentity();													//reset transforms
																		//set ring pos, agust the y pos based on the rotation of ring
	glTranslatef(ringPosX, ringPosY + (RING_OUTER_R * cos(ringThetaZ * M_PI / 180)), -50);
	glRotatef(ringThetaZ, 0.0, 0.0, 1.0);								//set the rotaiton in z
	glRotatef(ringThetaY, 0.0, 1.0, 0.0);								//set roation in y
	
	glRotatef(90, 1.0, 0.0, 0.0);										//rotate ring to be in the right plane

	glutWireTorus(RING_INNER_R, RING_OUTER_R, RING_LOOPS, RING_SEGMENTS);	//draw the ring
}

//write encouragement based on where the ring is landing
void writeCheers(int val)
{
	char yeetTXT[] = "YEET!";
	char close1TXT[] = "So close!";
	char close2TXT[] = "Almost!";
	char fartherTXT[] = "Just a little farther!";
	char toFarTXT[] = "A little to far!";
	char noEvenCloseTXT[] = "Not even close";
	char spotOnTXT[] = "SPOT ON!";

	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	switch (val)
	{
	case 0:																//no message
		break;
	case 1:																//the ring is not even close
		glRasterPos3f(-((float)strlen(noEvenCloseTXT) * 9 / 2), -240, -50.0);
		writeBitmapString(GLUT_BITMAP_9_BY_15, noEvenCloseTXT);
		break;
	case 2:																//the ring was tossed with vel over 999
		glRasterPos3f(-((float)strlen(yeetTXT) * 9 / 2), -240, -50.0);
		writeBitmapString(GLUT_BITMAP_9_BY_15, yeetTXT);
		break;
	case 3:																//ring landed on post
		glRasterPos3f(-((float)strlen(spotOnTXT) * 9 / 2), -240, -50.0);
		writeBitmapString(GLUT_BITMAP_9_BY_15, spotOnTXT);
		break;
	case 4:																//ring is sort of close
		if (ringPosX < POST_X)
		{
			glRasterPos3f(-((float)strlen(close1TXT) * 9 / 2), -240, -50.0);
			writeBitmapString(GLUT_BITMAP_9_BY_15, close1TXT);
		}
		else
		{
			glRasterPos3f(-((float)strlen(close2TXT) * 9 / 2), -240, -50.0);
			writeBitmapString(GLUT_BITMAP_9_BY_15, close2TXT);
		}
		break;
	case 5:																//ring is really close
		if (ringPosX < POST_X)
		{
			glRasterPos3f(-((float)strlen(fartherTXT) * 9 / 2), -240, -50.0);
			writeBitmapString(GLUT_BITMAP_9_BY_15, fartherTXT);
		}
		else
		{
			glRasterPos3f(-((float)strlen(toFarTXT) * 9 / 2), -240, -50.0);
			writeBitmapString(GLUT_BITMAP_9_BY_15, toFarTXT);
		}
		break;
	}

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
	glClearColor(0.0, 0.0, 0.0, 1.0);					//set background to black
	glClear(GL_COLOR_BUFFER_BIT);						//clear buffer
	drawPost();
	if (gameOver)										//if game over
	{
		char gameOverTXT[] = "GAME OVER";				//wrinte game over text
		glLoadIdentity();
		glColor3f(1.0, 0.0, 0.0);
		glRasterPos3f(-((float)strlen(gameOverTXT) * 9 / 2)+ringPosX, ringPosY, -50.0);
		writeBitmapString(GLUT_BITMAP_9_BY_15, gameOverTXT);
	}
	else												//other wise draw the ring
	{
		drawRing();
	}
	drawUI();
	writeCheers(cheerState);
	glFlush();											//ask OS to draw frame
}


void timerFunction(int id)								//glutTimerFunc call back
{
	//id 1:		physic update
	//id 2:		reset ring
	//id 100:	animation update
	switch (id)
	{
	case 1:															//ID 1
		if (ringInFlight && !ringLanded)
		{															//PHYSICS_UPDATE_RATE/1000 scale the update reletive to the physic rate
			ringPosX += ringVelX * (PHYSICS_UPDATE_RATE/1000);		//update x pos based on x vel
			ringPosY += ringVelY * (PHYSICS_UPDATE_RATE / 1000);	//update y pos based on y vel
			ringVelY += GRAVITY_ACC * (PHYSICS_UPDATE_RATE / 1000);	//update y vel based on gravity
			ringThetaY += 20 * (PHYSICS_UPDATE_RATE / 1000);		//add to y roation to get the ring to spin

			if (ringPosY <= -210)									//if the ring is bellow -210
			{
				ringInFlight = false;								//its no longer in flight
				ringVelX = 0;										//and has no more x vel
			}
			
		}
		else if (!ringInFlight && !ringLanded && !readyToThrow)		//ring is landing
		{
			if (ringPosX < 225 && ringPosX > 195)					//the ring is on the post
			{
				ringPosY += ringVelY * (PHYSICS_UPDATE_RATE / 1000);		//contiue to simulate gravity
				ringVelY += GRAVITY_ACC * (PHYSICS_UPDATE_RATE / 1000);
				if (ringPosY <= (GROUND_PLANE_Y + RING_INNER_R))	//if the ring is at the gound plane
				{
					cheerState = 3;
					score += 25;									//update score
					ringLanded = true;								//set landed (this stops simulating physics
					glutTimerFunc(1000, timerFunction, 2);			//que up the reset timer
				}
			}
			else													//else the ring is interacting with the post
			{
				collisonDetect();									//and the bad ridged body sim needs to run
			}
		}
	
		glutTimerFunc(PHYSICS_UPDATE_RATE, timerFunction, 1);		//que up the next physic fram
		break;
	case 2:															//ID 2
		setRingStart();												//reset for next throw
	case 100:														//ID 3
		glutPostRedisplay();										//que up a display event
		glutTimerFunc(ANIMATION_UPDATE_RATE, timerFunction, 100);	//que up next animation frame
		break;
	}
	
}

void keyboardFunction(unsigned char c, int x, int y)				//glutKeyboardFunc call back
{
	float xgl = x - canvas_Width / 2;								//convert screen cords to gl cords
	float ygl = (y - canvas_Height / 2) *-1;
																	//if mouse is over input box let user type velocity
	if (xgl <= VEL_INPUT_BOX_X + (VEL_INPUT_BOX_WIDTH / 2) && xgl >= VEL_INPUT_BOX_X - (VEL_INPUT_BOX_WIDTH / 2) && ygl <= VEL_INPUT_BOX_Y + (VEL_INPUT_BOX_HEIGHT / 2) && ygl >= VEL_INPUT_BOX_Y - (VEL_INPUT_BOX_HEIGHT / 2))
	{
		if (c == 8)													//if backspace was pressed
		{
			int index = strlen(userVelocityTXT) - 1;				//get the length of string
			if (index > 0) userVelocityTXT[index] = '\0';			//if there is morethan 1 charater delete the last
			else if (index == 0) userVelocityTXT[0] = '0';			//else set the string to "0\0"
			glutPostRedisplay();									//REMOVE
		}
		if (c <= 57 && c >= 48)										//c is then a number 0-9
		{
			if (strlen(userVelocityTXT) < INPUT_BUFFER_VEL - 1)		//if there is room left in the string
			{
				if (userVelocityTXT[0] == '0') userVelocityTXT[0] = '\0';	//if the string is "0" empty it
				char character[2] = { c, '\0' };							//create a 2 char string with the added char
				strcat_s(userVelocityTXT, INPUT_BUFFER_VEL, character);		//concat the new char
				glutPostRedisplay();										//REMOVE
			}

		}
	}

}

//
void mouseFunction(int button, int state, int x, int y)				//glutMouseFunc Callback
{
	
	float xgl = x - canvas_Width / 2;								//convert screen cords to gl cords
	float ygl = (y - canvas_Height / 2) *-1;

	if (readyToThrow)												//if no ring is in flight, and one is ready to throw
	{																//check if mouse in on GO button
		if (xgl < GO_BOX_X + (GO_BOX_DIAMETER / 2) && xgl > GO_BOX_X - (GO_BOX_DIAMETER / 2) && ygl < GO_BOX_Y + (GO_BOX_DIAMETER / 2) && ygl > GO_BOX_Y - (GO_BOX_DIAMETER / 2))
		{
			ringInFlight = true;									//set flight flags
			readyToThrow = false;
			ringVelX = atof(userVelocityTXT);						//set the x vel to user input
			ringVelY = 10;											//set the y vel to +10
			if (ringVelX > 999)
			{
				cheerState = 2;
			}
		}
	}
	
}

//##################################################	UTILITY FUNCTIONS	####################################



void setRingStart()								//reset ring
{
	ringPosX = -60;								//reset pos
	ringPosY = 200;
	ringThetaY = 0;								//reset rotation
	ringThetaZ = 0;
	ringThetaVelZ = 0;
	ringColor++;								//increment color
	if (ringColor > 2) gameOver = true;			//loop color if last color reached
	ringLanded = false;							//reset flags
	ringInFlight = false;
	readyToThrow = true;						//set ready to throw
	userVelocityTXT[0] = '0';					//reset user input
	userVelocityTXT[1] = '\0';
	cheerState = 0;								//reset cheer
}


void collisonDetect()											//poor collison detection
{
	if (!ringLanded)											//if the ring physic still be used
	{
		if (ringPosX < POST_X - 36 || ringPosX > POST_X + 36)	//if the ring is to far from the post
		{														//just simulat gravity
			if (ringPosY <= (-280 + RING_INNER_R))				//ring hits the ground
			{
				if (ringPosX < POST_X - 55 || ringPosX > POST_X + 55)
				{
					cheerState = 1;
				}
				else
				{
					cheerState = 4;
				}
				
				glutTimerFunc(1000, timerFunction, 2);			//que reset
				ringLanded = true;								//stop physics updates
			}
			else
			{
				ringPosY += ringVelY * (PHYSICS_UPDATE_RATE / 1000);	//gravity update
				ringVelY += GRAVITY_ACC * (PHYSICS_UPDATE_RATE / 1000);
			}
		}
		else													//else work out the collison
		{
			float alpha = 0;
			float torque = 0;
			float xContact = 0;
			float yContact = 0;
			float xEdgeL = 0;
			float yEdgeL = 0;
			float xEdgeR = 0;
			float yEdgeR = 0;
			float momentOfI = 0;


			momentOfI = 0.5*ringMass*RING_OUTER_R*RING_OUTER_R;			// 0.5 m r^2 moment of Inertia, this should be changed based on axis of rotaion
																		//but I dont want to mess with that anymore than this

			xEdgeL = RING_OUTER_R * sin((ringThetaZ * M_PI / 180));		//find the loation x,y of the two edges of the ring
			yEdgeL = RING_OUTER_R * cos((ringThetaZ * M_PI / 180));
			xEdgeR = -xEdgeL + ringPosX;
			yEdgeR = -yEdgeL + ringPosY;
			xEdgeL += ringPosX;
			yEdgeL += ringPosY;

			if (yEdgeL <= GROUND_PLANE_Y || yEdgeR <= GROUND_PLANE_Y)	//if one of the edges are hitting ground plane
			{
				cheerState = 5;
				glutTimerFunc(1000, timerFunction, 2);					//que up reset
				ringLanded = true;										//freze physics
				return;
			}
																		//locate the intersection point between ring and post
			xContact = POST_X - ringPosX;								//x pos is just the diffrance or ring pos and post pos
			yContact = xContact * tan((ringThetaZ * M_PI / 180));		//y pos relies on the rotaion of the ring
			xContact += ringPosX;
			yContact += ringPosY;

			if (yContact <= POST_Y)										//if the contact point on the ring is interacting with post
			{
				torque = -1 * ringMass * GRAVITY_ACC * (xContact - ringPosX);//torque = force * distance, force=gravity, distance is xContact
				alpha = torque / momentOfI;									//Mass cancels out, so mass can be any value

				alpha = alpha * 180 / M_PI;									//conver the radians to degrees

				ringThetaVelZ += alpha * (PHYSICS_UPDATE_RATE / 1000);		//update rotational velocity, scale by update rate
				ringThetaZ += ringThetaVelZ * (PHYSICS_UPDATE_RATE / 1000);	//update rotaion pos, scale by update rate
			}
			else														//if the contact point is not interacting with the post
			{
				ringPosY += ringVelY * (PHYSICS_UPDATE_RATE / 1000);	//just simulate normal gravity
				ringVelY += GRAVITY_ACC * (PHYSICS_UPDATE_RATE / 1000);
			}
		}
	}
}


