#pragma once
#include "Orb.h"

class Comet : public Orb {
protected:
	double vx; 
	double vy;
	double ax;
	double ay;
public:
	Comet(double c_x, double c_y, double c_r, double c_density);
	double get_vx(void);
	double get_vy(void);
	double get_ax(void);
	double get_ay(void);
	void set_vx(double s_vx);
	void set_vy(double s_vy);
	void set_ax(double s_ax);
	void set_ay(double s_ay);
	~Comet(void);
};