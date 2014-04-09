//View.h

#pragma once

#include "Model.h"
#include <GL/GLUT.H>

class View{

	Model* model;

public:
	View(Model* model);
	~View(void);
	void draw_all(void);
	void draw_time_bar(void);
	void draw_board(void);
	void draw_comet(void);
	void draw_planets(void);
	void draw_samples(void);
	void draw_line(void);
};