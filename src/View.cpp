//View.cpp

#include <iostream>
#include "View.h"
#include "Definitions.h"

//View constructor
View::View(Model* model){
	//std::cout << "View::View()\n";
	this->model = model;
}

//View destructor
View::~View(void){
	//std::cout << "View::~View()\n";
	
}

//draws all objects
void View::draw_all(void){
	draw_time_bar();
	draw_board();
	draw_comet();
	draw_planets();
	draw_samples();
	draw_line();
}

//draws samples
void View::draw_samples(void) {
	if(model -> get_game_state() == SOLVING) {
		double x, y, r;
		for(int i = 0; i < SAMPLE_QUANTITY; i++) {
			if( model->get_samples()[i] == NULL)
				break;
			x = model->get_samples()[i]->get_x();
			y = model->get_samples()[i]->get_y();
			r = model->get_comet()->get_r();
	
			glPushMatrix();
				glTranslated(x,y,0);
				glColor3f( 0.5f, 0.5f, 0.5f );
				gluDisk(gluNewQuadric(), 0, r, 64, 16);
			glPopMatrix();
			glBegin(GL_LINES);
				glVertex3f( x, y,0.0f);
				glVertex3f( model->get_samples()[i]->get_x_end(),model->get_samples()[i]->get_y_end(),	0.0f);
			glEnd();
		}
	}
}

//draws line visualising strain
void View::draw_line(void) {
	if(model -> get_game_state() == START_COMET) {
		glBegin(GL_LINES);
			glColor3f( 1.0f, 0.0f, 0.0f );
			glVertex3f( model -> get_comet() -> get_x(),model -> get_comet() -> get_y(),0.0f);
			glVertex3f( model -> get_mouse_x(),model -> get_mouse_y(),	0.0f);
		glEnd();
	}
}

//draws time bar
void View::draw_time_bar(void) {
	float width =((float) model->get_frame_number() / FRAME_NUMBER ) * WIDTH;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		glColor3f( 0.0f, 1.0f, 0.0f );
		glVertex3f( 0.0, -TIME_BAR_HEIGHT, 0.0f);
		glVertex3f( 0.0, 0.0, 0.0);
		glVertex3f( width, 0.0,	0.0f);
		glVertex3f( width, -TIME_BAR_HEIGHT, 0.0f);
	glEnd();
}

//draws board
void View::draw_board(void){
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex3f( GAP,		 GAP,			0.0f);
		glVertex3f( GAP,		 HEIGHT - GAP,	0.0f);
		glVertex3f( WIDTH - GAP, HEIGHT - GAP,	0.0f);
		glVertex3f( WIDTH - GAP, GAP        ,	0.0f);
	glEnd();
	glPopMatrix();
}

//draws comet
void View::draw_comet(void){
	if(model -> get_game_state() != SOLVING) {
		double x = model->get_comet()->get_x();
		double y = model->get_comet()->get_y();
		double r = model->get_comet()->get_r();
	

		glPushMatrix();
			glTranslated(x,y,0);
			glColor3f( 0.5f, 0.5f, 1.0f );
			gluDisk(gluNewQuadric(), 0, r, 64, 16);
		glPopMatrix();
	}
}

//draws planets
void View::draw_planets(void){
	for( unsigned i=0; i < model->get_planets().size(); i++){
		double x = model->get_planets()[i]->get_x();
		double y = model->get_planets()[i]->get_y();
		double r = model->get_planets()[i]->get_r();

		glPushMatrix();
			glTranslated(x,y,0);
			glColor3f( 1.0f, 1.0f, 0.0f );
			gluDisk(gluNewQuadric(), 0, r, 64, 32);
		glPopMatrix();
	}
}