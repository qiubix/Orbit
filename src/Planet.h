#pragma once
#include "Orb.h"

class Planet : public Orb {
protected:

public:
	Planet(double c_x, double c_y, double c_r, double c_density);
	~Planet(void);
};