//Orbit.cpp
//Main project file, initialization of window and registering callbacks.

#include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include <GL/GLUT.H>

#include "Definitions.h"
#include "Controller.h"
#include "View.h"

Controller* controller;
View* view;

void init(void);
void end(void);
void reshape(GLsizei, GLsizei);
void display(void);
void mouse(int btn, int state, int x, int y);
void mouse_move_passive(int x, int y);
void mouse_move(int x, int y);


//main function, initializes GLUT window and registeres callbacks 
int main(int argc, char **argv){

	//init GLUT and create window
	glutInit(&argc, argv);													//initialization
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );			//setting display mode
	glutInitWindowPosition( 0, 0 );											//setting initial position of the window
	glutInitWindowSize ( WIDTH, WINDOW_HEIGHT );							//setting initial size of the window

	//creating window
	glutCreateWindow("Orbit");

	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutIdleFunc( display );
	glutMouseFunc( mouse );
	glutPassiveMotionFunc(mouse_move_passive);
	glutMotionFunc(mouse_move);
	
	
	//defines actions performed at program exit and initializes objects
	atexit( end );
	init();

	std::cout << "After init. Entering GLUT event processing cycle...\n";

	//enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}


//initialising objects and basic parameters. 
void init(void) {	
	controller = new Controller();
	view = new View( controller -> get_model() );
}

//actions taken at the end of the program
void end(void) {	
	delete view;
	delete controller;
}

//reshapes scene
void reshape(GLsizei w, GLsizei h) {
	if( h > 0 && w > 0 ) {
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, (double) w / h, 0.01, 300);
		glMatrixMode(GL_MODELVIEW);
	}
}

//displays scene
void display(void)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor(0.0,0.0,0.0,0.0);
	glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,WIDTH - 1.0, -TIME_BAR_HEIGHT, HEIGHT - 1.0, -1.0,1.0);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	controller->update_model();
	view->draw_all();
	
	glFlush();
	glutSwapBuffers();
}

//handles mouse buttons' actions
void mouse(int btn, int state, int x, int y) {
    if(btn == GLUT_LEFT_BUTTON && state==GLUT_DOWN)   
    {
        std::cout<<"x: "<<x<<std::endl;
        std::cout<<"y: "<<y<<std::endl;
		controller -> handle_mouse_click(x, y);
    }
	if(btn == GLUT_LEFT_BUTTON && state==GLUT_UP)   
    {
        controller -> handle_mouse_up(x, y);
    }
	if(btn == GLUT_RIGHT_BUTTON && state==GLUT_DOWN)   
    {
        controller -> handle_right_mouse_down();
    }
}

//handles mouse passive move
void mouse_move_passive(int x, int y) {
	controller -> handle_passive_motion(x, y);
}

//handles mouse move
void mouse_move(int x, int y) {
	controller -> handle_motion(x, y);
}
