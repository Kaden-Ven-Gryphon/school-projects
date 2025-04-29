#include <iostream>>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup.h"
#include "pch.h"

/***************************************************************************************************
Kaden Beaver 09.09.2021 Program 1

EXTRA CREDIT: both the 3 frame animated wings and reseting the animation when the geese leave the screen

DESIGN: There is the function drawGoose the draw a single goose at the given pos with the given wing pose.
		This is the timerPosFunction which is the event handler for timer events and updates the position 
		and wing pose of the geese, and calls the redisplay event.  There is a getKeyPress event handler
		that when it gets a key press starts the first timer event and then disables keyboard events. 
		Last if the display event whitch clears the buffer draws the three geese using the global pos and
		flushes the buffer.

****************************************************************************************************/




//Defines for the animation speed perameter
#define SPEED 5											//how far the goose more +x every frame, the geese where a bit to fast with the specifed speed of 10 units a frame
#define FRAME_RATE 75									//the number of ms between frames
#define GOOSE_FLAP_RATE 4								//the number of frame between each pose of the gooses flight

//Function prototypes
void drawGoose(int posX, int posY, int frame);
void display_func();
void timerPosManager(int id);
void getKeyPress(unsigned char key, int x, int y);

//The Canvas perameters
#define canvas_Width 480
#define canvas_Height 480
char canvas_Name[] = "THE GOOSE";

//The gobal varibles for the flocks pos and wing pose
int gooseOriginX = 80;
int gooseOriginY = 480 - (50 + 35);
int gooseFrame = 0;

//***************************MAIN************************************************
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);								//set up glut	
	my_setup(canvas_Width, canvas_Height, canvas_Name); //set up function from provied header file

	std::cout << "Press any key to start\n";			//prompt user

	glutDisplayFunc(display_func);						//set the diplay event handler
	glutKeyboardFunc(getKeyPress);						//set the keyboard event handler
	glutMainLoop();										//start the main glut loop
	return 0;
}


//**************************DRAWGOOSE********************************************
//This takes the x,y pos and draws a goose at the point
//The origin of the goose is the left point of the body
//int frame specifies which wing pose to draw
void drawGoose(int posX, int posY, int frame)
{
	glColor3f(0.0, 0.0, 0.0);								//set the pen color to black


	glBegin(GL_LINES);										//begin drawing lines
		glVertex3i(posX, posY,-1);							//the main body line
		glVertex3i(posX+90, posY,-1);
		switch (frame)										//choose a wing pose
		{
		case 0:												//wing flaped all the way back
			glVertex3i(posX + 35, posY,-1);					//bottom wing
			glVertex3i(posX + 7, posY - 28,-1);	
			glVertex3i(posX + 35, posY,-1);					//top wing
			glVertex3i(posX + 7, posY + 28,-1);
			break;
		case 1:												//wing is folding for the upstroke
			glVertex3i(posX + 35, posY,-1);					//bottom wings first segment
			glVertex3i(posX + 35+14, posY - (28/2),-1);
			glVertex3i(posX + 35 + 14, posY - (28 / 2),-1);//bottom wings second segment
			glVertex3i(posX + 35, posY - 28,-1);
			glVertex3i(posX + 35, posY,-1);					//top wings first segment
			glVertex3i(posX + 35 + 14, posY + (28 / 2),-1);
			glVertex3i(posX + 35 + 14, posY + (28 / 2),-1);	//top wings second segment
			glVertex3i(posX + 35, posY + 28,-1);
			break;
		case 2:												//the wings are half way through the down stroke, making a line stright across
			glVertex3i(posX + 35, posY + 40,-1);			//both wings are a stright line
			glVertex3i(posX + 35, posY - 40,-1);
			break;
		}
	glEnd();											//end of lines
}

//***************************TIMERPOSMANAGER************************************************************
//This is the event handler for the timerfunc
//There is only one timer going so id is unused
//This updates the position of the flock and it wing pose before reseting the timer
void timerPosManager(int id)
{
	if (gooseOriginX > 510)								//The goose is now off screen
	{
		std::cout << "Press any key to restart animation\n";	//prompt the user
		glutKeyboardFunc(getKeyPress);					//reset the keyboard event handeler
	}													//this will also mean that the time will not get recalled
	else												//otherwise the geese are still on screen
	{
		static int flapRateCounter = 0;					//a counter to keep track of when to change wing pose
		gooseOriginX += SPEED;							//change the flocks pos by the speed
		if (flapRateCounter++ >= GOOSE_FLAP_RATE)		//increment the counter, and if the coutner is greater than flap rate change the pose
		{
			gooseFrame++;								//go to the next pose
			if (gooseFrame > 2) gooseFrame = 0;			//go to the fist pose if leaving the last pose
			flapRateCounter = 0;						//reset the counter
		}

		
		glutPostRedisplay();							//create a redisplay event
		glutTimerFunc(FRAME_RATE, timerPosManager, 1);	//create a timer event recalling this timerPosManager funciton
	}
}

//*****************************DISPLAY_FUNC***************************************
//This is the event handler for a display event
void display_func()										
{
	glClearColor(1.0, 1.0, 1.0, 1.0);					//the the clear color to white
	glClear(GL_COLOR_BUFFER_BIT);						//clear the buffer
	

	drawGoose(gooseOriginX, gooseOriginY, gooseFrame);	//draw the three geese,  These are farther apart than specified in the handout beacue they where overlaping using those numbers
	drawGoose(gooseOriginX-30, gooseOriginY+45, gooseFrame);
	drawGoose(gooseOriginX-30, gooseOriginY-45, gooseFrame);
	glFlush();											//ask the operating system to draw the frame to the window
}

//*********************************GETKEYPRESS*************************************
//This is the event handler for a keyboard event
void getKeyPress(unsigned char key, int x, int y)
{
	gooseOriginX = 80;									//set the starting point of the flock,  this is needed for when the user resets the animation
	gooseOriginY = 480 - (50 + 35);						
	glutTimerFunc(FRAME_RATE, timerPosManager, 1);		//start the first timer 
	glutKeyboardFunc(NULL);								//disable keyboard events so that the user can not reset the animation early
}