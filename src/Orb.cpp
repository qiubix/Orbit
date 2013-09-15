#include "Orb.h"
#define _USE_MATH_DEFINES
#include <cmath>

//orb constructor
Orb::Orb(double c_x, double c_y, double c_r, double c_density) {
	x = c_x;
	y = c_y;
	r = c_r;
	density = c_density;
	mass = density * M_PI * 4 / 3 * r * r * r;
}

//gets x
double Orb::get_x(void) {
	return x;
}

//gets y
double Orb::get_y(void) {
	return y;
}

//gets r
double Orb::get_r(void) {
	return r;
}

//gets density
double Orb::get_density(void) {
	return density;
}

//gets mass
double Orb::get_mass(void) {
	return mass;
}

//sets x
void Orb::set_x(double s_x) {
	x = s_x;
}

//sets y
void Orb::set_y(double s_y) {
	y = s_y;
}

//sets r
void Orb::set_r(double s_r) {
	r = s_r;
}

//sets density
void Orb::set_density(double s_density) {
	density = s_density;
}

//sets mass
void Orb::set_mass(double s_mass) {
	mass = s_mass;
}

//orb destructor
Orb::~Orb(void) {

}