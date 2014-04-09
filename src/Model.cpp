//Model.cpp

#include "Model.h"
#include "Definitions.h"
#include <iostream>
#include <time.h>

//constructor
Model::Model(void):level_number(0),game_state(LOADING),frame_number(FRAME_NUMBER){
	comet = new Comet(COMET_START_X, COMET_START_Y, COMET_RADIUS, COMET_DENSITY);
	for(int i = 0; i < SAMPLE_QUANTITY; i++) {
		samples[i] = NULL;
	}
	is_population_generated = false;
}

//destructor
Model::~Model(void){ 
	for( int i=0; i < static_cast<int>(planets.size()); i++ )
		delete planets[i];
	delete comet;
}

//sets game_state
void Model::set_game_state(Game_state game_state){
	this->game_state = game_state;
}

//prepares board for next level
void Model::prepare_next_level(void){
	for(unsigned int i = 0; i < planets.size(); i++) {
		delete planets[i];
	}
	planets.clear();

	level_number++;
}

//gets comet
Comet* Model::get_comet(void){
	return comet;
}

//sets comet
void Model::set_comet(Comet* s_comet){
	comet = s_comet;
}

//gets planets
std::vector<Planet*>& Model::get_planets(void){
	return planets;
}

//gets game_state
Game_state Model::get_game_state(void){
	return game_state;
}

//gets mouse_x
double Model::get_mouse_x(void){
	return mouse_x;
}

//gets mouse_y
double Model::get_mouse_y(void){
	return mouse_y;
}

//gets frame_number
int Model::get_frame_number(void) {
	return frame_number;
}

//sets mouse_x
void  Model::set_mouse_x(int s_x) {
	mouse_x = s_x;
}

//sets mouse_y
void  Model::set_mouse_y(int s_y) {
	mouse_y = s_y;
}

//sets frame_nubmer
void Model::set_frame_number(int s_frame_number) {
	frame_number = s_frame_number;
}

//gets level_number
int Model::get_level_number(void){
	return level_number;
}

//gets samples
Sample** Model::get_samples(void){
	return samples;
}

//gets is_population_generated
bool Model::get_is_population_generated(void) {
	return is_population_generated;
}

//sets is_population_generated
void Model::set_is_population_generated(bool b) {
	is_population_generated = b;
}

//gets solve_frame_number
int Model::get_solve_frame_number(void) {
	return solve_frame_number;
}

//sets solve_frame_number
void Model::set_solve_frame_number(int s) {
	solve_frame_number = s;
}

//sets sample
void Model::set_sample(int i, Sample* sample) {
	samples[i] = sample;
}

char * Model::get_window_title(void) {
	window_title.getline(tab, 256);
	return tab;
}

void Model::reset_window_title(void) {
	window_title.clear();
	window_title<<"Orbit. Level "<<level_number<<".";
}

void Model::make_window_title(int it_no) {
	window_title.clear();
	window_title<<"Orbit. Level "<<level_number<<". Solving. Iteration: "<<it_no;
}