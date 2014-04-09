//Sample.cpp

#include "Sample.h"

//Sample constructor
Sample::Sample(int c_x, int c_y, double c_angle, double c_force): x(c_x), y(c_y), angle(c_angle), force(c_force) {
	x_end = 0;
	y_end = 0;
	fitness = 0;
	ro = 1;
	times_chosen = 0;
}

//Sample destructor
Sample::~Sample(void){
}

//gets x
int Sample::get_x(void){
	return x;
}

//increments times_chosen
void Sample::inc_times_chosen(void) {
	times_chosen++;
}

//gets times_chosen
int Sample::get_times_chosen(void) {
	return times_chosen;
}

//sets times_chosen
void Sample::set_times_chosen(int i) {
	times_chosen = i;
}

//gets y
int Sample::get_y(void){
	return y;
}

//gets angle
double Sample::get_angle(void){
	return angle;
}

//gets force
double Sample::get_force(void){
	return force;
}

//gets x_end
int Sample::get_x_end(void){
	return x_end;
}

//gets y_end
int Sample::get_y_end(void){
	return y_end;
}

//sets x_end
void Sample::set_x_end(int s_x){
	x_end = s_x;
}

//sets y_end
void Sample::set_y_end(int s_y){
	y_end = s_y;
}

//gets fitness
int Sample::get_fitness(void){
	return fitness;
}

//sets fitness
void Sample::set_fitness(int s_fitness){
	fitness = s_fitness;
}

//gets ro
double Sample::get_ro(void){
	return ro;
}

//sets ro
void Sample::set_ro(double s_ro){
	ro = s_ro;
}