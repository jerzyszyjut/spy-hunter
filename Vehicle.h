#pragma once

#include "Sprite.h"
#include "Display.h"


#define MIN_Y_VELOCITY_FACTOR 0.1
#define MAX_Y_VELOCITY_FACTOR 1.0
#define DEFAULT_Y_VELOCITY_FACTOR 0.62
#define DEFAULT_Y_VELOCITY 700
#define DEFAULT_Y_OFFSET 200
#define DEFAULT_Y_ACCELERATION 0.01
#define DEFAULT_X_VELOCITY 1000.0

enum movement_direction
{
	LEFT,
	RIGHT,
	NONE,
};

enum acceleration_direction {
	ACCELERATE,
	DECELERATE,
	NO_ACCELERATION,
};

class Vehicle: public Sprite
{
	public:
		double x_velocity, y_velocity;
		int direction, acceleration;
		
		Vehicle(Display*);
		void HandleMovement(double);
		
	private:
		double y_velocity_factor;
};

