#include "Comet.h"

//orb constructor
Comet::Comet(double c_x, double c_y, double c_r, double c_density) : Orb(c_x, c_y, c_r, c_density) {
	vx = vy = ax = ay = 0.0;
}

//gets vx
double Comet::get_vx(void) {
	return vx;
}

//gets vy
double Comet::get_vy(void) {
	return vy;
}

//gets ax
double Comet::get_ax(void) {
	return ax;
}

//gets ay
double Comet::get_ay(void) {
	return ay;
}

//sets vx
void Comet::set_vx(double s_vx) {
	vx = s_vx;
}

//sets vy
void Comet::set_vy(double s_vy) {
	vy = s_vy;
}

//sets ax
void Comet::set_ax(double s_ax) {
	ax = s_ax;
}

void Comet::set_ay(double s_ay) {
	ay = s_ay;
}

//orb destructor
Comet::~Comet(void) {
}
