#pragma once

class Orb {
protected:
	double x;
	double y;
	double r;
	double density;
	double mass;
public:
	Orb(double c_x, double c_y, double c_r, double c_density);
	double get_x(void);
	double get_y(void);
	double get_r(void);
	double get_density(void);
	double get_mass(void);
	void set_x(double s_x);
	void set_y(double s_y);
	void set_r(double s_r);
	void set_density(double s_density);
	void set_mass(double s_mass);
	~Orb(void);
};