//Controller.cpp

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <time.h>
#include <GL/GLUT.H>
#include "Controller.h"
#include "Definitions.h"

//Controller constructor
Controller::Controller(void){
	this->model = new Model();
	it_no = 0;
}

//Controller destructor
Controller::~Controller(void){
	std::cout << "Controller::~Controller\n";
}

//updates model depending on the game_state
//LOADING - loads next level and resets comet
//SOLVING - generates base population of samples and aplies evolutionary algorithm
//			to find solution, mutating populations in succesive iterations and checking
//			their fitness
//COMET_FLYING - moves the comet updating its velocities and accelerations and changing
//			frame number
void Controller::update_model(void){
	if(model -> get_game_state() == LOADING){
		load_next_level();
		planet_collision();
		model -> reset_window_title();
		glutSetWindowTitle(model -> get_window_title());
	}
	//=====================================================
	if( model->get_game_state() == SOLVING ){
		if (model -> get_solve_frame_number() == frames_per_iter) {
			model -> set_solve_frame_number(0);
			it_no++;
			if (it_no % 10 == 0)
				if (frames_per_iter > 1)
					frames_per_iter--;
			//std::cout<<it_no<<std::endl;
			model -> make_window_title(it_no);
			glutSetWindowTitle(model -> get_window_title());
			Sample * solution;
			if(!model -> get_is_population_generated()) {
				generate_samples();
				test_population();
				model -> set_is_population_generated(true);
			}
			solution = next_iteration(generate_new_population());
			if( solution != NULL ){
				Comet * comet = new Comet( solution->get_x(), solution->get_y(), COMET_RADIUS, COMET_DENSITY);
				set_start_velocities( solution->get_x_end(), solution->get_y_end(), comet);
				model -> set_is_population_generated(false);
				model -> set_comet(comet);
				model -> set_game_state(COMET_FLYING);
				for(int i = 0; i < SAMPLE_QUANTITY; i++) {
					if (model->get_samples()[i] != NULL)
						delete model->get_samples()[i];
				}
				it_no = 0;
			}
		}
		else {
			model -> set_solve_frame_number(model -> get_solve_frame_number() + 1);
		}
	}
	//====================================================
	if(model -> get_game_state() == COMET_FLYING){
		update_frame_number();
		if(model -> get_frame_number() > 0) {
			model -> get_comet() -> set_x(model -> get_comet() -> get_x() + model -> get_comet() -> get_vx() * TIME);
			model -> get_comet() -> set_y(model -> get_comet() -> get_y() + model -> get_comet() -> get_vy() * TIME);
			check_collisions( model->get_comet() );
			update_acceleration( );
			update_velocities( model->get_comet() );
		}
		else {
			model -> set_game_state(LOADING);
		}
	}
	else {
		model -> set_frame_number(FRAME_NUMBER);
	}
}

//decrements frame_number
void Controller::update_frame_number(void) {
	model -> set_frame_number(model -> get_frame_number() - 1);
}

//checks if comet collided with borders of the board and sets 
void Controller::check_collisions(void){
	double comet_x = model->get_comet()->get_x();
	double comet_y = model->get_comet()->get_y();
	double comet_radius = model->get_comet()->get_r();

	if(comet_x - comet_radius < 0) {
		model->get_comet()->set_x(-comet_x + 2 * comet_radius);
		model->get_comet()->set_vx(-model->get_comet()->get_vx());
	}
	if(comet_y - comet_radius < 0){
		model->get_comet()->set_y(-comet_y + 2 * comet_radius);
		model->get_comet()->set_vy(-model->get_comet()->get_vy());
	}
	if(comet_x + comet_radius > WIDTH - 1){
		model->get_comet()->set_x((WIDTH - 1) - 2 * comet_radius + (WIDTH - 1 - comet_x));
		model->get_comet()->set_vx(-model->get_comet()->get_vx());
	}
	if(comet_y + comet_radius > HEIGHT - 1){
		model->get_comet()->set_y((HEIGHT - 1) - 2 * comet_radius + (HEIGHT - 1 - comet_y));
		model->get_comet()->set_vy(-model->get_comet()->get_vy());
	}
}

void Controller::check_collisions(Comet * comet){
	double comet_x = comet->get_x();
	double comet_y = comet->get_y();
	double comet_radius = comet->get_r();

	if(comet_x - comet_radius < 0) {
		comet->set_x(-comet_x + 2 * comet_radius);
		comet->set_vx(-comet->get_vx());
	}
	if(comet_y - comet_radius < 0){
		comet->set_y(-comet_y + 2 * comet_radius);
		comet->set_vy(-comet->get_vy());
	}
	if(comet_x + comet_radius > WIDTH - 1){
		comet->set_x((WIDTH - 1) - 2 * comet_radius + (WIDTH - 1 - comet_x));
		comet->set_vx(-comet->get_vx());
	}
	if(comet_y + comet_radius > HEIGHT - 1){
		comet->set_y((HEIGHT - 1) - 2 * comet_radius + (HEIGHT - 1 - comet_y));
		comet->set_vy(-comet->get_vy());
	}
}

int Controller::update_acceleration(void) {
	double M;
	double a;
	double R;
	double comet_x = model -> get_comet() -> get_x();
	double comet_y = model -> get_comet() -> get_y();
	double comet_r = model -> get_comet() -> get_r();
	double x_diff;
	double y_diff;
	double cos_a;
	double sin_a;
	double ax = 0;
	double ay = 0;
	std::vector<Planet *> planets = model -> get_planets();
	for(unsigned int i = 0; i < planets.size(); i++) {
		M = planets[i] -> get_mass();
		x_diff = planets[i] -> get_x() - comet_x;
		y_diff = planets[i] -> get_y() - comet_y;
		R = sqrt(x_diff * x_diff + y_diff * y_diff);
		if(R < comet_r + planets[i] -> get_r()){
			planet_collision();
			return -1;
		}
		cos_a = x_diff / R;
		sin_a = y_diff / R;
		a = G * M / (R * R);
		ax += a * cos_a;
		ay += a * sin_a;
	}

	model -> get_comet() -> set_ax(ax);
	model -> get_comet() -> set_ay(ay);
	return 0;
}

int Controller::update_acceleration(Comet * comet) {
	double M;
	double a;
	double R;
	double comet_x = comet -> get_x();
	double comet_y = comet -> get_y();
	double comet_r = comet -> get_r();
	double x_diff;
	double y_diff;
	double cos_a;
	double sin_a;
	double ax = 0;
	double ay = 0;
	std::vector<Planet *> planets = model -> get_planets();
	for(unsigned int i = 0; i < planets.size(); i++) {
		M = planets[i] -> get_mass();
		x_diff = planets[i] -> get_x() - comet_x;
		y_diff = planets[i] -> get_y() - comet_y;
		R = sqrt(x_diff * x_diff + y_diff * y_diff);
		if(R < comet_r + planets[i] -> get_r()){
			planet_collision(comet);
			return -1;
		}
		cos_a = x_diff / R;
		sin_a = y_diff / R;
		a = G * M / (R * R);
		ax += a * cos_a;
		ay += a * sin_a;
	}

	comet -> set_ax(ax);
	comet -> set_ay(ay);
	return 1;
}

void Controller::planet_collision(void) {
	model->get_comet()->set_vx(0);
	model->get_comet()->set_vy(0);
	model->get_comet()->set_ax(0);
	model->get_comet()->set_ay(0);
	model->set_game_state(PLACE_COMET);
	int x = static_cast<int>(model -> get_mouse_x());
	int y = static_cast<int>(model -> get_mouse_y());
	if(x > model->get_comet()->get_r() && x < WIDTH - 1 - model->get_comet()->get_r() && y > model->get_comet()->get_r() && y < HEIGHT - 1 - model->get_comet()->get_r()) {
		model->get_comet()->set_x(model->get_mouse_x());
		model->get_comet()->set_y(model->get_mouse_y());
	}
	else {
		model->get_comet()->set_x(-1000);
		model->get_comet()->set_y(-1000);
	}
}

void Controller::planet_collision(Comet * comet) {
	comet->set_vx(0);
	comet->set_vy(0);
	comet->set_ax(0);
	comet->set_ay(0);
}

void Controller::update_velocities(void) {
	double vx;
	double vy;
	vx = model -> get_comet() -> get_vx();
	vy = model -> get_comet() -> get_vy();
	vx += model -> get_comet() -> get_ax();
	vy += model -> get_comet() -> get_ay();
	model -> get_comet() -> set_vx(vx);
	model -> get_comet() -> set_vy(vy);
}

void Controller::update_velocities(Comet * comet) {
	double vx;
	double vy;
	vx =  comet->get_vx();
	vy =  comet->get_vy();
	vx += comet->get_ax();
	vy += comet->get_ay();
	comet->set_vx(vx);
	comet->set_vy(vy);
}

void Controller::handle_passive_motion(int x, int y) {
	 y = (int)(HEIGHT - y);
	if(model -> get_game_state() == PLACE_COMET) {
		if(x > model->get_comet()->get_r() && x < WIDTH - 1 - model->get_comet()->get_r() && y > model->get_comet()->get_r() && y < HEIGHT - 1 - model->get_comet()->get_r()) {
			model -> get_comet() -> set_x((double)x);
			model -> get_comet() -> set_y((double)y);
			check_collisions();
		}
	}
	model -> set_mouse_x(x);
	model -> set_mouse_y(static_cast<int>(y));
}

void Controller::handle_motion(int x, int y) {
	if(model -> get_game_state() == START_COMET) {
		model -> set_mouse_x(x);
		model -> set_mouse_y(static_cast<int>(HEIGHT - y));
	}
}

void Controller::handle_mouse_click(int x, int y) {
	if(model -> get_game_state() == PLACE_COMET) {
		if(x > model->get_comet()->get_r() && x < WIDTH - 1 - model->get_comet()->get_r() && y > model->get_comet()->get_r() && y < HEIGHT - 1 - model->get_comet()->get_r()) {
			set_game_state(START_COMET);
			model -> get_comet() -> set_x(x);
			model -> get_comet() -> set_y(HEIGHT - y);
			model -> set_mouse_x(x);
			model -> set_mouse_y(static_cast<int>(HEIGHT - y));
		}
	}
}

void Controller::handle_right_mouse_down(void) {
	if(model -> get_game_state() == PLACE_COMET) {
		model->set_game_state(SOLVING);
		model->set_solve_frame_number(0);
		frames_per_iter = FRAMES_PER_ITER;
	}
}

void Controller::handle_mouse_up(int x, int y) {
	if(model -> get_game_state() == START_COMET) {
		set_game_state(COMET_FLYING);
		set_start_velocities((double)x, (double)(HEIGHT - y), model->get_comet() );
	}
}

void Controller::set_start_velocities(double x, double y) {
	double vx, vy;

	double x_diff = (model -> get_comet() -> get_x() - x);
	double y_diff = (model -> get_comet() -> get_y() - y);

	if(x_diff == 0.0 && y_diff == 0.0) {
		return;
	}

	if(x_diff == 0.0) {
		vx = 0;
		if(y_diff > MAX_SPEED_DIST)
			y_diff = MAX_SPEED_DIST;
		vy = ((int)(y_diff)) * SPEED_FRACTION;
		model -> get_comet() -> set_vx(vx);
		model -> get_comet() -> set_vy(vy);
		return;
	}
	if(y_diff == 0.0) {
		vy = 0;
		if(x_diff > MAX_SPEED_DIST)
			x_diff = MAX_SPEED_DIST;
		vx = ((int)(x_diff)) * SPEED_FRACTION;
		model -> get_comet() -> set_vx(vx);
		model -> get_comet() -> set_vy(vy);
		return;
	}

	double prop = x_diff / y_diff;

	if (abs(x_diff) > abs(y_diff)) {
		if(x_diff > MAX_SPEED_DIST)
			x_diff = MAX_SPEED_DIST;
		if(x_diff < -MAX_SPEED_DIST)
			x_diff = -MAX_SPEED_DIST;
		vx = ((int)(x_diff)) * SPEED_FRACTION;
		vy = vx / prop;
	}
	else {
		if(y_diff > MAX_SPEED_DIST)
			y_diff = MAX_SPEED_DIST;
		if(y_diff < -MAX_SPEED_DIST)
			y_diff = -MAX_SPEED_DIST;
		vy = ((int)(y_diff)) * SPEED_FRACTION;
		vx = vy * prop;
	}

	model -> get_comet() -> set_vx(vx);
	model -> get_comet() -> set_vy(vy);
}

void Controller::set_start_velocities(double x, double y, Comet * comet) {
	double vx, vy;

	double x_diff = (comet->get_x() - x);
	double y_diff = (comet->get_y() - y);

	if(x_diff == 0.0 && y_diff == 0.0) {
		return;
	}

	if(x_diff == 0.0) {
		vx = 0;
		if(y_diff > MAX_SPEED_DIST)
			y_diff = MAX_SPEED_DIST;
		vy = ((int)(y_diff)) * SPEED_FRACTION;
		comet->set_vx(vx);
		comet->set_vy(vy);
		return;
	}
	if(y_diff == 0.0) {
		vy = 0;
		if(x_diff > MAX_SPEED_DIST)
			x_diff = MAX_SPEED_DIST;
		vx = ((int)(x_diff)) * SPEED_FRACTION;
		comet->set_vx(vx);
		comet->set_vy(vy);
		return;
	}

	double prop = x_diff / y_diff;

	if (abs(x_diff) > abs(y_diff)) {
		if(x_diff > MAX_SPEED_DIST)
			x_diff = MAX_SPEED_DIST;
		if(x_diff < -MAX_SPEED_DIST)
			x_diff = -MAX_SPEED_DIST;
		vx = ((int)(x_diff)) * SPEED_FRACTION;
		vy = vx / prop;
	}
	else {
		if(y_diff > MAX_SPEED_DIST)
			y_diff = MAX_SPEED_DIST;
		if(y_diff < -MAX_SPEED_DIST)
			y_diff = -MAX_SPEED_DIST;
		vy = ((int)(y_diff)) * SPEED_FRACTION;
		vx = vy * prop;
	}

	comet->set_vx(vx);
	comet->set_vy(vy);
}

void Controller::set_game_state( Game_state game_state){
	model -> set_game_state(game_state);
}

void Controller::load_next_level(void){
	model->prepare_next_level();
	srand( (unsigned int)time(NULL) );
	for( int i=0; i < model->get_level_number() ; i++){
		double r = PLANET_RADIUS_MIN + ((double)rand()) * (PLANET_RADIUS_MAX - PLANET_RADIUS_MIN) / RAND_MAX;
		double x = r + ((double)rand()) * (WIDTH - 2 * r) / RAND_MAX;
		double y = r + ((double)rand()) * (HEIGHT - 2 * r) / RAND_MAX;
		double d = PLANET_DENSITY_MIN + ((double)rand()) * (PLANET_DENSITY_MAX - PLANET_DENSITY_MIN) / RAND_MAX;

		//for( unsigned int i=0; i < planets.size(); i++)
		//	if( planets[i]->get_x() == x && planets[i]->get_y() == y){
		//		double x = r + ((double)rand()) * (WIDTH) / RAND_MAX;
		//		double y = r + ((double)rand()) * (HEIGHT) / RAND_MAX;
		//	}

		model->get_planets().push_back(new Planet(x,y,r,d));

		//std::cout << "Planet no." << i << ": " << r << ", " << x << ", " << y << ", " << d << std::endl;
	}
}

//==============================================================================================================

void Controller::update_sample_comet(Comet * comet){
	check_collisions(comet);
}


Sample* Controller::next_iteration(Sample** samples){
	double x,y, x_end, y_end;
	calculate_strain_coords(samples);
	Comet* comets[SAMPLE_QUANTITY];
	Comet * comet;
	Sample * return_sample = NULL;

	for( int i=0; i < SAMPLE_QUANTITY; i++){
		x = samples[i]->get_x();
		y = samples[i]->get_y();
		x_end = samples[i]->get_x_end();
		y_end = samples[i]->get_y_end();
		Comet * comet = new Comet(x,y,COMET_RADIUS, COMET_DENSITY);
		set_start_velocities( x_end, y_end, comet);
		comets[i] = comet;
	}
	
	int skoczek = 0;
	for( int i=0; i < FRAME_NUMBER; i++){
		for( int j = skoczek; j < SAMPLE_QUANTITY; j++){
			comet = comets[j];
			comet->set_x( comet -> get_x() + comet -> get_vx() * TIME);
			comet->set_y( comet -> get_y() + comet -> get_vy() * TIME);
			check_collisions(comet);
			if( update_acceleration(comet) < 0 ){
				samples[j]->set_fitness(i);
				Sample* temp = samples[skoczek];
				samples[skoczek] = samples[j];
				samples[j] = temp;
				skoczek++;
			}
			else{
				update_velocities(comet);
				if(i == FRAME_NUMBER - 1){
					samples[j] -> set_fitness(FRAME_NUMBER);
					return_sample = samples[j];
				}
			}
		}
	}
	
	Sample* parents[SAMPLE_QUANTITY];
	for( int k = 0; k < SAMPLE_QUANTITY; k++ ){
		parents[k] = model->get_samples()[k];
	}

	int index1 = SAMPLE_QUANTITY - 1;
	int index2 = SAMPLE_QUANTITY - 1;
	for( int i = SAMPLE_QUANTITY - 1; i >= 0; i-- ){
		//std::cout<<parents[index1]->get_fitness()<<std::endl;
		if( parents[index1]->get_fitness() >= samples[index2]->get_fitness()){
			//model->get_samples()[i] = parents[index1];
			model -> set_sample(i, parents[index1]);
			model->get_samples()[i] -> inc_times_chosen();
			if(model->get_samples()[i] -> get_times_chosen() == 5) {
				model->get_samples()[i] -> set_ro(model->get_samples()[i] -> get_ro() * 1.4);
				model->get_samples()[i] -> set_times_chosen(0);
			}
			index1--;
		}
		else{
			//model->get_samples()[i] = samples[index2];
			//std::cout<<"wybrano."<<std::endl;
			model -> set_sample(i, samples[index2]);
			index2--;
		}
	}

	//czyszczenie jeszcze
	while (index1 >= 0) {
		delete parents[index1];
		index1--;
	}
	while (index2 >= 0) {
		delete samples[index2];
		index2--;
	}
	delete []samples;

	return return_sample;
}


void Controller::generate_samples(void){
	double R = 10.0;
	int x, y;
	double angle_deg, force;
	it_no++;
	srand((unsigned int)time(NULL));
	for( int i = 0; i < SAMPLE_QUANTITY; i++ ){
		x = (int)( R + ((double)rand()/RAND_MAX) * (WIDTH - 2 * R));
		y = (int)(R + ((double)rand()/RAND_MAX) * (HEIGHT - 2 * R));
		angle_deg = ((double)rand()/RAND_MAX) * 360; //w stopniach
		force = ((double)rand()/RAND_MAX) * MAX_SPEED_DIST;
		Sample* sample = new Sample(x,y,angle_deg, force);
		/*if(model->get_samples()[i] != NULL)
			delete model->get_samples()[i];*/
		model->get_samples()[i] = sample;
	}
	std::cout<<it_no<<std::endl;
}


void Controller::calculate_strain_coords(Sample** samples){

	for( int i = 0; i < SAMPLE_QUANTITY; i++ ){
		int x_multi = 1;
		int y_multi = 1;
		double angle_deg = samples[i]->get_angle();
		double force = samples[i]->get_force();

		switch(((int) (angle_deg / 90)) % 4 + 1) {
			case 1: break;									//pierwsza cwiartka
			case 2: x_multi *= -1; break;					//druga cwiartka
			case 3: x_multi *= -1; y_multi *= -1; break;	//trzecia cwiartka
			case 4: y_multi *= -1; break;					//czwarta cwiartka
		}

		angle_deg = fmod(angle_deg, 90.0);
	
		double angle_rad = angle_deg * M_PI / 180;

		int x_end;
		int y_end;

		if (angle_deg == 0.0) {
			if(x_multi == 1) {
				if(y_multi == 1) {//pierwsza cwiartka
					y_end = 0;
					if(fmod(force, 1.0) < 0.5)
						x_end =(int)(force);
					else
						x_end =(int)(force) + 1;
				}
				else {//czwarta cwiartka
					x_end = 0;
					if(fmod(force, 1.0) < 0.5)
						y_end =(int)(force);
					else
						y_end =(int)(force) + 1;
				}
			}
			else {
				if(y_multi == 1) {//druga cwiartka
					x_end = 0;
					if(fmod(force, 1.0) < 0.5)
						y_end =(int)(force);
					else
						y_end =(int)(force) + 1;
				}
				else { //trzecia cwiartka
					y_end = 0;
					if(fmod(force, 1.0) < 0.5)
						x_end =(int)(force);
					else
						x_end =(int)(force) + 1;
				}
			}
		}
		else {
			double prop = tan(angle_rad); //      y / x

			if(prop < 1) { //x wiekszy
				if ((x_multi == 1 && y_multi ==1) || (x_multi == -1 && y_multi == -1)) {
					if(fmod(force, 1.0) < 0.5)
						x_end =(int)(force);
					else
						x_end =(int)(force) + 1;
					if(fmod(force * prop, 1.0) < 0.5)
						y_end = (int)(force * prop);
					else 
						y_end = (int)(force * prop) + 1;
				}
				else {
					if(fmod(force, 1.0) < 0.5)
						y_end =(int)(force);
					else
						y_end =(int)(force) + 1;
					if(fmod(force / prop, 1.0) < 0.5)
						x_end = (int)(force * prop);
					else 
						x_end = (int)(force * prop) + 1;
				}
			}
			else { //y wiekszy
				if (x_multi == 1 && y_multi ==1 || x_multi == -1 && y_multi == -1) {
					if(fmod(force, 1.0) < 0.5)
						y_end =(int)(force);
					else
						y_end =(int)(force) + 1;
					if(fmod(force / prop, 1.0) < 0.5)
						x_end = (int)(force / prop);
					else 
						x_end = (int)(force / prop) + 1;
				}
				else {
					if(fmod(force, 1.0) < 0.5)
						x_end =(int)(force);
					else
						x_end =(int)(force) + 1;
					if(fmod(force * prop, 1.0) < 0.5)
						y_end = (int)(force / prop);
					else 
						y_end = (int)(force / prop) + 1;
				}
			}
		}
		x_end = samples[i]->get_x() + x_multi * x_end;
		y_end = samples[i]->get_y() + y_multi * y_end;

		samples[i] ->set_x_end(x_end);
		samples[i] ->set_y_end(y_end);

	}// for every sample

}


void Controller::test_population(void){
	double x,y, x_end, y_end;
	calculate_strain_coords(model->get_samples());
	Comet* comets[SAMPLE_QUANTITY];

	for( int i=0; i < SAMPLE_QUANTITY; i++){
		x = model->get_samples()[i]->get_x();
		y = model->get_samples()[i]->get_y();
		x_end = model->get_samples()[i]->get_x_end();
		y_end = model->get_samples()[i]->get_y_end();
		Comet * comet = new Comet(x,y,COMET_RADIUS, COMET_DENSITY);
		set_start_velocities( x_end, y_end, comet);
		comets[i] = comet;
	}
	int skoczek = 0;
	Comet * comet;
	for( int i=0; i < FRAME_NUMBER; i++){
		for( int j = skoczek; j < SAMPLE_QUANTITY; j++){
			comet = comets[j];
			comet->set_x( comet -> get_x() + comet -> get_vx() * TIME);
			comet->set_y( comet -> get_y() + comet -> get_vy() * TIME);
			check_collisions(comet);
			if( update_acceleration(comet) < 0 ){
				model->get_samples()[j]->set_fitness(i);
				Sample* temp = model->get_samples()[skoczek];
				model->get_samples()[skoczek] = model->get_samples()[j];
				model->get_samples()[j] = temp;
				skoczek++;
			}
			else{
				update_velocities(comet);
			}
		}
	}
}

Sample** Controller::generate_new_population(void){
	Sample** samples;
	samples = new Sample*[SAMPLE_QUANTITY];
	int x, y;
	double angle_deg, force;
	double ro;
	for( int i = 0; i < SAMPLE_QUANTITY; i++ ) {
		ro = model->get_samples()[i]->get_ro();
		if (ro > 400) {
			ro = 400;
			model->get_samples()[i]->set_ro(200.0);
		}
		do {
		x = (int)(model->get_samples()[i]->get_x() + ro * normal_deviate());
		} while (x < COMET_RADIUS || x > WIDTH - COMET_RADIUS);
		do {
		y = (int)(model->get_samples()[i]->get_y() + ro * normal_deviate());
		} while (y < COMET_RADIUS || y > HEIGHT - COMET_RADIUS);
		if (ro > 100)
			ro = 100;
		//	ro /= 2.5f;
		angle_deg = model->get_samples()[i]->get_angle() + ro * normal_deviate();
		angle_deg = fmod(angle_deg, 90.0);
		if (ro > 40)
			ro /= 40;
		//	ro /= 6.6f;
		do {
		force = model->get_samples()[i]->get_force() + ro * normal_deviate();
		} while (force < 0.0 || force > MAX_SPEED_DIST);
		Sample* sample = new Sample(x,y,angle_deg,force);
		sample->set_ro(ro);
		samples[i] = sample;
	}
	return samples;
}

double Controller::normal_deviate(void) {
	double u, v;
	double s;
	do {
		u = (double)rand() * 2 / RAND_MAX - 1.0;
		v = (double)rand() * 2 / RAND_MAX - 1.0;
		s = u * u + v * v;
	} while (s >= 1.0 || s == 0.0);
	double z = u * sqrt((-2 * log(s)) / s);
	return z;
}

void Controller::normal_deviate(double& z1, double& z2) {
	double u, v;
	double s;
	do {
		u = (double)rand() * 2 / RAND_MAX - 1.0;
		v = (double)rand() * 2 / RAND_MAX - 1.0;
		s = u * u + v * v;
	} while (s >= 1.0 || s == 0.0);
	z1 = u * sqrt((-2 * log(s)) / s);
	z2 = v * sqrt((-2 * log(s)) / s);
}

//==============================================================================================================



Model* Controller::get_model(void){
	return model;
}