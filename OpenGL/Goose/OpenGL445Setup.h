#pragma once

/******************************************
OpenGL445Setup.h

	This header file contains the initialization functions calls and setups
	Copyed from the hand out in class
*******************************************/

//#include <GL/glew.h>
//#include <GL/freeglut.h>

void my_3d_projection(int width, int height)
{
	GLfloat width_bound, height_bound;
	width_bound = (GLfloat)width; height_bound = (GLfloat)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width_bound, 0.0, height_bound, 0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

#define STRT_X_POS 25
#define STRT_Y_POS 25

void my_setup(int width, int height, char *window_name_str)
{
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	// To get double buffering, uncomment the next line
	// glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// comment out the next line for double buffering
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(STRT_X_POS, STRT_Y_POS);

	glutCreateWindow(window_name_str);
	glewExperimental = GL_TRUE;
	glewInit();

	glutReshapeFunc(my_3d_projection);
}