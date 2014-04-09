//Model.h
//Declaration of class representing model of the game

#pragma once

#include "Comet.h"
#include "Planet.h"
#include "Sample.h"
#include "Definitions.h"
#include <vector>
#include <sstream>

enum Game_state{ 
	PLACE_COMET,
	START_COMET,
	COMET_FLYING,
	LOADING,
	SOLVING
};
  
class Model{

	//our little comet
	Comet* comet;

	//vectors of pointers to planets on the particular level
	std::vector<Planet*> planets;

	//State of the game
	Game_state game_state;

	int level_number;

	double mouse_x;
	double mouse_y;

	int frame_number;

	Sample* samples[SAMPLE_QUANTITY];

	bool is_population_generated;

	int solve_frame_number;

	std::stringstream window_title;
	char tab[256];

public:
	//constructors
	Model(void); 
	~Model(void); 

	//updates
	void set_game_state(Game_state game_state);
	void prepare_next_level(void);
	void set_mouse_x(int s_x);
	void set_mouse_y(int s_y);
	void set_frame_number(int s_frame_number);
	void set_is_population_generated(bool b);
	void set_solve_frame_number(int s);
	void set_comet(Comet* s_comet);
	void set_sample(int i, Sample* sample);

	//getters
	Comet* get_comet(void);
	std::vector<Planet*>& get_planets(void);
	Game_state get_game_state(void);
	double get_mouse_x(void);
	double get_mouse_y(void);
	int get_frame_number(void);
	int get_level_number(void);
	Sample** get_samples(void);
	bool get_is_population_generated(void);
	int get_solve_frame_number(void);

	void reset_window_title(void);
	void make_window_title(int it_no);char * get_window_title(void);
};