//Controller.h

#pragma once

#include "Model.h"

class Controller{

	Model* model;

	void check_collisions(void);
	void check_collisions(Comet* comet);
	int frames_per_iter;
	int it_no;

public:
	Controller(void);
	~Controller(void);

	//updates
	void update_model(void);
	void update_frame_number(void);
	void set_game_state( Game_state game_state);
	void load_next_level(void); 
	int update_acceleration(void);
	int update_acceleration(Comet * comet);
	void handle_passive_motion(int x, int y);
	void handle_mouse_click(int x, int y);
	void handle_right_mouse_down(void);
	void handle_motion(int x, int y);
	void handle_mouse_up(int x, int y);
	void set_start_velocities(double x, double y);
	void set_start_velocities(double x, double y, Comet * comet);
	void planet_collision(void);
	void planet_collision(Comet* comet);
	void update_velocities(void);
	void update_velocities(Comet* comet);
	void update_sample_comet(Comet* comet);
	Sample* next_iteration(Sample** samples);

	void generate_samples(void);
	void calculate_strain_coords(Sample** samples);
	void test_population(void);
	Sample** generate_new_population(void);
	double normal_deviate(void);
	void normal_deviate(double& z1, double& z2);

	//getters
	Model* get_model(void);
};