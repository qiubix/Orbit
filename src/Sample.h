#pragma once

class Sample{
	int x;
	int y;
	double angle;
	double force;
	int x_end;
	int y_end;
	int fitness;
	double ro;
	int times_chosen;

public:
	Sample(int x, int y, double angle, double force);
	~Sample(void);
	
	int get_x(void);
	int get_y(void);
	double get_angle(void);
	double get_force(void);
	int get_x_end(void);
	int get_y_end(void);
	void set_x_end(int s_x);
	void set_y_end(int s_y);
	int get_fitness(void);
	void set_fitness(int s_fitness);
	double get_ro(void);
	void set_ro(double s_ro);
	void set_times_chosen(int i);
	int get_times_chosen(void);
	void inc_times_chosen(void);
};